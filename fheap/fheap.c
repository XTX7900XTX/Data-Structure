#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <limits.h>

typedef struct Node
{
    int key;
    int degree;
    struct Node *child;
    struct Node *leftsib;
    struct Node *rightsib;
    struct Node *parent;
    int flag; // record cascading cut
} n;

typedef struct FibonacciHeap
{
    n *min; // fh's entry
    int nodeCount;
} fh;

void consolidate(fh *heap);

// initial fh
fh *createheap()
{
    fh *heap = (fh *)malloc(sizeof(fh));
    heap->min = NULL;
    heap->nodeCount = 0;
    return heap;
}
// initial node
n *createnode(int key)
{
    n *node = (n *)malloc(sizeof(n));
    node->key = key;
    node->degree = 0;
    node->child = NULL;
    node->parent = NULL;
    node->leftsib = node; // circular linked list so point to itself
    node->rightsib = node;
    node->flag = 0;
    return node;
}

n *matrix[10001]; // record the node for some operation
void insert(fh *heap, int key)
{
    n *node = createnode(key);
    matrix[key] = node; // store in the matrix
    if (heap->min == NULL)
    { // means no node in heap right now
        heap->min = node;
    }
    else
    {
        node->leftsib = heap->min;
        node->rightsib = heap->min->rightsib;
        heap->min->rightsib->leftsib = node;
        heap->min->rightsib = node;

        // update heap min
        if (key < heap->min->key)
        {
            heap->min = node;
        }
    }
    // remember to add nodecount
    heap->nodeCount++;
}

n *extractmin(fh *heap)
{
    n *z = heap->min;
    if (z != NULL)
    {
        // Step1: add z's child into root list
        if (z->child != NULL)
        {
            n *cur = z->child;
            do
            { // separate z's child to root list
                n *next = cur->rightsib;
                cur->leftsib = heap->min;
                cur->rightsib = heap->min->rightsib;
                heap->min->rightsib->leftsib = cur;
                heap->min->rightsib = cur;
                cur->parent = NULL;
                cur = next;
            } while (cur != z->child);
        }
        // Step2: remove z from root list
        if (z == z->rightsib)
        { // only z one node in root list
            heap->min = NULL;
        }
        else
        {
            z->leftsib->rightsib = z->rightsib;
            z->rightsib->leftsib = z->leftsib;
            heap->min = z->rightsib;
            // update heap -> min
            n *cur = heap->min;
            do
            {
                if (cur->key < heap->min->key)
                {
                    heap->min = cur;
                }
                cur = cur->rightsib;
            } while (cur != z->rightsib);
            consolidate(heap);
        }
        // Step3: update nodecount
        heap->nodeCount--;
    }
    return z;
}

void link(n *y, n *x)
{
    // remove y from root list
    y->rightsib->leftsib = y->leftsib;
    y->leftsib->rightsib = y->rightsib;
    // y's parent is x
    y->parent = x;
    if (x->child == NULL)
    {
        x->child = y;
        y->leftsib = y;
        y->rightsib = y;
    }
    else
    {
        // Find the right pos to insert into child list in order
        n *temp = x->child;
        if (y->key < temp->key)
        {
            y->rightsib = temp;
            y->leftsib = temp->leftsib;
            temp->leftsib->rightsib = y;
            temp->leftsib = y;
            x->child = y;
        }
        else
        {
            n *cur = x->child;
            while (y->key > cur->rightsib->key && cur->rightsib != x->child)
            {
                cur = cur->rightsib;
            }
            y->leftsib = cur;
            y->rightsib = cur->rightsib;
            cur->rightsib->leftsib = y;
            cur->rightsib = y;
        }
    }
    x->degree++;
    y->flag = 0;
}

void consolidate(fh *heap)
{
    int maxdegree = (int)(log(heap->nodeCount) / log(2)) + 1;
    n **deg = malloc(sizeof(n *) * maxdegree);
    for (int i = 0; i < maxdegree; i++)
    {
        deg[i] = NULL;
    }

    n *cur = heap->min;
    // maintain a new sorted list
    n *sortlist = NULL;
    n *next;
    do
    {
        next = cur->rightsib;
        if (sortlist == NULL)
        {
            sortlist = cur;
            cur->rightsib = cur;
            cur->leftsib = cur;
        }
        else
        {
            n *temp = sortlist;
            while (cur->key > temp->rightsib->key && temp->rightsib != sortlist)
            {
                temp = temp->rightsib;
            }
            cur->rightsib = temp->rightsib;
            cur->leftsib = temp;
            temp->rightsib->leftsib = cur;
            temp->rightsib = cur;
        }
        cur = next;
    } while (cur != heap->min);
    // now we got the sorted list
    heap->min = NULL;
    // Link the same degree node
    n *x = sortlist;
    do
    {
        next = x->rightsib;
        int nums = x->degree;
        while (deg[nums] != NULL)
        {
            n *y = deg[nums];
            if (x->key > y->key)
            {
                n *temp = x;
                x = y;
                y = temp;
            }
            link(y, x);
            deg[nums] = NULL;
            nums++;
        }
        deg[nums] = x;
        x = next;
    } while (x != sortlist);
    //  update heap->min & rebuild root list
    for (int i = 0; i < maxdegree; i++)
    {
        if (deg[i] != NULL)
        {
            if (heap->min == NULL)
            {
                heap->min = deg[i];
                deg[i]->rightsib = deg[i];
                deg[i]->leftsib = deg[i];
            }
            else
            {
                deg[i]->rightsib = heap->min->rightsib;
                deg[i]->leftsib = heap->min;
                heap->min->rightsib->leftsib = deg[i];
                heap->min->rightsib = deg[i];
                if (deg[i]->key < heap->min->key)
                {
                    heap->min = deg[i];
                }
            }
        }
    }
}

void cut(fh *heap, n *x, n *y)
{
    // Step1: remove x from y's child list
    // handle situation that x is not the only child of y
    if (x->rightsib == x)
    {
        y->child = NULL;
    }
    else
    {
        // y->child pointer point at another child, just adjust pointer to get rid of x
        x->leftsib->rightsib = x->rightsib;
        x->rightsib->leftsib = x->leftsib;
        if (y->child == x)
            y->child = x->rightsib;
    }
    y->degree--;
    // Step2: add x into root list
    x->parent = NULL;
    x->flag = 0;

    x->leftsib = heap->min;
    x->rightsib = heap->min->rightsib;
    heap->min->rightsib->leftsib = x;
    heap->min->rightsib = x;
}

void cascadingcut(fh *heap, n *y)
{
    n *z = y->parent;

    if (z != NULL)
    {
        if (y->flag == 0)
        {
            y->flag = 1;
        }
        else
        {
            //  if marked do cut then recursion up until meet the unmarked node
            cut(heap, y, z); // remove y from z's child list
            cascadingcut(heap, z);
        }
    }
}

void decreasekey(fh *heap, n *cur, int amount)
{
    // Step1: compute newkey value
    int newkey = cur->key - amount;
    if (newkey > cur->key)
    {
        printf("New key is invalid\n");
        return;
    }

    // Step2: update key to newkey
    cur->key = newkey;
    n *y = cur->parent;
    // Step3: if key violate heap property(min/max) then cut
    if (y != NULL && cur->key < y->key)
    {
        cut(heap, cur, y);
        cascadingcut(heap, y);
    }

    // Step4: update heap->min
    if (cur->key < heap->min->key)
    {
        heap->min = cur;
    }
}

// Since Fib heap didn't support delete directly
// We need to update the target node's value to INT_MIN
// then do the extract-min
void delete(fh *heap, n *keytodelete)
{
    // Step2: decrease key value to INT_MIN
    decreasekey(heap, keytodelete, keytodelete->key + INT_MAX);

    // Step3: extract min
    n *minNode = extractmin(heap);
    if (minNode != NULL)
    {
        free(minNode);
    }
}

void printheap(n *root)
{
    n *queue[100];
    int f = 0, r = 0;
    queue[r++] = root;
    while (f < r)
    {
        n *node = queue[f++];
        printf("%d ", node->key);
        n *child = node->child;
        if (child != NULL)
        {
            n *cur = child;
            do
            {
                queue[r++] = cur;
                cur = cur->rightsib;
            } while (cur != child);
        }
    }
    printf("\n");
}

int main()
{
    fh *heap = createheap();
    char command[32];
    n *min = NULL;

    while (scanf("%s", command) != EOF)
    {
        if (strcmp(command, "insert") == 0)
        {
            int key;
            scanf("%d", &key);
            insert(heap, key);
        }
        if (strcmp(command, "delete") == 0)
        {
            int key;
            scanf("%d", &key);
            delete (heap, matrix[key]);
        }
        if (strcmp(command, "decrease") == 0)
        {
            int key, val;
            scanf("%d %d", &key, &val);
            decreasekey(heap, matrix[key], val);
        }
        if (strcmp(command, "extract-min") == 0)
        {
            min = extractmin(heap);
        }
        if (strcmp(command, "exit") == 0)
        {
            break;
        }
    }
    // find degree-0 node
    n *tmp = heap->min;
    do
    {
        if (heap->min->degree == 0)
            break;
        heap->min = heap->min->rightsib;
    } while (heap->min != tmp && heap->min->degree != 0);

    n *cur = heap->min;
    do
    {
        printheap(cur);
        cur = cur->rightsib;
    } while (cur != heap->min);
    free(heap);
    // 釋放 heap、節點記憶體，結束程式
    return 0;
}
