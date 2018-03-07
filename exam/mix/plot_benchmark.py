#!/usr/bin/env python3

import time
import numpy as np
import matplotlib.pyplot as plt

import bestbst

def execute(tree, test_set):
    start = time.process_time()
    for i in test_set:
        tree.find(i)
    stop = time.process_time()

    return stop - start


if __name__ == "__main__":

    _n_tests = 1000
    _tree_size_max = 100000
    _seed = 314

    np.random.seed(_seed)

    tree_length = range(0, _tree_size_max, 100)
    time_unbalanced = []
    time_balanced = []

    for length in tree_length:

        test_set = np.random.rand(_n_tests) * length # To get keys that actually exist
        test_set = [int(x) for x in test_set]

        tree = bestbst.BTree()
        [tree.insert(x, 0) for x in range(length)]

        time_unbalanced.append(execute(tree, test_set))

        tree.balance()

        time_balanced.append(execute(tree, test_set))

    plt.plot(tree_length, time_unbalanced, label='unbalanced')
    plt.plot(tree_length, time_balanced, label='balanced')
    plt.xlabel('Size of the tree')
    plt.xlabel('Elapsed time ({} finds)'.format(_n_tests))
    plt.legend()

    plt.savefig('benchmark.png')

    with open('benchmark.dat', 'w') as f:
        for i in range(len(tree_length)):
            line = "{} {} {}".format(tree_length[i], time_unbalanced[i], time_balanced[i])
            f.write(line)
