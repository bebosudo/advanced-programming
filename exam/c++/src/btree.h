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

    const cmp comparator;
    bool _compare(const K &key1, const K &key2) const { return !comparator(key1, key2); }

    Node *_traverse_to_closest(K key);

    // For our convenience, we create a find version that returns a Node*, which can be used in
    // many other methods.
    Node *_find(K key);

   public:
    // BTree(){};
    BTree(cmp op = cmp{}) : comparator{op} {};

    // The first insert takes key and value as references; then we call the method insert(pair)
    // internally, so we copy the values only once when calling the second method.
    bool insert(const K &key, const V &value);
    bool insert(const std::pair<K, V> &pair);
    void print();
    bool clear();
    void balance();

    std::pair<K, V> erase(K key);

    class Iterator;
    Iterator begin() { return Iterator{this}; };
    Iterator end() { return Iterator{nullptr}; };

    class ConstIterator;
    ConstIterator cbegin();
    ConstIterator cend();

    Iterator find(K key);

    unsigned int size() { return _size; };
    unsigned int traversal_size() { return (root) ? root->traverse() : 0; };

    // Public version that envelops the private _find method: only for internal use, to be
    // dropped.
    Node *_find_public(K key) { return _find(key); }

#ifdef DEBUG
    Node *get_root() { return root.get(); }

#endif
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Node {
   public:
    std::pair<K, V> _pair;
    Node *_parent;
    std::unique_ptr<BTree::Node> left, right;

    Node(std::pair<K, V> pair, Node *parent = nullptr) : _pair{pair}, _parent{parent} {};

    void set_left(Node &child) { left = child; };
    void set_right(Node &child) { right = child; };
    const K &key() const { return _pair.first; }
    const V &value() const { return _pair.second; }
    const V &val() const { return value(); }
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

    Node *get_leftmost() {
        if (not left)
            return this;

        Node *temp_iter = left.get();
        while (temp_iter->left) {
            temp_iter = temp_iter->left.get();
        }
        return temp_iter;
    }

    // bool operator<(const Node &other) {
    //     bool res = BTree<K, V, cmp>::comparator(key(), other.key());
    //     return res;
    // }

    // bool operator>(const Node &other) const {
    //     return BTree<K, V, cmp>::_compare(other.key(), key());
    // }

    // bool operator==(const Node &other) const { return not(*this < other) and not(*this > other);
    // }
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Iterator {
    Node *_current;
    BTree *_tree_ref;

   public:
    explicit Iterator(BTree *tree_ref) : _tree_ref{tree_ref} {
        if (_tree_ref and _tree_ref->root) {
            _current = _tree_ref->root->get_leftmost();
        } else {
            _current = nullptr;
        }
    };
    // Iterator(K key);
    // V &operator*() const { return _current->val(); }
    const K &key() const { return _current->key(); }
    const V val() const { return _current->val(); }

    // ++it
    Iterator &operator++();

    // it++
    Iterator operator++(int) {
        Iterator it{_current};
        ++(*this);
        return it;
    }

    bool operator==(const Iterator &other) { return _current == other._current; }
    bool operator!=(const Iterator &other) { return !(*this == other); }
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
typename BTree<K, V, cmp>::Node *BTree<K, V, cmp>::_traverse_to_closest(K key) {
    Node *temp_iter = root.get();

    if (temp_iter->key() == key)
        return temp_iter;
    bool go_left = _compare(temp_iter->key(), key);

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

        go_left = _compare(temp_iter->key(), key);
    }

    return temp_iter;
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(const K &key, const V &value) {
    std::pair<K, V> pair = std::make_pair(key, value);
    return insert(pair);
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::insert(const std::pair<K, V> &pair) {
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
    } else if (_compare(temp_iter->key(), pair.first)) {
        DEBUG_MSG("inserting: {" << pair.first << ": " << pair.second << "} a left");
        temp_iter->left = std::unique_ptr<Node>(new Node(pair, temp_iter));
    } else {
        DEBUG_MSG("inserting: {" << pair.first << ": " << pair.second << "} a right");
        temp_iter->right = std::unique_ptr<Node>(new Node(pair, temp_iter));
    }

    _size++;
    return true;
}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::clear() {
    if (root) {
        root.reset();
        _size = 0;
    }
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

template <typename K, typename V, typename cmp>
typename BTree<K, V, cmp>::Iterator &BTree<K, V, cmp>::Iterator::operator++() {
    if (not _current)
        return *this;

    // If a child with a greater key exists (right pointer), move to it and then go to the
    // leftmost branch.
    if (_current->right) {
        _current = _current->right->get_leftmost();
        return *this;
    }

    Node *starting_node = _current;

    do {
        _current = _current->_parent;

        if (not _current->_parent and _tree_ref->_compare(starting_node->key(), _current->key())) {
            // This means that we tried searching for a parent in all the parents of the current
            // node, but we couldn't find it, which means that the current node is the rightmost,
            // thus we set it to null so that it matches the end of the iterator.
            _current = nullptr;
            return *this;
        }

    } while (_tree_ref->_compare(starting_node->key(), _current->key()));

    return *this;
}
