#include <fstream>
#include <iomanip>
#include <set>
#include <sstream>    // for std::stringstream
#include <stack>
#include <string>
#include <vector>
#include <queue>

#include "catch.hpp"

// a simple type representing a position in the grid
struct grid_position
{
    int row;
    int column;

    friend bool operator==(grid_position const& lhs, grid_position const& rhs)
    {
        return lhs.row == rhs.row && lhs.column == rhs.column;
    }

    friend bool operator!=(grid_position const& lhs, grid_position const& rhs)
    {
        return !(lhs == rhs);
    }

    friend bool operator<(grid_position const& lhs, grid_position const& rhs)
    {
        return lhs.row < rhs.row ||
            (lhs.row == rhs.row && lhs.column < rhs.column);
    }

    // a > b --> !(b < a) || a == b
    friend bool operator>(grid_position const& lhs, grid_position const& rhs)
    {
        return !(rhs < lhs) || lhs == rhs;
    }

    // a >= b --> !(b > a)
    friend bool operator>=(grid_position const& lhs, grid_position const& rhs)
    {
        return !(lhs < rhs);
    }

    // a <= b ==> !(b < a)
    friend bool operator<=(grid_position const& lhs, grid_position const& rhs)
    {
        return !(rhs < lhs);
    }

    // input/output helper functions for grid_position
    friend std::istream& operator>>(std::istream& strm, grid_position& pos)
    {
        char c1, c2;
        strm >> c1 >> pos.row >> c2 >> pos.column;
        if (c1 != 'r' || c2 != 'c')
        {
            throw std::runtime_error("unrecognized format for grid_position");
        }
        return strm;
    }

    friend std::ostream& operator<<(
        std::ostream& strm, grid_position const& pos)
    {
        strm << 'r' << pos.row << 'c' << pos.column;
        return strm;
    }
};

// a simple grid type used to represent the maze
template <typename T>
class grid
{
public:
    grid(int size_x, int size_y, T init = T())
      : data(size_x * size_y, init)
      , columns(size_x)
      , rows(size_y)
    {
    }

    decltype(auto) operator[](grid_position const& pos)
    {
        return data[pos.column + pos.row * num_columns()];
    }
    decltype(auto) operator[](grid_position const& pos) const
    {
        return data[pos.column + pos.row * num_columns()];
    }

    int num_columns() const
    {
        return columns;
    }
    int num_rows() const
    {
        return rows;
    }

private:
    std::vector<T> data;
    int columns;
    int rows;
};

// read a maze description from a file
grid<bool> read_maze_from_file(std::string const& filename)
{
    // open file and verify that it was found
    std::ifstream strm(filename);
    if (!strm.is_open())
    {
        throw std::runtime_error("can't open file: " + filename);
    }

    // first read first line of file to extract the sizes of the maze
    int rows = 0, columns = 0;
    strm >> rows >> columns;
    if (rows == 0 || columns == 0)
    {
        throw std::runtime_error("unrecognized file format: " + filename);
    }

    // now read the maze data character by character
    grid<bool> maze(columns, rows);
    char c;

    std::noskipws(strm);    // prevent stream from skipping whitespace
    strm >> c;              // skip '\n'

    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < columns; ++x)
        {
            strm >> c;
            maze[grid_position{y, x}] = (c == ' ') ? true : false;
        }
        strm >> c;    // skip '\n'
    }

    return maze;
}

PROVIDED_TEST("verify reading maze file")
{
    grid<bool> g = read_maze_from_file("../mazes/maze11x11.txt");
    CHECK(g.num_columns() == 11);
    CHECK(g.num_rows() == 11);

    // test two arbitrary grid elements
    CHECK(!g[grid_position{0, 0}]);
    CHECK(g[grid_position{1, 2}]);
}

// read a solution for a maze from a file
std::vector<grid_position> read_solution_from_file(std::string const& filename)
{
    std::ifstream strm(filename);
    if (!strm.is_open())
    {
        throw std::runtime_error("can't open file: " + filename);
    }

    int positions = 0;
    strm >> positions;
    if (positions == 0)
    {
        throw std::runtime_error("unrecognized file format: " + filename);
    }

    std::vector<grid_position> path;
    path.reserve(positions);

    while (--positions >= 0)
    {
        grid_position pos;
        if (!(strm >> pos))
        {
            throw std::runtime_error("unrecognized file format: " + filename);
        }
        path.push_back(pos);
    }

    return path;
}

PROVIDED_TEST("verify reading solutions file")
{
    std::vector<grid_position> path =
        read_solution_from_file("../mazes/maze11x11.solution");

    std::vector<grid_position> expected = {grid_position{1, 1},
        grid_position{1, 2}, grid_position{1, 3}, grid_position{1, 4},
        grid_position{1, 5}, grid_position{1, 6}, grid_position{1, 7},
        grid_position{1, 8}, grid_position{1, 9}, grid_position{2, 9},
        grid_position{3, 9}, grid_position{4, 9}, grid_position{5, 9},
        grid_position{6, 9}, grid_position{7, 9}, grid_position{8, 9},
        grid_position{9, 9}};

    CHECK(path == expected);
}

void print_maze_line_to_stream(
    std::ostream& strm, grid<bool> const& maze, int y)
{
    for (int x = 0; x < maze.num_columns(); ++x)
    {
        strm << (maze[grid_position{y, x}] ? " " : (x & 1 ? "-" : "+"));
    }
    strm << "\n";

    for (int x = 0; x < maze.num_columns(); ++x)
    {
        strm << (maze[grid_position{y + 1, x}] ? " " : "|");
    }
    strm << "\n";
}

void print_to_stream(std::ostream& strm, grid<bool> const& maze)
{
    for (int y = 0; y < maze.num_rows() - 1; y += 2)
    {
        print_maze_line_to_stream(strm, maze, y);
    }

    for (int x = 0; x < maze.num_columns(); ++x)
    {
        strm << (maze[grid_position{maze.num_rows() - 1, x}] ?
                " " :
                (x & 1 ? "-" : "+"));
    }
    strm << "\n";
}

char const* maze11x11 = R"(
+-+-+-+-+-+
|         |
+-+-+-+-+ +
|       | |
+ +-+-+ + +
| |   | | |
+ + + + + +
| | |   | |
+ +-+-+-+ +
|         |
+-+-+-+-+-+
)";

PROVIDED_TEST("verify maze printing")
{
    std::stringstream strm;
    strm << '\n';    // account for additional leading '\n' in maze11x11

    print_to_stream(strm, read_maze_from_file("../mazes/maze11x11.txt"));
    CHECK(maze11x11 == strm.str());
}

// Given a maze represented as a `grid<bool>` and a current `grid_position`
// cur, this function returns a `std::set` of all valid moves from `cur`.
std::set<grid_position> generate_valid_moves(
    grid<bool> const& maze, grid_position const& cur)
{
    std::set<grid_position> valid_moves;

    // (move up)
    if (cur.row > 0 && maze[grid_position{cur.row - 1, cur.column}])
    {
        valid_moves.insert(grid_position{cur.row - 1, cur.column});
    }

    // (move down)
    if (cur.row < maze.num_rows() - 1 && maze[grid_position{cur.row + 1, cur.column}])
    {
        valid_moves.insert(grid_position{cur.row + 1, cur.column});
    }

    // (move right)
    if (cur.column < maze.num_columns() - 1 && maze[grid_position{cur.row, cur.column + 1}])
    {
        valid_moves.insert(grid_position{cur.row, cur.column + 1});
    }

    // (move left)
    if (cur.column > 0 && maze[grid_position{cur.row, cur.column - 1}])
    {
        valid_moves.insert(grid_position{cur.row, cur.column - 1});
    }

    return valid_moves;
}

PROVIDED_TEST("verifying generating valid moves from given position")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze11x11.txt");
    std::set<grid_position> valid_moves =
        generate_valid_moves(maze, grid_position{1, 1});

    // we know there is just one possible move
    CHECK(valid_moves.size() == 1);

    // current position should not be part of valid moves
    CHECK(valid_moves.find(grid_position{1, 1}) == valid_moves.end());

    // we know that grid_position{1, 2} should be a valid move
    CHECK(valid_moves.find(grid_position{1, 2}) != valid_moves.end());
}

STUDENT_TEST("More comprehensive verification of generating valid moves")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze11x11.txt");

    std::set<grid_position> valid_moves = generate_valid_moves(maze, grid_position{9, 1});
    CHECK(valid_moves.size() == 2);

    valid_moves = generate_valid_moves(maze, grid_position{3, 1});
    CHECK(valid_moves.size() == 2);

    valid_moves = generate_valid_moves(maze, grid_position{9, 3});

    CHECK(valid_moves.size() == 2);
}

// Find the number of occurrences of a given grid_position contained in a
// given solution path
int count_known_positions(
    grid_position const& pos, std::vector<grid_position> const& p)
{
    std::vector<grid_position> sorted_p = p;
    std::sort(sorted_p.begin(), sorted_p.end());
    auto range = std::equal_range(sorted_p.begin(), sorted_p.end(), pos);
    return std::distance(range.first, range.second);
}

PROVIDED_TEST("testing of a given position comprises a valid next step")
{
    std::vector<grid_position> path =
        read_solution_from_file("../mazes/maze11x11.solution");

    CHECK(1 == count_known_positions(grid_position{1, 1}, path));
    CHECK(0 == count_known_positions(grid_position{0, 0}, path));
}

STUDENT_TEST("More testing of a given position comprises a valid next step")
{
    std::vector<grid_position> path =
        read_solution_from_file("../mazes/maze11x11.solution");

    CHECK(1 == count_known_positions(grid_position{1, 9}, path));
    CHECK(0 == count_known_positions(grid_position{9, 1}, path));
    CHECK(1 == count_known_positions(grid_position{9, 9}, path));

}

// The function `validate_path` completes successfully (returns `true`) if
// all of the criteria for a valid solution are met. If it instead detects
// that the path violates one of the constraints as outlined in the
// assignment description, `validate_path`  should return `false`.
bool validate_path(
    grid<bool> const& maze, std::vector<grid_position> const& path)
{

    // Check if path starts at entry or ends at exit or empty 
    if (path.empty() || path.front() != grid_position{1, 1} || path.back() != grid_position{maze.num_rows() - 2, maze.num_columns() - 2}) {
        return false;
    }

    for (size_t i = 0; i < path.size() - 1; ++i) {
        auto valid_moves = generate_valid_moves(maze, path[i]);

        // Check if next position in path is a valid move
        if (valid_moves.find(path[i + 1]) == valid_moves.end()) {
            return false;
        }

        // Check for loops in the path
        if (count_known_positions(path[i + 1], path) > 1) {
            return false;
        }
    }

    return true;
}

PROVIDED_TEST("verifying validity of given solution path")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze11x11.txt");
    std::vector<grid_position> path =
        read_solution_from_file("../mazes/maze11x11.solution");

    CHECK(validate_path(maze, path));
}

STUDENT_TEST("More comprehensive verification of path validity")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze7x9.txt");
    std::vector<grid_position> path =
        read_solution_from_file("../mazes/maze7x9.solution");

    CHECK(validate_path(maze, path));
    maze = read_maze_from_file("../mazes/maze23x23.txt");
    path = read_solution_from_file("../mazes/maze23x23.solution");
    CHECK(validate_path(maze, path));
    maze = read_maze_from_file("../mazes/maze43x43.txt");
    path = read_solution_from_file("../mazes/maze43x43.solution");
    CHECK(validate_path(maze, path));
}

// Solve the given maze by finding the shortest solution path for it.
// Use the breadth-first search algorithm.
std::vector<grid_position> solve_maze_bfs(grid<bool> const& maze)
{
    std::vector<grid_position> path;
    std::queue<std::vector<grid_position>> paths_queue;

    grid_position start{1, 1};
    paths_queue.push({start});
    grid_position exit{maze.num_rows() - 2, maze.num_columns() - 2};

    while (!paths_queue.empty())
    {
        path = paths_queue.front();
        paths_queue.pop();

        grid_position current = path.back();
        
        if (current == exit)
        {
            return path;
        }
        std::set<grid_position> neighbors = generate_valid_moves(maze, current);

        for (const grid_position& next : neighbors)
        {
            if (count_known_positions(next, path) == 0)
            {
                std::vector<grid_position> new_path = path;
                new_path.push_back(next);
                paths_queue.push(new_path);
            }
        }
    }

    return path;
}

PROVIDED_TEST("verifying solving maze using breadth-first search")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze11x11.txt");
    std::vector<grid_position> expected_path =
        read_solution_from_file("../mazes/maze11x11.solution");

    std::vector<grid_position> path = solve_maze_bfs(maze);

    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);
}

STUDENT_TEST("More verifying solving maze using breadth-first search")
{

    grid<bool> maze = read_maze_from_file("../mazes/maze7x9.txt");
    std::vector<grid_position> expected_path =
        read_solution_from_file("../mazes/maze7x9.solution");

    std::vector<grid_position> path = solve_maze_bfs(maze);

    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);

    maze = read_maze_from_file("../mazes/maze23x23.txt");
    expected_path = read_solution_from_file("../mazes/maze23x23.solution");
    path = solve_maze_bfs(maze);
    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);

    maze = read_maze_from_file("../mazes/maze43x43.txt");
    expected_path = read_solution_from_file("../mazes/maze43x43.solution");
    path = solve_maze_bfs(maze);
    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);
}

// Solve the given maze by finding a solution path for it. Use the
// depth-first search algorithm.
std::vector<grid_position> solve_maze_dfs(grid<bool> const& maze)
{
    std::vector<grid_position> path;

    std::stack<std::vector<grid_position>> paths_stack; 

    grid_position start{1, 1};
    paths_stack.push({start});
    grid_position exit{maze.num_rows() - 2, maze.num_columns() - 2};

    while (!paths_stack.empty())
    {
        path = paths_stack.top();
        paths_stack.pop();

        grid_position current = path.back();

        if (current == exit)
        {
            return path;
        }
        std::set<grid_position> neighbors = generate_valid_moves(maze, current);

        for (const grid_position& next : neighbors)
        {
            if (count_known_positions(next, path) == 0)
            {
                std::vector<grid_position> new_path = path;
                new_path.push_back(next);
                paths_stack.push(new_path);  
            }
        }
    }

    return path;
}

PROVIDED_TEST("verifying solving maze using depth-first search")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze11x11.txt");
    std::vector<grid_position> expected_path =
        read_solution_from_file("../mazes/maze11x11.solution");

    std::vector<grid_position> path = solve_maze_dfs(maze);

    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);
}

STUDENT_TEST("More verifying solving maze using depth-first search")
{
    grid<bool> maze = read_maze_from_file("../mazes/maze7x9.txt");
    std::vector<grid_position> expected_path =
        read_solution_from_file("../mazes/maze7x9.solution");

    std::vector<grid_position> path = solve_maze_dfs(maze);

    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);

    maze = read_maze_from_file("../mazes/maze23x23.txt");
    expected_path = read_solution_from_file("../mazes/maze23x23.solution");
    path = solve_maze_dfs(maze);
    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);

    maze = read_maze_from_file("../mazes/maze43x43.txt");
    expected_path = read_solution_from_file("../mazes/maze43x43.solution");
    path = solve_maze_dfs(maze);
    CHECK(validate_path(maze, path));
    CHECK(expected_path == path);

}

void print_maze_line_with_solution_to_stream(
    std::ostream& strm, 
    grid<bool> const& maze, 
    int y,
    std::vector<grid_position> const& path)
{
    for (int x = 0; x < maze.num_columns(); ++x)
    {
        if (count_known_positions(grid_position{y, x}, path) > 0)
            strm << "*";
        else
            strm << (maze[grid_position{y, x}] ? " " : (x & 1 ? "-" : "+"));
    }
    strm << "\n";

    for (int x = 0; x < maze.num_columns(); ++x)
    {
        if (count_known_positions(grid_position{y + 1, x}, path) > 0)
            strm << "*";
        else
            strm << (maze[grid_position{y + 1, x}] ? " " : "|");
    }
    strm << "\n";
}

void print_maze_with_solution_to_stream(
    std::ostream& strm, 
    grid<bool> const& maze, 
    std::vector<grid_position> const& path)
{
    for (int y = 0; y < maze.num_rows() - 1; y += 2)
    {
        print_maze_line_with_solution_to_stream(strm, maze, y, path);
    }

    for (int x = 0; x < maze.num_columns(); ++x)
    {
        if (count_known_positions(grid_position{maze.num_rows() - 1, x}, path) > 0)
            strm << "*";
        else
            strm << (maze[grid_position{maze.num_rows() - 1, x}] ? " " : (x & 1 ? "-" : "+"));
    }
    strm << "\n";
}

void write_solution_to_file(const std::vector<grid_position>& path, const std::string& filename) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
    }

    file << path.size() << "\n";

    for (const auto& pos : path) {
        file << "r" << pos.row << "c" << pos.column;
        if (&pos != &path.back()) {
            file << " ";
        }
    }

    file.close();
}

STUDENT_TEST("verify maze printing with solution")
{
    // 7x9
    grid<bool> maze = read_maze_from_file("../mazes/maze7x9.txt");
    std::vector<grid_position> path = read_solution_from_file("../mazes/maze7x9.solution");
    std::ofstream file("../mazes/maze7x9.solution.txt");
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: ../mazes/maze7x9.solution.txt");
    }
    print_maze_with_solution_to_stream(file, maze, path);
    file.close();

    // 11x11
    maze = read_maze_from_file("../mazes/maze11x11.txt");
    path = read_solution_from_file("../mazes/maze11x11.solution");
    file.open("../mazes/maze11x11.solution.txt");
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: ../mazes/maze11x11.solution.txt");
    }
    print_maze_with_solution_to_stream(file, maze, path);
    file.close();

    // 23x23
    maze = read_maze_from_file("../mazes/maze23x23.txt");
    std::vector<grid_position> solution = solve_maze_bfs(maze); 
    write_solution_to_file(solution, "../mazes/maze23x23.solution");
    path = read_solution_from_file("../mazes/maze23x23.solution");
    file.open("../mazes/maze23x23.solution.txt");
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: ../mazes/maze23x23.solution.txt");
    }
    print_maze_with_solution_to_stream(file, maze, path);
    file.close();

    // 43x43
    maze = read_maze_from_file("../mazes/maze43x43.txt");
    solution = solve_maze_bfs(maze);
    write_solution_to_file(solution, "../mazes/maze43x43.solution");
    path = read_solution_from_file("../mazes/maze43x43.solution");
    file.open("../mazes/maze43x43.solution.txt");
    if (!file.is_open())
    {
        throw std::runtime_error("Unable to open file: ../mazes/maze43x43.solution.txt");
    }
    print_maze_with_solution_to_stream(file, maze, path);
    file.close();
}
