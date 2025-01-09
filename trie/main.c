#include <stdio.h>
#include <stdlib.h>
typedef struct nn
{
    node *left;
    node *right;
    int key;
} node;

node *create(int key)
{
    node *newNode = malloc(sizeof(node));
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->key = key;
}

void insert(node *root, int key)
{
    node *element = create(key);
    int level = ;
}
int main()
{
    node *root = create(-1);
    insert(root, 0);
}