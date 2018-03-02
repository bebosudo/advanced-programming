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

    DEBUG_MSG("root key=" << tree.get_root()->key() << ", value=" << tree.get_root()->val());
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
    REQUIRE(tree._find_public(keys[0])->_pair.second == tree.get_root()->val());

    // Searching for a not existing node should return a nullptr.
    REQUIRE(tree._find_public(999999) == nullptr);
}

TEST_CASE("tree balancing") {
    BTree<int, float, std::less<int>> tree;

    REQUIRE(tree.size() == 0);
    REQUIRE(tree.traversal_size() == 0);
    REQUIRE(tree.height() == 0);

    SUBCASE("check unbalancing") {
        int key = 12;
        float value = 3.14;

        // Let's build an unbalanced tree
        for (int i = 0; i < 12; i++) {
            tree.insert(key, value);
            key--;
        }

        REQUIRE(tree.size() == 12);
        REQUIRE(tree.traversal_size() == 12);

        CHECK(tree.height() == 12);
        REQUIRE(!tree.is_balanced());

        key = 20;
        for (int i = 0; i < 10; i++) {
            tree.insert(key, value);
            key++;
        }

        CHECK(tree.height() == 12);
        REQUIRE(!tree.is_balanced());
    }

    SUBCASE("check balancing function") {
        int keys[] = {12, 4, 15, 1, 22, 8, 9, 10, 11};  // len: 9
        float value = 2.7;

        for (int i = 0; i < 9; i++)
            tree.insert(keys[i], value);

        REQUIRE(tree.size() == 9);
        REQUIRE(tree.height() == 6);

        tree.balance();
        REQUIRE(tree.height() == 4);  // should be ceil(log2(9)) == 4 if balanced
        REQUIRE(tree.size() == 9);
    }
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

        BTree<int, float, std::less<int>>::iterator it = tree.begin();  // or `::iterator it{&tree}`

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
        for (BTree<int, float, std::less<int>>::iterator it = tree.begin(); it != tree.end();
             ++it) {
            // We use less_equal so that the lowest number previously seen satisfies the check.
            CHECK(std::less_equal<int>()(last_element_seen, it.key()));
            last_element_seen = it.key();
        }
    }
}

TEST_CASE("iterator_implementation_tricky") {
    BTree<int, float, std::less<int>> tree;

    CHECK((tree.begin() == tree.end()));
}

TEST_CASE("const_iterator implementation") {
    BTree<int, float, std::less<int>> tree;

    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13}, last_element_seen = 1;
    float value = 3.14;

    for (int i = 0; i < 15; i++) {
        tree.insert(keys[i], value);
    }

    SUBCASE("test normal iteration") {
        BTree<int, float, std::less<int>>::const_iterator cit = tree.cbegin();

        for (; cit != tree.cend(); ++cit) {
            // We use less_equal so that the lowest number previously seen satisfies the check.
            CHECK(std::less_equal<int>()(last_element_seen, cit.key()));
            last_element_seen = cit.key();
        }
    }
}

TEST_CASE("print iterator") {
    BTree<int, float, std::less<int>> tree;
    float value = 3.14;

    SUBCASE("single element") {
        std::cout << std::endl;
        tree.insert(14, value);
        tree.print();
    }

    SUBCASE("two elements") {
        tree.insert(14, value);
        tree.insert(10, value);
        tree.print();
    }

    SUBCASE("three elements") {
        tree.insert(14, value);
        tree.insert(10, value);
        tree.insert(15, value);
        tree.print();
    }

    SUBCASE("multiple elements") {
        int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13};

        for (int i = 0; i < 15; i++) {
            tree.insert(keys[i], value);
        }

        std::cout << "\nTesting the printing function: ";
        tree.print();
    }
}

TEST_CASE("find method with iterator") {
    BTree<int, float, std::less<int>> tree;
    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13}, last_element_seen = 9;
    float value = 3.14;

    for (int i = 0; i < 15; i++)
        tree.insert(keys[i], value);

    SUBCASE("key found") {
        BTree<int, float, std::less<int>>::iterator it = tree.find(last_element_seen);
        for (; it != tree.end(); ++it) {
            // This should iterate over half of the three, from 9 to 15 included.
            CHECK(std::less_equal<int>()(last_element_seen, it.key()));
            last_element_seen = it.key();
        }
    }

    SUBCASE("key not found makes iterator go to end()") {
        BTree<int, float, std::less<int>>::iterator it = tree.find(9999999);
        CHECK((it == tree.end()));  // Parenthesis around the condition are required in these cases.
    }
}

TEST_CASE("erase fast test") {
    BTree<int, float, std::less<int>> tree;
    int keys[] = {9, 14, 4, 10};
    float value = 3.14;
    for (int i = 0; i < 4; i++)
        tree.insert(keys[i], value);

    SUBCASE("simple erase") {
        REQUIRE(tree.size() == 4);
        tree.erase(4);
        CHECK((tree.find(4) == tree.end()));
        REQUIRE(tree.size() == 3);

        BTree<int, float, std::less<int>>::iterator it = tree.begin();
        for (; it != tree.end(); ++it) {
            CHECK(it.key() != 2);
        }
    }
}

TEST_CASE("erase method") {
    BTree<int, float, std::less<int>> tree;
    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13};
    float value = 3.14;
    for (int i = 0; i < 15; i++)
        tree.insert(keys[i], value);

    SUBCASE("simple erase") {
        REQUIRE(tree.size() == 15);
        tree.erase(2);
        CHECK((tree.find(2) == tree.end()));
        REQUIRE(tree.size() == 14);

        BTree<int, float, std::less<int>>::iterator it = tree.begin();
        for (; it != tree.end(); ++it) {
            CHECK(it.key() != 2);
        }
    }

    SUBCASE("erase in the middle of iteration") {
        BTree<int, float, std::less<int>>::iterator it = tree.begin();
        while (it.key() < 11) {
            ++it;
        }
        tree.erase(14);
        for (; it != tree.end(); ++it) {
            CHECK(it.key() != 14);
        }
    }

    SUBCASE("erase missing key") { REQUIRE_THROWS_AS(tree.erase(999999), KeyNotFound); }
}

TEST_CASE("erase method limit cases") {
    BTree<int, float, std::less<int>> tree;
    REQUIRE(tree.size() == 0);

    SUBCASE("erase on empty") {
        tree.erase(2);
        REQUIRE(tree.size() == 0);

        tree.clear();
        tree.erase(2);
        REQUIRE(tree.size() == 0);
    }

    SUBCASE("erase on one") {
        tree.insert(2, 5.7);
        tree.erase(2);
        REQUIRE(tree.size() == 0);
    }
}

TEST_CASE("square brackets operator") {
    BTree<int, float, std::less<int>> tree;
    int keys[] = {9, 14, 4, 6, 2, 5, 12, 7, 3, 1, 8, 11, 10, 15, 13};
    for (int i = 0; i < 15; i++)
        tree.insert(keys[i], keys[i]);

    SUBCASE("retrieve all the elements") {
        for (int i = 0; i < 15; i++)
            CHECK(tree[keys[i]] == doctest::Approx(keys[i]));
    }
}

TEST_CASE("semantics") {
    BTree<int, float, std::less<int>> tree;
    int keys[] = {9, 14, 4};
    for (int i = 0; i < 3; i++)
        tree.insert(keys[i], keys[i]);

    SUBCASE("copy constructor") {
        BTree<int, float, std::less<int>> tree2 = tree;
        tree.insert(1, 3.14);
        CHECK(tree.size() - tree2.size() == 1);
        tree.insert(2, 3.14);
        tree.insert(3, 3.14);
        CHECK(tree.size() - tree2.size() == 3);
        tree2.insert(10, 3.14);
        CHECK(tree.size() - tree2.size() == 2);
    }

    SUBCASE("move constructor") {
        unsigned int size_before = tree.size();
        BTree<int, float, std::less<int>> tree2{std::move(tree)};
        CHECK(tree2.size() == size_before);
        CHECK(tree.size() == 0);
    }

    SUBCASE("copy assignment operator") {
        BTree<int, float, std::less<int>> tree2;
        tree2 = tree;
        tree.insert(1, 3.14);
        CHECK(tree.size() - tree2.size() == 1);
        tree.insert(2, 3.14);
        tree.insert(3, 3.14);
        CHECK(tree.size() - tree2.size() == 3);
        tree2.insert(10, 3.14);
        CHECK(tree.size() - tree2.size() == 2);
    }

    SUBCASE("move assignment operator") {
        unsigned int size_before = tree.size();
        BTree<int, float, std::less<int>> tree2;
        tree2 = std::move(tree);
        CHECK(tree2.size() == size_before);
        CHECK(tree.size() == 0);
    }
}

#endif
