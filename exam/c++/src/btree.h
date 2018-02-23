#include <functional>  // std::less
#include <memory>
#include <utility>
#include <iostream>

// #define DEBUG

template <typename K, typename V, typename cmp = std::less<K>>
class BTree {
   private:
    class Node;

    std::unique_ptr<Node> root;
    unsigned int _size{0};

    // `cmp` is a templated class, so we need to instantiate it before we can use it.
    std::function<bool(K, K)> comparator = cmp();

    bool _go_left_direction(K key_node_on_tree, K new_node_key);

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
    unsigned int traversal_size() { return (root) ? root->traverse() : 0; };

#ifdef DEBUG
    Node *get_root() { return root.get(); }
#endif
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Node {
   public:
    std::pair<K, V> _pair;
    std::unique_ptr<BTree::Node> left, right;

    Node(std::pair<K, V> pair) : _pair{pair} {};

    void set_left(Node &child) { left = child; };
    void set_right(Node &child) { right = child; };
    K key() { return _pair.first; }
    V value() { return _pair.second; }
    K get_key() { return key(); }
    V get_value() { return value(); }
    unsigned int traverse() {
        unsigned int sub_nodes = 1;

        if (left)
            sub_nodes += left->traverse();
        if (right)
            sub_nodes += right->traverse();
        return sub_nodes;
    };
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
//

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::_go_left_direction(K key_node_on_tree, K new_node_key) {
    // Here we decide where to move according to the comparator; to change the overall tree
    // ordering, we could just negate the comparator here once for the whole tree.
    // E.g. we receive a key_node_on_tree = 42 and a new_node_key = 43, so the default less operator
    // returns true, because 42<43, but we flip the result because we want to go right if the new
    // key is larger than the existing. At the end of the day, this is done only to improve the
    // visual structure for us humans, computers don't care if our tree is ordered in the inverse
    // way.
    return !comparator(key_node_on_tree, new_node_key);
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(K &key, V &value) {
    std::pair<K, V> pair = std::make_pair(key, value);
    return insert(pair);
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(std::pair<K, V> pair) {
    // Basic case, the tree is empty, so the new pair becomes the root object.
    if (!root) {
        root = std::unique_ptr<Node>(new Node(pair));
        _size++;
        return true;
    }

    // otherwise, we must traverse the tree and find where to place the new node.
    // TODO: when there will be a find method, use that to find the correct place to place the
    // node.
    Node *temp_iter = root.get();
    bool go_left = _go_left_direction(temp_iter->key(), pair.first);

    while ((go_left and temp_iter->left) or (!go_left and temp_iter->right)) {
        if (go_left) {
            temp_iter = temp_iter->left.get();
        } else {
            temp_iter = temp_iter->right.get();
        }

        go_left = _go_left_direction(temp_iter->key(), pair.first);
    }

    // Now we reached the bottom part of the three, following one of the branches.
    // `temp_iter` is now a pointer to the last valid node.
    if (go_left) {
        temp_iter->left = std::unique_ptr<Node>(new Node(pair));
    } else {
        temp_iter->right = std::unique_ptr<Node>(new Node(pair));
    }

    _size++;
    return true;
}