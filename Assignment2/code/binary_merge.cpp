
#include <queue>
#include <vector>

#include "catch.hpp"

bool queue_is_sorted(const std::queue<int>& q)
{
    std::queue<int> temp = q;
    std::vector<int> elements;
    while (!temp.empty()) {
        elements.push_back(temp.front());
        temp.pop();
    }
    return std::is_sorted(elements.begin(), elements.end());
}

std::queue<int> binary_merge(std::queue<int> one, std::queue<int> two)
{
    if (!queue_is_sorted(one) || !queue_is_sorted(two))
        throw std::runtime_error("input queues are not sorted");

    std::queue<int> result;

    while (!one.empty() && !two.empty()) {
        if (one.front() <= two.front()) {
            result.push(std::move(one.front())); // avoid copying elements using move()
            one.pop();
        } else {
            result.push(std::move(two.front()));
            two.pop();
        }
    }

    while (!one.empty()) {
        result.push(std::move(one.front()));
        one.pop();
    }
    while (!two.empty()) {
        result.push(std::move(two.front()));
        two.pop();
    }

    return result;
}

std::queue<int> fill_queue(std::vector<int> const& v)
{
    std::queue<int> result;
    for (int i : v)
        result.push(i);
    return result;
}


STUDENT_TEST("binary_merge")
{
    // Add a comprehensive set of tests here that verifies your implementation
    // of binary_merge operates as expected. Don't forget to test corner cases
    // (e.g. one or both arguments are empty queues, queues are not sorted,
    // etc.)
    
    // expect empty result when both queues are empty
    std::queue<int> emptyQueue;
    CHECK(binary_merge(emptyQueue, emptyQueue).empty());

    // Merge an empty queue with a non-empty queue, expect the non-empty queue
    std::queue<int> queue1, queue2;
    queue2.push(1);
    queue2.push(3);
    queue2.push(5);
    CHECK(binary_merge(emptyQueue, queue2) == queue2);
    CHECK(binary_merge(queue1, queue2) == queue2);

    // Merge two non-empty sorted queues
    std::queue<int> sortedQueue1, sortedQueue2;
    for (int i = 1; i <= 5; ++i) {
        sortedQueue1.push(i * 2);  // 2, 4, 6, 8, 10
        sortedQueue2.push(i * 2 - 1);  // 1, 3, 5, 7, 9
    }
    std::queue<int> mergedSortedQueue = binary_merge(sortedQueue1, sortedQueue2);
    for (int i = 1; i <= 10; ++i) {
        CHECK(mergedSortedQueue.front() == i);
        mergedSortedQueue.pop();
    }

    // Merge two non-empty unsorted queues
    std::queue<int> unsortedQueue1, unsortedQueue2;
    unsortedQueue1.push(4);
    unsortedQueue1.push(1);
    unsortedQueue1.push(3);
    unsortedQueue2.push(6);
    unsortedQueue2.push(2);
    unsortedQueue2.push(5);
    CHECK_THROWS(binary_merge(unsortedQueue1, unsortedQueue2));

    std::queue<int> q1 = fill_queue({0, 1, 2, 3, 4});
    std::queue<int> q2 = fill_queue({8, 6, 4, 2});
    CHECK_THROWS(binary_merge(q1, q2));

}

STUDENT_TEST("testing binary merge")
{
    for (int i = 10000; i/10000 <= 8; i *= 2)
    {
        std::queue<int> q1, q2;
        for (int j = 0; j < i; j++)
        {
            q1.push(j);
            q2.push(j + i);
        }
        BENCHMARK("Binary Merge for Size = " + std::to_string(i)) {
            return binary_merge(q1, q2);
        };
    }
    
}

std::queue<int> naive_multi_merge(std::vector<std::queue<int>> const& all)
{
    std::queue<int> result;
    for (std::queue<int> const& q : all)
    {
        result = binary_merge(q, result);
    }
    return result;
}

PROVIDED_TEST("naive_multi_merge")
{
    // check merging an empty list of queues
    CHECK(
        naive_multi_merge(std::vector<std::queue<int>>{}) == std::queue<int>{});

    // check merging three sorted queues
    std::queue<int> q1 = fill_queue({0, 1, 2, 3, 4});
    std::queue<int> q2 = fill_queue({5, 6, 7, 8});
    std::queue<int> q3 = fill_queue({2, 4, 6, 8});

    std::vector<std::queue<int>> sorted = {q1, q2, q3};
    std::queue<int> expected =
        fill_queue({0, 1, 2, 2, 3, 4, 4, 5, 6, 6, 7, 8, 8});

    CHECK(naive_multi_merge(sorted) == expected);

    // check merging unsorted queues
    std::queue<int> q4 = fill_queue({8, 6, 4, 2});
    std::vector<std::queue<int>> unsorted = {q1, q4};

    CHECK_THROWS(naive_multi_merge(unsorted));
}

STUDENT_TEST("more naive_multi_merge")
{
    // add 2-3 more tests verify the correctness of naive_multi_merge

    // merging a single sorted queue should return the same queue
    std::queue<int> q1 = fill_queue({0, 1, 2, 3, 4});
    std::vector<std::queue<int>> singleQueue = {q1};
    CHECK(naive_multi_merge(singleQueue) == q1);

    // merging two empty queues should return an empty queue
    std::vector<std::queue<int>> emptyQueues = {std::queue<int>{}, std::queue<int>{}};
    CHECK(naive_multi_merge(emptyQueues) == std::queue<int>{});

}

std::queue<int> generate_sorted_queue(int n)
{
    std::queue<int> q;
    for (int i = 0; i < n; ++i) {
        q.push(i);
    }
    return q;
}
STUDENT_TEST("Time naive_multi_merge")
{
    for (int n = 100; n <= 1000000; n *= 10) {
        std::queue<int> q1 = generate_sorted_queue(n);
        std::queue<int> q2 = generate_sorted_queue(n);

        BENCHMARK("k = 2, n = " + std::to_string(n * 2)) {
            return naive_multi_merge({q1, q2});
        };
    }

    // Varying k while keeping n fixed
    int n = 10000; 
    for (int k = 1; k <= 5; ++k) {
        std::vector<std::queue<int>> queues;
        for (int i = 0; i < k; ++i) {
            queues.push_back(generate_sorted_queue(n / k));
        }

        BENCHMARK("n = 10000, k = " + std::to_string(k)) {
            return naive_multi_merge(queues);
        };
    }
}

// Extract a contiguous range of elements from a given vector. Use this
// function to split the argument to recursive_multi_merge (the 
// std::vector<std::queue<int>>) into two parts in order to recursively
// merge all the queues.
template <typename T>
std::vector<T> extract_elements(
    std::vector<T> const& v,        // vector to extract a range from
    std::size_t first,              // index of first element to extract
    std::size_t length)             // length of the range to extract
{
    std::vector<T> result;
    if (first < v.size())
    {
        // copy as many elements as possible
        std::size_t num_elements_to_copy = 
            std::min(v.size(), first + length) - first;

        // make sure result vector has sufficient space
        result.resize(num_elements_to_copy);

        // copy the correct amount of elements to the result
        auto begin = std::next(v.begin(), first);
        std::copy(begin, begin + num_elements_to_copy, result.begin());
    }
    return result;
}

std::queue<int> recursive_multi_merge(std::vector<std::queue<int>> const& all)
{
    std::queue<int> result;

    if (all.empty()) {
        return std::queue<int>{};
    }

    if (all.size() == 1) {
        return all[0];
    }


    std::size_t size = all.size() / 2;
    std::vector<std::queue<int>> left = extract_elements(all, 0, size);
    std::vector<std::queue<int>> right = extract_elements(all, size, all.size() - size);


    std::queue<int> merged_left = recursive_multi_merge(left);
    std::queue<int> merged_right = recursive_multi_merge(right);

    result = binary_merge(merged_left, merged_right);

    return result;
}

STUDENT_TEST("recursive_multi_merge")
{
    // compare the result returned from naive_multi_merge with that returned
    // from recursive_multi_merge for at least 3 cases

    CHECK(naive_multi_merge(std::vector<std::queue<int>>{}) == recursive_multi_merge(std::vector<std::queue<int>>{}));
    
    // check merging three sorted queues
    std::queue<int> q1 = fill_queue({0, 1, 2, 3, 4});
    std::queue<int> q2 = fill_queue({5, 6, 7, 8});
    std::queue<int> q3 = fill_queue({2, 4, 6, 8});
    std::vector<std::queue<int>> sorted = {q1, q2, q3};
    CHECK(naive_multi_merge(sorted) == recursive_multi_merge(sorted));

    std::vector<std::queue<int>> singleQueue = {q1};
    CHECK(naive_multi_merge(singleQueue) == recursive_multi_merge(singleQueue));

    // merging two empty queues should return an empty queue
    std::vector<std::queue<int>> emptyQueues = {std::queue<int>{}, std::queue<int>{}};
    CHECK(naive_multi_merge(emptyQueues) == recursive_multi_merge(emptyQueues));

    // check merging unsorted queues
    std::queue<int> q4 = fill_queue({8, 6, 4, 2});
    std::vector<std::queue<int>> unsorted = {q1, q4};
    CHECK_THROWS(recursive_multi_merge(unsorted));

}

STUDENT_TEST("Time recursive_multi_merge")
{
    for (int n = 100; n <= 1000000; n *= 10) {
        std::queue<int> q1 = generate_sorted_queue(n);
        std::queue<int> q2 = generate_sorted_queue(n);

        BENCHMARK("k = 2, n = " + std::to_string(n * 2)) {
            return recursive_multi_merge({q1, q2});
        };
    }

    // Varying k while keeping n fixed
    int n = 10000; 
    for (int k = 1; k <= 16; k *= 2) {
        std::vector<std::queue<int>> queues;
        for (int i = 0; i < k; ++i) {
            queues.push_back(generate_sorted_queue(n / k));
        }

        BENCHMARK("n = 10000, k = " + std::to_string(k)) {
            return recursive_multi_merge(queues);
        };
    }
}
