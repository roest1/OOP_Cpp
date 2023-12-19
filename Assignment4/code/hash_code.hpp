// This file declares global hashing functions for various common data types.

#pragma once

#include <cstdint>
#include <string>
#include <utility>

// Returns a hash code for the specified key, which is always a
// nonnegative integer.  This function is overloaded to support
// all of the primitive types and the C++ std::string type.
int hash_code(bool key);
int hash_code(char key);
int hash_code(double key);
int hash_code(float key);
int hash_code(long double key);
int hash_code(int key);
int hash_code(unsigned int key);
int hash_code(long key);
int hash_code(unsigned long key);
int hash_code(short key);
int hash_code(unsigned short key);
int hash_code(char const* str);
int hash_code(std::string const& str);
int hash_code(void* key);
int hash_code(unsigned long long key);

// Constants that are used to help implement these functions
int hash_seed();          // Starting point for first cycle
int hash_multiplier();    // Multiplier for each cycle
int hash_mask();          // All 1 bits except the sign

// Computes a composite hash code from a list of multiple values.
// The components are scaled up so as to spread out the range of values
// and reduce collisions.
// The type of each value passed must have a suitable hash_code() function.
template <typename T1, typename T2, typename... Ts>
int hash_code(T1&& t1, T2&& t2, Ts&&... ts)
{
    int result = hash_seed();

    // Compute the hash code for the last n - 1 arguments.
    result += hash_code(std::forward<T2>(t2), std::forward<Ts>(ts)...);

    // Update the hash to factor in the hash of the first element
    result *= hash_multiplier();
    result += hash_code(std::forward<T1>(t1));

    // Hash the resulting integer to mask off any unneeded bits
    return hash_code(result);
}
