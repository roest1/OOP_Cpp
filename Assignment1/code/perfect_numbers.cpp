#include <iostream>

// The divisor_sum function takes one argument `n` and calculates the sum of
// proper divisors of `n` excluding itself. To find divisors, a loop iterates
// over all numbers from 1 to n-1, testing for a zero remainder from the
// division using the modulus operator %.
long divisor_sum(long n)
{
    long total = 0;
    for (long divisor = 1; divisor < n; ++divisor)
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
void find_perfect_numbers(long stop)
{
    int num_perfect = 0;

    for (long num = 1; num < stop; num++)
    {
        if (is_perfect(num))
        {
            std::cout << "Found perfect number: " << num << "\n";
            num_perfect++;
        }
        if (num % 10000 == 0)
        {
           std::cout << "." << std::flush;    // just for formatting
        }
    }
    std::cout << "\nDone searching up to " << stop << "\n";
    std::cout << "Found " << num_perfect << " perfect numbers.\n";
}

int main()
{
    find_perfect_numbers(40000);
    return 0;
}