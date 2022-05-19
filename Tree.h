#ifndef LAB4A_TREE_H
#define LAB4A_TREE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>



typedef struct Node{
    const char* key;
    size_t data;
    struct Node* right;
    struct Node* left;
    struct Node* parent;
} Node;

typedef struct Tree{
    Node* root;
    size_t size;
} Tree;



void WalkTree(Node* node, void (*cb)(Node *node, void *arg), void *arg);
Node* Find(Tree*, const char*);
bool Insert(Tree*, char*, size_t);
bool DeleteByKey(Tree* tree, char* key);
Node* FindMinElemGreaterThen(Node*, const char*, Node**);

void SwapNodes(Node*, Node*);
char* get_str(const Node*);
void FreeTree(Tree*);
#endif

