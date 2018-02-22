
template <typename K, typename V, typename cmp>
class BTree {
   private:
    std::unique_ptr<int> root;
    unsigned int _size{0};

    class Node;

   public:
    BTree(){};

    bool insert(K key, V value);
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

    void set_left(Node& child) { left = child; };
    void set_right(Node& child) { right = child; };
};
