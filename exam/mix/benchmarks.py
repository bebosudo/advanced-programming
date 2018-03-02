#!/usr/bin/env python3

import timeit
import random

import bestbst

random.seed(42)


def insert_once(tree):
    tree.insert(random.randint(-10000, 10000), 1234)


def test_iterate_linearly(self):
    last_seen = None

    for pair in self.tree:
        if not last_seen:
            last_seen = pair.key()

        else:
            self.assertGreater(pair.key(), last_seen)


if __name__ == "__main__":
    tree = bestbst.BTree()
    for _ in range(1000):
        insert_once(tree)

    time_before = timeit.timeit('tree.find(random.randint(-10000, 10000))', number=10000)
    tree.balance()
    time_after = timeit.timeit('tree.find(random.randint(-10000, 10000))', number=10000)

    print("Before balancing it took: {} seconds to find each number.".format(time_before))
    print("After balancing it took: {} seconds to find each number.".format(time_after))
