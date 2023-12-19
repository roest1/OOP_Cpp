
#include <array>
#include <iomanip>
#include <string>
#include <sstream>      // for std::stringstream
#include <utility>      // for std::pair
#include <vector>

#include "catch.hpp"

// a simple matrix type used to represent the board
template <typename T>
class square_matrix
{
private:
    std::vector<T> data;
    int width;

public:
    square_matrix(int size, T init = T())
      : data(size * size, init)
      , width(size)
    {
    }

    T& operator()(int x, int y)
    {
        return data[x + y * size()];
    }
    T const& operator()(int x, int y) const
    {
        return data[x + y * size()];
    }

    int size() const
    {
        return width;
    }
};

// the list of coordinate offsets representing all possible
// jumps of a knight
constexpr std::array<std::pair<int, int>, 8> jumps = {
    std::pair{2, 1}, std::pair{1, 2}, std::pair{-1, 2}, std::pair{-2, 1}, 
    std::pair{-2, -1}, std::pair{-1, -2}, std::pair{1, -2}, std::pair{2, -1}};

// the board used to record the successful moves of a knight's tour
class board
{
private:
    square_matrix<int> h;

    // helper functions for printing the board
    void print_separator_line(std::ostream& os) const;
    void print_number_line(std::ostream& os, int line) const;

public:
    board(int size)
      : h(size, 0)
    {
    }

    int board_size() const
    {
        return h.size();
    }

    // record a successful move of the knight to
    // coordinate (x, y)
    void record_move(int x, int y, int i);

    // cancel an unsuccessful move of the knight to
    // coordinate (x, y)
    void cancel_move(int x, int y);

    // return true if the field with the given coordinate
    // was already visited before
    bool field_visited(int x, int y) const;

    // recursively try to fill the board starting at the
    // coordinate (x, y)
    bool try_next_move(int x, int y, int i);

    // print the content of the board to the given output
    // stream
    void print(std::ostream& os) const;
};

// record a successful move of the knight to
// coordinate (x, y)
void board::record_move(int x, int y, int i)
{
    h(x, y) = i;
}

// cancel an unsuccessful move of the knight to
// coordinate (x, y)
void board::cancel_move(int x, int y)
{
    h(x, y) = 0;
}

// return true if the field with the given coordinate
// was already visited before
bool board::field_visited(int x, int y) const
{
    return h(x, y) != 0;
}


std::pair<int, int> next_move_defined_by_rules_of_chess(
    int attempt, int x, int y)
{
    auto [dist_x, dist_y] = jumps[attempt];
    return std::pair{x + dist_x, y + dist_y};
}

// recursively try to fill the board starting at the
// coordinate (x, y), the argument 'i' represents the sequence
// number of the move (starting at '1')
bool board::try_next_move(int x, int y, int i)
{
    // initialize_selection_for_moves; 
    int current_candidate = 0; // assuming candidates are indexed starting from 0

    bool successful = false;
    do 
    {
        successful = false;
        auto [u, v] = next_move_defined_by_rules_of_chess(current_candidate, x, y); 
    
        if (0 <= u && u < board_size() && 0 <= v && v < board_size() && !field_visited(u, v)) 
        {
            record_move(u, v, i);
            if (i < board_size() * board_size()) 
            {
                successful = try_next_move(u, v, i + 1);
                if (!successful) 
                {
                    cancel_move(u, v);
                }
            } 
            else 
            {
                successful = true; 
            }
        }
        current_candidate++;
    } while(!successful && current_candidate < jumps.size()); // more_candidates_available

    return successful;
}


// Print a line to the given output stream that delimits the fields
// of a chess board.
// For a board size of four, the generated output looks something like:
//
//    +----+----+----+----+
//
void board::print_separator_line(std::ostream& os) const
{
    os << "+";
    for (int i = 0; i < board_size(); ++i)
        os << "----+";
    os << "\n";
}

// Print a line to the given output stream that represents the currently
// recorded moves on a chess board.
// For a board size of four, the generated output looks something like:
//
//    |  1 |  0 |  5 |  0 |
//
void board::print_number_line(std::ostream& os, int line) const
{
    os << "|";
    for (int i = 0; i < board_size(); ++i)
        os << " " << std::setw(2) << h(i, line) << " |";
    os << "\n";
}

// print the content of the board to the given output
// stream
void board::print(std::ostream& os) const
{
    // TODO: add your implementation here, use the following
    //       functions defined above: 
    //          - board::print_separator_line() 
    //          - board::print_number_line()
    os << "\n";
    for (int line = 0; line < board_size(); ++line) {
        print_separator_line(os);
        print_number_line(os, line);
    }
    print_separator_line(os);

}

// The function knights_tour will return true if given
// the size of the board and the starting coordinates
// (x, y) there exists a set of moves that uniquely
// fills all of the board.
bool knights_tour(int size, int x, int y)
{
    // Create a new object representing the board
    board b(size);

    // record the first position on our board
    b.record_move(x, y, 1);

    // Start traversing the board from the given starting
    // condition and the (next) sequence index of '2'.
    return b.try_next_move(x, y, 2);
}

// print the resulting board for a Knight's tour with the given
// parameters
std::string knights_tour_board(int size, int x, int y)
{
    std::stringstream strm;

    // Create a new object representing the board
    board b(size);

    // record the first position on our board
    b.record_move(x, y, 1);

    // Start traversing the board from the given starting
    // condition and the (next) sequence index of '2'.
    if (!b.try_next_move(x, y, 2))
    {
        b.cancel_move(x, y);
    }

    // generate a string representation of the board
    b.print(strm);

    return strm.str();
}

// Tests
PROVIDED_TEST("Possibility of a full Knight's tour")
{
    CHECK(knights_tour(5, 0, 0));
    CHECK(knights_tour(5, 2, 2));
    CHECK(!knights_tour(3, 0, 0));
}

STUDENT_TEST("More tests of possibility of a full Knight's tour")
{
    // TODO: add at least 3 more test cases here
    CHECK(!knights_tour(2, 0, 0));
    CHECK(!knights_tour(3, 1, 1));
    CHECK(!knights_tour(5, 1, 0));
}

char const* knights_tour_5_0_0 = R"(
+----+----+----+----+----+
|  1 | 14 | 19 |  8 | 25 |
+----+----+----+----+----+
|  6 |  9 |  2 | 13 | 18 |
+----+----+----+----+----+
| 15 | 20 |  7 | 24 |  3 |
+----+----+----+----+----+
| 10 |  5 | 22 | 17 | 12 |
+----+----+----+----+----+
| 21 | 16 | 11 |  4 | 23 |
+----+----+----+----+----+
)";

char const* knights_tour_5_2_2 = R"(
+----+----+----+----+----+
| 23 | 16 | 11 |  6 | 21 |
+----+----+----+----+----+
| 10 |  5 | 22 | 17 | 12 |
+----+----+----+----+----+
| 15 | 24 |  1 | 20 |  7 |
+----+----+----+----+----+
|  4 |  9 | 18 | 13 |  2 |
+----+----+----+----+----+
| 25 | 14 |  3 |  8 | 19 |
+----+----+----+----+----+
)";

char const* knights_tour_3_0_0 = R"(
+----+----+----+
|  0 |  0 |  0 |
+----+----+----+
|  0 |  0 |  0 |
+----+----+----+
|  0 |  0 |  0 |
+----+----+----+
)";

PROVIDED_TEST("Printing Knight's tour board")
{
    CHECK(knights_tour_board(5, 0, 0) == knights_tour_5_0_0);
    CHECK(knights_tour_board(5, 2, 2) == knights_tour_5_2_2);
    CHECK(knights_tour_board(3, 0, 0) == knights_tour_3_0_0);    
}

char const* knights_tour_2_0_0 = R"(
+----+----+
|  0 |  0 |
+----+----+
|  0 |  0 |
+----+----+
)";

char const* knights_tour_4_0_0 = R"(
+----+----+----+----+
|  0 |  0 |  0 |  0 |
+----+----+----+----+
|  0 |  0 |  0 |  0 |
+----+----+----+----+
|  0 |  0 |  0 |  0 |
+----+----+----+----+
|  0 |  0 |  0 |  0 |
+----+----+----+----+
)";

char const* knights_tour_5_1_1 = R"(
+----+----+----+----+----+
| 23 | 16 | 11 |  6 | 21 |
+----+----+----+----+----+
| 10 |  1 | 22 | 17 | 12 |
+----+----+----+----+----+
| 15 | 24 |  5 | 20 |  7 |
+----+----+----+----+----+
|  2 |  9 | 18 | 13 |  4 |
+----+----+----+----+----+
| 25 | 14 |  3 |  8 | 19 |
+----+----+----+----+----+
)";


STUDENT_TEST("More printing Knight's tour board")
{
    // TODO: add at least two additional test cases verifying
    //       the print functionality you created
    CHECK(knights_tour_board(2, 0, 0) == knights_tour_2_0_0);
    CHECK(knights_tour_board(4, 0, 0) == knights_tour_4_0_0);
    CHECK(knights_tour_board(5, 1, 1) == knights_tour_5_1_1);
}
