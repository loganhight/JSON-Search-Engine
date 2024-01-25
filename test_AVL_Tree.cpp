#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "AVL_Tree.h"

#include <string>

TEST_CASE("Check that AVL_Tree<int, int> works", "[AVL_Tree]")
{
    std::cout << "AVL_Tree<int, int> tests" << std::endl;
    AVL_Tree<int, int> testTree;

    // test is_empty, size, contains, insert, clear, balance, height, max, increaseCount, and all rotation functions
    // also test traversal

    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);

    for (int i = 1; i <= 10; i++) {
        testTree.insert(i);
        CHECK_NOTHROW(testTree.is_balanced());
    }

    testTree.traverseLNR();

    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.size() == 10);
    CHECK(testTree.contains(5) == 1);
    CHECK(testTree.contains(99) == 0);

    testTree.clear();
    CHECK_NOTHROW(testTree.is_balanced());

    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);
    CHECK(testTree.contains(1) == 0);

    for (int i = 10; i >= 1; i--) {
        testTree.insert(i);
        CHECK_NOTHROW(testTree.is_balanced());
    }

    testTree.traverseLNR();

    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.size() == 10);
    CHECK(testTree.contains(5) == 1);
    CHECK(testTree.contains(99) == 0);

    // test copy constructor
    // also tests clone function
    std::cout << std::endl << "Copy Constructor Tests:" << std::endl;
    AVL_Tree<int, int> testTreeCopy(testTree);
    CHECK_NOTHROW(testTreeCopy.is_balanced());

    testTreeCopy.traverseLNR();

    CHECK(testTreeCopy.is_empty() == 0);
    CHECK(testTreeCopy.size() == 10);
    CHECK(testTreeCopy.contains(5) == 1);
    CHECK(testTreeCopy.contains(99) == 0);

    // test copy assignment operator
    // also tests clone function
    std::cout << std::endl << "Copy Assignment Operator Tests:" << std::endl;

    AVL_Tree<int, int> testTreeCopyAssign;
    testTreeCopyAssign = testTree;
    CHECK_NOTHROW(testTreeCopyAssign.is_balanced());
 
    testTreeCopyAssign.traverseLNR();

    CHECK(testTreeCopyAssign.is_empty() == 0);
    CHECK(testTreeCopyAssign.size() == 10);
    CHECK(testTreeCopyAssign.contains(5) == 1);
    CHECK(testTreeCopyAssign.contains(99) == 0);

    std::cout << std::endl << "Final Tests:" << std::endl;

    CHECK(testTree.find(7) != nullptr);
    CHECK(testTree.find(99) == nullptr);

    testTree.clear();
    CHECK_NOTHROW(testTree.is_balanced());
    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);

    testTree[1];
    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.size() == 1);
    CHECK(testTree.contains(1) == 1);
    CHECK(testTree.find(1) != nullptr);

    testTree[2];
    testTree[10];
    testTree[7];
    testTree[5];
    testTree[4];
    testTree[6];
    testTree[3];
    testTree[8];
    testTree[9];
    CHECK_NOTHROW(testTree.is_balanced());

    testTree.traverseLNR();

    testTree[1] = 21;
    testTree[11] = 11;
    CHECK_NOTHROW(testTree.is_balanced());

    CHECK(testTree.at(1) == 21);
    CHECK(testTree.at(11) != 10);
    CHECK(testTree.size() == 11);

    testTree.print();
    std::cout << std::endl;

    testTree.increaseCount(1);
    testTree.increaseCount(11);
    testTree.increaseCount(7);

    CHECK(testTree.at(1) == 22);
    CHECK(testTree.at(11) == 12);
    CHECK(testTree.at(7) == 1);

    testTree.print();
    std::cout << std::endl;
}

TEST_CASE("Check that AVL_Tree<string, string> works", "[AVL_Tree]")
{
    std::cout << "AVL_Tree<string, string> tests" << std::endl;
    AVL_Tree<std::string, std::string> testTree;

    // test is_empty, contains, insert, clear, balance, height, max, and all rotation functions
    // also test traversal

    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);

    testTree.insert("a");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("b");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("c");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("d");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("e");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("f");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("g");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("h");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("i");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("j");
    CHECK_NOTHROW(testTree.is_balanced());

    testTree.traverseLNR();

    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.size() == 10);
    CHECK(testTree.contains("c") == 1);
    CHECK(testTree.contains("z") == 0);

    testTree.clear();
    CHECK_NOTHROW(testTree.is_balanced());

    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);
    CHECK(testTree.contains("a") == 0);

    testTree.insert("j");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("i");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("h");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("g");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("f");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("e");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("d");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("c");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("b");
    CHECK_NOTHROW(testTree.is_balanced());
    testTree.insert("a");
    CHECK_NOTHROW(testTree.is_balanced());

    testTree.traverseLNR();

    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.size() == 10);
    CHECK(testTree.contains("a") == 1);
    CHECK(testTree.contains("z") == 0);

    // test copy constructor
    // also tests clone function
    std::cout << std::endl << "Copy Constructor Tests:" << std::endl;
    AVL_Tree<std::string, std::string> testTreeCopy(testTree);
    CHECK_NOTHROW(testTreeCopy.is_balanced());

    testTreeCopy.traverseLNR();

    CHECK(testTreeCopy.is_empty() == 0);
    CHECK(testTreeCopy.size() == 10);
    CHECK(testTreeCopy.contains("a") == 1);
    CHECK(testTreeCopy.contains("z") == 0);

    // test copy assignment operator
    // also tests clone function
    std::cout << std::endl << "Copy Assignment Operator Tests:" << std::endl;

    AVL_Tree<std::string, std::string> testTreeCopyAssign;
    testTreeCopyAssign = testTree;
    CHECK_NOTHROW(testTreeCopyAssign.is_balanced());
 
    testTreeCopyAssign.traverseLNR();

    CHECK(testTreeCopyAssign.is_empty() == 0);
    CHECK(testTreeCopyAssign.size() == 10);
    CHECK(testTreeCopyAssign.contains("a") == 1);
    CHECK(testTreeCopyAssign.contains("z") == 0);

    std::cout << std::endl << "Final Tests:" << std::endl;

    CHECK(testTree.find("f") != nullptr);
    CHECK(testTree.find("z") == nullptr);

    testTree.clear();
    CHECK_NOTHROW(testTree.is_balanced());
    CHECK(testTree.is_empty() == 1);
    CHECK(testTree.size() == 0);

    testTree["a"];
    CHECK(testTree.is_empty() == 0);
    CHECK(testTree.contains("a") == 1);
    CHECK(testTree.find("a") != nullptr);

    testTree["b"];
    testTree["j"];
    testTree["g"];
    testTree["e"];
    testTree["d"];
    testTree["f"];
    testTree["c"];
    testTree["h"];
    testTree["i"];
    CHECK_NOTHROW(testTree.is_balanced());

    testTree.traverseLNR();

    testTree["a"] = "a-value";
    testTree["k"] = "k-Value";
    CHECK_NOTHROW(testTree.is_balanced());
    CHECK(testTree.size() == 11);

    CHECK(testTree.at("a") == "a-value");
    CHECK(testTree.at("k") != "l-value");

    testTree.print();

    std::cout << std::endl;
}

TEST_CASE("Check that AVL_Tree<string, AVL_Tree<string, int>> works", "[AVL_Tree]")
{
    std::cout << "AVL_Tree<string, AVL_Tree<string, int>> tests" << std::endl;
    // this test replicates how the tree will be implemented in the search engine
    
    AVL_Tree<std::string, AVL_Tree<std::string, int>> testTree;

    std::string doc1 = "1234", doc2 = "5678", doc3 = "2121", doc4 = "1010", doc5 = "1111", doc6 = "4444", doc7 = "9998", doc8 = "5643";
    std::string words[] = {"happy", "hello", "logan", "data", "computer", "SMU", "finance", "blockchain", "student", "assignment", "tree"};

    AVL_Tree<std::string, int> currValues;

    for (int i = 0; i < 11; i++) {
        currValues.clear();
        CHECK(currValues.is_empty() == 1);
        currValues[doc1] = rand() % 10 + 1;
        currValues[doc2] = rand() % 10 + 1;
        currValues[doc3] = rand() % 10 + 1;
        currValues[doc4] = rand() % 10 + 1;
        currValues[doc5] = rand() % 10 + 1;
        currValues[doc6] = rand() % 10 + 1;
        currValues[doc7] = rand() % 10 + 1;
        currValues[doc8] = rand() % 10 + 1;

        CHECK_NOTHROW(currValues.is_balanced());

        testTree[words[i]] = currValues;
        std::cout << words[i] << " values:\n"; 
        testTree[words[i]].print();
        std::cout << '\n';
    }

    testTree["tree"].increaseCount(doc8);

    std::cout << "new tree values:\n";
    testTree["tree"].print();
    
    CHECK_NOTHROW(testTree.is_balanced());
    CHECK(testTree.size() == 11);

    std::cout << std::endl;
}

TEST_CASE("Check that large capacity AVL_Tree<int, int>> works", "[AVL_Tree]")
{
    std::cout << "Large capacity AVL_Tree<int, int> tests" << std::endl;
    
    AVL_Tree<int, int> testTree;
    CHECK(testTree.is_empty() == 1);

    for (int i = 0; i < 10000; i++) {
        testTree[rand() % 10000 + 1] = rand() % 100000 + 1;
        CHECK_NOTHROW(testTree.is_balanced());
    }

    std::cout << std::endl;
}
