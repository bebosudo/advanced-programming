/*
  clang++ --std=c++11 -Wall -Wextra llist.cc -o ../build/llist && \
    ../build/llist
*/

#include "llist.h"

const bool DEBUG = false;

int main() {
  // To test the empty constructor.
  List<int> empty_l;
  empty_l.print();
  std::cout << "-------\n";

  // To test the constructor with one argument.
  List<int> l{42};
  l.print();
  l.insert(2, Insertion_method::push_back);
  l.insert(2, Insertion_method::push_back);
  l.print();
  l.prune_node(2);
  l.print();

  l.insert(99, Insertion_method::push_back);
  l.print();
  l.insert(102, Insertion_method::push_back);
  l.insert(102, Insertion_method::push_front);
  l.insert(102, Insertion_method::push_front);
  l.insert(100, Insertion_method::push_front);
  l.insert(102, Insertion_method::push_front);
  l.insert(102, Insertion_method::push_front);
  l.insert(101, Insertion_method::push_front);
  l.insert(102, Insertion_method::push_front);
  l.insert(102, Insertion_method::push_front);
  l.insert(200, Insertion_method::push_front);
  l.print();
  l.prune_node(102);
  l.print();

  l.reset();
  l.print();

  return 0;
}

/* ---------------------------- llist.cpp ---------------------------- */
template <typename value_type>
List<value_type>::List(value_type value) : _size{1} {
  head = std::unique_ptr<node>{new node(value)};
  tail = head.get();
}

template <typename value_type>
void List<value_type>::print() {
  if (not head) {
    std::cout << "[] (size " << size() << ")\n";
    return;
  }

  // Use a pointer to the node object to iterate over the elements, because we
  // don't want to get ownership of pointers by moving unique pointers.
  node* node_iter = head.get();

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
  // Release the head so another element can be promoted as head of the list.
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

template <typename value_type>
void List<value_type>::insert(const value_type& v, const Insertion_method m) {
  if (DEBUG) std::cout << "inserting " << v << " at the ";

  switch (m) {
    case Insertion_method::push_back:
      push_back(v);
      if (DEBUG) std::cout << "back";
      break;
    case Insertion_method::push_front:
      push_front(v);
      if (DEBUG) std::cout << "front";
      break;
    default:
      throw std::invalid_argument("Insertion method not known.");
  }
  if (DEBUG) std::cout << " (list size " << size() << ")\n";
}

template <typename value_type>
void List<value_type>::reset() {
  head.reset();
  _size = 0;
}

/* We need to take into account a specific corner case: there are multiple
 * matching values right after the head, and then the rest can be managed using
 * a while loop iterating over the element following the one currently on
 * iteration.
  asd */
template <typename value_type>
void List<value_type>::prune_node(const value_type v) {
  if (DEBUG)
    std::cout << "pruning " << v << " (list size before " << size() << ")\n";
  if (not head or (head->val != v and not head->next)) {
    // There's nothing or nothing to remove in the list (made of a single el).
    return;
  } else if (head->val == v and not head->next) {
    // The list is made only of a single element, which matches. Delete all and
    // exit the function.
    reset();
    return;
  }

  unsigned n_removed = 0;
  while (head->val == v) {
    // Remove responsibility from the second element and move the head to the
    // second element of the list.
    head = std::unique_ptr<node>(head->next.release());
    n_removed++;
  }

  node* node_iter = head.get();
  while (node_iter->next) {
    if (node_iter->next->val == v) {
      node_iter->next = std::unique_ptr<node>(node_iter->next->next.release());
      n_removed++;
    } else {
      node_iter = node_iter->next.get();
    }
  }
  tail = node_iter;

  _size -= n_removed;
  if (DEBUG)
    std::cout << "pruned " << v << " (list size after " << size() << ")\n";
}
