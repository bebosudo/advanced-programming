/*
clang++ --std=c++11 getters.cc -o ../../build/getters && ../../build/getters
*/

#include <iostream>
#include <limits>
#include <string>

template <typename num> int get_num() {
  num n;
  while (std::cout << "enter a float\n" && !(std::cin >> n)) {
    std::cin.clear();
    std::string line;
    std::getline(std::cin, line);
    std::cout << line << " is not a integer\n";
  }
  std::cout << "Inserted number " << n << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}

int get_int() {
  int n;
  while (std::cout << "enter an integer\n" && !(std::cin >> n)) {
    std::cin.clear();
    std::string line;
    std::getline(std::cin, line);
    std::cout << line << " is not a integer\n";
  }
  std::cout << "Inserted number " << n << std::endl;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}

int get_double() {
  double n;
  while (std::cout << "enter a double \n" && !(std::cin >> n)) {
    std::cin.clear();
    std::string line;
    std::getline(std::cin, line);
    std::cout << line << " is not a double\n";
  }
  std::cout << "Inserted number " << n << std::endl;
  std::cin.clear();
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return 0;
}

int main() {
  get_int();
  get_double();
  get_num<float>();
  return 0;
}
