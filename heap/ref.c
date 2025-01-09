#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#define INIT_HEAP_CAPACITY 20
#define MAX_HEAP_CAPACITY INT_MAX
#define HEAP_EMPTY_VALUE INT_MAX
#define HEAP_START_INDEX 0
#define PARENT(i) ((i - 1) / 2)
#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)
#define SWAP(type, a, b) \
    {                    \
        type temp = a;   \
        a = b;           \
        b = temp;        \
    }

typedef struct MinHeap
{
    int capacity;
    int size;
    int *arr;
} MinHeap;

MinHeap *createMinHeap();

void insertMinHeap(MinHeap *h, int key);

void deleteMinHeap(MinHeap *h, int key);

void heapifyMinHeap(MinHeap *h, int rootIndex);

void updateKeyMinHeap(MinHeap *h, int oldKey, int newKey);

int extractMinOfMinHeap(MinHeap *h);

int getMinOfMinHeap(MinHeap *h);

void levelOrderTraversal(MinHeap *h);

void freeHeap(MinHeap *h);
MinHeap *createMinHeap()
{
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    h->capacity = INIT_HEAP_CAPACITY;
    h->size = HEAP_START_INDEX;
    h->arr = (int *)malloc(sizeof(int) * h->capacity);
    return h;
}

void insertMinHeap(MinHeap *h, int key)
{
    if (h->size == h->capacity && (h->capacity * 2) < MAX_HEAP_CAPACITY)
    {
        h->capacity *= 2;
        h->arr = (int *)realloc(h->arr, sizeof(int) * h->capacity);
    }

    h->arr[h->size++] = key;

    int cur = h->size - 1;
    while (cur >= 0 && h->arr[PARENT(cur)] > h->arr[cur])
    {
        SWAP(int, h->arr[PARENT(cur)], h->arr[cur]);
        cur = PARENT(cur);
    }
}

void deleteMinHeap(MinHeap *h, int key)
{
    if (h->size == 0)
    {
        printf("Heap is empty.\n");
        return;
    }

    bool hasKey = false;
    int current;
    for (current = 0; current < h->size; current++)
    {
        if (h->arr[current] == key)
        {
            h->arr[current] = h->arr[h->size - 1];
            h->size--;
            hasKey = true;
            break;
        }
    }

    if (hasKey)
    {
        for (current; current >= 0; current--)
        {
            heapifyMinHeap(h, current);
        }
    }
    else
    {
        printf("Key %d not found.\n", key);
    }
}

void heapifyMinHeap(MinHeap *h, int rootIndex)
{
    int key = h->arr[rootIndex];
    int childIndex = LEFT(rootIndex);

    while (childIndex <= (h->size - 1))
    {
        if (childIndex < (h->size - 1))
        {
            if (h->arr[childIndex] > h->arr[childIndex + 1])
            {
                childIndex++; // right child is smaller
            }
        }

        if (key <= h->arr[childIndex])
        {
            break;
        }
        else
        {
            h->arr[PARENT(childIndex)] = h->arr[childIndex];
            childIndex = LEFT(childIndex); // move to left child
        }
    }
    h->arr[PARENT(childIndex)] = key;
}

void updateKeyMinHeap(MinHeap *h, int oldKey, int newKey)
{
    if (h->size == 0)
    {
        printf("Heap is empty.\n");
        return;
    }

    bool hasKey = false;
    int current;
    for (current = 0; current < h->size; current++)
    {
        if (h->arr[current] == oldKey)
        {
            h->arr[current] = newKey;
            hasKey = true;
            break;
        }
    }

    if (hasKey)
    {
        for (current; current >= 0; current--)
        {
            heapifyMinHeap(h, current);
        }
    }
    else
    {
        printf("Key %d not found.\n", oldKey);
    }
}

int extractMinOfMinHeap(MinHeap *h)
{
    if (h->size == 0)
    {
        printf("Heap is empty.\n");
        return HEAP_EMPTY_VALUE;
    }

    int min = h->arr[HEAP_START_INDEX];
    deleteMinHeap(h, min);
    return min;
}

int getMinOfMinHeap(MinHeap *h)
{
    if (h->size == 0)
    {
        printf("Heap is empty.\n");
        return HEAP_EMPTY_VALUE;
    }
    return h->arr[HEAP_START_INDEX];
}

void levelOrderTraversal(MinHeap *h)
{
    for (int i = 0; i < h->size; i++)
    {
        printf("%d ", h->arr[i]);
    }
    printf("\n");
}

void freeHeap(MinHeap *h)
{
    free(h->arr);
    free(h);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main()
{
    MinHeap *heap = createMinHeap();

    insertMinHeap(heap, 1);
    insertMinHeap(heap, 30);
    insertMinHeap(heap, 20);
    insertMinHeap(heap, 25);
    insertMinHeap(heap, 21);
    deleteMinHeap(heap, 1);
    insertMinHeap(heap, 5);
    insertMinHeap(heap, 40);
    deleteMinHeap(heap, 5);

    levelOrderTraversal(heap);                             // 20 21 25 30 40
    printf("Min of minheap: %d\n", getMinOfMinHeap(heap)); // 20

    updateKeyMinHeap(heap, 21, 13);
    levelOrderTraversal(heap); // 13 20 25 30 40

    printf("Extracted min of minheap: %d\n", extractMinOfMinHeap(heap)); // 13
    levelOrderTraversal(heap);                                           // 20 30 25 40

    freeHeap(heap);
    return 0;
}