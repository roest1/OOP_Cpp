# Assignment 2 - Short Answers

Please edit this file by filling in your answers to the questions as described in the assignment description. You submit your answers together with the commit of the code you are going to produce as part of this assignment.

## Question 1

Looking at a call stack listing in a debugger, what is the indication that the program being debugged uses recursion?

(Ans)

All the levels in the call stack indicate that the factorial() function is being successively called, which suggests that the factorial function uses recursion.

## Question 2

Estimate the maximum count of stack frames that fit in the capacity of the call stack before a stack overflow happens. How many stack frames fit in your system's call stack?

(Ans)

My debugger wouldn't show me the original call of factorial(-3), however at the level just before the bad memory access, n = -261408 and the bottom most level that I can expand n = -260409. So my debugger is only displaying 1000 frames. I can estimate that the maximum stack frames on my system is about $261405$ given the value of n just before illegal access and knowing that factorial(-3) is where n started. 

## Question 3

Describe how the symptoms of infinite recursion differ from the symptoms of an infinite loop.

(Ans)

In infinite recursion, function calls take up the stack memory and cause stack overflow exception. However, in an infinite loop, the stack memory doesn't increase and the program will run forever.

## Question 4

In place of selecting values over a defined range, an alternate approach would be to randomly select values for the two terms. Such a test case would test something different each time you ran it. This test approach is part of a larger approach known as "fuzz" testing. What do you see as possible benefit and downside of randomness being used in a test case?

(Ans)


A benefit of fuzz testing is that you can test a larger range of inputs that might not have been previously thought of, however the random values must fit the constrains of the function. A downside of fuzz testing is that it is difficult to reproduce the same test case, which makes it difficult to debug.

## Question 5

What was the number of iterations of `recursive_multiply` it did take on your system to amount to a quarter second? Copy/paste the time results of the two time trial sequences running on that number of iterations (for `recursive_multiply` and `iterative_multiply`).

(Ans)

Recursive multiply = 63.4307 ms

Iterative multiply = 254.171 us

## Question 6

Explain how the structure of `iterative_multiply` results in a Big-O runtime of O(logN) relative to the size of its `term1` argument.

(Ans)

The worst case for the algorithm is when term1 is a power of 2. To avoid unnecessary computation, we can do more work when n is even (ie: halving it and doubling term2 until term1 becomes odd). 

So the while loop:

```c
while (!odd(term1))
{
    term2 = term2 + term2;
    term1 = half(term1);
}
```

has logarithmic complexity : O(log(term1))

If term1 = 1 after this, then we are done. 

Otherwise, term1 is odd and greater than 1 and we can use mult_acc() to compute the final result. 

Since mult_acc() also uses a similar halving and doubling strategy, it also works in O(log(n)) time. 

### Question 7

Give a rough estimate of the maximum length sequence that could be successfully merged on your system assuming a recursive implementation of `binary_merge`.


(Ans)

Given that the maximum stack size I reached in the debugger was 261405, I would estimate that the maximum length sequence that could be successfully merged on my system is 261405, using a recursive implementation.

## Question 8

What would be the observed behavior if attempting to recursively merge a sequence larger than that maximum?

(Ans)

Stack overflow exception

### Qustion 9

Include the data from your execution timing and explain how it supports your Big-O prediction for `binary_merge`.

(Ans)

Prediction : O(n) = O(size(one) + size(two))

For size 10000 = 1.56 ms

For size 20000 = 3.14 ms

For size 40000 = 6.23 ms

For size 80000 = 12.52 ms

So the prediction was correct since we get doubled execution times when we double the input size.


### Question 10

Include the data from your execution timing and explain how it supports your Big-O prediction for `naive_multi_merge`.

(Ans)

Prediction : O(nk), where n is the size of the merged output and k is the number of sequences to merge.

k = 2, n = 200 : 61.52 us

k = 2, n = 2000 : 450.85 us

k = 2, n = 20000 : 4.55 ms

k = 2, n = 200000 : 44.29 ms

k = 2, n = 2000000 : 438.23 ms

n = 10000, k = 1 : 1.26 ms

n = 10000, k = 2 : 2.01 ms

n = 10000, k = 3 : 2.69 ms

n = 10000, k = 4 : 3.52 ms

n = 10000, k = 5 : 4.03 ms

Thus, the prediction was correct since both n and k are linearly related to the execution time.


### Question 11

Include the data from your execution timing and explain how it demonstrates $$O(n log(k))$$ runtime for `recursive_multi_merge`.

(Ans)

k = 2, n = 200 : 53.65 us

k = 2, n = 2000 : 382.07 us

k = 2, n = 20000 : 3.78 ms

k = 2, n = 200000 : 37.1 ms

k = 2, n = 2000000 : 370.2 ms

n = 10000, k = 1 : 127.19 us

n = 10000, k = 2 : 1.65 ms

n = 10000, k = 4 : 3.27 ms

n = 10000, k = 8 : 4.91 ms

n = 10000, k = 16 : 6.52 ms

Thus, n is a more significant factor than k in the execution time, which supports the prediction that the runtime is O(n log(k)).

### Question 12

You run `recursive_multi_merge` on a sequence of size 1 million and see that it completes just fine. Explain why this is not running afoul of the call stack capacity limitation. Hint: How many stack frames (levels) are expected to be on the call stack at the deepest point in the recursion in `recursive_multi_merge`?

(Ans)

At each level of recursion, the number of queues being merged is halved. Thus, the depth of the recursion tree is logarithmic in the number of input queues. This allows a greater sequence size.