// clang++ uniq_cc.cc -o ../../build/uniq_cc && ../../build/uniq_cc <text.txt

#include <iostream>
#include <string>

int main() {
  std::string last_line;
  std::string second_to_the_last = "";
  while (std::getline(std::cin, last_line)) {
    if (last_line != second_to_the_last) {
      std::cout << last_line << std::endl;
    }
    second_to_the_last = last_line;
  }
  return 0;
}
