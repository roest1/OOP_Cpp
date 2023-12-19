
#include <array>
#include <sstream>    // for std::stringstream
#include <string>
#include <utility>    // for std::pair

#include "catch.hpp"

class board
{
    // position of the queen in the ith column
    std::array<int, 8> positions;
    // no queen lies in jth row
    std::array<bool, 8> no_queen_in_row;
    // no queen occupies the up diagonal
    std::array<bool, 15> no_queen_in_up_diagonal;   
    // no queen occupies the down diagonal
    std::array<bool, 15> no_queen_in_down_diagonal; 


    bool valid_position(int column, int row) const;
    int solutions_found = 0; 

public:
    board();

    bool try_next_step(int queen, int nth_solution);
    bool try_next_step(int i);

    void place_queen(int column, int row);
    void remove_queen(int column, int row);

    std::array<int, 8> const& get_result() const;
};

board::board()
{
    std::fill(positions.begin(), positions.end(), -1);
    std::fill(no_queen_in_row.begin(), no_queen_in_row.end(), true);
    std::fill(no_queen_in_up_diagonal.begin(), no_queen_in_up_diagonal.end(), true);
    std::fill(no_queen_in_down_diagonal.begin(), no_queen_in_down_diagonal.end(), true);

}

void board::place_queen(int column, int row)
{
    positions[column] = row;
    no_queen_in_row[row] = false;
    no_queen_in_up_diagonal[column + row] = false;
    no_queen_in_down_diagonal[column - row + 7] = false;
}

void board::remove_queen(int column, int row)
{
    no_queen_in_row[row] = true;
    no_queen_in_up_diagonal[column + row] = true;
    no_queen_in_down_diagonal[column - row + 7] = true;
}

bool board::valid_position(int column, int row) const
{
    return no_queen_in_row[row] && 
           no_queen_in_up_diagonal[column + row] && 
           no_queen_in_down_diagonal[column - row + 7];
}

bool::board::try_next_step(int i)
{
    if (i == 8)
    {
        return true;
    }

    for (int j = 0; j < 8; ++j)
    {
        if (valid_position(i, j))
        {
            place_queen(i, j);
            
            if (try_next_step(i + 1))
            {
                return true; 
            }
            
            remove_queen(i, j);
        }
    }

    return false; 
}

// try next step in finding nth solution
bool board::try_next_step(int column, int nth_solution)
{
    if (column == 8)
    {
        ++solutions_found;
        if (solutions_found == nth_solution)
        {
            return true;  
        }
        return false; 
    }

    for (int row = 0; row < 8; ++row)
    {
        if (valid_position(column, row))
        {
            place_queen(column, row);
            
            if (try_next_step(column + 1, nth_solution))
            {
                return true;  
            }
            
            remove_queen(column, row);  
        }
    }

    return false;  
}

STUDENT_TEST("verify try_next_step")
{
    board b;
    CHECK(b.try_next_step(0));

}

// Returns an array of eight integers, each represents the columns a
// queen was placed in. 
// This array already exists as a member of our board, expose it through
// this. function
std::array<int, 8> const& board::get_result() const
{
    return positions;
}

// Find nth solution for the Eight Queens problem
std::pair<bool, std::array<int, 8>> eight_queens(int nth_solution = 1)
{
    board b;
    bool found_solution = b.try_next_step(0, nth_solution);
    return std::make_pair(found_solution, b.get_result());
}

STUDENT_TEST("Verify eight_queens function for first solution")
{
    auto [valid, result] = eight_queens(); 
    CHECK(valid);  
    CHECK(result == std::array<int, 8>{0, 4, 7, 5, 2, 6, 1, 3});  
}


// Print nth solution for the Eight Queens problem
// helper functions for printing the board
void print_separator_line(std::ostream& os)
{
    for (int i = 0; i < 8; ++i)
    {
        os << "+---";
    }
    os << "+\n"; 
}

void print_queens_line(std::ostream& os, int position)
{
    for (int i = 0; i < 8; ++i)
    {
        os << "| ";
        if (i == position)  
            os << "* "; // queen
        else
            os << "  ";     
    }
    os << "|\n"; 
}

std::string print_eight_queens(std::array<int, 8> const& positions)
{
    std::stringstream strm;

    strm << "\n";
    print_separator_line(strm);

    for (int queen = 0; queen < 8; ++queen)
    {
        print_queens_line(strm, positions[queen]);
        print_separator_line(strm);
    }

    return strm.str();
}

// Verifying solutions
PROVIDED_TEST("First two Eight Queens Solutions")
{
    {
        auto [valid, result] = eight_queens();
        CHECK(valid);
        CHECK(result == std::array<int, 8>{0, 4, 7, 5, 2, 6, 1, 3});
    }
    {
        auto [valid, result] = eight_queens(2);
        CHECK(valid);
        CHECK(result == std::array<int, 8>{0, 5, 7, 2, 6, 3, 1, 4});
    }
}

STUDENT_TEST("More Eight Queens solutions")
{
    
    {
        auto [valid, result] = eight_queens(3); 
        CHECK(valid);  
        CHECK(result == std::array<int, 8>{0, 6, 3, 5, 7, 1, 4, 2});  
    }
    {
        auto [valid, result] = eight_queens(4); 
        CHECK(valid);  
        CHECK(result == std::array<int, 8>{0, 6, 4, 7, 1, 3, 5, 2});  
    }
}

// Printing the board
char const* first_eight_queens_solution = R"(
+---+---+---+---+---+---+---+---+
| * |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   | * |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | * |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   | * |   |   |
+---+---+---+---+---+---+---+---+
|   |   | * |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   | * |   |
+---+---+---+---+---+---+---+---+
|   | * |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   | * |   |   |   |   |
+---+---+---+---+---+---+---+---+
)";

PROVIDED_TEST("Print First Eight Queens Solution")
{
    auto [valid, result] = eight_queens();
    CHECK(valid);
    CHECK(print_eight_queens(result) == first_eight_queens_solution);
}

char const* third_solution = R"(
+---+---+---+---+---+---+---+---+
| * |   |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   | * |   |
+---+---+---+---+---+---+---+---+
|   |   |   | * |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   | * |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   |   |   |   | * |
+---+---+---+---+---+---+---+---+
|   | * |   |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   |   |   | * |   |   |   |
+---+---+---+---+---+---+---+---+
|   |   | * |   |   |   |   |   |
+---+---+---+---+---+---+---+---+
)";

STUDENT_TEST("Printing more Eight Queens solutions")
{
    auto [valid, result] = eight_queens(3);
    CHECK(valid);
    CHECK(print_eight_queens(result) == third_solution);

}

int find_all_solutions()
{
    int nth_solution = 1;
    do {
        auto [valid, result] = eight_queens(nth_solution);
        if (!valid)
            break;

        ++nth_solution;
    }
    while (true);
    return nth_solution - 1;    // return number of found solutions
}

STUDENT_TEST("Verify number of found Eight Queens solutions")
{
    CHECK(find_all_solutions() == 92);
}
