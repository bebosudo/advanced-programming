#include <functional>  // std::less
#include <memory>
#include <utility>
#include <iostream>

template <typename K, typename V, typename cmp>
class BTree {
   private:
    class Node;

    std::unique_ptr<Node> root;
    unsigned int _size{0};

   public:
    BTree(){};

    // The first insert is taken as reference, since we call the method
    // insert(pair) internally, so we can avoid copying again two input values.
    bool insert(K &key, V &value);
    bool insert(std::pair<K, V> pair);
    void print();
    bool clear();
    void balance();

    std::pair<K, V> erase(K key);

    class Iterator;
    Iterator begin();
    Iterator end();

    class ConstIterator;
    ConstIterator cbegin();
    ConstIterator cend();

    Iterator find(K key);

    unsigned int size() { return _size; };
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Node {
   public:
    K key;
    V value;
    std::unique_ptr<BTree::Node> left, right;

    Node(std::pair<K, V> pair) : key{pair.first}, value{pair.second} {};

    void set_left(Node &child) { left = child; };
    void set_right(Node &child) { right = child; };
};

//
//
//
//
//
//
//
//
//
//
// --------------------------------------------------------
//
//
//
//
//
//
//
//
//

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(K &key, V &value) {
    std::pair<K, V> pair = std::make_pair(key, value);
    return insert(pair);
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(std::pair<K, V> pair) {
    root = std::unique_ptr<Node>(new Node(pair));
    _size++;
    return true;
}
