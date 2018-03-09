# Report assignment - Advanced Programming class 2017/2018

## Assignment overview

The assignment was divided into two main parts: C++ and Python.

In the C++ part it was required to implement a `Binary Search Tree` (_BST_ from now on), templated on the key and value, that are stored into each node, as well as a compare function that can be chosen among the standard library or being defined by the user.

In the Python part it was required to write a `class PostcardList` that can manage some postcards (characterized by three fields: `date`, `from` and `to`). The aim was to create some methods that could allow the fast retrieve of the data querying the created class.


## C++ section

We built the `BTree` class following the so called ["test-driven development"](https://en.wikipedia.org/wiki/Test-driven_development) approach: this means that for every method of the class that we are exposing, we have created some test cases to validate and test all the components (units) of the code. We used the C/C++ library [`doctest`](https://github.com/onqtam/doctest), "The fastest feature-rich C++98/C++11 single-header testing framework for unit tests and TDD", which is essentially a powerful collection of macros to define tests in a very easy and "fun" way (the initial setup time took around 15 minutes), and which improved the overall programming experience and time requested.

To install doctest, clone the repo and type:

    $ git submodule init
    $ git submodule update

The preceding commands setup and clone the submodules/sub-repository needed: both `doctest` and `pybind11`, which is needed in the `mix` section below.

This should create a directory structure like the following:

    $ tree -L 1 --dirsfirst -F
    .
    ├── doctest/
    ├── src/
    ├── tests/
    ├── Makefile
    └── readme.md


We have implemented all the required and optional methods described in the [`C++` assignment requirements](https://github.com/asartori86/advanced-programming/blob/master/exam/c++/readme.md).

We have structured the class `BTree` in order to not expose private components such as the Node objects to the outside. It encapsulates the class `Node` that actually is the building block of the whole BST.
We have separated the logic so as to reflect the separation among the two classes: all the methods that manage more nodes has been written into the class `BTree`, and the methods that simply operate on a single node have been implemented into the class `Node`.

To compile the code, move to the directory [`exam/c++/`](https://github.com/bebosudo/advanced-programming/blob/master/exam/c++/) and run a simple `make`: this compiles the tests provided into an executable `bin/btree.x`, using the options `-Wall -Wextra` and the `-DDEBUG` macro. When the program is executed, it tests more than 13 cases, with a total of around 200 assertions.


## Python section

In the file `exam_solution.py` it is possible to find the implementation of the class `PostcardList` requested from the assignment.

The class built passes all the tests written into the `exam_request.py` with success.

Moreover, we have also added some more unit tests to those already provided, to verify the implementation of some methods in a finer way.


## Mixing: C++ ♥ Python !

In order to mix C++ with Python we used [`pybind11`](https://github.com/pybind/pybind11/), a lightweight open-source framework, which made the integration process smooth, without the hassles of installing a gigantic framework such as Boost.

Python3 was chosen, since is the most developed and active version nowadays.

We suggest to use a virtualenv (the official [documentation](https://virtualenv.pypa.io/en/stable/installation/) is available); to install `pybind11` type:

    $ pip install pybind11


Moreover, the `pybind` headers are requested, and we installed them with the submodule update in the `C++ chapther` above.

By running a `make` in the directory `exam/mix/` you can build the `.so` library; which is available as a script in `exam/mix/benchmarks.py`: this show the difference before and after executing a re-balancing on the three, completely done in Python!

## Benchmarking

To highlight the advantages of having a _balanced_ _BST_, we have written a simple benchmark. In this benchmark we measure the time spent in carrying out a given operation (in this case 10K `find` calls) on the same randomly populated _BST_ before and after the `balance()` call.

All the code to make the comparison has been written in Python thanks to the binding we have created for our `BTree` class.

![benchmarking plot](https://github.com/bebosudo/advanced-programming/raw/master/exam/mix/benchmark.png "Find operation in a BST (balanced vs unbalanced)")

It is possible to note how much efficient is the balanced version in operating the lookups. In particular, increasing the size of the tree, this scales dramatically.

From the theorical point of view, due to the structure of the balanced _BST_, it is possible to demostrate that the cost for making a single lookup is in the worst case `log2(N)`, where `N` is the size of the tree.

#### Benchmark notes:

In the timings obtained together with the professors, we kept obtaining similar values of elapsed time in performing the same operation on the balanced and on the unbalanced tree, that behaviour was simply due to the fact that we were looking for random items. This means that the chance of having a number not present in the tree was very high and, probably, the search was interrupted already in the early nodes of the tree, leading to very short response time for both the structures.

Now the benchmark has been fixed and works properly.
