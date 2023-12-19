
#include <string>

#include "catch.hpp"

std::string operators_from(std::string str)
{
    if (str.empty()) {
        return "";
    }

    char c = str[0];

    std::string result;

    if (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
        result += c;
    }

    std::string rest = operators_from(str.substr(1));

    return result + rest;
}

bool operators_are_matched(std::string ops)
{
    if (ops.empty()) {
        return true;
    }

    size_t found = ops.find("()");
    if (found != std::string::npos) {
        ops.erase(found, 2);
        return operators_are_matched(ops);
    }

    found = ops.find("[]");
    if (found != std::string::npos) {
        ops.erase(found, 2);
        return operators_are_matched(ops);
    }

    found = ops.find("{}");
    if (found != std::string::npos) {
        ops.erase(found, 2);
        return operators_are_matched(ops);
    }

    return false;
}

bool is_balanced(std::string str) 
{
    std::string ops = operators_from(str);
    return operators_are_matched(ops);
}

char const* operators_test1 =
    "int main() { int x = 2 * (vec[2] + 3); x = (1 + random()); }";
char const* operators_test2 = "( ( [ a ] )";

PROVIDED_TEST("Extract braces")
{
    CHECK(operators_from(operators_test1) == "(){([])(())}");
    CHECK(operators_from(operators_test2) == "(([])");
    CHECK(operators_from("") == "");
}

PROVIDED_TEST("Match braces")
{
    CHECK(operators_are_matched("(){([])(())}"));
    CHECK(!operators_are_matched("(([])"));
    CHECK(!operators_are_matched("abc"));
    CHECK(operators_are_matched(""));
}

PROVIDED_TEST("Is balanced")
{
    CHECK(is_balanced(operators_test1));
    CHECK(!is_balanced(operators_test2));
    CHECK(is_balanced("abc"));
    CHECK(is_balanced(""));
}

STUDENT_TEST("Testing Operators from")
{
    CHECK(operators_from("()") == "()");
    CHECK(operators_from("[]") == "[]");
    CHECK(operators_from("{}") == "{}");
    CHECK(operators_from("()[]{}") == "()[]{}");
}

STUDENT_TEST("Testing operators are matched")
{
    CHECK(operators_are_matched("()") == true);
    CHECK(operators_are_matched("[]") == true);
    CHECK(operators_are_matched("{}") == true);
    CHECK(operators_are_matched("()[]{}") == true);
}