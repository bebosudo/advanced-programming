/*
  clang++ --std=c++11 -Wall primes.cc -o ../../build/primes && \
    ../../build/primes
*/

#include <iostream>

const int NUM_PRIMES_TO_SEARCH = 1000000;

const int MAX_PRIMES = NUM_PRIMES_TO_SEARCH / 2;

int main(int argc, char const *argv[]) {
  int primes_seen[MAX_PRIMES] = {0};

  primes_seen[0] = 2;
  int last_prime_seen_idx = 0;

  int s = 0;

  for (int num_to_test = 3; num_to_test < NUM_PRIMES_TO_SEARCH;
       num_to_test += 2) {
    bool is_n_prime = true;
    // std::cout << "number to test prime = " << num_to_test << std::endl;

    for (s = 0; s < last_prime_seen_idx; ++s) {

      if (num_to_test % primes_seen[s] == 0) {
        is_n_prime = false;
        break;
      }
    }

    if (is_n_prime) {
      primes_seen[last_prime_seen_idx] = num_to_test;
      ++last_prime_seen_idx;
    }
  }

  // Print out the prime numbers up to NUM_PRIMES_TO_SEARCH.
  for (s = 0; s < last_prime_seen_idx; ++s)
    std::cout << primes_seen[s] << " ";

  std::cout << std::endl;
  return 0;
}
