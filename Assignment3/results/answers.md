# Assignment 3 - Short Answers

Please edit this file by filling in your answers to the questions as described in the assignment description. You submit your answers together with the commit of the code you are going to produce as part of this assignment.

## Question 1

The display of the `std::stack` in the debugger shows the elements that are stored in the queue or stack. How does the contents change when the Stack contains no elements, how does it display things once the stack holds at least one?

(Ans)

* When the stack is empty, the stack shows contents being {...} and size 'c' = 0. Once elements are added, 'c' takes the values of being pushed to the stack and size gets incremented. The stack shows the contents of the stack in the order of the last element pushed to the stack being the first element shown in the stack.

## Question 2

For which type of inputs does the function go into an infinite loop? Rather than identifying one specific input, describe the general characteristics of all such inputs.

(Ans)

duplicate_negatives() goes into infinite loop when passed any queue with a negative number. This is because the negative number is never removed from the queue and the function doesn't exit the while loop until the queue is empty. 


## Question 3

Show your edited code for `duplicate_negatives` that fixes the problem with the infinite loop.

```C++
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
        data.pop(); // always remove top element (edited)
    }
    return result;
}
```

## Question 4

Describe what you are seeing if an unexpected exception is being thrown on your system.

(Ans)

* The program stops due to a segmentation violation signal (SIGSEGV)

## Question 5

What is the better fix to `sum_stack` that corrects the bug?

(Ans)

* The better fix is to change the condition for the while loop to only execute when the stack is not empty. In other words, removing the do-while and replacing it with just while will prevent the loop-code from being executed on an empty stack (as passed by the argument). 

## Question 6

Explain why using `std::equal_range` over a sorted sequence will be more efficient than using `std::find` repeatedly in this use case.

(Ans)

* `std::equal_range` ∈ Ο(log(n)) (since it does binary search and only needs to find min or max which is at the beginning and the end in a sorted sequence)

* `std::find` ∈ Ο(n) (since it searches the entire sequence)


## Question 7

After you have written your test cases, describe your testing strategy to confirm that your validate_path operates as intended. How confident are you that it works correctly in all cases?


(Ans)

The solution in 7x9 was incorrect, so I corrected it to check my imlpementation for `validate_path()`. I manually tested the solution on the text files. The function should work as intended in all cases since it checks the base conditions:
* empty path
* if the starting position in path isn't (1, 1)
* if the ending position isn't the bottom right corner

Then it loops through the solution path and checks if each position is valid

## Question 8

While BFS and DFS are equivalently capable in terms of finding a solution, the two can differ in which solution is found (when there are multiple) and how quickly a solution is discovered. An advantage of BFS is that it always finds the shortest possible solution, explain why this is guaranteed for BFS and not for DFS. An advantage of DFS is that for some mazes it will find a solution more quickly than BFS, explain how this could be so.

(Ans)

* BFS explores the nearest neighboring nodes first and DFS explores the farthest path from the root node first, then it backtracks. Thus, BFS will always find the shortest path since it explores the nearest nodes until a solution is found (which will be the shortest). DFS has the potential to discover a solution in the deeper layers of the maze that might not be the shortest. Since BFS is exploring the nearest neighbors in each iteration and DFS explores a path, DFS has the potential to find the solution path more quickly. For example, DFS might be able to find a solution in its first depth search before having to backtrack. This would result in a faster execution than BFS, since BFS would have to explore all the neighboring nodes before moving on. 


## Question 9

Most students are sold on the benefit of the fancy features provided by `std::vector`, `grid`, and `std::set` but the standard data structures skeptics may question the value of [`std::stack`][stack_ref] and [`std::queue`][queue_ref] types, seeing them as nothing more than a restricted `std::vector`. Make your best argument to convince them of the benefits these container types have over `std::vector`.

(Ans)

Stacks and queues are optimized for certain operations like inserting and deleting and some queries. The advantage of using these is if it benefits to contain your data in LIFO (stack) or FIFO (queue) order. That way you can avoid having to search the entire vector, instead you can just access the top or front elements of a queue or stack in constant time. Another advantage of using a stack or queue over a vector is that you can't accidentally access an element in the middle of the sequence. This prevents accidentally modifying the data. Lastly, when using a vector, the system might allocate more memory than needed to optimize the support for random memory access which in turn allows for less need to rearrange elements dynamically. A stack or queue only allocates the amount of memory needed. 


UML:

knights_tour_activity.png
knights_tour_component.png

eight_queens_activity.png
eight_queens_component.png

maze_activity.png
maze_component.png
