#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "btree.h"
#include "doctest.h"
#include <functional>  // std::less

// In doctest, there are three kind of assertion macros: REQUIRE, CHECK and WARN.
// If a REQUIRE fails, it stops the whole test execution, if a CHECK fails, the single unit tests
// continues thw whole test suite, and then graciously stop reporting all the CHECKs that failed.

TEST_CASE("testing the btree implementation of the insert method and size+traversal_size methods") {
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);

    int key = 42;
    float value = 3.14;
    tree.insert(key, value);

    REQUIRE(tree.size() == 1);
    REQUIRE(tree.traversal_size() == 1);

    DEBUG_MSG(std::boolalpha);
    DEBUG_MSG("left is set? " << (bool)tree.get_root()->left);
    DEBUG_MSG("right is set? " << (bool)tree.get_root()->right);

#ifdef DEBUG
    DEBUG_MSG("root key=" << tree.get_root()->key() << ", value=" << tree.get_root()->value());
    REQUIRE(tree.get_root()->key() == key);
#endif

    SUBCASE("test more insertions increase key") {
        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 2);
        CHECK(tree.traversal_size() == 2);

        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 3);
        CHECK(tree.traversal_size() == 3);

#ifdef DEBUG
        DEBUG_MSG("left is set? " << (bool)tree.get_root()->left << std::endl);
        DEBUG_MSG("right is set? " << (bool)tree.get_root()->right << std::endl);
        CHECK(tree.get_root()->right->right->key() == key);
#endif
    }

    SUBCASE("test more insertions decrease key") {
        key--;
        tree.insert(key, value);
        CHECK(tree.size() == 2);
        CHECK(tree.traversal_size() == 2);

        key--;
        tree.insert(key, value);
        CHECK(tree.size() == 3);
        CHECK(tree.traversal_size() == 3);
    }
}

TEST_CASE("testing the btree implementation of the insert method and size+traversal_size methods") {
    DEBUG_MSG("\n\n\n\n\n\n\n");
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);

    int key = 42;
    float value = 3.14;

    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    REQUIRE(tree._find_public(key)->_pair.second == value);

    key = 30;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    REQUIRE(tree._find_public(key)->_pair.second == value);

    key = 10;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);
    key++;
    tree.insert(key, value);

    DEBUG_MSG(std::endl << std::endl);
    REQUIRE(tree._find_public(key)->_pair.second == value);

    REQUIRE(tree._find_public(34)->_pair.second == value);

#ifdef DEBUG
    // Test the root retrieval.
    std::cout << tree._find_public(42)->_pair.second;
    std::cout << tree.get_root()->value();
    REQUIRE(tree._find_public(42)->_pair.second == tree.get_root()->value());
#endif

    // Search for a not existing node should return a nullptr.
    REQUIRE(tree._find_public(999999) == nullptr);
}
