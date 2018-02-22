#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "btree.h"
#include "doctest.h"
#include <functional>  // std::less

TEST_CASE("testing the btree implementation of the insert method") {
    BTree<int, float, std::less<int>> tree;
    int key = 42;
    float value = 3.14;
    tree.insert(key, value);

    REQUIRE(tree.size() == 1);

    SUBCASE("") { CHECK(); }
}
