#pragma once

#include <string>
#include <utility>
#include <vector>

#include "labyrinth.hpp"

// Given a graphical representation of a maze, returns a 'maze' instance
// containing the contents of that maze.
//
// For example, here's the input you'd give to represent the first example
// maze from the handout:
//
//         {"* *-W *",
//          "| |   |",
//          "*-* * *",
//          "  | | |",
//          "S *-*-*",
//          "|   | |",
//          "*-*-* P"}
//
// Here, stars indicate empty spots and the remaining letters indicate where
// the Wand, Spellbook, and Potion are. (Since we're handing the whole maze
// back to you, you can choose where you want to begin.) The | and - symbols
// are used to indicate which cells link to which other cells.
//
// Mazes can have any size and any number of copies of the items, even though
// a "real" maze would only have one of each item.
maze to_maze(std::vector<std::string> const& text_maze);

constexpr int NUM_ROWS = 4;
constexpr int NUM_COLUMNS = 4;

// Returns a maze of size NUM_ROWS x NUM_COLUMNS, specifically tailored to
// the given name.
//
// We've implemented this function for you. You don't need to write it
// yourself.
//
// Please don't make any changes to this function - we'll be using our
// reference version when testing your code, and it would be a shame if
// the maze you solved wasn't the maze we wanted you to solve!
std::pair<maze, maze_iterator> maze_for(std::string const& name);

constexpr int NUM_TWISTY_ROOMS = 12;

// Returns a Twisty Maze with the number of nodes given by
// NUM_TWISTY_ROOMS, specifically tailored to the given name.
//
// Please don't make any changes to this function - we'll be using our
// reference version when testing your code, and it would be a shame if
// the maze you solved wasn't the maze we wanted you to solve!
std::pair<maze, maze_iterator> twisty_maze_for(std::string const& name);
