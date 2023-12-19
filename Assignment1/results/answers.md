# Assignment 1 - Short Answers

Please edit this file by filling in your answers to the questions as described in the assignment description. You submit your answers as part of the commit of the code you are going to produce as part of this assignment.

## Question 1

- Roughly how long did it take your computer to do the search when you kept an eye on the clock?
- How many perfect numbers were found and what were they?

**Answer 1**

- It took roughly 2 seconds to complete the search up to 4000. There were 4 perfect numbers found and they were: 6, 28, 496, and 8128

## Question 2

- Make a table of the timing results for `find_perfect_numbers` that you observed (old-school table of text rows and columns is just fine).

**Answer 2**

| Benchmark Name                              | (Samples, Mean, Std Dev)           | (Iterations, Low Mean, Low Std Dev) | (Estimated, High Mean, High Std Dev)       |
|--------------------------------------------|------------------------------------|------------------------------------|-------------------------------------------|
| Calculate perfect numbers up to 10000       | (10, 63.7198 ms, 199.253 us)      | (1, 63.6023 ms, 151.971 us)       | (637.48 ms, 63.8496 ms, 257.811 us)      |
| Calculate perfect numbers up to 20000       | (10, 255.058 ms, 2.28214 ms)        | (1, 253.802 ms, 1.60132 ms)       | (2.5415 s, 256.64 ms, 3.30472 ms)        |
| Calculate perfect numbers up to 40000       | (10, 1.01425 s, 11.4929 ms)       | (1, 1.00952 s, 3.81534 ms)        | (10.4278 s, 1.02616 s, 19.1148 ms)      |
| Calculate perfect numbers up to 80000       | (10, 4.05744 s, 32.6688 ms)        | (1, 4.03312 s, 20.5487 ms)        | (40.443 s, 4.0745 s, 45.1712 ms)        |


## Question 3

- Does it take the same amount of work to compute `is_perfect` on the number 10 as it does on the number 1000? Why or why not?
- Does it take the same amount of work for `find_perfect_numbers` to search the range of numbers from 1-1000 as it does to search the numbers from 1000-2000? Why or why not?


**Answer 3**

- ∵ Time Complexity(`is_perfect`) ∈ O(n), the algorithm will compute in time proportioinal to the size of input n. Thus, 1000 will take longer than 10. This means that `find_perfect_numbers` will take longer computing the range 1000-2000 than 1-1000. 

## Question 4

- Extrapolate from the data you gathered and make a prediction: how long will it take `find_perfect_numbers` to reach the fifth perfect number?

**Answer 4** 

Using 

$m :=$ Slope $= n ×  ∑_{i = 1}^n ( x_i y_i ) - \frac{∑_{i = 1}^n(x) × ∑_{i = 1}^n (y)}{n × ∑_{i = 1}^n (x^2) - (∑_{i = 1}^n (x))^2}$

Intercept $= \frac{∑_{i = 1}^n (y) - m × ∑_{i = 1}^n (x)}{n}$

where n is the number of data points, x is the input size to find perfect numbers up to, and y is the time it takes to compute.

$$predictedTime = m(33,000,000,000) + Intercept$$

$$≈ 1936 \text{ days}$$

## Question 5

- Do any of the tests still pass after you changed the `divisor_sum` function to initialize `total` to `1` instead of `0`? Why or why not?

**Answer 5**

No, the tests don't pass since the divisors are incorrectly calculated due to initializing the sum $≠ 0$. The tests for `is_perfect`  to test false for negative numbers still pass though. 


## Question 6

- Explain your testing strategy for `smarter_sum` and how you chose your specific test cases that lead you to be confident the function is working correctly.

**Answer 6**

We can stop iterating after divisor $> \sqrt{n}$ 

$\sqrt{n} \text{ | } n ⟹ total = total + divisor$

Otherwise, 

$total = total + divisor + \frac{n}{divisor}$

Since we can garauntee we're not double counting:

$∵ divisor > \sqrt{n} ⟹ \frac{n}{divisor} < \sqrt{n} ⟹ \frac{n}{divisor} \text{ is already counted}$

## Question 7

- Record your timing results for `find_perfects_numbers_faster` into a table.

**Answer 7**

| Benchmark Name                              | (Samples, Mean, Std Dev)           | (Iterations, Low Mean, Low Std Dev) | (Estimated, High Mean, High Std Dev)       |
|--------------------------------------------|------------------------------------|------------------------------------|-------------------------------------------|
| Calculate perfect numbers up to 10000       | (10, 1.2183 ms, 2.142 us)      | (1, 1.21694 ms, 1.57 us)       | (12.1713 ms, 1.2196 ms, 3.029 us)      |
| Calculate perfect numbers up to 20000       | (10, 3.18773 ms, 7.964 us)        | (1,  3.1835 ms, 5.007 us)   | (31.9642 ms, 3.19357 ms, 10.404 us)        |
| Calculate perfect numbers up to 40000       | (10, 8.46491 ms, 20.615 us)       | (1, 8.45538 ms, 9.903 us)        | (85.312 ms, 8.48304 ms, 33.03 us)      |
| Calculate perfect numbers up to 80000       | (10, 22.7776 ms,  77.082 us)        | (1, 22.7411 ms, 41.565 us)        | (229.077 ms, 22.8442 ms, 117.039 us)        |


## Question 8

- Make a prediction: how long will `find_perfects_numbers_faster` take to reach the fifth perfect number?

**Answer 8**

10 Days

## Question 9

- Explain how you chose your specific test cases and why they lead you to be confident `find_nth_perfect_euclid` is working correctly.

**Answer 9**


We know that the first 4 perfect primes are : 6, 28, 496, and 8128.
We also know that our function `is_perfect` is correct, so we can also use that for testing the first four and along with the fifth perfect number. 


## Question 10

- What is the Soundex code for "Angelou"? What is the code for your own surname?

**Answer 10**

Angelou = A524

Oest = O230


## Question 11

- Before writing any code, brainstorm your plan of attack and sketch how you might decompose the work into smaller tasks. Briefly describe your decomposition strategy.

**Answer 11**

1. Create a dictionary of the soundex codes for each letter
2. Create a function that takes in a string and returns the soundex code
3. Create a helper function that coalesces adjacent duplicate digits
4. Create a helper function that removes all zeros from the code
5. Create a helper function that pads the code with zeros to length 4 (or truncates)


## Question 12

- Once you have implemented your version of the Soundex algorithm, please suggest ways you think would improve the overall performance. Give us a short description here.

**Answer 12**

I could consider using multithreading. 
Or I could precompute all the soundcodes in the database.