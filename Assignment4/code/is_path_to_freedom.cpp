
#include <string>
#include <set>
#include <stdexcept>
#include "labyrinth.hpp"

// This function checks whether the path given will be valid, i.e., 
// whether it helps you escaping the labyrinth as described in the
// instructions
//
//      start:  An iterator pointing to the maze_cell you should
//              start escaping from.
//      end:    An iterator you can use to compare against the
//              iterators held by a maze_cell. If your iterator
//              compares equal to this then there is no connection
//              to another cell along the given direction.
//      moves:  A sequence of characters 'N', 'S', 'E', and 'W'
//              describing the path out of the labyrinth to check
//              for validity.
//
// This function should return 'true' if the given path is valid
// and 'false' otherwise.
bool is_path_to_freedom(maze_iterator start, maze_iterator end, const std::string& moves) {
    std::set<maze_item> items_collected;

    if (start->whats_here != maze_item::nothing) {
        items_collected.insert(start->whats_here);
    }

    for (char move : moves) {
        
        if (move != 'N' && move != 'S' && move != 'E' && move != 'W') {
            throw std::runtime_error("Invalid character in path");
        }

        switch (move) {
            case 'N':
                if (start->north != end) start = start->north;
                else return false;
                break;
            case 'S':
                if (start->south != end) start = start->south;
                else return false;
                break;
            case 'E':
                if (start->east != end) start = start->east;
                else return false;
                break;
            case 'W':
                if (start->west != end) start = start->west;
                else return false;
                break;
        }
        if (start->whats_here != maze_item::nothing) {
            items_collected.insert(start->whats_here);
        }
    }
    return items_collected.size() == 3;
}
