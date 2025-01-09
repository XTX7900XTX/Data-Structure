#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef struct PatriciaNode {
    char *key;               
    void *value;             
    struct PatriciaNode *left;  
    struct PatriciaNode *right; 
    int bitIndex;
} PatriciaNode;

PatriciaNode* createNode(const char *key, void *value, int bitIndex) {
    PatriciaNode *node = (PatriciaNode *)malloc(sizeof(PatriciaNode));
    node->key = strdup(key); 
    node->value = value;
    node->left = node;      
    node->right = node;
    node->bitIndex = bitIndex;
    return node;
}

bool getBit(const char *key, int bitIndex) {
    int byteIndex = bitIndex / 8;       
    int bitInByte = 7 - (bitIndex % 8); 
    if (key[byteIndex] == '\0')   
        return false;
    return (key[byteIndex] & (1 << bitInByte)) != 0;
}


PatriciaNode* insertNode(PatriciaNode *root, const char *key, void *value) {
    if (root == NULL) {
        return createNode(key, value, -1);
    }


    PatriciaNode *current = root;
    PatriciaNode *parent = NULL;
    while (current->bitIndex > (parent ? parent->bitIndex : -1)) {
        parent = current;
        current = getBit(key, current->bitIndex) ? current->right : current->left;
    }

    int diffBit = 0;
    while (key[diffBit / 8] == current->key[diffBit / 8] &&
           diffBit / 8 < strlen(key) && diffBit / 8 < strlen(current->key)) {
        diffBit++;
    }

    PatriciaNode *newNode = createNode(key, value, diffBit);

    PatriciaNode *insertPos = getBit(key, diffBit) ? newNode : current;
    newNode->left = getBit(key, diffBit) ? current : newNode;
    newNode->right = getBit(key, diffBit) ? newNode : current;

    if (!parent) {
        return newNode;
    }

    if (parent->left == current) {
        parent->left = insertPos;
    } else {
        parent->right = insertPos;
    }

    return root;
}

void freeNode(PatriciaNode *node) {
    if (node) {
        free(node->key);
        free(node);
    }
}

bool isLeaf(PatriciaNode *node) {
    return node->left == node && node->right == node;
}

PatriciaNode* deleteNode(PatriciaNode *root, const char *key) {
    if (root == NULL) {
        return NULL; 
    }

    PatriciaNode *current = root;
    PatriciaNode *parent = NULL;
    PatriciaNode *grandparent = NULL;

    while (current->bitIndex > (parent ? parent->bitIndex : -1)) {
        grandparent = parent;
        parent = current;
        current = getBit(key, current->bitIndex) ? current->right : current->left;
    }

    if (strcmp(current->key, key) != 0) {
        printf("Key not found: %s\n", key);
        return root; 
    }

    if (parent == NULL) {
        freeNode(current);
        return NULL; 
    }

    if (parent->left == current) {
        parent->left = (current == current->left) ? parent : current->left;
    } else {
        parent->right = (current == current->right) ? parent : current->right;
    }

    freeNode(current);

    if (isLeaf(parent)) {
        if (grandparent) {
            if (grandparent->left == parent) {
                grandparent->left = (parent == parent->left) ? grandparent : parent->left;
            } else {
                grandparent->right = (parent == parent->right) ? grandparent : parent->right;
            }
        } else {
            root = parent->left == parent ? parent->right : parent->left;
        }
        freeNode(parent);
    }

    return root;
}

int main() {
    PatriciaNode *root = NULL;

    root = insertNode(root, "apple", (void *)"Fruit");
    root = insertNode(root, "orange", (void *)"Fruit");
    root = insertNode(root, "grape", (void *)"Fruit");
    root = deleteNode(root, "orange");

    printf("After deletion, root: %s\n", root->key);
    printf("Left: %s\n", root->left->key);
    printf("Right: %s\n", root->right->key);
}