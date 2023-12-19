/*
Hello, I'm Riley Oest!

I'm from Illinois and this is my 3rd-to-last semester here @ LSU.
I hope to land a job in AI tools of algorithmic trading, bioinformatics, or music.
One thing I learned from this part of the assignment is that Euclid's algorithm is 
so much more efficient than other traditional ones. 

*/
#include <cmath>
#include <iostream>

#include "catch.hpp"

using namespace std;




long faster_sum(long n) {
    long total = 1;

    for (long divisor = 2; divisor * divisor <= n; divisor++) // can start from first prime : 2
    {
        if (n % divisor == 0)
        {
            if (divisor * divisor != n) // divisor < sqrt(n) ⟹ add both divisors of n 
            {
                total += divisor + (n / divisor); 
        
            } else {
                total += divisor; // Exists(divisor > sqrt(n) that we've already counted)
            }
        }
    }
    return total;
}

int find_perfects_numbers_faster(long stop)
{
    int found = 0;
    for (long num = 1; num < stop; num++)
    {
        if (num != 1 && num == faster_sum(num))
        {
            ++found;
        }
    }
    return found;
}


long find_nth_perfect_euclid(long n) {
    long perfect = 0;
    long num_perfects = 0;
    long m = 0;
    bool isPrime = false;
    int k = 1;
    while(num_perfects != n) {
        m = (1 << k) - 1; // bit left shit = 2^k - 1
        isPrime = m != 1 && faster_sum(m) == 1; // 1 is not prime
        if (isPrime) {
            num_perfects++;
            perfect = (1 << (k - 1)) * m;
        }
        k++;
    }
    return perfect;
}


// The divisor_sum function takes one argument `n` and calculates the sum of
// proper divisors of `n` excluding itself. To find divisors, a loop iterates
// over all numbers from 1 to n-1, testing for a zero remainder from the
// division using the modulus operator %.
long divisor_sum(long n)
{
    long total = 0;

    for (long divisor = 1; divisor < n; divisor++)
    {
        if (n % divisor == 0)
        {
            total += divisor;
        }
    }
    return total;
}

// The is_perfect function takes one argument `n` and returns a boolean
// (true/false) value indicating whether or not `n` is perfect. A perfect number
// is a non-zero positive number whose sum of its proper divisors is equal to
// itself.
bool is_perfect(long n)
{
    return n != 0 && n == divisor_sum(n);
}

// The findPerfects function takes one argument `stop` and performs an
// exhaustive search for perfect numbers over the range 1 to `stop`. Each
// perfect number found is printed to the console.
int find_perfect_numbers(long stop)
{
    int found = 0;
    for (long num = 1; num < stop; num++)
    {
        if (is_perfect(num))
        {
            ++found;
        }
    }
    return found;
}

// Test cases
PROVIDED_TEST("count perfect numbers")
{
    CHECK(find_perfect_numbers(10) == 1);
    CHECK(find_perfect_numbers(100) == 2);
    CHECK(find_perfect_numbers(10000) == 4);
}

PROVIDED_TEST("Calculate perfect numbers")
{
    BENCHMARK("Calculate perfect numbers up to 40000")
    {
        return find_perfect_numbers(40000);
    };
}

STUDENT_TEST("Calculate my perfect numbers")
{
    int size = 10;
    for (int i = 0; i != 4; ++i)
    {
        BENCHMARK("Calculate perfect numbers up to " + std::to_string(size))
        {
            return find_perfect_numbers(size);
        };
        size = 2 * size;
    }
}

STUDENT_TEST("Single BENCHMARK of find_perfect_numbers()")
{
    int sizes [4] = {10000, 20000, 40000, 80000};

    for (int i = 0; i < 4; i++)
    {
        BENCHMARK("Calculate perfect numbers up to " + std::to_string(sizes[i]))
        {
            return find_perfect_numbers(sizes[i]);
        };
    }
}

STUDENT_TEST("Testing negative numbers for is_perfect()")
{
    CHECK(is_perfect(-6) == false);
    CHECK(is_perfect(-28) == false);
    CHECK(is_perfect(-496) == false);
    CHECK(is_perfect(-8128) == false);
}

STUDENT_TEST("Testing faster sum")
{
    CHECK(find_perfects_numbers_faster(10) == 1);
    CHECK(find_perfects_numbers_faster(100) == 2);
    CHECK(find_perfects_numbers_faster(10000) == 4);
    CHECK(find_perfects_numbers_faster(40000) == 4);
}

STUDENT_TEST("Single BENCHMARK of find_perfects_numbers_faster()")
{
    int sizes [4] = {10000, 20000, 40000, 80000};

    for (int i = 0; i < 4; i++)
    {
        BENCHMARK("Calculate perfect numbers up to " + std::to_string(sizes[i]))
        {
            return find_perfects_numbers_faster(sizes[i]);
        };
    }
}


STUDENT_TEST("Testing n'th perfect Euclid")
{
    CHECK(find_nth_perfect_euclid(1) == 6);
    CHECK(find_nth_perfect_euclid(2) == 28);
    CHECK(find_nth_perfect_euclid(3) == 496);
    CHECK(find_nth_perfect_euclid(4) == 8128);


    CHECK(is_perfect(find_nth_perfect_euclid(1)));
    CHECK(is_perfect(find_nth_perfect_euclid(2)));
    CHECK(is_perfect(find_nth_perfect_euclid(3)));
    CHECK(is_perfect(find_nth_perfect_euclid(4)));
    CHECK(is_perfect(find_nth_perfect_euclid(5)));
}
