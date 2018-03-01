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

    std::pair<K, V> erase(K key);

    class iterator;
    iterator begin() { return iterator{this}; };
    iterator end() { return iterator{this, nullptr}; };

    class const_iterator;
    const_iterator cbegin() { return const_iterator{this}; };
    const_iterator cend() { return const_iterator{this, nullptr}; };

    iterator find(K key);

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

    void set_left(Node &child) { left = child; };
    void set_right(Node &child) { right = child; };
    const K &key() const { return _pair.first; }
    const V &value() const { return _pair.second; }
    const V &val() const { return value(); }
    K get_key() { return key(); }
    V get_value() { return value(); }

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
    const V val() const { return _current->val(); }
    const std::pair<K, V> pair() const
    {
        
        
        return _current->_pair;
    
    
    }


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

    const K &key() const {
    
        return BTree<K, V, cmp>::iterator::key();
    
    }

    const V val() const {
        
        return BTree<K, V, cmp>::iterator::val();
    
    }



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
void BTree<K, V, cmp>::print() {
    iterator it = begin();

    std::cout << "{";

    if (it != end()) {
        std::cout << "'" << it.key() << "': '" << it.val() << "'";
        it++;
    }

    for (; it != end(); ++it) {
        std::cout << ", '" << it.key() << "': '" << it.val() << "'";
    }

    std::cout << "}" << std::endl;
}

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
void BTree<K, V, cmp>::balance() {
    int pos{0}, steps, denominator;
    unsigned int len{_size};

    std::pair<K, V> temp_array[_size];


    if (!root)
        return;


    for (iterator it = begin(); it != end(); it++)
    {
    
        temp_array[pos++] = it.pair();
       /*
        *
        * std::cout << temp_array[pos-1].first << ", ";
        */
    }

    // Empties the tree
    clear();


    steps = std::ceil(std::log2(len));
    denominator = 2;

    //std::cout << steps << "  " << denominator << std::endl;
    
    
    for (int i = 1; i <= steps; i++) 
    {
        //std::cout << "qui i= " << i << std::endl;
        
        
        for (int j = 0; j < denominator; j++) {
        
        //std::cout << "qui pos= " << j << std::endl;
            
        
            if (j % 2 != 0) {
                pos = std::floor( ((double) j / denominator) * len);
                
                
                //std::cout << "num " << pos << std::endl;
                
                
                insert(temp_array[pos]);
            }

        }

       /*
        for (pos = offset; pos < len; pos += offset)
        {
        
            if (pos % 2 != 0)
                insert(temp_array[pos]);

        }
*/
        denominator *= 2;
    }



    /*
    for (m = (r - l); m > 0; m /= 2)
    {
       bool c{true};
        std::cout << "qui\n";

        for (pos = m; pos < len; pos += m)
        {

            c = insert(temp_array[pos]); 
            std::cout << std::boolalpha << temp_array[pos].first << c << " ";
        }


    }
    
    */

}

template <typename K, typename V, typename cmp>
bool BTree<K, V, cmp>::is_balanced() {
    if (height() <= std::ceil(std::log2(_size)))
        
        return true;
    
    else
    
        return false;

}

template <typename K, typename V, typename cmp>
unsigned int BTree<K, V, cmp>::height() {
    return height(root.get());
}

template <typename K, typename V, typename cmp>
unsigned int BTree<K, V, cmp>::height(Node *root) {
    unsigned int left_children, right_children;

    if (root) {
        left_children = root->left ? height(root->left.get()) : 0;
        right_children = root->right ? height(root->right.get()) : 0;

        return std::max(left_children, right_children) + 1;
    } else
        return 0;
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

    // Now we reached the bottom part of the tree, following one of the branches.
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
typename BTree<K, V, cmp>::iterator BTree<K, V, cmp>::find(K key) {
    return iterator{this, _find(key)};
}

template <typename K, typename V, typename cmp>
typename BTree<K, V, cmp>::iterator &BTree<K, V, cmp>::iterator::operator++() {
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
