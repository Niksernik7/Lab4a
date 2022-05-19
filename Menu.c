#include "Menu.h"
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef WIN32
#include <windows.h>
#endif

char* enterString()
{
    char buf[81] = { 0 };
    char* res = NULL;
    size_t len = 0;
    int n;
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0)
        {
            if (!res)
            {
                return NULL;
            }
        }
        else if (n > 0) {
            size_t chunk_len = strlen(buf);
            size_t str_len = len + chunk_len;
            res = realloc(res, str_len + 2);
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        }
        else {
            scanf("%*c");
        }
    } while (n > 0);
    if (len > 0)
    {
        res[len] = '\0';
    }
    else {
        res = calloc(1, sizeof(char));
        if (res != NULL)
            *res = '\0';
    }
    return res;
}
void ShowFindMenu() {
    printf("Find\n");
    printf("Choice\n");
    printf("1) Special find by key(minimal, that is more than key\n");
    printf("2) Find by key)\n");
    printf("0) Exit\n");
    printf(" : ");
}

void ShowDeleteMenu() {
    printf("1) Delete by key\n");
    printf("0) Exit\n");
    printf(" : ");
}

void ShowError() {
    printf("Miss choice\n");
}

void ShowMainMenu() {
    printf("Menu\n");
    printf("Choice\n");
    printf("1. Print tree\n");
    printf("2. Insert node\n");
    printf("3. Delete node\n");
    printf("4. Find node in tree\n");
    printf("0. Quit\n");
    printf(" : ");
}

void printNode(const Node* i) {
    char* s;
    if (i != NULL) {
        s = get_str(i);
        printf("%s\n", s);
        free(s);
    }
    else {
        printf("Node with this key doesn't exist\n");
    }
}

//?????????????????????????????????????????????????
//void printTree(const Tree* t) {
//    char* str = NULL;
//    if (t != NULL) {
//        str = TransformTableString(t);
//    } else {
//        printf("Doesn't exist\n");
//        return;
//    }
//    printf("Output\n");
//    if (str != NULL) {
//        printf("%s\n", str);
//        free(str);
//    } else printf("Table is empty\n");
//}

int GetInt() {
    char* str = "Doesn't number!\n : ";
    char* ptr;
    do {
        char* input = (char*) enterString();
        int n = (int)strtol(input, &ptr, 10);
        if (input == ptr - strlen(input) && strlen(input) != 0) {
            free(input);
            return n;
        }
        printf("%s", str);
        free(input);
    } while (1);
}



void Menu() {
    Tree *tree = Create();
    do {
        ShowMainMenu();
        int n = GetInt();
        switch (n) {
            case 0: {
                FreeTree(tree);
                return;
            }
            case 1: {
                PrintGV(tree);
                break;
            }
            case 2: {
                AddNode(tree);
                break;
            }
            case 3: {
                ShowDeleteMenu();
                n = GetInt();
                Delete(tree, n);
                break;
            }
            case 4: {
                ShowFindMenu();
                n = GetInt();
                FindInTree(tree, n);
                break;
            }
            default: {
                ShowError();
                break;
            }
        }
    } while (1);
}

Tree* Create() {
    Tree* tree = calloc(1, sizeof(Tree));
    return tree;
}

void Delete(Tree *tree, int mod) {
    char* key, * msg = "";
    switch (mod) {
        case 0:
            break;

        case 1: {
            printf("Enter:\n");
            printf(" Key : ");
            do {
                printf("%s", msg);
                key = enterString();
                msg = "Key cannot be empty\n Key : ";
                if(strlen(key) != 0) break;
                free(key);
            } while (1);
            if(!DeleteByKey(tree, key))
                printf("Doesn't exist\n");
            free(key);
            break;
        }
    }
}

void AddNode(Tree* tree) {
    char* str = "Data cannot be empty\n Data : ";
    printf(" Enter\n");
    printf(" Key : ");
    char* key;
    do {
        key = (char*) enterString();
        if (strlen(key) != 0)
            break;
        free(key);
        printf("%s", str);
    } while (1);
    printf(" Data : ");
    size_t data;
    data = GetInt();
    if (!Insert(tree, key, data)) {
        printf("The elem with this key exists\n");
    }
    free(key);
}

void FindInTree(Tree* tree, size_t mod) {
    switch (mod) {
        case 0:
            break;
        case 1: {
            printf("Enter:\n");
            printf(" Key : ");
            char* key = enterString();
            Node** res = NULL;
            Node* node;
            node = FindMinElemGreaterThen(node, key, res);
            if (node != NULL) {
                printNode(node);
            } else printf("Doesn't exist\n");
            free(key);
            break;
        }
        case 2: {
            printf("Enter:\n");
            printf(" Key : ");
            char *key = enterString();
            Node* node = Find(tree, key);
            free(key);
            printNode(node);
            break;
        }
        default:
            ShowError();
            break;
    }
}
char* get_str(const Node* item) {
    if (item == NULL)
        return NULL;
    size_t lenkey = strlen(item->key);
    size_t buflen = lenkey + 3 + 30 + 1;
    char* s = malloc(buflen);
    snprintf(s, buflen, "%s | %zu");
    return s;
}

#include <time.h>
#include <unistd.h>

void PrintGV(Tree* tree){
    fprintf(stderr, "generating GraphViz file...\n");
    FILE* f;
    char nfile[] = "TreeGV.XXXXXX";
    char gfile[L_tmpnam + 4] = "Graph.XXXXXX";
    char cmd[PATH_MAX + 10];

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);

    int fd = mkstemp(nfile);
    mktemp(gfile);
    strcat(gfile, ".png");

    f = fdopen(fd, "w");
    if (f == NULL) {
        fprintf(stderr, "Could not create temporary file: %s\n", strerror(errno));
        return;
    }
    fprintf(f, "digraph Tree {\n");
    WalkTree(tree->root, GenerateGV, f);
    fprintf(f, "}\n");
    fclose(f);

    fprintf(stderr, "running GraphViz...\n");
    snprintf(cmd, sizeof(cmd),"dot -o%s -Tpng %s", gfile, nfile);
    system(cmd);

    fprintf(stderr, "showing picture...\n");
#ifdef WIN32
    snprintf(cmd, sizeof(cmd),",mspaint %s", gfile);
#else
    snprintf(cmd, sizeof(cmd),"xdg-open %s", gfile);
#endif
    system(cmd);

#ifdef WIN32
    DeleteFile(nfile);
    DeleteFile(gfile);
#else
    unlink(nfile);
    unlink(gfile);
#endif
}

void GenerateGV(Node* node, void* p){      //callback(cb)
    FILE* f = p;
    if (node->left != NULL){
        fprintf(f, "\"%s: %zu\" -> \"%s: %zu\"\n", node->key, node->data, node->left->key, node->left->data);
    }
    if (node->right != NULL){
        fprintf(f, "\"%s: %zu\" -> \"%s: %zu\"\n", node->key, node->data, node->right->key, node->right->data);
    }
}