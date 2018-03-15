#ifndef __BTREE_H__
#define __BTREE_H__

#include <cmath>
#include <functional>  // std::less
#include <iterator>    // needed to derive from std::iterator
#include <iostream>
#include <memory>
#include <utility>

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
    class Node;

    std::unique_ptr<Node> root;
    unsigned int _size{0};
    const cmp comparator;

    bool _compare(const K &key1, const K &key2) const noexcept { return !comparator(key1, key2); }

    bool _equal_compare(const K &key1, const K &key2) const noexcept {
        return (_compare(key1, key2) && _compare(key2, key1));
    }

    Node *_traverse_to_closest(const K &key) const noexcept;

    // For our convenience, we create a find version that returns a Node*, which can be used in
    // many other methods.
    Node *_find(const K &key) const noexcept;

    bool insert(std::unique_ptr<Node> node_to_insert) noexcept;

    unsigned int height(Node *root) const noexcept;

    void insert_recursive(Node *current, Node *parent) noexcept {
        std::unique_ptr<Node> temp{new Node(current->key(), current->val())};
        temp->_parent = parent;
        insert(std::move(temp));

        if (current->left)
            insert_recursive(current->left.get(), current);
        if (current->right)
            insert_recursive(current->right.get(), current);
    }

   public:
    BTree(cmp op = cmp{}) noexcept : comparator{op} {};

    const unsigned int &size() const noexcept { return _size; }

    bool insert(const K &key, const V &value) noexcept {
        return insert(std::unique_ptr<Node>{new Node(key, value)});
    };

    void print() const noexcept;
    bool clear() noexcept;
    void balance() noexcept;

    unsigned int height() const noexcept {
        if (not root)
            return 0;
        return height(root.get());
    }

    // unsigned int height() const { return height() <= std::ceil(std::log2(_size)); }
    bool is_balanced() const noexcept { return height() <= std::ceil(std::log2(_size)); };
    // bool is_balanced() const { return height(root.get()); };

    class iterator;
    class const_iterator;
    iterator begin() noexcept { return iterator{this}; }
    iterator end() noexcept { return iterator{this, nullptr}; }
    const_iterator begin() const noexcept { return cbegin(); }
    const_iterator end() const noexcept { return cend(); }

    const_iterator cbegin() const noexcept { return const_iterator{this}; }
    const_iterator cend() const noexcept { return const_iterator{this, nullptr}; }

    iterator find(const K &key) const noexcept { return iterator{this, _find(key)}; }
    std::pair<K, V> erase(const K &key);

    // Provide two different versions to access the value: rw and ro.
    V &operator[](const K &key) noexcept;
    const V &operator[](const K &key) const noexcept { return operator[](key); }

    /* copy ctor */
    BTree(const BTree &other) noexcept : _size{0}, comparator{other.comparator} {
        insert_recursive(other.root.get(), nullptr);
    }

    /* move ctor */
    BTree(BTree &&other) noexcept
        : root{std::move(other.root)}, _size{other._size}, comparator{other.comparator} {
        other._size = 0;
    }

    /* copy assignment operator */
    BTree &operator=(const BTree &other) noexcept {
        BTree tmp(other);        // re-use copy-constructor
        *this = std::move(tmp);  // re-use move-assignment
        return *this;
    }

    /* move assignment operator */
    BTree &operator=(BTree &&other) noexcept {
        if (this == &other)
            return *this;

        root = std::move(other.root);
        _size = std::move(other._size);
        other._size = 0;

        return *this;
    }

#ifdef DEBUG
    unsigned int traversal_size() const noexcept { return (root) ? root->traverse() : 0; };
    Node *_find_public(const K key) const noexcept { return _find(key); }
    Node *get_root() const noexcept { return root.get(); }
#endif
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Node {
   public:
    const K _key;
    V _val;
    Node *_parent;
    std::unique_ptr<BTree::Node> left, right;

    // Node(std::pair<K, V> pair, Node *parent = nullptr) : _pair{pair}, _parent{parent} {};
    Node(const K &key, const V &val, Node *parent = nullptr) noexcept
        : _key{key}, _val{val}, _parent{parent} {};

    const std::pair<K, V> pair() const noexcept { return std::make_pair(_key, _val); }
    const K &key() const noexcept { return _key; }

    // rw and ro versions.
    V &val() noexcept { return _val; }
    const V &val() const noexcept { return val(); }

#ifdef DEBUG
    unsigned int traverse() const noexcept {
        unsigned int sub_nodes = 1;

        if (left)
            sub_nodes += left->traverse();
        if (right)
            sub_nodes += right->traverse();

        return sub_nodes;
    };
#endif

    Node *get_leftmost() noexcept {
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
class BTree<K, V, cmp>::iterator : public std::iterator<std::forward_iterator_tag, K> {
    const BTree *_tree_ref;
    Node *_current;

   public:
    // If an iterator is called only with the pointer to a tree, place it at the beginning.
    explicit iterator(const BTree *tree_ref) noexcept : _tree_ref{tree_ref}, _current{nullptr} {
        if (_tree_ref != nullptr and _tree_ref->root)
            _current = _tree_ref->root->get_leftmost();
    }
    // If a Node is passed to the iterator, place it to that node in the tree.
    explicit iterator(const BTree *tree_ref, Node *current) noexcept
        : _tree_ref{tree_ref}, _current{current} {}

    const K &key() const noexcept { return _current->key(); }
    V &val() noexcept { return _current->val(); }
    const V &val() const noexcept { return _current->val(); }

    const std::pair<K, V> pair() const noexcept { return _current->pair(); }

    V &operator*() const noexcept { return _current->val(); }

    // ++it
    iterator &operator++() noexcept;

    // it++
    iterator operator++(int)noexcept {
        iterator it{_tree_ref, _current};
        ++(*this);
        return it;
    }

    bool operator==(const iterator &other) const noexcept {
        if (this->_current == nullptr and other._current == nullptr)
            return true;
        if (not this->_current != not other._current)
            return false;
        return this->_current == other._current;
    }
    bool operator!=(const iterator &other) const noexcept { return !(*this == other); }
};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::const_iterator : public BTree<K, V, cmp>::iterator {
   public:
    // using iterator::iterator;

    explicit const_iterator(const BTree *tree_ref) noexcept : iterator{tree_ref} {};
    explicit const_iterator(const BTree *tree_ref, Node *current) noexcept
        : iterator{tree_ref, current} {}

    const V &operator*() const noexcept { return BTree<K, V, cmp>::iterator::operator*(); }
};

#include "btree.hcc"

#endif
