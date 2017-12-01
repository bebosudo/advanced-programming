/*
clang++ --std=c++11 getters_except.cc -o ../build/getters_except && \
      ../build/getters_except
*/

#include <iostream>
#include <limits>
#include <string>

struct notProperNumber {
  std::string what() const {
    return "Error: not a proper number passed to the function.";
  }
};

template <typename num>
int get_num() {
  num n;
  std::cout << "enter a number\n";

  if (!(std::cin >> n)) {
    throw notProperNumber{};
  }

  std::cout << "Inserted number " << n << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}

int main() {
  // get_int();
  // get_double();
  try {
    get_num<float>();
  } catch (const notProperNumber& e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}

// int get_int() {
//   int n;
//   while (std::cout << "enter an integer\n" && !(std::cin >> n)) {
//     std::cin.clear();
//     std::string line;
//     std::getline(std::cin, line);
//     std::cout << line << " is not a integer\n";
//   }
//   std::cout << "Inserted number " << n << std::endl;
//   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//   return 0;
// }
//
// int get_double() {
//   double n;
//   while (std::cout << "enter a double \n" && !(std::cin >> n)) {
//     std::cin.clear();
//     std::string line;
//     std::getline(std::cin, line);
//     std::cout << line << " is not a double\n";
//   }
//   std::cout << "Inserted number " << n << std::endl;
//   std::cin.clear();
//   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//   return 0;
// }
