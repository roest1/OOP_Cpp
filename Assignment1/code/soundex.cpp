
#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "catch.hpp"

// converts char to std::string
std::string char_to_string(char c)
{
    return std::string(1, c);
}

// returns the first character of a given string as a char
char string_to_char(std::string const& s)
{
    return s.size() == 0 ? '\0' : s[0];
}

// Extract only the letters from the surname, discarding all
// non-letters (no dashes, spaces, apostrophes, ...).
std::string letters_only(std::string s)
{
    s.erase(std::remove_if(s.begin(), s.end(), [](char c) { return !std::isalpha(c); }), s.end()); // bonus for using standard algorithm

    return s;
}

PROVIDED_TEST("Test letters_only")
{
    CHECK(letters_only("a") == "a");
    CHECK(letters_only("abc") == "abc");
    CHECK(letters_only("abc12") == "abc");
    CHECK(letters_only("abc'12") == "abc");
}

STUDENT_TEST("Test more letters_only")
{
    // add test cases that unveil the bug in letters_only
    CHECK(letters_only("1a_") == "a");
    CHECK(letters_only("*a/b$c") == "abc");
    CHECK(letters_only("~ab/c1'2") == "abc");
    CHECK(letters_only("a`!,bc'12") == "abc");
}

std::unordered_map<char, int> soundexCode = {
    {'A', 0}, {'E', 0}, {'I', 0}, {'O', 0}, {'U', 0}, {'H', 0}, {'W', 0}, {'Y', 0},
    {'B', 1}, {'F', 1}, {'P', 1}, {'V', 1},
    {'C', 2}, {'G', 2}, {'J', 2}, {'K', 2}, {'Q', 2}, {'S', 2}, {'X', 2}, {'Z', 2},
    {'D', 3}, {'T', 3},
    {'L', 4},
    {'M', 5}, {'N', 5},
    {'R', 6}
};
// Encode each letter as a digit.
std::string encode(std::string const& s)
{
    std::string result;
    result.reserve(s.length()); // avoid allocating more than necessary
    std::transform(s.begin(), s.end(), std::back_inserter(result), [&](char c) {
        char upperC = std::toupper(c); // Convert to uppercase
        auto code = soundexCode.find(upperC);
        if (code != soundexCode.end()) {
            return static_cast<char>('0' + code->second); // Convert digit to string
        } 
        return c;
    });

    return result;
}

STUDENT_TEST("Test more encode")
{
    CHECK(encode(std::string("AEIOUHWY")) == "00000000");
    CHECK(encode(std::string("BFPV")) == "1111");
    CHECK(encode(std::string("CGJKQSXZ")) == "22222222");
    CHECK(encode(std::string("DT")) == "33");
    CHECK(encode(std::string("L")) == "4");
    CHECK(encode(std::string("MN")) == "55");
    CHECK(encode(std::string("R")) == "6");
}

// Coalesce adjacent duplicate digits
std::string coalesce(std::string const& s)
{
    std::string result;

    // remove all adjacent duplicate characters from the argument 's'
    // fill the 'result' string with the remaining characters
    std::unique_copy(s.begin(), s.end(), std::back_inserter(result));

    return result;
}

STUDENT_TEST("Test more coalesce")
{
    CHECK(coalesce("1111223") == "123");
    CHECK(coalesce("11228880880000023") == "12808023");
}

// Discard any zeros
std::string discard_zeros(std::string const& s)
{
    std::string result;

    // fill 'result' with all characters that are not '0'
    std::remove_copy_if(s.begin(), s.end(), std::back_inserter(result), [](char c) {
        return c == '0';
    });

    return result;
}

STUDENT_TEST("Test more discard_zeros")
{
    // add test cases for the function discard_zeros
    CHECK(discard_zeros("0000000") == "");
    CHECK(discard_zeros("1020304050") == "12345");
}

// Make the code exactly length 4
std::string truncate_or_pad(std::string const& s)
{
    // return a padded or a truncated version of s, depending on
    // its length (number of characters)
    std::string result = "";

    if (s.length() < 4) {
        result = s;
        result.append(4 - s.length(), '0'); // pad zeros
    } else if (s.length() > 4) {
        result = s.substr(0, 4); // truncate
    } else {
        return s;// already length 4
    }
    return result; 
}

STUDENT_TEST("Test more truncate_or_pad")
{
    // add test cases for the function truncate_or_pad

    CHECK(truncate_or_pad("123") == "1230");
    CHECK(truncate_or_pad("12345") == "1234");
    CHECK(truncate_or_pad("123456") == "1234");
}

// Calculate the Soundex code for the given name `s`
std::string soundex(std::string const& s)
{
    if (s.empty())
    {
        return s;    // return when input is empty
    }

    // 1. Extract only the letters
    std::string letters = letters_only(s);
    // 2.  Encode each letter as a digit
    std::string encoded = encode(letters);
    // 3. Coalesce adjacent duplicate digits
    std::string coalesced = coalesce(encoded);
    // 4. Replace first character, if there is one
    coalesced[0] = std::toupper(s[0]);
    // 5. Discard any zeros
    std::string no_zeros = discard_zeros(coalesced);
    // 6. Make the code exactly length 4
    return truncate_or_pad(no_zeros);
}

PROVIDED_TEST("Test soundex")
{
    CHECK(soundex("") == "");
    CHECK(soundex("Elenski") == "E452");
}

STUDENT_TEST("Test more soundex")
{
    CHECK(soundex("Angelou") == "A524");
    CHECK(soundex("Oest") == "O230");
}

// Read a list of names from the given file, one name per line. The function
// returns a std::vector containing the read names, one name per element.
std::vector<std::string> read_surnames_from_file(
    std::string const& filepath)    // the name of the file to read from
{
    std::ifstream strm(filepath);
    if (!strm.is_open())
    {
        throw std::runtime_error("could not open file: " + filepath);
    }

    std::vector<std::string> result;
    std::string name;
    while (strm >> name)
    {
        result.push_back(name);
    }
    return result;
}

std::vector<std::string> soundex_search(
    std::vector<std::string> const& names,    // list of all names
    std::string const& soundex_code)          // soundex code to find
{
    std::vector<std::string> matches;

    // fill in the code that iterates over all the names, calculates
    // the soundex code for each of them and stores all names that have
    // a matching soundex code in the result vector

    for (std::string const& name : names)
    {
        if (soundex(name) == soundex_code)
        {
            matches.push_back(name);
        }
    }

    return matches;
}

int main(int argc, char* argv[])
{
    // first run all tests (you may comment that out once all tests pass)
    int result = Catch::Session().run(argc, argv);

    /*
    // read file with names
    std::string filepath("../data/us_surnames.txt");
    std::vector<std::string> names = read_surnames_from_file(filepath);

    std::cout << "Read file " << filepath << ", " << names.size()
              << " names found.\n\n";

    while (true)
    {
        // prompt user to enter surname 
        std::cout << "Enter a surname (RETURN to quit): ";

        // read name from user, exit when error or when read empty string
        std::string name;
        if (!std::getline(std::cin, name) || name.empty())
        {
            break;
        }

        // calculate soundex code for the name read
        std::string soundex_code = soundex(name);
        std::cout << "Soundex code is " << soundex_code << '\n';

        // go through database and find all names that have the same
        // soundex code
        std::vector<std::string> matches = soundex_search(names, soundex_code);
        std::cout << "Matches from database: ";
        if (matches.empty())
        {
            std::cout << "<none>\n";
        }
        else
        {
            int count = 0;
            for (auto const& m : matches)
            {
                if (count >= 4)
                {
                    std::cout << "...";
                    break;
                }
                std::cout << m << ",";
                count++;
            }
            std::cout << '\n';
        }
    }

    std::cout << "All done!\n";

    */
   
    return result;
}
