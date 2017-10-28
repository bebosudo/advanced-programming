/*
  clang++ --std=c++11 -Wall -Wextra llist.cc -o ../build/llist && \
    ../build/llist
*/

#include "llist.h"

int main() {
  // To test the empty constructor.
  List<int> empty_l;
  empty_l.print();
  std::cout << "-------\n";

  // To test the constructor with one argument.
  List<int> l{42};
  l.print();
  l.push_front(13);
  l.print();
  l.push_back(334);
  l.print();

  return 0;
}

/* ---------------------------- llist.cpp ---------------------------- */
template <typename value_type>
List<value_type>::List(value_type value) : _size{1} {
  head = std::unique_ptr<node>(new node(value));
  tail = head.get();
}

template <typename value_type>
void List<value_type>::print() {
  if (!head) {
    std::cout << "[] (size " << size() << ")\n";
    return;
  }

  // Use a pointer to the node object to iterate over the elements, because we
  // don't want to get ownership of pointers by moving unique pointers.
  List<value_type>::node* node_iter = head.get();

  // Always print the first, and then the following, `sep` separated.
  std::cout << "[" << node_iter->val;

  std::string sep = ", ";
  while (node_iter->next) {
    node_iter = node_iter->next.get();
    std::cout << sep << node_iter->val;
  }
  std::cout << "] (size " << size() << ")\n";
}

template <typename value_type>
void List<value_type>::push_front(const value_type& v) {
  // Release the head so another element can be promoted to head of the list.
  head = std::unique_ptr<node>(new node(v, head.release()));
  _size++;
}

template <typename value_type>
void List<value_type>::push_back(const value_type& v) {
  // Use the tail to reach the node at the end of the list.
  tail->next = std::unique_ptr<node>(new node(v));
  tail = tail->next.get();
  _size++;
}
