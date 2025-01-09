#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_SIZE 1000
int smmh[MAX_SIZE] = {0}; // 堆陣列
int point = 2;            // 指向下一個空位（從索引 2 開始，因為 1 是根節點）

void show()
{
    for (int i = 2; i < point; i++)
    {
        printf("%d ", smmh[i]);
    }
    printf("\n");
}

// 判斷節點深度是奇數層（最小層）還是偶數層（最大層）
int is_min_level(int index)
{
    return ((int)(log2(index)) % 2 == 0);
}

void bubble_up_min(int curr)
{
    while (curr / 4 > 0 && smmh[curr] < smmh[curr / 4])
    { // 與祖父節點比較
        int tmp = smmh[curr];
        smmh[curr] = smmh[curr / 4];
        smmh[curr / 4] = tmp;
        curr /= 4; // 上浮到祖父節點
    }
}

void bubble_up_max(int curr)
{
    while (curr / 4 > 0 && smmh[curr] > smmh[curr / 4])
    { // 與祖父節點比較
        int tmp = smmh[curr];
        smmh[curr] = smmh[curr / 4];
        smmh[curr / 4] = tmp;
        curr /= 4; // 上浮到祖父節點
    }
}

void insert(int key)
{
    smmh[point] = key; // 將新節點插入堆尾
    int curr = point;

    if (point > 2)
    { // 當前至少有一個節點
        int parent = curr / 2;

        if (is_min_level(curr))
        {
            // 最小層
            if (smmh[curr] > smmh[parent])
            {
                // 若新節點比父節點大，交換，並轉到最大層處理
                int tmp = smmh[curr];
                smmh[curr] = smmh[parent];
                smmh[parent] = tmp;
                bubble_up_max(parent); // 最大層的上浮
            }
            else
            {
                bubble_up_min(curr); // 最小層的上浮
            }
        }
        else
        {
            // 最大層
            if (smmh[curr] < smmh[parent])
            {
                // 若新節點比父節點小，交換，並轉到最小層處理
                int tmp = smmh[curr];
                smmh[curr] = smmh[parent];
                smmh[parent] = tmp;
                bubble_up_min(parent); // 最小層的上浮
            }
            else
            {
                bubble_up_max(curr); // 最大層的上浮
            }
        }
    }

    point++; // 插入完成後更新指標
}

int main()
{
    insert(20);
    insert(10);
    // insert(40);
    // insert(3);
    // insert(2);
    // insert(7);
    // insert(60);
    // insert(1);
    // insert(80);
    show(); // 顯示堆結構
    return 0;
}