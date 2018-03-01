#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "btree.h"
#include "doctest.h"

// In doctest, there are three kind of assertion macros: REQUIRE, CHECK and WARN.
// If a REQUIRE fails, it stops the whole test execution, if a CHECK fails, the tests continue to
// execute until the whole test suite is done, and then doctest graciously stops, reporting all the
// CHECKs that failed.

#ifdef DEBUG

TEST_CASE("insert, size+traversal_size and clear methods") {
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);

    int key = 42;
    float value = 3.14;
    tree.insert(key, value);

    REQUIRE(tree.size() == 1);
    REQUIRE(tree.traversal_size() == 1);

    DEBUG_MSG(std::boolalpha);
    DEBUG_MSG("is left set? " << (bool)tree.get_root()->left);
    DEBUG_MSG("is right set? " << (bool)tree.get_root()->right);

    DEBUG_MSG("root key=" << tree.get_root()->key() << ", value=" << tree.get_root()->value());
    REQUIRE(tree.get_root()->key() == key);

    SUBCASE("test more insertions increase key") {
        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 2);
        CHECK(tree.traversal_size() == 2);

        key++;
        tree.insert(key, value);

        CHECK(tree.size() == 3);
        CHECK(tree.traversal_size() == 3);

        DEBUG_MSG("is left set? " << (bool)tree.get_root()->left << std::endl);
        DEBUG_MSG("is right set? " << (bool)tree.get_root()->right << std::endl);
        CHECK(tree.get_root()->right->right->key() == key);
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

    SUBCASE("same key insertion") {
        // Inserting the same key twice should keep the three height fixed.
        tree.insert(key, value);
        CHECK(tree.size() == 1);
        CHECK(tree.traversal_size() == 1);
    }

    SUBCASE("tree clearing") {
        for (int i = 0; i < 10; i++) {
            tree.insert(key, value);
            key++;
        }
        CHECK(tree.size() == 10);
        tree.clear();
        CHECK(tree.size() == 0);
    }
}

void tree_keys(int *keys_array) {
    for (int i = 0; i < 20; ++i) {
        if (i < 6) {
            keys_array[i] = 30 + i;
        } else if (i < 12) {
            keys_array[i] = 40 + i - 6;
        } else {
            keys_array[i] = 20 + i - 12;
        }
    }
}

TEST_CASE("_find private method (exposed by `_find_public` when in debug mode)") {
    DEBUG_MSG("\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);

    int keys[20];
    tree_keys(keys);
    float value = 3.14;

    for (int i = 0; i < 20; ++i) {
        tree.insert(keys[i], value);
        REQUIRE(tree._find_public(keys[i])->_pair.second == value);
    }

    // Retrieve all the past keys.
    for (int i = 0; i < 20; ++i) {
        REQUIRE(tree._find_public(keys[i])->_pair.second == value);
    }

    // Test the root retrieval.
    REQUIRE(tree._find_public(keys[0])->_pair.second == tree.get_root()->value());

    // Searching for a not existing node should return a nullptr.
    REQUIRE(tree._find_public(999999) == nullptr);
}

TEST_CASE("iterator basic test") {
    BTree<int, float, std::less<int>> tree;

    int last_element_seen = 1;
    float value = 3.14;

    tree.insert(2, value);
    tree.insert(1, value);
    tree.insert(3, value);

    for (BTree<int, float, std::less<int>>::iterator it = tree.begin(); it != tree.end(); ++it) {
        CHECK(std::less_equal<int>()(last_element_seen, it.key()));
        last_element_seen = it.key();
    }
}

TEST_CASE("iterator_implementation_1") {
    BTree<int, float, std::less<int>> tree;

    int keys[20];
    tree_keys(keys);
    float value = 3.14;

    int lowest_value = keys[0];
    for (int i = 0; i < 20; ++i) {
        if (keys[i] < lowest_value)
            lowest_value = keys[i];

        tree.insert(keys[i], value);
    }

    SUBCASE("test normal iteration") {
        int last_element_seen = lowest_value;

        BTree<int, float, std::less<int>>::iterator it = tree.begin();
        // Or:
        // BTree<int, float, std::less<int>>::iterator it{&tree};

        for (; it != tree.end(); ++it) {
            // We use less_equal so that the lowest number previously seen satisfies the check.
            CHECK(std::less_equal<int>()(last_element_seen, it.key()));
            last_element_seen = it.key();
        }
    }
    // SUBCASE("test iteration on missing key") {
    //     CHECK_THROWS_AS(BTree<int, float, std::less<int>>::iterator it = tree.begin(99999),
    //                     iteratorInit_key_not_found);
    // }
}

TEST_CASE("iterator_implementation_2") {
    BTree<int, float, std::less<int>> tree;

    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13}, last_element_seen = 1;
    float value = 3.14;

    for (int i = 0; i < 15; i++) {
        tree.insert(keys[i], value);
    }

    SUBCASE("test normal iteration") {
        BTree<int, float, std::less<int>>::iterator it = tree.begin();
        // Or:
        // BTree<int, float, std::less<int>>::iterator it{&tree};

        for (; it != tree.end(); ++it) {
            // We use less_equal so that the lowest number previously seen satisfies the check.
            CHECK(std::less_equal<int>()(last_element_seen, it.key()));
            last_element_seen = it.key();
        }
    }
}

TEST_CASE("print iterator") {
    BTree<int, float, std::less<int>> tree;

    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13};
    float value = 3.14;

    for (int i = 0; i < 15; i++) {
        tree.insert(keys[i], value);
    }

    std::cout << "Testing the printing function:" << std::endl;

    tree.print();
}
#endif
