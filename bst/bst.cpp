#include <iostream>

class Node {
public:
    int value;
    Node* left;
    Node* right;

    Node(int val) : value(val), left(nullptr), right(nullptr) {}
};

class BST {
private:
    Node* root;

    Node* findNode(Node* node, int val) {
        if (node == nullptr) return nullptr;

        if (node->value == val) return node;

        if (val > node->value)
            return findNode(node->right, val);

        return findNode(node->left, val);
    }

    Node* insertNode(Node* node, int val) {
        if (node == nullptr) {
            return new Node(val);
        }

        if (val > node->value)
            node->right = insertNode(node->right, val);
        else
            node->left = insertNode(node->left, val);

        return node;
    }

    Node* removeNode(Node* node, int val){
        if(node == nullptr || node -> value == val){
            return nullptr;
        }

        if (val > node->value)
            node->right = removeNode(node->right, val);
        else
            node->left = removeNode(node->left, val);

        return node;
    }

public:
    BST() : root(nullptr) {}

    void insert(int val) {
        root = insertNode(root, val);
    }

    void remove(int val){
        removeNode(root, val);
    }

    bool find(int val) {
        return findNode(root, val);
    }
};

int main() {
    BST bst;

    bst.insert(5);
    bst.insert(3);
    bst.insert(6);

    std::cout << bst.find(3) << std::endl; 
    std::cout << bst.find(6) << std::endl; 
    bst.remove(6);
    std::cout << bst.find(6) << std::endl; 

    return 0;
}