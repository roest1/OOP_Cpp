
#include <algorithm>
#include <iostream>
#include <iterator>
#include <list>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>


#include "catch.hpp"

// Output the elements of the given list as a comma
// separated sequence.
//
//  os:     stream to use to generate the output
//  l:      list that should be used to generate the output from
template <typename T>
void print_list(std::ostream& os, std::list<T> const& l)
{
    bool first = true;
    for (auto it = l.begin(); it != l.end(); ++it)
    {
        if (!first)
            os << ", ";
        os << *it;
        first = false;
    }
}

PROVIDED_TEST("print_list utility tests")
{
    std::stringstream stm;

    std::list<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    print_list(stm, l);

    CHECK(stm.str() == "1, 2, 3");
}

// Create a list filled with the elements from the argument
//
//  values:     use the items stored in this argument to
//              initialize the elements of the returned list
template <typename T>
std::list<T> create_list(std::vector<T> const& values)
{
    std::list<T> result;
    for (const T& value : values) {
        result.push_back(value);
    }
    return result;
}

PROVIDED_TEST("create_list utility tests")
{
    std::stringstream stm;

    std::list<int> l = create_list(std::vector<int>{1, 2, 3});
    print_list(stm, l);

    CHECK(stm.str() == "1, 2, 3");
}

STUDENT_TEST("more create_list utility tests")
{
    // TODO: add at least two more tests verifying the create_list
    //       function works properly
    std::vector<int> emptyVector;
    std::list<int> resultList = create_list(emptyVector);
    CHECK(resultList.empty());

    std::vector<int> singleElementVector = {42};
    resultList = create_list(singleElementVector);
    CHECK(resultList.size() == 1);
    CHECK(resultList.front() == 42);

    std::vector<int> multiElementVector = {1, 3, 5, 7, 9};
    resultList = create_list(multiElementVector);
    CHECK(resultList.size() == multiElementVector.size());
    auto listIter = resultList.begin();
    for (int element : multiElementVector) {
        CHECK(*listIter == element);
        ++listIter;
    }
}

// Compare the given list and vector arguments and return whether both
// hold the same elements in the same order
//
//  l:          list to compare with the values
//  values:     values to compare with the list elements
template <typename T>
bool are_equivalent(std::list<T> const& l, std::vector<T> const& values)
{
    if (l.size() != values.size()) {
        return false;
    }

    auto list = l.begin();
    auto vector = values.begin();

    while (list != l.end() && vector != values.end()) {
        if (*list != *vector) {
            return false;
        }
        ++list;
        ++vector;
    }

    return true;
}

PROVIDED_TEST("are_equivalent utility tests")
{
    std::vector<int> v = {1, 2, 3};
    CHECK(are_equivalent(create_list(v), v));
}

STUDENT_TEST("more are_equivalent utility tests")
{
    // TODO: add at least two more tests verifying the are_equivalent
    //       function works properly
    std::list<int> emptyList;
    std::vector<int> emptyVector;
    CHECK(are_equivalent(emptyList, emptyVector));

    std::list<int> listElements = {1, 2, 3};
    std::vector<int> vectorElements = {3, 2, 1};
    CHECK(!are_equivalent(listElements, vectorElements));

    vectorElements = {1, 2, 3};
    CHECK(are_equivalent(listElements, vectorElements));
}

// Split given list 'l' into three sub-lists: each holding elements
// that are strictly less, equal, and greater than the 'pivot' value,
// respectively.
template <typename T>
std::tuple<std::list<T>, std::list<T>, std::list<T>> partition(
    std::list<T>& l, T const& pivot)
{
    // Create the tuple as a whole and use structured bindings to get
    // access to the elements of the tuple by reference.
    std::tuple<std::list<T>, std::list<T>, std::list<T>> result;
    auto& [less, equal, greater] = result;

    auto it = l.begin();
    while (it != l.end()) {
        // Since the iterator gets invalidated after splice, we use a next iterator.
        auto next_it = std::next(it);
        
        if (*it < pivot) {
            less.splice(less.end(), l, it);
        } else if (*it == pivot) {
            equal.splice(equal.end(), l, it);
        } else {
            greater.splice(greater.end(), l, it);
        }
        
        it = next_it;
    }

    // return tuple of the three result lists
    return result;
}

PROVIDED_TEST("partition tests")
{
    std::list<int> l = create_list(std::vector<int>{23, 3, 7, 4, 42, 13});
    auto [less, equal, greater] = partition(l, 23);

    CHECK(are_equivalent(less, {3, 7, 4, 13}));
    CHECK(are_equivalent(equal, {23}));
    CHECK(are_equivalent(greater, {42}));
}

STUDENT_TEST("more partition tests")
{
    std::list<int> l = create_list(std::vector<int>{1, 2, 3});
    auto [less, equal, greater] = partition(l, 4);

    CHECK(are_equivalent(less, {1, 2, 3}));
    CHECK(equal.empty());
    CHECK(greater.empty());

    l = create_list(std::vector<int>{5, 3, 7, 2, 5, 8});
    auto [lt, eq, gt] = partition(l, 5);

    CHECK(are_equivalent(lt, {3, 2}));
    CHECK(are_equivalent(eq, {5, 5}));
    CHECK(are_equivalent(gt, {7, 8}));

    l = create_list(std::vector<int>{1, 4, 6, 3});
    auto [ls, el, gr] = partition(l, 5);

    CHECK(are_equivalent(ls, {1, 4, 3}));
    CHECK(el.empty());
    CHECK(are_equivalent(gr, {6}));
}

// Transfer all elements of the three argument lists to the result
// list
template <typename T>
std::list<T> concatenate(
    std::list<T>& one, std::list<T>& two, std::list<T>& three)
{
    std::list<T> result;

    result.splice(result.end(), one);
    result.splice(result.end(), two);
    result.splice(result.end(), three);
    
    return result;
}

PROVIDED_TEST("concatenate tests")
{
    std::list<int> l1 = create_list(std::vector<int>{3, 4, 7, 13});
    std::list<int> l2 = create_list(std::vector<int>{23});
    std::list<int> l3 = create_list(std::vector<int>{42});
    CHECK(are_equivalent(concatenate(l1, l2, l3), {3, 4, 7, 13, 23, 42}));
}

STUDENT_TEST("more concatenate tests")
{
    // complete lists
    std::list<int> l1 = create_list(std::vector<int>{1, 2});
    std::list<int> l2 = create_list(std::vector<int>{3});
    std::list<int> l3 = create_list(std::vector<int>{4, 5});
    CHECK(are_equivalent(concatenate(l1, l2, l3), {1, 2, 3, 4, 5}));

    // some empty
    l1 = create_list(std::vector<int>{1, 2});
    std::list<int> empty2; 
    l3 = create_list(std::vector<int>{3, 4});
    CHECK(are_equivalent(concatenate(l1, empty2, l3), {1, 2, 3, 4}));

    // all empty 
    std::list<int> empty1; 
    
    std::list<int> empty3; 
    CHECK(are_equivalent(concatenate(empty1, empty2, empty3), {}));

}

// Use the quicksort algorithm to sort the given argument list. This
// function returns the sorted list
template <typename T>
std::list<T> quick_sort(std::list<T>& l)
{
    std::list<T> result;

    // if the list is empty or has only one element, it is already sorted
    if (l.size() <= 1) {
        return l;
    }

    T pivot = l.front();
    auto [less, equal, greater] = partition(l, pivot);

    less = quick_sort(less);
    greater = quick_sort(greater);

    return concatenate(less, equal, greater);
}

PROVIDED_TEST("quick_sort tests")
{
    std::list<int> l = create_list(std::vector<int>{23, 3, 7, 4, 42, 13});
    CHECK(are_equivalent(quick_sort(l), std::vector<int>{3, 4, 7, 13, 23, 42}));
}

STUDENT_TEST("more quick_sort tests")
{
    // empty list
    std::list<int> emptyList;
    CHECK(are_equivalent(quick_sort(emptyList), {}));

    // single element
    std::list<int> singleElementList = {42};
    CHECK(are_equivalent(quick_sort(singleElementList), {42}));

    // already sorted
    std::list<int> sortedList = {1, 2, 3, 4, 5};
    CHECK(are_equivalent(quick_sort(sortedList), {1, 2, 3, 4, 5}));

    // reverse sorted 
    std::list<int> reverseSortedList = {5, 4, 3, 2, 1};
    CHECK(are_equivalent(quick_sort(reverseSortedList), {1, 2, 3, 4, 5}));

    // has duplicates
    std::list<int> duplicatesList = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    CHECK(are_equivalent(quick_sort(duplicatesList), {1, 1, 2, 3, 3, 4, 5, 5, 5, 6, 9}));

    // all duplicates
    std::list<int> allDuplicatesList = {7, 7, 7, 7, 7};
    CHECK(are_equivalent(quick_sort(allDuplicatesList), {7, 7, 7, 7, 7}));

    // signed integers
    std::list<int> mixedList = {-3, 1, 4, -1, 0, -5, 2};
    CHECK(are_equivalent(quick_sort(mixedList), {-5, -3, -1, 0, 1, 2, 4}));

    // long list
    std::vector<int> longVector(10000);
    iota(longVector.begin(), longVector.end(), -5000); // Filling with values from -5000 to 4999
    std::shuffle(longVector.begin(), longVector.end(), std::mt19937(std::random_device()()));
    std::list<int> longList(longVector.begin(), longVector.end());

    longList = quick_sort(longList);
    std::sort(longVector.begin(), longVector.end());
    CHECK(are_equivalent(longList, longVector));

    int sizes[] = {1000, 2000, 4000, 8000};

    for (int size : sizes) {
        std::vector<int> testVector(size);
        iota(testVector.begin(), testVector.end(), 0); // Creating a sorted list
        std::shuffle(testVector.begin(), testVector.end(), std::mt19937(std::random_device()())); // Randomizing it

        BENCHMARK("QuickSort Random List Size " + std::to_string(size)) {
            std::list<int> testList(testVector.begin(), testVector.end());
            return quick_sort(testList);
        };

        // For worst-case scenario (already sorted list)
        std::sort(testVector.begin(), testVector.end()); 
        BENCHMARK("QuickSort Worst Case List Size " + std::to_string(size)) {
            std::list<int> testList(testVector.begin(), testVector.end());
            return quick_sort(testList);
        };
    }

    for (int size : sizes) {
        std::vector<int> testVector(size);
        iota(testVector.begin(), testVector.end(), 0); // Creating a sorted list
        std::shuffle(testVector.begin(), testVector.end(), std::mt19937(std::random_device()())); // Randomizing it

        BENCHMARK("QuickSort List Size " + std::to_string(size)) {
            std::list<int> testList(testVector.begin(), testVector.end());
            return quick_sort(testList);
        };

        BENCHMARK("std::sort() Vector Size " + std::to_string(size)) {
            std::vector<int> testVec = testVector; 
            return std::sort(testVec.begin(), testVec.end());
        };
    }
}

// Bonus run_sort()
template <typename T>
std::pair<std::list<T>, std::list<T>> split_run(std::list<T>& l) {
    std::list<T> run;
    if (l.empty()) {
        return {run, l};
    }

    run.splice(run.begin(), l, l.begin());
    auto it = l.begin();

    while (it != l.end()) {
        if (*run.rbegin() <= *it) {
            auto next_it = std::next(it); // Save next iterator before splice
            run.splice(run.end(), l, it);
            it = next_it;
        } else {
            break;
        }
    }

    return {run, l};
}


STUDENT_TEST("split_run on a list with multiple runs") {
    std::list<int> l = {3, 5, 5, 4, 10, 4};
    auto [run1, remainder1] = split_run(l);
    CHECK(are_equivalent(run1, {3, 5, 5}));
    CHECK(are_equivalent(remainder1, {4, 10, 4}));

    auto [run2, remainder2] = split_run(remainder1);
    CHECK(are_equivalent(run2, {4, 10}));
    CHECK(are_equivalent(remainder2, {4}));

    auto [run3, remainder3] = split_run(remainder2);
    CHECK(are_equivalent(run3, {4}));
    CHECK(remainder3.empty());
}

template <typename T>
std::list<T> merge(std::list<T>& one, std::list<T>& two) {
    std::list<T> result;
    auto it_one = one.begin();
    auto it_two = two.begin();

    while (it_one != one.end() && it_two != two.end()) {
        if (*it_one <= *it_two) {
            result.splice(result.end(), one, it_one++);
        } else {
            result.splice(result.end(), two, it_two++);
        }
    }

    result.splice(result.end(), one);
    result.splice(result.end(), two);
    return result;
}

STUDENT_TEST("merge two sorted lists") {
    std::list<int> l1 = {1, 3, 5};
    std::list<int> l2 = {2, 4, 6};
    CHECK(are_equivalent(merge(l1, l2), {1, 2, 3, 4, 5, 6}));
}


template <typename T>
std::list<T> run_sort(std::list<T>& l) {
    std::list<T> result;

    while (!l.empty()) {
        auto [run, remainder] = split_run(l);
        result = merge(result, run);
        l = std::move(remainder);
    }

    return result;
}

STUDENT_TEST("run_sort on various lists") {
    std::list<int> emptyList;
    CHECK(are_equivalent(run_sort(emptyList), {}));

    std::list<int> singleElementList = {42};
    CHECK(are_equivalent(run_sort(singleElementList), {42}));

    std::list<int> randomList = {5, 1, 4, 2, 8, 3, 7, 6};
    CHECK(are_equivalent(run_sort(randomList), {1, 2, 3, 4, 5, 6, 7, 8}));

    std::list<int> longList;
    for (int i = 0; i < 1000; ++i) longList.push_back(rand() % 1000);
    auto sortedLongList = run_sort(longList);
    CHECK(std::is_sorted(sortedLongList.begin(), sortedLongList.end()));
}




