// 插入，從slot0開始尋找空格，找到空格就插入，結束程序。
// 刪除，像搜尋一樣從陣列頭找每一格，找到後刪除該格，結束。不要把slot內其他格往前搬，不要管其他bucket的元素位置是否為其應在位置。

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int table[100][20] = {0};
int how_many_item_in_row[100] = {0};
int bucket;
int slot;

void initialize_table()
{
    for (int i = 0; i < 100; i++)
        for (int j = 0; j < 20; j++)
            table[i][j] = -1;
}

void show_table()
{
    for (int i = 0; i < bucket; i++)
    {
        printf("%d : %d |", how_many_item_in_row[i], i);
        for (int j = 0; j < slot; j++)
            printf("%d ", table[i][j]);
        printf("\n");
    }
}

int hash_val(int key)
{
    return key % bucket;
}

void insert(int key)
{
    // // 像stack一樣只從後面插入
    // int row = hash_val(key);
    // while (how_many_item_in_row[row] == slot) // full
    //     row = (row + 1) % bucket;
    // table[row][how_many_item_in_row[row]++] = key;

    // 看過每一格有空位就插入，不照順序
    int row = hash_val(key);
    while (how_many_item_in_row[row] == slot) // full
        row = (row + 1) % bucket;
    for (int i = 0; i < slot; i++)
    {
        if (table[row][i] == -1)
        {
            table[row][i] = key;
            break;
        }
    }
    how_many_item_in_row[row]++;
}

void search(int key)
{
    for (int i = 0; i < bucket; i++)
        for (int j = 0; j < slot; j++)
            if (table[i][j] == key)
            {
                printf("%d %d\n", i, j);
                return;
            }
}

int isGoodBucket(int item)
{
    int position;
    for (int i = 0; i < bucket; i++)
        for (int j = 0; j < slot; j++)
            if (table[i][j] == item)
            {
                position = i;
            }
    return item % bucket == position;
}

void delete(int key)
{
    // 刪除
    int i_check = -1, j_check = -1;
    for (int i = 0; i < bucket; i++)
    {
        for (int j = 0; j < slot; j++)
        {
            if (table[i][j] == key)
            {
                i_check = i;
                j_check = j;
                break;
            }
        }
        if (i_check != -1 && j_check != -1)
        {
            break;
        }
    }

    // 刪除，並把對應row裝的元素數減一
    table[i_check][j_check] = -1;
    how_many_item_in_row[i_check]--;

    // 把後面往前移
    // ==================================================
    //     // Rehash only the elements that were after the deleted key in the same slot
    //     for (int j = j_check + 1; j < slot; j++)
    //     {
    //         if (table[i_check][j] != -1)
    //         {
    //             int current = table[i_check][j];
    //             table[i_check][j] = -1;
    //             how_many_item_in_row[i_check]--;
    //             insert(current);
    //         }
    //     }
    // ==================================================
    // 找下一個元素的index(i, j)
    // j_check = (j_check + 1) % slot;
    // if (j_check == 0)
    //     i_check = (i_check + 1) % bucket;

    // int current = table[i_check][j_check];

    // while (current != -1) // 當還沒遇到空間
    // {
    //     if (isGoodBucket(current))
    //     {
    //         ;
    //     }
    //     else
    //     {
    //         table[i_check][j_check] = -1;
    //         how_many_item_in_row[i_check]--;
    //         insert(current);
    //     }
    //     // 找下一個元素的index(i,j)
    //     j_check = (j_check + 1) % slot;
    //     if (j_check == 0)
    //         i_check = (i_check + 1) % bucket;

    //     current = table[i_check][j_check];
    // }
    // --------------------------------------------------
}

int main()
{
    initialize_table();
    char command[20];
    while (1)
    {
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;
        if (strcmp(command, "exit") == 0)
            break;
        if (strncmp(command, "bucket", 6) == 0)
            sscanf(command + 7, "%d", &bucket);
        if (strncmp(command, "slot", 4) == 0)
            sscanf(command + 5, "%d", &slot);

        if (strncmp(command, "insert", 6) == 0)
        {
            int key;
            sscanf(command + 7, "%d", &key);
            insert(key);
        }
        else if (strncmp(command, "delete", 6) == 0)
        {
            int key;
            sscanf(command + 7, "%d", &key);
            delete (key);
        }
        else if (strncmp(command, "search", 6) == 0)
        {
            int key;
            sscanf(command + 7, "%d", &key);
            search(key);
        }
        // show_table();
    }
}
