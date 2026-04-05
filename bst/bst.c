#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct tree tree_t;

struct tree {
    uint32_t root;
    tree_t* left;
    tree_t* right;
};

tree_t left_tree = {.root = 3};
tree_t right_tree = {.root = 6};
tree_t tree = {.root = 5, .left = &left_tree, .right = &right_tree};


/**
    @description Returns true if the BST contains a specific value. 
    @complexity O(n) in worst case (non-balanced BST tree)
**/
bool search_bst(tree_t* tree, uint32_t val){
    if(tree == NULL) return false;
    uint32_t root = tree -> root;
    if(root == val) return true;
    if(val > root) return search_bst(tree -> right, val);
    return search_bst(tree -> left, val);
}

int main(){

    bool in_bst = search_bst(&tree, 7);
    if(in_bst == true) printf("found \n");
    else printf("not found \n");
    
    return 0; 
}