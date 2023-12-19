
#include <stdexcept>

#include "catch.hpp"

// recursively calculating factorial
int factorial(int n)
{
    if (n < 0) {
        throw std::runtime_error("Factorial is only defined for n > 0");
    }
    // precondition: n >= 0
    if (n == 0)
    {
        return 1;
    }
    return n * factorial(n - 1);
}

PROVIDED_TEST("Recursive factorial calculation")
{
    CHECK(factorial(10) == 3628800);
}

STUDENT_TEST("More recursive factorial test cases")
{
    // add your test cases here
    CHECK_THROWS(factorial(-3));
}

// multiplying numbers, recursively and iteratively
long recursive_multiply(long term1, long term2)
{
    if (term1 == 1)
        return term2;
    return recursive_multiply(term1 - 1, term2) + term2;
}

bool odd(long n)
{
    return n & 1;
}
long half(long n)
{
    return n >> 1;
}

long mult_acc4(long r, long n, long a)
{
    while (true)
    {
        if (odd(n))
        {
            r = r + a;
            if (n == 1)
                return r;
        }
        n = half(n);
        a = a + a;
    }
}

long iterative_multiply(long term1, long term2)
{
    while (!odd(term1))
    {
        term2 = term2 + term2;
        term1 = half(term1);
    }

    if (term1 == 1)
        return term2;

    // even(term1 - 1) ==> term1 - 1 != 1
    return mult_acc4(term2, half(term1 - 1), term2 + term2);
}

PROVIDED_TEST("Test recursive multiply against iterative multiply")
{
    CHECK(recursive_multiply(7, 3) == iterative_multiply(7, 3));
}

PROVIDED_TEST(
    "Test recursive multiply against iterative multiply, generated inputs")
{
    for (int term1 = 1; term1 < 60; ++term1)
    {
        for (int term2 = 1; term2 < 10; ++term2)
        {
            CHECK(recursive_multiply(term1, term2) ==
                iterative_multiply(term1, term2));
        }
    }
}

// we define term1 as a global variable to avoid for the compiler to optimize
// away the measured code segments
long term1 = 1000;

PROVIDED_TEST("Benchmark multiply")
{
    BENCHMARK("Benchmark recursive multiply")
    {
        long result = 0;

        // repeat measurements
        for (long i = 1; i < 10000; ++i)
        {
            result += recursive_multiply(term1, 34698);
        }
        return result;
    };

    BENCHMARK("Benchmark iterative multiply")
    {
        long result = 0;

        // repeat measurements
        for (long i = 1; i < 10000; ++i)
        {
            result += iterative_multiply(term1, 34698);
        }
        return result;
    };
}
