#include <cstddef>
#include <string>

#include "catch.hpp"

// This code is taken from: https://github.com/ReneNyffenegger.
// The code was distributed under the LICENSE:
// https://github.com/ReneNyffenegger/cpp-base64/blob/master/LICENSE

constexpr char const* base64_chars = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};

std::string base64_encode(std::string const& bytes_to_encode)
{
    std::size_t in_len = bytes_to_encode.size();
    std::size_t len_encoded = (in_len + 2) / 3 * 4;

    constexpr unsigned char trailing_char = '=';

    std::string ret;
    ret.reserve(len_encoded);

    unsigned int pos = 0;

    while (pos < in_len)
    {
        ret.push_back(base64_chars[(bytes_to_encode[pos + 0] & 0xfc) >> 2]);
        if (pos + 1 < in_len)
        {
            ret.push_back(
                base64_chars[((bytes_to_encode[pos + 0] & 0x03) << 4) +
                    ((bytes_to_encode[pos + 1] & 0xf0) >> 4)]);

            if (pos + 2 < in_len)
            {
                ret.push_back(
                    base64_chars[((bytes_to_encode[pos + 1] & 0x0f) << 2) +
                        ((bytes_to_encode[pos + 2] & 0xc0) >> 6)]);
                ret.push_back(base64_chars[bytes_to_encode[pos + 2] & 0x3f]);
            }
            else
            {
                ret.push_back(
                    base64_chars[(bytes_to_encode[pos + 1] & 0x0f) << 2]);
                ret.push_back(trailing_char);
            }
        }
        else
        {
            ret.push_back(base64_chars[(bytes_to_encode[pos + 0] & 0x03) << 4]);
            ret.push_back(trailing_char);
            ret.push_back(trailing_char);
        }
        pos += 3;
    }
    return ret;
}

STUDENT_TEST("Calculate the base64 encoding for your name")
{
    CHECK(base64_encode("Riley Oest") == "UmlsZXkgT2VzdA==");
}
