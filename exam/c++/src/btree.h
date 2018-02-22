//#include <stdlib>

template <typename K, typename V, typename cmp>
class BTree {
    private:
        std::unique_ptr<int> root;

        class Node;

    public:
        BTree() {};

        //insert(std::pair<K, V>);

};

template <typename K, typename V, typename cmp>
class BTree<K, V, cmp>::Node {
    public:
        K key;
        V value;
        std::unique_ptr<BTree::Node> left, right;

        Node(std::pair<K, V> pair): key{pair.first}, value{pair.second} {};

        void set_left(Node &child) { left = child; };
        void set_right(Node &child) { right = child; };
};
