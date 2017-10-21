/*
  clang++ --std=c++11 -Wall pr_erath.cc -o ../../build/pr_erath && \
    ../../build/pr_erath
*/

#include <iostream>
#include <math.h>
#include <sstream>
#include <string.h>

int main(int argc, char const *argv[]) {
  if (argc != 2) {
    printf("Insert a number to search for primes up to it.\n");
    return -1;
  }

  long int n;
  if (!(std::stringstream(argv[1]) >> n) || n < 2) {
    printf("Insert an integer greater than 1.\n");
    return 1;
  }

  bool *A = new bool[n - 2];
  // Valid only when working on bytes.
  memset(A, true, sizeof(bool) * (n - 2));

  int n_sqrt = sqrt(n);
  for (int i = 2; i <= n_sqrt; ++i) {
    if (A[i - 2]) {
      for (int j = i * i; j < n; j += i) {
        A[j - 2] = false;
      }
    }
  }

  // Print out the prime numbers up to NUM_PRIMES_TO_SEARCH.
  for (int i = 2; i < n; ++i) {
    if (A[i - 2])
      std::cout << i << " ";
  }
  std::cout << std::endl;

  delete[] A;
  return 0;
}
