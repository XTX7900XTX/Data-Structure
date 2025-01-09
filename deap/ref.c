#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <stdbool.h>
#include <assert.h>
#define INIT_HEAP_CAPACITY 20
#define MAX_HEAP_CAPACITY INT_MAX
#define HEAP_START_INDEX 1
#define MIN_HEAP_START_INDEX 1
#define MAX_HEAP_START_INDEX 2
#define PARENT(i) (((i) - 1) / 2)
#define LEFT(i) ((2 * (i)) + 1)
#define RIGHT(i) ((2 * (i)) + 2)
#define SWAP(type, a, b) \
    {                    \
        type temp = a;   \
        a = b;           \
        b = temp;        \
    }

typedef struct
{
    int key;
    void *data;
} Element;

typedef struct
{
    int capacity;
    int size;
    Element **arr;
} Deap;

Deap *createDeap();

int getLevel(int index);

bool _isInMinHeap(Deap *h, int index);

int _getPartner(Deap *h, int index);

void _minHeapInsert(Deap *h, int index);

void _maxHeapInsert(Deap *h, int index);

void insertDeap(Deap *h, Element *item);

void modifyInsertDeap(Deap *h, int cur);

Element *extractMinOfDeap(Deap *h);

Element *getMinOfDeap(Deap *h);

Element *extractMaxOfDeap(Deap *h);

Element *getMaxOfDeap(Deap *h);

void levelOrderTraversal(Deap *h);

#define LEVEL_ORDER_TRAVERSAL(type, heap, printFunc)              \
    do                                                            \
    {                                                             \
        for (int i = MIN_HEAP_START_INDEX; i < (heap)->size; i++) \
        {                                                         \
            printFunc((type)((heap)->arr[i]));                    \
        }                                                         \
        printf("\n");                                             \
    } while (0)

void freeDeap(Deap *h);

Deap *createDeap()
{
    Deap *h = (Deap *)malloc(sizeof(Deap));
    h->capacity = INIT_HEAP_CAPACITY;
    h->size = MIN_HEAP_START_INDEX;
    h->arr = (Element **)calloc(h->capacity, sizeof(Element *));
    return h;
}

int getLevel(int index)
{
    int level = 1; // root level
    while (index > 0)
    {
        index = PARENT(index);
        level++;
    }
    return level;
}

bool _isInMinHeap(Deap *h, int index)
{
    int level = getLevel(index);
    // total nodes = 2^level - 1
    int max_index = ((1 << level) - 1) - 1;
    int min_index = (1 << (level - 1)) - 1;
    int half = (max_index - min_index + 1) / 2;
    if (index < min_index + half)
        return true;
    else
        return false;
}

int _getPartner(Deap *h, int index)
{
    int level = getLevel(index);
    int max_index = ((1 << level) - 1) - 1;
    int min_index = (1 << (level - 1)) - 1;
    int half = (max_index - min_index + 1) / 2;
    if (index < min_index + half)
    {
        /* min heap, max partner*/
        if (index + half < h->size)
            return index + half;
        else
            return PARENT(index + half); // no partner
    }
    else
    {
        /* max heap, min partner */
        if (index - half < h->size)
            return index - half;
        else
            return PARENT(index - half); // no partner
    }
}

void _minHeapInsert(Deap *h, int index)
{
    while (index > MIN_HEAP_START_INDEX && h->arr[PARENT(index)]->key > h->arr[index]->key)
    {
        SWAP(Element *, h->arr[index], h->arr[PARENT(index)]);
        index = PARENT(index);
    }
}

void _maxHeapInsert(Deap *h, int index)
{
    while (index > MAX_HEAP_START_INDEX && h->arr[PARENT(index)]->key < h->arr[index]->key)
    {
        SWAP(Element *, h->arr[index], h->arr[PARENT(index)]);
        index = PARENT(index);
    }
}

void insertDeap(Deap *h, Element *item)
{
    if (h->size == h->capacity && (h->capacity * 2) < MAX_HEAP_CAPACITY)
    {
        h->capacity *= 2;
        h->arr = (Element **)realloc(h->arr, h->capacity * sizeof(Element *));
        for (int i = h->size; i < h->capacity; i++)
        {
            h->arr[i] = NULL;
        }
    }
    h->arr[h->size++] = item;

    if (h->size == 2)
        return; /*  insert into empty deap */

    int cur = h->size - 1;
    if (!_isInMinHeap(h, cur))
    {
        int partner = _getPartner(h, cur);
        // printf("%d %d %d\n", item->key, cur, partner);
        if (item->key < h->arr[partner]->key)
        {
            SWAP(Element *, h->arr[cur], h->arr[partner]);
            _minHeapInsert(h, partner);
        }
        else
        {
            _maxHeapInsert(h, cur);
        }
    }
    else
    {
        int partner = _getPartner(h, cur);
        // printf("%d %d %d\n", item->key, cur, partner);
        if (item->key > h->arr[partner]->key)
        {
            SWAP(Element *, h->arr[cur], h->arr[partner]);
            _maxHeapInsert(h, partner);
        }
        else
        {
            _minHeapInsert(h, cur);
        }
    }
}

void modifyInsertDeap(Deap *h, int cur)
{
    if (!_isInMinHeap(h, cur))
    {
        int partner = _getPartner(h, cur);
        if (h->arr[cur]->key < h->arr[partner]->key)
        {
            SWAP(Element *, h->arr[cur], h->arr[partner]);
            _minHeapInsert(h, partner);
        }
        else
        {
            _maxHeapInsert(h, cur);
        }
    }
    else
    {
        int partner = _getPartner(h, cur);
        if (h->arr[cur]->key > h->arr[partner]->key)
        {
            SWAP(Element *, h->arr[cur], h->arr[partner]);
            _maxHeapInsert(h, partner);
        }
        else
        {
            _minHeapInsert(h, cur);
        }
    }
}

Element *extractMinOfDeap(Deap *h)
{
    if (h->size == 1)
        return NULL;
    if (h->size == 2)
    {
        Element *min = h->arr[MIN_HEAP_START_INDEX];
        h->arr[MIN_HEAP_START_INDEX] = NULL;
        h->size--;
        return min;
    }
    Element *min = h->arr[MIN_HEAP_START_INDEX];
    Element *tmp = h->arr[h->size - 1];
    h->arr[h->size - 1] = NULL;
    h->size--;

    int cur = MIN_HEAP_START_INDEX;
    while (LEFT(cur) <= (h->size - 1))
    { /* cur has left child */
        int child = LEFT(cur);
        if (child < (h->size - 1))
        { /* cur has right child */
            if (h->arr[child]->key > h->arr[child + 1]->key)
                child++; /* right child is smaller */
        }
        h->arr[cur] = h->arr[child];
        h->arr[child] = NULL;
        cur = child;
    }

    h->arr[cur] = tmp;
    modifyInsertDeap(h, cur);
    return min;
}

Element *getMinOfDeap(Deap *h)
{
    if (h->size == 1)
        return NULL;
    return h->arr[MIN_HEAP_START_INDEX];
}

Element *extractMaxOfDeap(Deap *h)
{
    if (h->size == 1)
        return NULL;
    if (h->size == 2)
    {
        Element *max = h->arr[MIN_HEAP_START_INDEX];
        h->arr[MIN_HEAP_START_INDEX] = NULL;
        h->size--;
        return max;
    }
    Element *max = h->arr[MAX_HEAP_START_INDEX];
    Element *tmp = h->arr[h->size - 1];
    h->arr[h->size - 1] = NULL;
    h->size--;

    int cur = MAX_HEAP_START_INDEX;
    while (LEFT(cur) <= (h->size - 1))
    { /* cur has left child */
        int child = LEFT(cur);
        if (child < (h->size - 1))
        { /* cur has right child */
            if (h->arr[child]->key < h->arr[child + 1]->key)
                child++; /* right child is larger */
        }
        h->arr[cur] = h->arr[child];
        h->arr[child] = NULL;
        cur = child;
    }

    h->arr[cur] = tmp;
    modifyInsertDeap(h, cur);
    return max;
}

Element *getMaxOfDeap(Deap *h)
{
    if (h->size == 1)
        return NULL;
    if (h->size == 2)
        return h->arr[1];
    return h->arr[MAX_HEAP_START_INDEX];
}

void levelOrderTraversal(Deap *h)
{
    for (int i = MIN_HEAP_START_INDEX; i < h->size; i++)
    {
        printf("%d ", h->arr[i]->key);
    }
    printf("\n");
}

void freeDeap(Deap *h)
{
    free(h->arr);
    free(h);
}

typedef struct
{
    int age;
    char name[20];
} Person;

void printKey(const Element *item)
{
    printf("%d ", item->key);
}

void printAgeOfPerson(const Element *item)
{
    printf("%d ", ((Person *)item->data)->age);
}

void test_getPartner()
{
    Deap *h = createDeap();
    h->size = 11;
    assert(_getPartner(h, 1) == 2);
    assert(_getPartner(h, 2) == 1);
    assert(_getPartner(h, 3) == 5);
    assert(_getPartner(h, 4) == 6);
    assert(_getPartner(h, 5) == 3);
    assert(_getPartner(h, 6) == 4);
    assert(_getPartner(h, 7) == 5);
    assert(_getPartner(h, 8) == 5);
    assert(_getPartner(h, 9) == 6);
    assert(_getPartner(h, 10) == 6);
    assert(_getPartner(h, 11) == 7);
    assert(_getPartner(h, 12) == 8);
    assert(_getPartner(h, 13) == 9);
    assert(_getPartner(h, 14) == 10);
    freeDeap(h);
}

void test_isInMinHeap()
{
    Deap *h = createDeap();
    h->size = 11;
    assert(_isInMinHeap(h, 1) == true);
    assert(_isInMinHeap(h, 2) == false);
    assert(_isInMinHeap(h, 3) == true);
    assert(_isInMinHeap(h, 4) == true);
    assert(_isInMinHeap(h, 5) == false);
    assert(_isInMinHeap(h, 6) == false);
    assert(_isInMinHeap(h, 7) == true);
    assert(_isInMinHeap(h, 8) == true);
    assert(_isInMinHeap(h, 9) == true);
    assert(_isInMinHeap(h, 10) == true);
    assert(_isInMinHeap(h, 11) == false);
    assert(_isInMinHeap(h, 12) == false);
    assert(_isInMinHeap(h, 13) == false);
    assert(_isInMinHeap(h, 14) == false);
    freeDeap(h);
}

int main()
{
    test_getPartner();
    test_isInMinHeap();

    Deap *h = createDeap();
    Person p1 = {20, "John"};
    Element e1 = {20, &p1};
    Person p2 = {10, "Jane"};
    Element e2 = {10, &p2};
    Person p3 = {40, "Doe"};
    Element e3 = {40, &p3};
    Person p4 = {3, "Doe"};
    Element e4 = {3, &p4};
    Person p5 = {60, "Doe"};
    Element e5 = {2, &p5};
    Person p6 = {7, "Doe"};
    Element e6 = {7, &p6};
    Person p7 = {78, "Doe"};
    Element e7 = {60, &p7};
    Person p8 = {10, "Jane"};
    Element e8 = {1, &p8};
    Person p9 = {80, "Doe"};
    Element e9 = {80, &p9};
    insertDeap(h, &e1);
    insertDeap(h, &e2);
    insertDeap(h, &e3);
    insertDeap(h, &e4);
    insertDeap(h, &e5);
    insertDeap(h, &e6);
    insertDeap(h, &e7);
    insertDeap(h, &e8);
    insertDeap(h, &e9);

    assert(h->size == 10);
    assert(getMinOfDeap(h)->key == 1);
    assert(((Person *)getMinOfDeap(h)->data)->age == 10);
    assert(strcmp(((Person *)getMinOfDeap(h)->data)->name, "Jane") == 0);
    assert(getMaxOfDeap(h)->key == 80);
    assert(((Person *)getMaxOfDeap(h)->data)->age == 80);
    assert(strcmp(((Person *)getMaxOfDeap(h)->data)->name, "Doe") == 0);

    LEVEL_ORDER_TRAVERSAL(Element *, h, printKey);         // 1 80 2 7 40 60 20 3 10
    LEVEL_ORDER_TRAVERSAL(Element *, h, printAgeOfPerson); // 10 80 60 7 40 78 20 3 10

    Element *min = extractMinOfDeap(h);
    assert(h->size == 9);
    assert(min->key == 1);
    assert(((Person *)min->data)->age == 10);
    assert(strcmp(((Person *)min->data)->name, "Jane") == 0);
    LEVEL_ORDER_TRAVERSAL(Element *, h, printKey); // 2 80 3 7 40 60 20 10

    Element *max = extractMaxOfDeap(h);
    assert(h->size == 8);
    assert(max->key == 80);
    assert(((Person *)max->data)->age == 80);
    assert(strcmp(((Person *)max->data)->name, "Doe") == 0);
    LEVEL_ORDER_TRAVERSAL(Element *, h, printKey); // 2 60 3 7 40 10 20

    freeDeap(h);
    return 0;
}