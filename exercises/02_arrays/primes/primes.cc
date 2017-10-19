/*
  clang++ --std=c++11 -Wall primes.cc -o ../../build/primes && \
    ../../build/primes
*/

#include <iostream>
#include <math.h>
#include <string>

const short int MAX_PRIME = 100;

int main(int argc, char const *argv[]) {
  int primes_seen[MAX_PRIME / 2];
  int last_prime_seen_idx = 0;

  int prime_already_seen_idx = 0;

  for (int number_test_prime = 2; number_test_prime < MAX_PRIME;
       number_test_prime += 2) {

    std::cout << "number_test_prime = " << number_test_prime << std::endl;

    for (prime_already_seen_idx = 0;
         prime_already_seen_idx < last_prime_seen_idx;
         prime_already_seen_idx++) {

      std::cout << "prime_already_seen_idx = " << prime_already_seen_idx
                << std::endl;

      if (number_test_prime % primes_seen[prime_already_seen_idx] == 0) {
        primes_seen[last_prime_seen_idx] = number_test_prime;
      }
    }
  }

  std::cout << "ciaone" << std::endl;
  for (int i = 0; i < last_prime_seen_idx; i++) {
    std::cout << "-" << primes_seen[i] << std::endl;
  }

  return 0;
}
