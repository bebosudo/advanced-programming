# Report assignment - Advanced Programming class 2017/2018

## Assignment overview

The assignment was divided into two main parts: C++ and Python.

In the C++ part it was required to implement a `Binary Search Tree` (`BST` from now on), templated on the key and value, that are stored into each node, as well as a compare function that can be chosen among the standard library or being defined by the user.

In the Python part it was required to write a `class PostcardList` that can manage some postcards (characterized by three fields: `date`, `from` and `to`). The aim was to create some methods that could allow the fast retrieve of the data querying the created class.


## The work done: C++

We built the `BTree` class following the so called ["test-driven development"](https://en.wikipedia.org/wiki/Test-driven_development) approach: this means that for every method of the class that we are exposing, we have created some test cases to validate and test all the components (units) of the code. We used the C/C++ library [`doctest`](https://github.com/onqtam/doctest), "The fastest feature-rich C++98/C++11 single-header testing framework for unit tests and TDD", which is essentially a powerful collection of macros to define tests in a very easy and "fun" way (the initial setup time took around 15 minutes), and which improved the overall programming experience and time requested.

We have implemented all the required and optional methods described in the [requirements](https://github.com/asartori86/advanced-programming/blob/master/exam/c++/readme.md).

We have structured the class `BTree` in order to not expose private components such as the Node objects to the outside. It encapsulates the class `Node` that actually is the building block of the whole BST.
We have separated the logic so as to reflect the separation among the two classes: all the methods that manage more nodes has been written into the class `BTree`, and the methods that simply operate on a single node have been implemented into the class `Node`.

To compile the code, move to the folder [`exam/c++/`](https://github.com/bebosudo/advanced-programming/blob/master/exam/c++/) and run a simple `make`: this compiles the tests provided into an executable `bin/btree.x`, using the options `-Wall -Wextra` and the `-DDEBUG` macro. When the program is executed, it tests more than 13 cases, with a total of around 200 assertions.


## The work done: Python

In the file `exam_solution.py` it is possible to find the implementation of the class `PostcardList` requested from the assignment.

The class built passes all the tests written into the `exam_request.py` with success.

Moreover, we have also added some more unit tests to those already provided, to verify the implementation of some methods in a finer way.
