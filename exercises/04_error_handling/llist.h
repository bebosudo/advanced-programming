#ifndef __LLIST_H__
#define __LLIST_H__

#include <iostream>
#include <memory>

enum class Insertion_method { push_back, push_front };

template <typename value_type>
class List {
  // private struct node with the proper value_type
  struct node {
    value_type val;
    std::unique_ptr<node> next;

    // Used by the default List constructor and push_back method.
    explicit node(value_type value) : val{value} {};
    // Used by the push_front method.
    explicit node(value_type value, node* ptr) : val{value}, next{ptr} {};
  };

  std::unique_ptr<node> head;
  // Store a pointer to the tail, so there's no need to traverse the whole
  // list to append a new element. Use a normal pointer, since the ownership is
  // managed by the next unique_ptr inside the node of the second to last
  // element.
  node* tail = nullptr;

  unsigned int _size;

  // append the newly created node at the end of the list
  void push_back(const value_type& v);

  // insert the newly created node in front of the list
  void push_front(const value_type& v);

 public:
  // insert a new node with the value v according to the method m
  // this method should be used to fill the list
  void insert(const value_type& v, const Insertion_method m);

  // print the values of the nodes
  void print();

  // return the size of the list
  unsigned int size() { return _size; };

  // delete all the nodes of the list
  void reset();

  // prune node storing the value v
  void prune_node(const value_type v);

  // constructor(s) for List
  explicit List() : _size{0} {};
  explicit List(value_type value);
};

#endif
