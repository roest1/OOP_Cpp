#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <string>

#include "ascii_image.hpp"
#include "catch.hpp"

// This function constructs the lines that constitute a Sierpinski
// triangle of the given order one by one as the strings stored
// in the vector `result`.
std::pair<int, image> draw_sierpinski_triangle(int order)
{
    if (order == 0)
    {
        return std::make_pair(1, image("*"));
    }

    // construct order n-1 triangle
    auto const& [count, previous] = draw_sierpinski_triangle(order - 1);

    // compose order-n triangle, use user defined type 'image' from
    // lecture 10
    image result;

    // 1. Take `previous`, prepend and append (horizontally concatenate)
    //    an empty image of size (1 << (order - 1), previous.height()).
    //    Use a gap of zero to avoid an additional space to be inserted
    //    in between.
    image empty((1 << (order - 1)), previous.height());
    image top = hcat(empty, previous, 0);
    top = hcat(top, empty,0);

    // 2. Take `previous` and horizontally concatenate it with itself
    //    with a space in between (gap == 1). 

    image bottom  = hcat(previous, previous, 1); 

    // 3. Vertically concatenate the results of step 1 and 2 with no gap
    //    in between.
    result = vcat(top, bottom, 0);

    return std::make_pair(3 * count, result);
}

char const* order_0_triangle = R"(
*
)";

PROVIDED_TEST("Order-0 Sierpinski triangle tests")
{
    auto [cnt, img] = draw_sierpinski_triangle(0);

    std::stringstream str;
    str << '\n' << img;    // allow for leading \n

    CHECK(cnt == 1);
    CHECK(str.str() == order_0_triangle);
}

char const* order_1_triangle = R"(
 * 
* *
)";

PROVIDED_TEST("Order-1 Sierpinski triangle tests")
{
    auto [cnt, img] = draw_sierpinski_triangle(1);

    std::stringstream str;
    str << '\n' << img;    // allow for leading \n

    CHECK(cnt == 3); // 3^1
    CHECK(str.str() == order_1_triangle);
}
char const* order_2_triangle = R"(
   *   
  * *  
 *   * 
* * * *
)";

STUDENT_TEST("Order-2 Sierpinski triangle tests")
{
    auto [cnt, img] = draw_sierpinski_triangle(2);

    std::stringstream str;
    str << '\n' << img;    // allow for leading \n

    CHECK(cnt == 9); // 3^2
    CHECK(str.str() == order_2_triangle);
}

char const* order_3_triangle = R"(
       *       
      * *      
     *   *     
    * * * *    
   *       *   
  * *     * *  
 *   *   *   * 
* * * * * * * *
)";
STUDENT_TEST("Order-3 Sierpinski triangle tests")
{
    auto [cnt, img] = draw_sierpinski_triangle(3);

    std::stringstream str;
    str << '\n' << img;    // allow for leading \n

    CHECK(cnt == 27); // 3^3
    CHECK(str.str() == order_3_triangle);
}
