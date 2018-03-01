# Report assignment - Advanced Programming class 2017/2018

## Assignment overview

The assignment was divided into two main parts: c++ and python.

In the c++ part it was required to implement a `Binary Search Tree` (BST from now on) able to be templated on the key and value stored into each node, as well as a compare function that can be chosen among the standard library or being user defined.

In the python part it was required to write a `class PostcardList` that can manage some postcards (characterized by three fields: `date`, `from` and `to`). The aim was to create some methods that could allow the fast retrieve of the data querying the created class.


## The work done - c++

We've built the `BTree` class following the so called test-driven programming, this means that for every method we're exposing from the class, we've created some test case to validate the code and ensure the proper functioning of the code.

We've implemented all the required and optional methods described in the [requirements](https://github.com/asartori86/advanced-programming/blob/master/exam/c++/readme.md).

We've structured the class `BTree` in order to be the only one accessible from the outside. It encapsulated the class `Node` that actually is the building block of the whole BST.
We've separated the logic in other to honor the separation among this two classes: all the methods that manage more nodes has been written into the class `BTree`, all the other methods that simply operate on a single node have been implemented into the class `Node`.


## The work done - python

In the file `exam_solution.py` it's possible to find the implementation of the class `PostcardList` requested from the assignment.

The class that has been built passes all the tests written into the `exam_request.py` with success.

We've also added some more checks to the base `unittest` in order to verify the implementation of some methods in a finer way.
