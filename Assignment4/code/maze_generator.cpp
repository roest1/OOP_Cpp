#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <queue>
#include <random>
#include <set>
#include <stdexcept>
#include <utility>
#include <vector>

#include "hash_code.hpp"
#include "maze_generator.hpp"

namespace {

    // Creates a blank grid maze of the given size
    maze blank_maze(int num_rows, int num_cols)
    {
        return maze(num_rows, num_cols);
    }

    // Type representing a possible edge in a maze. Two MazeCells are each
    // included here, along with the fields that would link them.
    struct edge_builder
    {
        edge_builder() = default;

        edge_builder(maze_iterator f, maze_iterator l,
            maze_iterator maze_cell::*ff, maze_iterator maze_cell::*sf)
          : first(f)
          , second(l)
          , first_field(ff)
          , second_field(sf)
        {
        }

        // Which two edges to link
        maze_iterator first;
        maze_iterator second;

        // How to link them
        maze_iterator maze_cell::*first_field = nullptr;
        maze_iterator maze_cell::*second_field = nullptr;
    };

    // Generates a random number in the specified range. We use this instead of
    // std::uniform_int_distribution because std::uniform_int_distribution is
    // not cross-platform compatible.
    int random_between(int low, int high, std::mt19937& gen)
    {
        return low + gen() % (high - low + 1);
    }

    // Generates a random real number between 0 and 1. We use this instead of
    // std::uniform_real_distribution because std::uniform_real_distribution is
    // not cross-platform compatible.
    double random_probability(std::mt19937& gen)
    {
        while (true)
        {
            std::uint32_t value = gen();
            std::uint32_t max = std::numeric_limits<decltype(value)>::max();

            if (value != max)
            {
                return double(value) / max;
            }
        }
    }

    // Returns all possible edges that could appear in a grid maze.
    std::vector<edge_builder> all_possible_edges_for(maze& maze)
    {
        std::vector<edge_builder> result;
        for (int row = 0; row < maze.num_rows(); ++row)
        {
            for (int col = 0; col < maze.num_columns(); ++col)
            {
                if (row + 1 < maze.num_rows())
                {
                    result.emplace_back(maze(row, col), maze(row + 1, col),
                        &maze_cell::south, &maze_cell::north);
                }
                if (col + 1 < maze.num_columns())
                {
                    result.emplace_back(maze(row, col), maze(row, col + 1),
                        &maze_cell::east, &maze_cell::west);
                }
            }
        }
        return result;
    }

    /* Union-find FIND operation. */
    maze_iterator rep_for(
        std::map<maze_iterator, maze_iterator> const& reps, maze_iterator cell)
    {
        while (reps.at(cell) != cell)
        {
            cell = reps.at(cell);
        }
        return cell;
    }

    // Shuffles the edges using the Fischer-Yates shuffle. We provide our own
    // implementation because the std::shuffle implementations on Windows, Mac,
    // and Linux are not guaranteed to work in the same way.
    void shuffle_edges(
        std::vector<edge_builder>& edges, std::mt19937& generator)
    {
        for (size_t i = 0; i < edges.size(); ++i)
        {
            size_t j = random_between(i, edges.size() - 1, generator);

            using std::swap;
            swap(edges[i], edges[j]);
        }
    }

    // Creates a random maze of the given size using a randomized Kruskal's
    // algorithm. Edges are shuffled and added back in one at a time, provided
    // that each insertion links two disconnected regions.
    maze make_maze(int num_rows, int num_columns, std::mt19937& generator)
    {
        auto maze = blank_maze(num_rows, num_columns);

        auto edges = all_possible_edges_for(maze);
        shuffle_edges(edges, generator);

        // Union-find structure, done without path compression because N is small.
        std::map<maze_iterator, maze_iterator> representatives;
        for (auto& elem : maze)
        {
            // initialize the iterators
            elem.north = maze.end();
            elem.south = maze.end();
            elem.east = maze.end();
            elem.west = maze.end();

            auto iter = maze.iterator_for(elem);
            representatives[iter] = iter;
        }

        // Run a randomized Kruskal's algorithm to build the maze
        int edges_left = num_rows * num_columns - 1;
        for (std::size_t i = 0; edges_left > 0 && i < edges.size(); ++i)
        {
            auto edge = edges[i];

            // See if they're linked already
            auto rep1 = rep_for(representatives, edge.first);
            auto rep2 = rep_for(representatives, edge.second);

            // If not, link them.
            if (rep1 != rep2)
            {
                representatives[rep1] = rep2;

                (*edge.first).*edge.first_field = edge.second;
                (*edge.second).*edge.second_field = edge.first;

                --edges_left;
            }
        }
        if (edges_left != 0)
            throw std::runtime_error("Unable to build maze.");

        return maze;
    }

    // Clears all the links between the given group of nodes
    void clear_graph(maze& nodes)
    {
        auto end = nodes.end();
        for (auto& node : nodes)
        {
            node = maze_cell(maze_item::nothing, end, end, end, end);
        }
    }

    // Returns a random unassigned link from the given node, or nullptr if
    // they are all assigned.
    maze_iterator maze_cell::*random_free_port_of(
        maze_iterator end, maze_iterator const& cell, std::mt19937& gen)
    {
        std::vector<maze_iterator maze_cell::*> ports;
        if (cell->east == end)
            ports.emplace_back(&maze_cell::east);
        if (cell->west == end)
            ports.emplace_back(&maze_cell::west);
        if (cell->north == end)
            ports.emplace_back(&maze_cell::north);
        if (cell->south == end)
            ports.emplace_back(&maze_cell::south);

        if (ports.empty())
            return nullptr;

        int port = random_between(0, ports.size() - 1, gen);
        return ports[port];
    }

    // Use a variation of the Erdos-Renyi random graph model. We set the
    // probability of any pair of nodes being connected to be ln(n) / n,
    // then artificially constrain the graph so that no node has degree
    // four or more. We generate mazes this way until we find one that's
    // connected.
    bool erdos_renyi_link(maze_iterator end, maze& nodes, std::mt19937& gen)
    {
        /* High probability that everything is connected. */
        double threshold = std::log(nodes.size()) / nodes.size();

        for (int i = 0; i < nodes.size(); ++i)
        {
            for (int j = i + 1; j < nodes.size(); ++j)
            {
                if (random_probability(gen) <= threshold)
                {
                    auto ilink = random_free_port_of(end, nodes(i), gen);
                    auto jlink = random_free_port_of(end, nodes(j), gen);

                    // Oops, no free links
                    if (ilink == nullptr || jlink == nullptr)
                    {
                        return false;
                    }

                    (*nodes(i)).*ilink = nodes(j);
                    (*nodes(j)).*jlink = nodes(i);
                }
            }
        }

        return true;
    }

    // Returns whether the given maze is fully connected
    bool is_connected(maze_iterator end, maze const& maze)
    {
        std::set<const_maze_iterator> visited;

        std::queue<const_maze_iterator> frontier;
        frontier.push(maze(0));

        while (!frontier.empty())
        {
            auto curr = frontier.front();
            frontier.pop();

            if (visited.find(curr) == visited.end())
            {
                visited.insert(curr);

                if (curr->east != end)
                    frontier.push(curr->east);
                if (curr->west != end)
                    frontier.push(curr->west);
                if (curr->north != end)
                    frontier.push(curr->north);
                if (curr->south != end)
                    frontier.push(curr->south);
            }
        }

        return visited.size() == maze.size();
    }

    // Generates a random twisty maze. This works by repeatedly generating
    // random graphs until a connected one is found.
    maze make_twisty_maze(int num_nodes, std::mt19937& generator)
    {
        maze result(num_nodes, 1);

        // Keep generating mazes until we get a connected one
        do
        {
            clear_graph(result);
        } while (!erdos_renyi_link(result.end(), result, generator) ||
            !is_connected(result.end(), result));

        return result;
    }

    // Returns if two nodes are adjacent
    bool are_adjacent(maze_iterator first, maze_iterator second)
    {
        return first->east == second || first->west == second ||
            first->north == second || first->south == second;
    }

    template <typename T>
    class grid
    {
        std::vector<T> data;
        std::size_t rows;
        std::size_t columns;

    public:
        grid(std::size_t rows, std::size_t columns, T init = T())
          : data(rows * columns, init)
          , rows(rows)
          , columns(columns)
        {
        }

        T& operator()(std::size_t row, std::size_t column)
        {
            return data[row * columns + column];
        }
        T const& operator()(std::size_t row, std::size_t column) const
        {
            return data[row * columns + column];
        }

        std::size_t num_rows() const
        {
            return rows;
        }
        std::size_t num_columns() const
        {
            return columns;
        }
    };

    // Uses the Floyd-Warshall algorithm to compute the shortest paths between all
    // pairs of nodes in the maze. The result is a table where table[i][j] is the
    // shortest path distance between maze[i] and maze[j].
    grid<int> all_pairs_shortest_paths(std::vector<maze_iterator> const& maze)
    {
        // Floyd-Warshall algorithm. Fill the grid with "infinity" values
        auto size = maze.size();
        grid<int> result(size, size, size + 1);

        /* Set distances of nodes to themselves at 0. */
        for (int i = 0; i < maze.size(); ++i)
        {
            result(i, i) = 0;
        }

        // Set distances of edges to 1
        for (int i = 0; i < size; ++i)
        {
            for (int j = 0; j < size; ++j)
            {
                if (are_adjacent(maze[i], maze[j]))
                {
                    result(i, j) = 1;
                }
            }
        }

        // Dynamic programming step. Keep expanding paths by allowing for paths
        // between nodes
        for (int i = 0; i < size; ++i)
        {
            grid<int> next(size, size);
            for (int j = 0; j < size; ++j)
            {
                for (int k = 0; k < size; ++k)
                {
                    next(j, k) =
                        std::min(result(j, k), result(j, i) + result(i, k));
                }
            }
            result = std::move(next);
        }

        return result;
    }

    // Given a list of distinct nodes, returns the "score" for their distances,
    // which is a sequence of numbers representing pairwise distances in sorted
    // order
    std::vector<int> score_of(
        std::vector<int> const& nodes, grid<int> const& distances)
    {
        std::vector<int> result;

        for (int i = 0; i < nodes.size(); i++)
        {
            for (int j = i + 1; j < nodes.size(); j++)
            {
                result.push_back(distances(nodes[i], nodes[j]));
            }
        }

        std::sort(result.begin(), result.end());
        return result;
    }

    // Given a grid, returns a combination of four nodes whose overall score
    // (sorted list of pairwise distances) is as large as possible in a
    // lexicographical sense
    std::vector<int> remote_locations_in(grid<int> const& distances)
    {
        std::vector<int> result = {0, 1, 2, 3};

        // We could do this recursively, but since it's "only" four loops
        // we'll just do that instead
        for (std::size_t i = 0; i < distances.num_rows(); ++i)
        {
            for (std::size_t j = i + 1; j < distances.num_rows(); ++j)
            {
                for (std::size_t k = j + 1; k < distances.num_rows(); ++k)
                {
                    for (std::size_t l = k + 1; l < distances.num_rows(); ++l)
                    {
                        std::vector<int> curr = {
                            int(i), int(j), int(k), int(l)};
                        if (score_of(curr, distances) >
                            score_of(result, distances))
                        {
                            result = std::move(curr);
                        }
                    }
                }
            }
        }

        return result;
    }

    /// Copies items from the text maze over to the grid maze
    void fill_items(maze& maze, std::vector<std::string> const& text_maze)
    {
        for (int row = 0; row < maze.num_rows(); ++row)
        {
            for (int col = 0; col < maze.num_columns(); ++col)
            {
                // Odd-numbered positions are interstitial positions
                switch (text_maze[2 * row][2 * col])
                {
                case 'S':
                    maze(row, col)->whats_here = maze_item::spellbook;
                    break;
                case 'P':
                    maze(row, col)->whats_here = maze_item::potion;
                    break;
                case 'W':
                    maze(row, col)->whats_here = maze_item::wand;
                    break;
                case '*':
                    maze(row, col)->whats_here = maze_item::nothing;
                    break;
                default:
                    throw std::runtime_error(
                        "Unknown character found when copying items.");
                }
            }
        }
    }

    // Fills in the walls in maze given a text pattern
    void add_walls(maze& maze, std::vector<std::string> const& text_maze)
    {
        for (int row = 0; row < maze.num_rows(); ++row)
        {
            for (int col = 0; col < maze.num_columns(); ++col)
            {
                // Look down if that's possible
                if (row + 1 < maze.num_rows() &&
                    text_maze[2 * row + 1][2 * col] == '|')
                {
                    maze(row, col)->south = maze(row + 1, col);
                    maze(row + 1, col)->north = maze(row, col);
                }
                // Look left if that's possible
                if (col + 1 < maze.num_columns() &&
                    text_maze[2 * row][2 * col + 1] == '-')
                {
                    maze(row, col)->east = maze(row, col + 1);
                    maze(row, col + 1)->west = maze(row, col);
                }
            }
        }
    }

    // Validates that a text maze is structured correctly
    void validate_maze(std::vector<std::string> const& text_maze)
    {
        // Number of rows should be odd since every other space is a location
        // Same with the number of columns
        if (text_maze.size() % 2 != 1)
        {
            throw std::runtime_error("Maze should have an odd number of rows.");
        }
        if (text_maze[0].size() % 2 != 1)
        {
            throw std::runtime_error(
                "Maze should have an odd number of columns.");
        }

        int numRows = 1 + text_maze.size() / 2;
        int numCols = 1 + text_maze[0].size() / 2;

        for (int row = 0; row < numRows; row++)
        {
            if (text_maze[row].size() != text_maze[0].size())
            {
                throw std::runtime_error(
                    "Not all rows in the maze have the same size.");
            }
        }

        // Confirm that there are blank spaces at all odd/odd pairs
        for (int row = 0; row + 1 < numRows; row++)
        {
            for (int col = 0; col + 1 < numCols; col++)
            {
                if (text_maze[2 * row + 1][2 * col + 1] != ' ')
                {
                    throw std::runtime_error(
                        "Found non-blank character where blank was expected.");
                }
            }
        }

        // Confirm each location is either *, S, P, or W
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                switch (text_maze[2 * row][2 * col])
                {
                case 'P':
                case 'S':
                case 'W':
                    [[fallthrough]];
                case '*':
                    // Do nothing
                    break;
                default:
                    throw std::runtime_error(
                        "Unknown character found at lattice point.");
                }
            }
        }

        // Confirm each wall is either -, |, or blank
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numCols; col++)
            {
                if (row + 1 < numRows &&
                    text_maze[2 * row + 1][2 * col] != ' ' &&
                    text_maze[2 * row + 1][2 * col] != '|')
                {
                    throw std::runtime_error("Unknown wall character.");
                }
                if (col + 1 < numCols &&
                    text_maze[2 * row][2 * col + 1] != ' ' &&
                    text_maze[2 * row][2 * col + 1] != '-')
                {
                    throw std::runtime_error("Unknown wall character.");
                }
            }
        }
    }
}    // namespace

// Please don't make any changes to this function - we'll be using our
// reference version when testing your code, and it would be a shame if
// the maze you solved wasn't the maze we wanted you to solve!
std::pair<maze, maze_iterator> maze_for(std::string const& name)
{
    // Seed a generator using the provided seed. From this point forward, only
    // use random numbers from this source
    std::mt19937 generator(hash_code(name, NUM_ROWS, NUM_COLUMNS));
    auto maze = make_maze(NUM_ROWS, NUM_COLUMNS, generator);

    // Place the items and start locations far away from one another
    std::vector<maze_iterator> linear_maze;
    for (auto& cell : maze)
    {
        linear_maze.push_back(maze.iterator_for(cell));
    }

    auto distances = all_pairs_shortest_paths(linear_maze);
    auto locations = remote_locations_in(distances);

    // Place the items
    linear_maze[locations[1]]->whats_here = maze_item::spellbook;
    linear_maze[locations[2]]->whats_here = maze_item::potion;
    linear_maze[locations[3]]->whats_here = maze_item::wand;

    return std::make_pair(std::move(maze), linear_maze[locations[0]]);
}

// Please don't make any changes to this function - we'll be using our
// reference version when testing your code, and it would be a shame if
// the maze you solved wasn't the maze we wanted you to solve!
std::pair<maze, maze_iterator> twisty_maze_for(std::string const& name)
{
    // Seed a generator using the provided seed. From this point forward, only
    // use random numbers from this source.
    std::mt19937 gen(hash_code(name, NUM_TWISTY_ROOMS));
    auto maze = make_twisty_maze(NUM_TWISTY_ROOMS, gen);

    std::vector<maze_iterator> linear_maze;
    for (auto& cell : maze)
    {
        linear_maze.push_back(maze.iterator_for(cell));
    }

    // Find the distances between all pairs of nodes
    auto distances = all_pairs_shortest_paths(linear_maze);

    // Select a 4-tuple maximizing the minimum distances between points,
    // and use that as our item/start locations.
    auto locations = remote_locations_in(distances);

    // Place the items there
    linear_maze[locations[1]]->whats_here = maze_item::spellbook;
    linear_maze[locations[2]]->whats_here = maze_item::potion;
    linear_maze[locations[3]]->whats_here = maze_item::wand;

    return std::make_pair(std::move(maze), linear_maze[locations[0]]);
}

// Converts a text representation of a maze into a maze objects
//
// Please don't make any changes to this function - we'll be using our
// reference version when testing your code, and it would be a shame if
// the maze you solved wasn't the maze we wanted you to solve!
maze to_maze(std::vector<std::string> const& text_maze)
{
    validate_maze(text_maze);

    int num_rows = 1 + text_maze.size() / 2;
    int num_columns = 1 + text_maze[0].size() / 2;

    auto result = blank_maze(num_rows, num_columns);
    fill_items(result, text_maze);
    add_walls(result, text_maze);

    return result;
}
