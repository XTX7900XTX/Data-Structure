#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int deap[1000] = {0};
int point = 2;

void show()
{
    for (int i = 2; i < point; i++)
        printf("%d ", deap[i]);
    printf("\n");
}

int isMinSide(int index)
{
    int depth = (int)log2(index);
    int start_index = (int)pow(2, depth);
    int end_index = (int)pow(2, depth + 1) - 1;
    int mid_index = (start_index + end_index) / 2;
    return (index <= mid_index);
}

int partner(int index)
{
    int ans;
    if (isMinSide(index))
        ans = index + pow(2, ((int)log2(index) - 1));
    else
        ans = index - pow(2, ((int)log2(index) - 1));
    return (ans >= point) ? (ans / 2) : ans;
}

void MinInsert(int insert_index)
{
    int index = insert_index;
    while (index > 2 && deap[index] < deap[index / 2])
    {
        int tmp = deap[index];
        deap[index] = deap[index / 2];
        deap[index / 2] = tmp;
        index /= 2;
    }
}

void MaxInsert(int insert_index)
{
    int index = insert_index;
    while (index > 3 && deap[index] > deap[index / 2])
    {
        int tmp = deap[index];
        deap[index] = deap[index / 2];
        deap[index / 2] = tmp;
        index /= 2;
    }
}

void insert(int key)
{
    deap[point] = key;
    if (point == 2)
    {
        point++;
        return;
    }
    else
    {
        int p = partner(point);
        if (!isMinSide(point))
        { // 插入在max heap
            if (deap[point] < deap[p])
            {
                int tmp = deap[point];
                deap[point] = deap[p];
                deap[p] = tmp;
                MinInsert(p);
            }
            else
            {
                MaxInsert(point);
            }
        }
        else if (isMinSide(point))
        { // 插入在min heap
            if (deap[point] > deap[p])
            {
                int tmp = deap[point];
                deap[point] = deap[p];
                deap[p] = tmp;
                MaxInsert(p);
            }
            else
            {
                MinInsert(point);
            }
        }
    }
    point++;
}

int delete_min()
{
    if (point <= 2)
        return -1;

    int ans = deap[2];
    deap[2] = deap[--point];
    int current = 2;

    while (1)
    {
        int left = 2 * current;      // 左子節點
        int right = 2 * current + 1; // 右子節點
        int smallest = current;      // 假設當前節點最小

        // 比較左子節點
        if (left < point && deap[left] < deap[smallest])
            smallest = left;

        // 比較右子節點
        if (right < point && deap[right] < deap[smallest])
            smallest = right;

        // 如果當前節點已經是最小值，則停止
        if (smallest == current)
            break;

        int tmp = deap[current];
        deap[current] = deap[smallest];
        deap[smallest] = tmp;

        current = smallest;
    }

    int partner_index = partner(current);
    if (partner_index < point && deap[current] > deap[partner_index])
    {
        int tmp = deap[current];
        deap[current] = deap[partner_index];
        deap[partner_index] = tmp;
        // 如果交換後進入 Max Heap，修正 Max Heap
        if (!isMinSide(partner_index))
        {
            MaxInsert(partner_index); // 調整 Max Heap
        }
    }
    return ans;
}

int delete_max()
{
    if (point <= 3)
        return -1;

    int ans = deap[3];       // 儲存最大值
    deap[3] = deap[--point]; // 將最後一個元素移到根節點

    int current = 3; // 從 Max Heap 根節點開始
    while (1)
    {
        int left = 2 * current;      // 左子節點
        int right = 2 * current + 1; // 右子節點
        int largest = current;       // 假設當前節點最大

        if (left < point && deap[left] > deap[largest])
            largest = left;

        if (right < point && deap[right] > deap[largest])
            largest = right;

        if (largest == current)
            break;

        int tmp = deap[current];
        deap[current] = deap[largest];
        deap[largest] = tmp;

        current = largest;
    }

    int partner_index = partner(current);
    if (partner_index < point && deap[current] < deap[partner_index])
    {
        int tmp = deap[current];
        deap[current] = deap[partner_index];
        deap[partner_index] = tmp;
        if (isMinSide(partner_index))
        {
            MinInsert(partner_index);
        }
    }
    return ans;
}

int main()
{

    insert(20);
    insert(10);
    insert(40);
    insert(3);
    insert(2);
    insert(7);
    insert(60);
    insert(1);
    insert(80);
    show();

    delete_min();
    show();

    delete_max();
    show();
}
