#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

template <typename Comparable, typename valueType>

class AVL_Tree {

private:
    struct AVLNode {
        Comparable key;
        valueType values; 
        AVLNode *left;
        AVLNode *right;
        int height;

        // default constructor - value defined
        AVLNode(const Comparable &theKey, const valueType &theValues, AVLNode *l, AVLNode *r, int h)
            : key{theKey}, values{theValues}, left{l}, right{r}, height{h} {}

        // additional constructor - no value defined
        AVLNode(const Comparable &theKey, AVLNode *l, AVLNode *r, int h)
            : key{theKey}, values{}, left{l}, right{r}, height{h} {}
    };

    AVLNode *root;

    // used for balancing
    static const int ALLOWED_IMBALANCE = 1;

public:

    // default constructor
    AVL_Tree() : root{nullptr} {}

    /*
    ** Rule-of-3
    */

    // copy constructor
    AVL_Tree(const AVL_Tree &rhs) : root{nullptr} {
        root = clone(rhs.root);
    }

    // destructor
    ~AVL_Tree() {
        clear();
    }

    // copy assignment operator
    AVL_Tree &operator=(const AVL_Tree &rhs) {
        if (this == &rhs) {
            return *this;
        }

        clear();
        root = clone(rhs.root);
        return *this;
    }

    // returns the keys of a tree as a vector of strings
    void treeToVect(std::vector<std::string> &toEdit) {
        return treeToVect(toEdit, root);
    }

    // overloaded[] operator so that the tree can properly function as a map
    valueType &operator[](const Comparable &rhs) {
        
        // inserts node with specified key if it is not already in the tree
        if (find(rhs, this->root) == nullptr) {
            insert(rhs, this->root);
        }

        // returns the values of the requested node
        AVLNode *found = find(rhs, this->root);
        return found->values;

    }

    // returns the values of the parameter key - error if key is not in tree
    valueType at(const Comparable &rhs) {
        if (find(rhs, this->root) == nullptr) {
            std::cerr << "Error with at() function - specified element not found" << std::endl;
        }

        AVLNode *found = find(rhs, this->root);
        return found->values;
    }

    // checks if the tree contains the parameter key
    bool contains(const Comparable &toCheck) {
        return contains(toCheck, root);
    }

    // checks if the tree is empty
    bool is_empty() const {
        return root == nullptr;
    }

    // returns the size of the tree - total number of elements
    int size() {
        return size(root);
    }

    // checks if the tree is balanced
    int is_balanced() {
        return is_balanced(root);
    }

    // deletes every node in the tree
    void clear() {
        clear(root);
    }

    // inserts the desired key - no value defined
    void insert(const Comparable &insertKey) {
        insert(insertKey, root);
    }

    // inserts the desired key - with value defined
    void insert(const Comparable &insertKey, const valueType &insertValue) {
        insert(insertKey, insertValue, root);
    }

    // returns a pointer to the paramter node - if not found: nullptr
    AVLNode *find(const Comparable &toFind) {
        return find(toFind, root);
    }

    // in-order traversal of the tree - only prints keys
    void traverseLNR() {
        traverseLNR(root);
        std::cout << std::endl;
    }

    // prints the tree's contents - used for testing
    void print() {
        print(root);
    }

    // prints the contents of a tree of trees - used for testing
    void printDoubleTree() {
        printDoubleTree(root);
    }

    // prints the contents of a tree with a vector valueType - used for testing
    void printTreeVector() {
        printTreeVector(root);
    }

    // prints the contents of a tree of trees where the inner trees have a vector valueType - used for testing
    void printDoubleTreeVector() {
        printDoubleTreeVector(root);
    }

    // increases the values of the parameter key by 1 (should only be used with integer valueType)
    void increaseCount(const Comparable &toIncrease) {
        if (find(toIncrease, this->root) == nullptr) {
            std::cerr << "Error with increaseCount() function - specified element not found" << std::endl;
        }

        AVLNode *found = find(toIncrease, this->root);
        found->values++;
    }

    // writes the wordIndex to the parameter file - structured format (should only be used with word index)
    void writeWordIndexToFile(const std::string &fileName) {
        std::ofstream writeStream;
        writeStream.open(fileName);
        if (!writeStream.is_open()) { std::cerr << "Error! Unable to create file: " << fileName << std::endl; }
        writeWordIndexToFile(writeStream, root);
        writeStream.close();
    }

    // writes an index to the parameter file - structured format (should only be used with organizaton index and person index)
    void writeIndexToFile(const std::string &fileName) {
        std::ofstream writeStream;
        writeStream.open(fileName);
        if (!writeStream.is_open()) { std::cerr << "Error! Unable to create file: " << fileName << std::endl; }
        writeIndexToFile(writeStream, root);
        writeStream.close();
    }

    // helper function for writeIndexToFile() - writes the inner value trees in structured format
    void writeValuesTreeToFile(std::ofstream &writeStream) {
        writeValuesTreeToFile(writeStream, root);
    }

    // helper function for writeWordIndexToFile() - writes the inner value trees in structured format
    void writeWordValuesTreeToFile(std::ofstream &writeStream) {
        writeWordValuesTreeToFile(writeStream, root);
    }

    // writes the docIndex to the parameter file - structured format (should only be used with doc index)
    void writeDocIndexToFile(const std::string &fileName) {
        std::ofstream writeStream;
        writeStream.open(fileName);
        if (!writeStream.is_open()) { std::cerr << "Error! Unable to create file: " << fileName << std::endl; }
        writeDocIndexToFile(writeStream, root);
        writeStream.close();
    }

private:

    // inserts the desired key - no value defined
    void insert(const Comparable &insertKey, AVLNode *&node) {
        if (node == nullptr) {
            // construct - without value defined
            node = new AVLNode{insertKey, nullptr, nullptr, 0};
            return;
        }

        if (insertKey < node->key) {
            insert(insertKey, node->left);
        }
        else if (insertKey > node->key) {
            insert(insertKey, node->right);
        }
        else {
            // duplicate
        }

        balance(node);
    }

    // inserts the desired key - with value defined
    void insert(const Comparable &insertKey, const valueType &insertValue, AVLNode *&node) {
        if (node == nullptr) {
            // construct - with value defined
            node = new AVLNode{insertKey, insertValue, nullptr, nullptr, 0};
            return;
        }

        if (insertKey < node->key) {
            insert(insertKey, node->left);
        }
        else if (insertKey > node->key) {
            insert(insertKey, node->right);
        }
        else {
            // duplicate
        }

        balance(node);
    }

    // returns a pointer to the parameter node - if not found: nullptr
    AVLNode *find(const Comparable &toFind, AVLNode *&node) const {
        // not found
        if (node == nullptr) {
            return nullptr;
        }

        if (toFind < node->key) {
            return find(toFind, node->left);
        }
        else if (toFind > node->key) {
            return find(toFind, node->right);
        }

        // now toFind == node->key

        return node;
    }

    // checks if the tree contains the parameter value
    bool contains(const Comparable &toCheck, AVLNode *&node) {
        // not found
        if (node == nullptr) {
            return false;
        }

        if (toCheck < node->key) {
            return contains(toCheck, node->left);
        }
        else if (toCheck > node->key) {
            return contains(toCheck, node->right);
        }

        // now toCheck == node->key
        return true;
    }

    // returns the size of the tree - total number of elements
    int size(AVLNode *&node) {
        if (node == nullptr) {
            return 0;
        }

        return (size(node->left) + size(node->right) + 1);
    }

    // checks if the tree is balanced - throws error if not balanced - used for testing
    int is_balanced(AVLNode *&node) {
        if (node == nullptr) {
            return -1;
        }

        int leftHeight = is_balanced(node->left) + 1;
        int rightHeight = is_balanced(node->right) + 1;

        if (std::abs(leftHeight - rightHeight) > 1) {
            throw std::invalid_argument("Error: Tree is not balanced");
        }

        int trueHeight = max(leftHeight, rightHeight);

        if (trueHeight != node->height) {
            throw std::invalid_argument("Error: Node does not have correct height");
        }

        return trueHeight;
    }

    // deletes every node in the tree
    void clear(AVLNode *&node) {
        if (node == nullptr) {
            return;
        }

        clear(node->left);
        clear(node->right);
        delete node;
        node = nullptr;
    }

    // creates a copy of the tree - used in copy constructor and copy assignment operator
    AVLNode *clone(AVLNode *node) const {
        if (node == nullptr) {
            return nullptr;
        }

        // construct - with value defined
        return new AVLNode(node->key, node->values, clone(node->left), clone(node->right), node->height);
    }

    // in-order traversal of the tree - only prints keys
    void traverseLNR(AVLNode *&node) const {
        if (node == nullptr) {
            return;
        }
        traverseLNR(node->left);
        std::cout << node->key << " ";
        traverseLNR(node->right);
    }

    // prints the tree's contents - used for testing
    void print(AVLNode *&node) const {
        if (node == nullptr) {
            return;
        }
        print(node->left);
        std::cout << "Key: " << node->key << " -- Values: " << node->values << std::endl;
        print(node->right);
    }

    // prints the contents of a tree of trees - used for testing
    void printDoubleTree(AVLNode *&node) const {
        if (node == nullptr) {
            return;
        }
        printDoubleTree(node->left);
        std::cout << node->key << " values:" << std::endl;
        node->values.print();
        std::cout << std::endl;
        printDoubleTree(node->right);
    }

    // prints the contents of a tree with a vector valueType - used for testing
    void printTreeVector(AVLNode *&node) const {
        if (node == nullptr) {
            return;
        }
        printTreeVector(node->left);
        std::cout << "DocID: " << node->key << " -- Title: " << node->values.at(0) << " -- Publication: " << node->values.at(1) << " -- Date: " << node->values.at(2) << std::endl;
        printTreeVector(node->right);
    }

    // prints the contents of a tree of trees where the inner trees have a vector valueType - used for testing
    void printDoubleTreeVector(AVLNode *&node) const {
        if (node == nullptr) {
            return;
        }
        printDoubleTreeVector(node->left);
        std::cout << node->key << " values:" << std::endl;
        node->values.printTreeVector();
        std::cout << std::endl;
        printDoubleTreeVector(node->right);
    }

    // returns the keys of a tree as a vector of strings
    void treeToVect(std::vector<std::string> &toEdit, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }

        treeToVect(toEdit, node->left);
        toEdit.push_back(node->key);
        treeToVect(toEdit, node->right);
    }

    // writes the wordIndex to the specified file - structured format (should only be used with word index)
    void writeWordIndexToFile(std::ofstream &writeStream, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }
        writeWordIndexToFile(writeStream, node->left);

        writeStream << node->key << "/";        // main key (word)
        node->values.writeWordValuesTreeToFile(writeStream);
        writeStream << std::endl;

        writeWordIndexToFile(writeStream, node->right);
    }

    // writes an index to the specified file - structured format (should only be used with organizaton index and person index)
    void writeIndexToFile(std::ofstream &writeStream, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }
        writeIndexToFile(writeStream, node->left);

        writeStream << node->key << "|";        // main key (organization or person)
        node->values.writeValuesTreeToFile(writeStream);
        writeStream << std::endl;

        writeIndexToFile(writeStream, node->right);
    }

    // helper function for writeIndexToFile() - writes the inner value trees in structured format
    void writeValuesTreeToFile(std::ofstream &writeStream, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }
        writeValuesTreeToFile(writeStream, node->left);

        writeStream << node->key << "|";      // docID

        writeValuesTreeToFile(writeStream, node->right);
    }

    // helper function for writeWordIndexToFile() - writes the inner value trees in structured format
    void writeWordValuesTreeToFile(std::ofstream &writeStream, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }
        writeWordValuesTreeToFile(writeStream, node->left);

        writeStream << node->key << "|";        // docID
        writeStream << node->values << "}";     // count

        writeWordValuesTreeToFile(writeStream, node->right);
    }

    // writes the docIndex to the specified file - structured format (should only be used with doc index)
    void writeDocIndexToFile(std::ofstream &writeStream, AVLNode *&node) {
        if (node == nullptr) {
            return;
        }
        writeDocIndexToFile(writeStream, node->left);

        writeStream << node->key << "|";            // docID
        writeStream << node->values.at(0) << "|";   // title
        writeStream << node->values.at(1) << "|";   // publication
        writeStream << node->values.at(2) << "|";   // date
        writeStream << node->values.at(3) << "^";   // full text
        writeStream << std::endl;

        writeDocIndexToFile(writeStream, node->right);
    }

    // returns the height of the specified node
    int height(AVLNode *&node) const {
        return node == nullptr ? -1 : node->height;
    }

    // balances the tree - used when inserting an node
    void balance(AVLNode *&node) {
        if (node == nullptr) {
            return;
        }

        if (height(node->left) - height(node-> right) > ALLOWED_IMBALANCE) {
            if (height(node->left->left) >= height(node->left->right)) {
                rotateWithLeftChild(node);
            }
            else {
                doubleRotationWithLeftChild(node);
            }
        }
        else if (height(node->right) - height(node->left) > ALLOWED_IMBALANCE) {
            if (height(node->right->right) >= height (node->right->left)) {
                rotateWithRightChild(node);
            }
            else {
                doubleRotationWithRightChild(node);
            }
        }

        // otherwise - no imbalance

        node->height = max(height(node->left) , height(node->right)) + 1;
    }

    // returns the greater value of the two parameters
    int max(const int &lhs, const int &rhs) {
        return lhs > rhs ? lhs : rhs;
    }

    // rotation method used for balancing
    void rotateWithLeftChild (AVLNode *&k2) {
        AVLNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2 = k1;
    }

    // rotation method used for balancing
    void rotateWithRightChild (AVLNode *&k1) {
        AVLNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1 = k2;
    }

    // rotation method used for balancing
    void doubleRotationWithLeftChild(AVLNode *&k3) {
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    // rotation method used for balancing
    void doubleRotationWithRightChild(AVLNode *&k1) {
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }

};

#endif
