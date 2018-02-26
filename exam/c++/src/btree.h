#include <functional>  // std::less
#include <memory>
#include <utility>
#include <iostream>

// #define VERBOSE

#if defined(DEBUG) && defined(VERBOSE)
#define DEBUG_MSG(str)                 \
    do {                               \
        std::cout << str << std::endl; \
    } while (false)

#else
#define DEBUG_MSG(str) \
    do {               \
    } while (false)
#endif

template <typename K, typename V, typename cmp = std::less<K>>
class BTree {
   private:
    class Node;

    std::unique_ptr<Node> root;
    unsigned int _size{0};

    // `cmp` is a templated class, so we need to instantiate it before we can use it.
    std::function<bool(K, K)> comparator = cmp();

    bool _go_left_direction(K key_node_on_tree, K new_node_key);

    Node *_traverse_to_closest(K key);

    // For our convenience, we create a find version that returns a Node*, which can be used in
    // many other methods.
    Node *_find(K key);

   public:
    BTree(){};

    // The first insert takes key and value as references; then we call the method insert(pair)
    // internally, so we copy the values only once when calling the second method.
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

    // Public version that envelops the private _find method: only for internal use, to be dropped.
    Node *_find_public(K key) { return _find(key); }

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
    V val() { return _pair.second; }
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
    // E.g. we receive a key_node_on_tree = 42 and a new_node_key = 43, so the default less
    // operator
    // returns true, because 42<43, but we flip the result because we want to go right if the
    // new
    // key is larger than the existing.
    // At the end of the day, this is done only to improve the visual structure for us humans,
    // computers don't care if our tree is ordered in the inverse way.
    return !comparator(key_node_on_tree, new_node_key);
}

template <typename K, typename V, typename cmp>
typename BTree<K, V, cmp>::Node *BTree<K, V, cmp>::_traverse_to_closest(K key) {
    Node *temp_iter = root.get();

    if (temp_iter->key() == key)
        return temp_iter;
    bool go_left = _go_left_direction(temp_iter->key(), key);

    DEBUG_MSG(std::boolalpha);
    DEBUG_MSG("the root node has key: " << temp_iter->key() << " and the search key is " << key);
    DEBUG_MSG("given the key: " << key << ", Imma look for the closest node");
    DEBUG_MSG("go_left is " << go_left << ", then Imma go " << (go_left ? "left" : "right"));

    while ((go_left and temp_iter->left) or (!go_left and temp_iter->right)) {
        if (go_left) {
            DEBUG_MSG("since go_left is true, Imma go left");
            temp_iter = temp_iter->left.get();
        } else {
            DEBUG_MSG("since go_left is false, Imma go right");
            temp_iter = temp_iter->right.get();
        }

        if (temp_iter->key() == key)
            return temp_iter;

        go_left = _go_left_direction(temp_iter->key(), key);
    }

    return temp_iter;
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(K &key, V &value) {
    std::pair<K, V> pair = std::make_pair(key, value);
    return insert(pair);
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(std::pair<K, V> pair) {
    DEBUG_MSG("inserting new pair: {" << pair.first << ": " << pair.second << "}");
    // Basic case, the tree is empty, so the new pair becomes the root object.
    if (!root) {
        DEBUG_MSG("no need to go more down, inserting as new root");
        root = std::unique_ptr<Node>(new Node(pair));
        _size++;
        return true;
    }

    // otherwise, we must traverse the tree and find where to place the new node.
    Node *temp_iter = _traverse_to_closest(pair.first);

    DEBUG_MSG("reached node: {" << temp_iter->key() << ": " << temp_iter->val()
                                << "}. gonna insert new pair {" << pair.first << ": " << pair.second
                                << "}");

    // Now we reached the bottom part of the three, following one of the branches.
    // `temp_iter` is now a pointer to the last valid node.
    if (temp_iter->key() == pair.first) {
        temp_iter->_pair = pair;
        return true;
    } else if (_go_left_direction(temp_iter->key(), pair.first)) {
        DEBUG_MSG("inserting: {" << pair.first << ": " << pair.second << "} a left");
        temp_iter->left = std::unique_ptr<Node>(new Node(pair));
    } else {
        DEBUG_MSG("inserting: {" << pair.first << ": " << pair.second << "} a right");
        temp_iter->right = std::unique_ptr<Node>(new Node(pair));
    }

    _size++;
    return true;
}

template <typename K, typename V, typename cmp>
typename BTree<K, V, cmp>::Node *BTree<K, V, cmp>::_find(K key) {
    Node *temp_iter = _traverse_to_closest(key);
    DEBUG_MSG("reached node temp_iter {" << temp_iter->key() << ": " << temp_iter->val() << "}");

    if (temp_iter->key() == key) {
        DEBUG_MSG("the node found has the same key we are searching for, returning it");
        return temp_iter;
    } else {
        DEBUG_MSG("no node exists with the given key, returning a nullptr");
        return nullptr;
    }
}
