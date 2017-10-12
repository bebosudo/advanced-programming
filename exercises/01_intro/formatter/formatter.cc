/*
 clang++ --std=c++11 formatter.cc -o ../../build/formatter && \
    echo "This is a very very very loooooooooooooooooooooooooooong long long \
      long long sf fasdf fsdf asfsdfa fafasf asf afasf asdf asfasd fasdf asd \
      fas fasdf as fasd f string" |../../build/formatter
*/

#define MAX_LEN 80

#include <iostream>
#include <string>

int main() {
  std::string input_text;
  std::getline(std::cin, input_text);
  // std::cout << "Input text: \n" << input_text << std::endl;
  unsigned int len = input_text.length();

  unsigned int num_new_lines_to_enter = len / MAX_LEN;

  for (size_t i = num_new_lines_to_enter; i > 0; i--) {
    // Search for the first white element before i.
    size_t where_to_chunk = i * MAX_LEN;
    while (!std::isspace(input_text[where_to_chunk])) {
      where_to_chunk--;
    }
    // Substitute the space between the words with a single char newline.
    input_text[where_to_chunk] = '\n';
  }

  // std::cout << "\nFinal string:\n";
  std::cout << input_text << std::endl;

  return 0;
}
