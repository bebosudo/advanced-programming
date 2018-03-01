#include <functional>  // std::less
#include <memory>
#include <utility>
#include <iostream>
#include <cmath>

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

struct KeyNotFound {
    std::string message;
};

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

    bool insert(std::unique_ptr<Node> node_to_insert);
    unsigned int height(Node *root);

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

    unsigned int height();
    bool is_balanced();

    class iterator;
    iterator begin() { return iterator{this}; };
    iterator end() { return iterator{this, nullptr}; };

    class const_iterator;
    const_iterator cbegin() { return const_iterator{this}; };
    const_iterator cend() { return const_iterator{this, nullptr}; };

    iterator find(K key);
    std::pair<K, V> erase(K key);
    const V &operator[](const K &key);

    unsigned int size() { return _size; };

#ifdef DEBUG
    unsigned int traversal_size() { return (root) ? root->traverse() : 0; };
    Node *_find_public(K key) { return _find(key); }
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

    const K &key() const { return _pair.first; }
    const V &val() const { return _pair.second; }

#ifdef DEBUG
    unsigned int traverse() {
        unsigned int sub_nodes = 1;

        if (left)
            sub_nodes += left->traverse();
        if (right)
            sub_nodes += right->traverse();

        return sub_nodes;
    };
#endif

    Node *get_leftmost() {
        if (not left)
            return this;

        Node *temp_iter = left.get();
        while (temp_iter->left) {
            temp_iter = temp_iter->left.get();
        }
        return temp_iter;
    }
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::iterator {
   protected:
    BTree *_tree_ref;
    Node *_current;

   public:
    // If an iterator is called only with the pointer to a tree, place it at the beginning.
    explicit iterator(BTree *tree_ref) : _tree_ref{tree_ref} {
        if (_tree_ref != nullptr and _tree_ref->root)
            _current = _tree_ref->root->get_leftmost();
        else
            _current = nullptr;
    }
    // If a Node is passed to the iterator, place it to that node in the tree.
    explicit iterator(BTree *tree_ref, Node *current) : _tree_ref{tree_ref}, _current{current} {};

    // iterator(K key);
    const K &key() const { return _current->key(); }
    const V &val() const { return _current->val(); }
    const std::pair<K, V> pair() const { return _current->_pair; }

    // ++it
    iterator &operator++();

    // it++
    iterator operator++(int) {
        iterator it{_tree_ref, _current};
        ++(*this);
        return it;
    }

    bool operator==(const iterator &other) { return _current == other._current; }
    bool operator!=(const iterator &other) { return !(*this == other); }
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::const_iterator : public BTree<K, V, cmp>::iterator {
   public:
    // using iterator::iterator;

    explicit const_iterator(BTree *tree_ref) : iterator{tree_ref} {};
    explicit const_iterator(BTree *tree_ref, Node *current) : iterator{tree_ref, current} {}

    const K &key() const { return BTree<K, V, cmp>::iterator::key(); }

    const V val() const { return BTree<K, V, cmp>::iterator::val(); }
};

#include "btree.hcc"
