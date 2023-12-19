#pragma once

#include <ostream>
#include <string>
#include <vector>

// Type representing an item in the maze
enum class maze_item
{
    nothing = 0,      // the cell is empty
    spellbook = 1,    // the cell holds the spellbook
    potion = 2,       // the cell holds the potion
    wand = 3          // the cell holds the wand
};

// Type representing a cell in the maze (forward declaration only).
struct maze_cell;

// The type that represents a maze. Internally all cells of the
// maze are stored in a std::vector<maze_cell>.
class maze
{
    std::vector<maze_cell> data;
    int rows, columns;

public:
    using iterator = std::vector<maze_cell>::iterator;
    using const_iterator = std::vector<maze_cell>::const_iterator;

    // Construct a new maze of size (rows, columns). No cells
    // are connected for newly created instances of a maze.
    maze(int rows, int columns);

    // return the number of rows in this maze
    int num_rows() const
    {
        return rows;
    }

    // return the number of columns in this maze
    int num_columns() const
    {
        return columns;
    }

    iterator iterator_for(maze_cell& cell);
    const_iterator iterator_for(maze_cell const& cell) const;

    // Access an iterator to the cell at the given position
    // (row, column).
    iterator operator()(int row, int column);
    const_iterator operator()(int row, int column) const;

    // Internal function to access one of the cell iterators
    iterator operator()(int index);
    const_iterator operator()(int index) const;

    auto& operator[](std::size_t index);
    auto const& operator[](std::size_t index) const;

    // Return an iterator to the cell in the upper-left corner
    // of the maze.
    auto begin();
    auto begin() const;

    // Return an iterator to the cell in the lower-right corner
    // of the maze.
    auto end();
    auto end() const;

    // Return the number of cells of this maze.
    auto size() const;
};

using maze_iterator = maze::iterator;
using const_maze_iterator = maze::const_iterator;

struct maze_cell
{
    maze_cell() = default;

    maze_cell(maze_item wh, maze_iterator n, maze_iterator s, maze_iterator e,
        maze_iterator w)
      : whats_here(wh)
      , north(n)
      , south(s)
      , east(e)
      , west(w)
    {
    }

    // Which item, if any, is present
    maze_item whats_here = maze_item::nothing;

    // Four iterators representing the connections of this cell
    // to its neighbors. The iterators compare equal to the end
    // iterator if no connection is available.
    maze_iterator north;
    maze_iterator south;
    maze_iterator east;
    maze_iterator west;
};

inline auto& maze::operator[](std::size_t index)
{
    return data[index];
}

inline auto const& maze::operator[](std::size_t index) const
{
    return data[index];
}

inline maze_iterator maze::iterator_for(maze_cell& cell)
{
    return data.begin() + (&cell - data.data());
}

inline const_maze_iterator maze::iterator_for(maze_cell const& cell) const
{
    return data.begin() + (&cell - data.data());
}

inline maze_iterator maze::operator()(int row, int column)
{
    return data.begin() + row * columns + column;
}
inline const_maze_iterator maze::operator()(int row, int column) const
{
    return data.begin() + row * columns + column;
}

inline maze_iterator maze::operator()(int index)
{
    return data.begin() + index;
}
inline const_maze_iterator maze::operator()(int index) const
{
    return data.begin() + index;
}

inline maze::maze(int rows, int columns)
  : data(rows * columns)
  , rows(rows)
  , columns(columns)
{
    for (auto& cell : data)
    {
        cell.whats_here = maze_item::nothing;
        cell.east = data.end();
        cell.west = data.end();
        cell.north = data.end();
        cell.south = data.end();
    }
}

// Return an iterator to the cell in the upper-left corner
// of the maze.
inline auto maze::begin()
{
    return data.begin();
}
inline auto maze::begin() const
{
    return data.begin();
}

// Return an iterator to the cell in the lower-right corner
// of the maze.
inline auto maze::end()
{
    return data.end();
}
inline auto maze::end() const
{
    return data.end();
}

// Return the number of cells of this maze.
inline auto maze::size() const
{
    return data.size();
}

// Given a location in a maze, returns whether the given sequence of
// steps will let you escape the maze.
//
// To escape the maze, you need to find the Potion, the Spellbook, and
// the Wand. You can only take steps in the four cardinal directions,
// and you can't move in directions that don't exist in the maze.
bool is_path_to_freedom(
    maze_iterator start, maze_iterator end, std::string const& moves);

// We provide this function for you so that, if you so choose, you can
// store Item variables in a std::unordered_map or std::unordered_set.
constexpr int hash_code(maze_item item)
{
    return static_cast<int>(item);
}

// This function lets you print out Item variables, which is useful for
// writing custom test cases.
std::ostream& operator<<(std::ostream&, maze_item item);
