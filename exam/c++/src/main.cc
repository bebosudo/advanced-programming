#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "btree.h"
#include "doctest.h"
#include <functional>  // std::less

TEST_CASE(
    "testing the btree implementation of the insert method and size and traversal_size methods") {
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);

    int key = 42;
    float value = 3.14;
    tree.insert(key, value);

    REQUIRE(tree.size() == 1);
    REQUIRE(tree.traversal_size() == 1);

#ifdef DEBUG
    std::cout << std::boolalpha;
    std::cout << "left is set? " << (bool)tree.get_root()->left << std::endl;
    std::cout << "right is set? " << (bool)tree.get_root()->right << std::endl;
    std::cout << "root key=" << tree.get_root()->key() << ", value=" << tree.get_root()->value()
              << std::endl;
    REQUIRE(tree.get_root()->key() == key);
#endif

    SUBCASE("test more insertions") {
        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 2);
        CHECK(tree.traversal_size() == 2);

        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 3);
        CHECK(tree.traversal_size() == 3);

#ifdef DEBUG
        std::cout << "left is set? " << (bool)tree.get_root()->left << std::endl;
        std::cout << "right is set? " << (bool)tree.get_root()->right << std::endl;
        CHECK(tree.get_root()->right->key() == key);
#endif
    }
}
