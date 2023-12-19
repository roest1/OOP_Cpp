
#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "labyrinth.hpp"
#include "maze_generator.hpp"

// Change this constant to contain your name.
//
// WARNING: Once you've set set this constant and started exploring your maze,
// do NOT edit the value of 'my_name'. Changing 'my_name' will change what
// maze you get back, which might invalidate all your hard work!
std::string const my_name = "Riley Oest";

// Change these constants to contain the paths out of your mazes.
std::string const the_path_out_of_my_maze =
    "SSSSEENNNNSSEENNEWSSWSSEESSWWWW";
    
std::string const the_path_out_of_my_twisty_maze =
    "SESNESESWESWENWSESWSWESESWSESWSENSEWNSEWNS";
    
PROVIDED_TEST("Escape from the labyrinth!")
{
    // A maze for you to escape from. This maze will be personalized
    // based on the constant my_name.
    //
    // Do not set a breakpoint on this line. Instead, set it at the
    // CHECK statement below.
    auto [maze, start_location] = maze_for(my_name);

    // This test case is for part two of the assignment.
    //
    // Set a breakpoint on the next line. As a reminder, the labyrinth you
    // get will be personalized to you, so don't start exploring the labyrinth
    // unless you've edited the constant 'my_name' to include your name(s)!
    // Otherwise, you'll be escaping the wrong labyrinth.
    //
    // `maze` is the labyrinth you'll escape from in part two of the assignment.
    // The maze will be in the shape of a regular grid, with pointers pointing
    // in the directions you CHECK them to point.
    // `start_location` is the position you start escaping from.
    CHECK(is_path_to_freedom(
        start_location, maze.end(), the_path_out_of_my_maze));
}

PROVIDED_TEST("Escape from the twisty labyrinth!")
{
    // Do not set a breakpoint here; set it at the CHECK statement below.
    auto [maze, start_location] = twisty_maze_for(my_name);

    // This test case is for part three of the assignment.
    //
    //      Do not start working on this section until you've read the
    //      instructions for part three of the assignment, the one that
    //      talks about twisty mazes (i.e., where the maze is not a
    //      regular grid of cells). The maze here is structured differently
    //      than the maze in part two of the assignment, and escaping it
    //      requires a different set of skills.
    //
    // `maze` is the labyrinth you'll escape from in part three of the assignment.
    // The maze is not necessarily a rectangular grid, and the iterators in each
    // maze cell might not point in the direction they're labeled as pointing.
    // However, you can rely on the fact that if one maze_cell links to a
    // second, the second (somehow) links directly back to the first.
    // `start_location` is the position you start escaping from.
    CHECK(is_path_to_freedom(
        start_location, maze.end(), the_path_out_of_my_twisty_maze));
}
