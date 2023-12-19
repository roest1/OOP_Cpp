
#include <queue>
#include <stack>

#include "catch.hpp"

std::queue<int> reverse(std::queue<int> data)
{
    int num;

    std::stack<int> s;
    while (!data.empty())
    {
        num = data.front();
        data.pop();

        s.push(num);
    }

    std::queue<int> result;
    while (!s.empty())
    {
        result.push(s.top());
        s.pop();
    }

    return result;
}

PROVIDED_TEST("reversing a queue")
{
    std::queue<int> q;
    for (int i = 1; i < 6; ++i)
    {
        q.push(i);
    }

    std::queue<int> reversed_q = reverse(q);

    std::queue<int> expected_q;
    for (int i = 5; i > 0; --i)
    {
        expected_q.push(i);
    }
    CHECK(reversed_q == expected_q);
}

std::queue<int> duplicate_negatives(std::queue<int> data)
{
    std::queue<int> result;
    while (!data.empty())
    {
        int num = data.front();
        result.push(num);
        if (num < 0)
        {
            // duplicate negative number
            result.push(num);
        }
        // else
        // {
        //     data.pop();
        // }
        data.pop(); // always remove top element
    }
    return result;
}

PROVIDED_TEST("duplicating negative numbers in a queue")
{
    std::queue<int> q;
    for (int i = 1; i < 6; ++i)
    {
        q.push(i);
    }

    std::queue<int> duplicated_q = duplicate_negatives(q);

    CHECK(duplicated_q == q);
}

STUDENT_TEST("more duplicating negative numbers in a queue")
{
    // TODO: write a test that excercises the function
    //       `duplicate_negatives` by passing a queue containing
    //       nothing but a single negative number
    std::queue<int> q;
    q.push(-1);
    std::queue<int> duplicated_q = duplicate_negatives(q);
    std::queue<int> expected;
    expected.push(-1);
    expected.push(-1);
    CHECK(duplicated_q == expected);

    // 1. Negative number at start of queue
    q = {};
    q.push(-2);
    q.push(1);
    q.push(3);
    duplicated_q = duplicate_negatives(q);
    expected = {};
    expected.push(-2);
    expected.push(-2);
    expected.push(1);
    expected.push(3);
    CHECK(duplicated_q == expected);

    // // 2. Negative number at end of queue
    q = {}; 
    q.push(1);
    q.push(3);
    q.push(-2);
    duplicated_q = duplicate_negatives(q);
    expected = {};
    expected.push(1);
    expected.push(3);
    expected.push(-2);
    expected.push(-2);
    CHECK(duplicated_q == expected);

    // // 3. Negative number in middle of queue
    q = {}; // Clear the queue
    q.push(1);
    q.push(-2);
    q.push(3);
    duplicated_q = duplicate_negatives(q);
    expected = {};
    expected.push(1);
    expected.push(-2);
    expected.push(-2);
    expected.push(3);
    CHECK(duplicated_q == expected);
}

int sum_stack(std::stack<int> s)
{
    // int sum = 0;
    // do
    // {
    //     sum += s.top();
    //     s.pop();
    // } while (!s.empty());
    // return sum;
    int sum = 0;
    while (!s.empty())
    {
        sum += s.top();
        s.pop();
    }
    return sum;
}

PROVIDED_TEST("summing all integers in a stack")
{
    std::stack<int> s;
    for (int i = 1; i < 6; ++i)
    {
        s.push(i);
    }
    CHECK(sum_stack(s) == 15);
}

// test whether empty stack gives correct results
STUDENT_TEST("summing all integers in an empty stack")
{
    // TODO: uncomment the following line to trigger a bug
    //       in the function `sum_stack
    CHECK(sum_stack(std::stack<int>()) == 0);
}
