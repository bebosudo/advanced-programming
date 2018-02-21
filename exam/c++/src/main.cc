#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

int factorial(int number) {
  int *p, n;
  n = 100;
  p = &n;
  printf("%d\n", n);
  printf("%d\n", *p);
  return number > 1 ? factorial(number - 1) * number : 1;
}

TEST_CASE("testing the factorial function") {
  CHECK(factorial(0) == 1);
  CHECK(factorial(1) == 1);
  CHECK(factorial(2) == 2);
  CHECK(factorial(3) == 6);
  CHECK(factorial(10) == 3628800);
}

// int main(int argc, char const *argv[]) { return 0; }
