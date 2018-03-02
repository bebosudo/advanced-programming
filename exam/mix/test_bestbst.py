#!/usr/bin/env python3
import unittest

import bestbst


class MixTest(unittest.TestCase):

    def setUp(self):
        self.tree = bestbst.BTree()

    def test_inserting(self):
        self.tree.insert(12, 1234)
        self.tree.insert(12, 1234)
        self.tree.insert(12, 1234)


if __name__ == "__main__":
    unittest.main()
