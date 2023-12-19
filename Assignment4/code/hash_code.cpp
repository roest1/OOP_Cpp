// This file implements the interface declared in hash_code.h.

#include <cstddef>    // For size_t
#include <cstdint>    // For uintptr_t
#include <cstring>    // For strlen

#include "hash_code.hpp"

constexpr int HASH_SEED = 5381;        // Starting point for first cycle
constexpr int HASH_MULTIPLIER = 33;    // Multiplier for each cycle
constexpr int HASH_MASK = unsigned(-1) >> 1;    // All 1 bits except the sign

int hash_seed()
{
    return HASH_SEED;
}

int hash_multiplier()
{
    return HASH_MULTIPLIER;
}

int hash_mask()
{
    return HASH_MASK;
}

int hash_code(int key)
{
    return key & HASH_MASK;
}

int hash_code(bool key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(char key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(unsigned int key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(long key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(unsigned long key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(short key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(unsigned short key)
{
    return hash_code(static_cast<int>(key));
}

int hash_code(unsigned long long key)
{
    return hash_code(static_cast<int>(key));
}

// Catch-all handler for pointers not matched by other
// overloads just treats the pointer value numerically.
int hash_code(void* key)
{
    return hash_code(reinterpret_cast<std::size_t>(key));
}

// This function takes a string key and uses it to derive a hash code,
// which is a nonnegative integer related to the key by a deterministic
// function that distributes keys well across the space of integers.
// The general method is called linear congruence, which is also used
// in random-number generators.  The specific algorithm used here is
// called djb2 after the initials of its inventor, Daniel J. Bernstein,
// Professor of Mathematics at the University of Illinois at Chicago.
int hash_code(char const* base, std::size_t num_bytes)
{
    unsigned hash = HASH_SEED;
    for (std::size_t i = 0; i < num_bytes; ++i)
    {
        hash = HASH_MULTIPLIER * hash + base[i];
    }
    return hash_code(hash);
}

int hash_code(char const* str)
{
    return hash_code(str, strlen(str));
}

int hash_code(std::string const& str)
{
    return hash_code(str.data(), str.length());
}

int hash_code(double key)
{
    return hash_code(reinterpret_cast<char const*>(&key), sizeof(double));
}

int hash_code(float key)
{
    return hash_code(reinterpret_cast<char const*>(&key), sizeof(float));
}

int hash_code(long double key)
{
    return hash_code(reinterpret_cast<char const*>(&key), sizeof(long double));
}
