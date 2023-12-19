
#include <iostream>

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "labyrinth.hpp"
#include "maze_generator.hpp"

// Test Cases Below This Point

// Optional: Add your own custom tests here!
STUDENT_TEST("Student Test") {
    auto maze = to_maze({
        "S-*-*",
        "| | |",
        "*-W-P"
    });

    CHECK(is_path_to_freedom(maze(0, 0), maze.end(), "EESW"));
    CHECK(!is_path_to_freedom(maze(0, 0), maze.end(), "ES"));
    CHECK(!is_path_to_freedom(maze(0, 0), maze.end(), "EES"));
}

// Provided Tests Below This Point
PROVIDED_TEST("Checks paths in the sample maze.")
{
    // The textual representation of the maze uses the following
    // characters to describe corridors and cells:
    //
    //  corridors:
    //      '-' or '|': this is a path between cells
    //      ' ':        there is no connection between the cells
    //                  (it's a wall)
    //  cells:
    //      '*':        this cell is empty
    //      'W':        this cell has the wand
    //      'S':        this cell has the spellbook
    //      'P':        this cell has the potion
    //
    // clang-format off
    auto maze = to_maze({
    /*     0 1 2 3 */
    /*0*/ "* *-W *",  // the cell at position (0, 2) has the wand
          "| |   |", 
    /*1*/ "*-* * *", 
          "  | | |", 
    /*2*/ "S *-*-*",  // the cell at position (2, 0) has the spellbook
          "|   | |", 
    /*3*/ "*-*-* P)"  // the cell at position (3, 3) has the potion
    });
    // clang-format on

    // These paths are the ones in the handout. They all work.
    // 
    //      `maze(2, 2)` returns an iterator to the cell that should be
    //      used as the starting position, in this case position (2, 2).
    //
    //      maze.end() returns an iterator that can be used to compare
    //      with the iterators stored in every maze_cell. If an iterator
    //      compares equal to this there will be no path in the direction
    //      represented by this iterator.
    //
    CHECK(is_path_to_freedom(maze(2, 2), maze.end(), "ESNWWNNEWSSESWWN"));
    CHECK(is_path_to_freedom(maze(2, 2), maze.end(), "SWWNSEENWNNEWSSEES"));
    CHECK(is_path_to_freedom(maze(2, 2), maze.end(), "WNNEWSSESWWNSEENES"));

    // These paths don't work, since they don't pick up all items.
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "ESNW"));
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "SWWN"));
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "WNNE"));

    // These paths don't work, since they aren't legal paths.
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "WW"));
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "NN"));
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "EE"));
    CHECK(!is_path_to_freedom(maze(2, 2), maze.end(), "SS"));
}

PROVIDED_TEST("Can't walk through walls.")
{
    // clang-format off
    auto maze = to_maze({
        "* S *", 
        "     ", 
        "W * P", 
        "     ", 
        "* * *"
    });
    // clang-format on

    CHECK(!is_path_to_freedom(maze(1, 1), maze.end(), "WNEES"));
    CHECK(!is_path_to_freedom(maze(1, 1), maze.end(), "NWSEE"));
    CHECK(!is_path_to_freedom(maze(1, 1), maze.end(), "ENWWS"));
    CHECK(!is_path_to_freedom(maze(1, 1), maze.end(), "SWNNEES"));
}

PROVIDED_TEST("Works when starting (or ending) on an item.")
{
    auto maze = to_maze({"P-S-W"});

    CHECK(is_path_to_freedom(maze(0, 0), maze.end(), "EE"));
    CHECK(is_path_to_freedom(maze(0, 1), maze.end(), "WEE"));
    CHECK(is_path_to_freedom(maze(0, 2), maze.end(), "WW"));
}

// Printer for maze_items.
std::ostream& operator<<(std::ostream& out, maze_item item)
{
    if (item == maze_item::nothing)
    {
        return out << "maze_item::nothing";
    }
    if (item == maze_item::wand)
    {
        return out << "maze_item::wand";
    }
    if (item == maze_item::potion)
    {
        return out << "maze_item::potion";
    }
    if (item == maze_item::spellbook)
    {
        return out << "maze_item::spellbook";
    }
    return out << "<unknown item type>";
}
