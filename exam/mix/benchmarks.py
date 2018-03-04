#!/usr/bin/env python3

#import timeit
import time
import random

import bestbst

random.seed(42)


def insert_once(tree):
    tree.insert(random.randint(-10000, 10000), 1234)

def execute_benchmark(tree, rep):
    start = time.process_time()
    for _ in range(rep):
        tree.find(random.randint(-10000, 10000))
    end = time.process_time()

    print("Size: ", len(tree))
    print("Height: ", tree.height())
    print("Time elapsed: ", end - start)
    

if __name__ == "__main__":
    tree = bestbst.BTree()
    for _ in range(100000):
        insert_once(tree)

    rep = 1000000
  
    print("Before balancing...") 
    execute_benchmark(tree, rep)
    
    print("\n===== Balancing =====\n")
    tree.balance()

    print("After balancing...")
    execute_benchmark(tree, rep)

