#!/usr/bin/env python3

import time
import numpy as np

import bestbst

def execute():
    print("\nStarting on a tree of height", tree.height(), "...", end='')

    start = time.process_time()
    for i in test_set:
        tree.find(i)
    stop = time.process_time()

    print(stop - start, "s")


if __name__ == "__main__":

    _n_tests = 10000
    _tree_size = 100000
    _seed = 314

    np.random.seed(_seed)
    test_set = np.random.rand(_n_tests) * _tree_size # To get keys that actually exist
    test_set = [int(x) for x in test_set]

    print("====================================================")
    print("== Benchmarking Binary Search Tree implementation ==")
    print("== Tree size:", _tree_size, "      Number of tries:", _n_tests, "==")
    print("====================================================")

    print("Filling the tree...")

    tree = bestbst.BTree()
    [tree.insert(x, 0) for x in range(_tree_size)]

    execute()

    print("\nBalancing...")
    tree.balance()

    execute()
