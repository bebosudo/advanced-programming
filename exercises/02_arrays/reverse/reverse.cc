/*
  clang++ --std=c++11 -Wall reverse.cc -o ../../build/reverse && \
    ../../build/reverse
*/

#include <iostream>
#include <sstream>
#include <string>

class conversion_excp : public std::exception {
  virtual const char *what() const throw() {
    return "Error when converting from string.";
  }
} conv_excp;

template <typename T> T convert_from_string(std::string input_string) {
  T n;
  if (!(std::stringstream(input_string) >> n)) {
    throw conv_excp;
  }
  return n;
}

template <typename T> T *allocate(int number_of_elements) noexcept {
  return new T[number_of_elements];
}

template <typename T> void deallocate(T *element_to_deallocate) noexcept {
  return delete[] element_to_deallocate;
}

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    std::cout
        << "insert at least two numbers to be reversed on the command line\n";
    return -1;
  }

  int *input_elements = allocate<int>(argc - 1);

  for (int i = 1; i < argc; i++) {
    try {
      input_elements[i] = convert_from_string<int>(argv[i]);
    } catch (const std::exception &e) {
      std::cerr << e.what() << '\n';
      return -1;
    }
  }

  std::cout << "input   : ";
  for (int i = 1; i < argc; i++) {
    std::cout << "'" << input_elements[i] << "' ";
  }
  std::cout << std::endl;

  std::cout << "reversed: ";
  for (int i = 1; i < argc; i++) {
    std::cout << "'" << input_elements[argc - i] << "' ";
  }
  std::cout << std::endl;

  deallocate(input_elements);
  return 0;
}
