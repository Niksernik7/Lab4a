#include "Tree.h"
#include <unistd.h>

Node* Find(Tree* tree, const char* key) {
    if (tree == NULL)
        return NULL;
    Node* ptr = tree->root;
    while (ptr != NULL) {
        int i = strcmp(ptr->key, key);

        if (i < 0){
            ptr = ptr->right;
        }
        else if (i > 0) {
            ptr = ptr->left;
        }
        else {
            return ptr;
        }
    }
    return NULL;
}

void MyFunc() {
    int x =
#ifdef WIN32
//  Код для Windows
        42
#else
//  Код для *nix
        24
#endif
        ;
}

bool Insert(Tree* tree, char* key, size_t data){
    if (tree == NULL)
        return false;
    int i;
    Node* ptr = NULL;
    for (Node* ptrnext = tree->root; ptrnext; ) {
        ptr = ptrnext;
        i = strcmp(ptr->key, key);
        if (i < 0) {
            ptrnext = ptr->right;
            continue;
        }
        if (i > 0) {
            ptrnext = ptr->left;
            continue;
        }
        if (i == 0) {
            ptr->data = data;
            return false; //эл с таким ключом есть
        }
    }
    Node* newElem = calloc(1, sizeof(Node));
    newElem->key = strdup(key);
    newElem->data = data;
    newElem->parent = ptr;
    if (ptr == NULL) {
        // tree was empty
        tree->root = newElem;
    }
    else
    {
        if (i < 0)
            ptr->right = newElem;
        else
            ptr->left = newElem;
    }
    tree->size++;
    return true;
}

void WalkTree(Node* node, void (*cb)(Node *node, void *arg), void *arg){
    if (node == NULL)
        return;
    WalkTree(node->left, cb, arg);
    cb(node, arg);
    WalkTree(node->right, cb, arg);
}



//Поиск элемента c наименьшим значением ключа, превышающим заданное.

Node* FindMinElemGreaterThen(Tree* tree, const char* key) {
    //  Если мы в узле с ключом <= key, то двигаемся вправо
    // Иначе сравниваем ключ с ранее найденным (если такой был)
    // и сохраняем наименьший, затем двигаемся влево


    if (tree == NULL)
        return NULL;
    Node* ptr = tree->root;
    Node* res = NULL;
    while (ptr != NULL) {
        int i = strcmp(ptr->key, key);
        if (i <= 0){
            ptr = ptr->right;
        }
        else if (i > 0) {
            if (res == NULL || strcmp(res->key, ptr->key) < 0){
                res = ptr;
                ptr = ptr->left;
            }
        }
    }
    return res;
}

bool DeleteByKey(Tree* tree, char* key){
    Node *node = Find(tree, key);
    if (node == NULL) return false;

    if (node->right == NULL && node->left == NULL){
        delete_leaf:
        if (node->parent != NULL){
            if (node->parent->left == node)
                node->parent->left = NULL;
            else
                node->parent->right = NULL;
        }
        if (node == tree->root)
            tree->root = NULL;
        free(node->key);
        free(node);
        return true;
    }
    if (node->left != NULL){
        Node* ptr = node->left;
        while(ptr->right != NULL)
            ptr = ptr->right;
        SwapNodes(node, ptr);
        node = ptr;
        goto delete_leaf;
    } else {
        Node* ptr = node->right;
        while(ptr->left != NULL)
            ptr = ptr->left;
        SwapNodes(node, ptr);
        node = ptr;
        goto delete_leaf;
    }
}


void SwapNodes(Node* node1, Node* node2) {
    const char* sbuf;
    size_t ibuf;
    sbuf = node1->key;
    ibuf = node1->data;
    node1->key = node2->key;
    node1->data = node2->data;
    node2->key = sbuf;
    node2->data = ibuf;
}

void FreeNode(Node *node) {
    if (!node) {
        return;
    }

    FreeNode(node->left);
    FreeNode(node->right);

    free(node);
}

void FreeTree(Tree *tree) {
    FreeNode(tree->root);
    free(tree);
}