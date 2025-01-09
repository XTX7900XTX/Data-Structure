#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int mmh[1000] = {0};
int ip = 0;

void show()
{
    for (int i = 0; i < ip; i++)
        printf("%d ", mmh[i]);
    printf("\n");
}

// node編號從0開始
int level(int index)
{
    int p = log2(index + 1);
    int l = p + 1;
    return l;
}

// isMinLevel()==1代表在min層，isMinLevel()==0代表在max層
// root層level為1，min層level為奇數
int isMinLevel(int level)
{
    return level % 2 == 1;
}

void verifyMin(int current)
{
    while (current >= 3 && mmh[current] < mmh[(current - 3) / 4])
    {
        int temp = mmh[current];
        mmh[current] = mmh[(current - 3) / 4];
        mmh[(current - 3) / 4] = temp;
        current = (current - 3) / 4;
    }
}

void verifyMax(int current)
{
    while (current >= 3 && mmh[current] > mmh[(current - 3) / 4])
    {
        int temp = mmh[current];
        mmh[current] = mmh[(current - 3) / 4];
        mmh[(current - 3) / 4] = temp;
        current = (current - 3) / 4;
    }
}

void insert(int key)
{
    mmh[ip] = key;
    int current = ip;
    ip++;
    if (isMinLevel(level(current)))
    { // 插入在min層
        if (current > 0 && mmh[current] > mmh[(current - 1) / 2])
        {
            // 與父節點交換，切換到 Max 層處理
            int temp = mmh[current];
            mmh[current] = mmh[(current - 1) / 2];
            mmh[(current - 1) / 2] = temp;
            current = (current - 1) / 2;
            verifyMax(current);
        }
        else
        {
            verifyMin(current);
        }
    }
    else
    { // 插入在max層
        if (current > 0 && mmh[current] < mmh[(current - 1) / 2])
        {
            // 與父節點交換，切換到 Min 層處理
            int temp = mmh[current];
            mmh[current] = mmh[(current - 1) / 2];
            mmh[(current - 1) / 2] = temp;
            current = (current - 1) / 2;
            verifyMin(current);
        }
        else
        {
            verifyMax(current);
        }
    }
}

int min_grandchild(int current)
{
    int leftLeft = current * 4 + 3;
    int leftRight = current * 4 + 4;
    int rightLeft = current * 4 + 5;
    int rightRight = current * 4 + 6;
    int smallest = current;
    if (leftLeft < ip && mmh[leftLeft] < mmh[smallest])
        smallest = leftLeft;
    if (leftRight < ip && mmh[leftRight] < mmh[smallest])
        smallest = leftRight;
    if (rightLeft < ip && mmh[rightLeft] < mmh[smallest])
        smallest = rightLeft;
    if (rightRight < ip && mmh[rightRight] < mmh[smallest])
        smallest = rightRight;
    return smallest;
}

int extract_min()
{
    int ans = mmh[0];
    mmh[0] = mmh[--ip];
    if (ip <= 1) // 如果堆已空，或只有一個節點，直接返回
    {
        return ans;
    }
    else if (2 <= ip && ip <= 3)
    { // 無孫節點，只有子節點
        int current = 0;
        int left = current * 2 + 1;
        int right = current * 2 + 2;
        // 找到最小的子節點
        int smallest = left;
        if (right < ip && mmh[right] < mmh[smallest])
            smallest = right;
        if (mmh[current] > mmh[smallest])
        {
            int tmp = mmh[0];
            mmh[0] = mmh[smallest];
            mmh[smallest] = tmp;
        }
        return ans;
    }
    else
    { // 有孫節點
        int current = 0;
        while (current < ip)
        {
            int smallest = min_grandchild(current);
            if (mmh[current] <= mmh[smallest])
                break;

            int temp = mmh[current];
            mmh[current] = mmh[smallest];
            mmh[smallest] = temp;

            current = smallest;
        }
    }

    return ans;
}

int max_grandchild(int current)
{
    int leftLeft = current * 4 + 3;
    int leftRight = current * 4 + 4;
    int rightLeft = current * 4 + 5;
    int rightRight = current * 4 + 6;
    int biggest = current;
    if (leftLeft < ip && mmh[leftLeft] > mmh[biggest])
        biggest = leftLeft;
    if (leftRight < ip && mmh[leftRight] > mmh[biggest])
        biggest = leftRight;
    if (rightLeft < ip && mmh[rightLeft] > mmh[biggest])
        biggest = rightLeft;
    if (rightRight < ip && mmh[rightRight] > mmh[biggest])
        biggest = rightRight;
    return biggest;
}

int extract_max()
{
    if (ip <= 1)
        return -1;

    int maxIndex = 1;
    if (ip > 2 && mmh[2] > mmh[maxIndex])
        maxIndex = 2;

    int ans = mmh[maxIndex];
    mmh[maxIndex] = mmh[--ip];
    int current = maxIndex;
    int left = current * 2 + 1;
    int right = current * 2 + 2;
    int largest = current;
    if (left < ip && mmh[left] > mmh[largest])
        largest = left;
    if (right < ip && mmh[right] > mmh[largest])
        largest = right;
    if (mmh[current] < mmh[largest])
    {
        int tmp = mmh[current];
        mmh[current] = mmh[largest];
        mmh[largest] = tmp;
    }
    while (current < ip)
    {
        int largest = max_grandchild(current);
        // 如果當前節點已是最大的，結束調整
        if (mmh[current] >= mmh[largest])
            break;

        int temp = mmh[current];
        mmh[current] = mmh[largest];
        mmh[largest] = temp;

        current = largest;
    }

    return ans;
}

int main()
{

    insert(9);
    insert(31);
    insert(40);
    insert(70);
    insert(7);
    insert(12);
    insert(15);
    insert(45);
    insert(50);
    insert(30);
    insert(20);
    insert(10);
    show();

    extract_min();
    show();

    extract_min();
    show();

    extract_max();
    show();
}