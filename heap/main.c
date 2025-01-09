#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 110
int Heap[SIZE] = {0};
int len = 0; // len是當前的長度，也是資料開始插入的index。Heap內最後一筆為len-1
void show_Heap()
{
    for (int i = 0; i < len; i++)
        printf("%d ", Heap[i]);
    printf("\n");
}
void insert(int data)
{
    if (len >= SIZE)
    {
        printf("FULL");
        return;
    }
    Heap[len] = data;
    int current = len;
    while (current > 0 && Heap[current] < Heap[(current - 1) / 2])
    {
        int tmp = Heap[(current - 1) / 2];
        Heap[(current - 1) / 2] = Heap[current];
        Heap[current] = tmp;
        current = (current - 1) / 2;
    }
    len++;
}
void delete(int data)
{
    if (len == 0) // 空
        return;

    int find = 0;
    int position;
    for (int i = 0; i < len; i++)
    {
        if (Heap[i] == data)
        {
            find = 1;
            position = i;
            break;
        }
    }
    if (!find)
        return;
    Heap[position] = Heap[len - 1];
    len--;
    int current = position;

    while (current > 0 && Heap[current] < Heap[(current - 1) / 2])
    {
        int tmp = Heap[(current - 1) / 2];
        Heap[(current - 1) / 2] = Heap[current];
        Heap[current] = tmp;
        current = (current - 1) / 2;
    }

    if (current == position)
    {
        while (current * 2 + 1 < len)
        {
            int left = current * 2 + 1;
            int right = current * 2 + 2;
            int min = left;
            if (right < len && Heap[right] < Heap[left])
                min = right;

            if (Heap[current] > Heap[min])
            {
                int tmp = Heap[current];
                Heap[current] = Heap[min];
                Heap[min] = tmp;
                current = min;
            }
            else
                break;
        }
    }
}
int main()
{
    char command[100];
    while (1)
    {
        if (fgets(command, sizeof(command), stdin) == NULL)
            break;

        if (strcmp(command, "\n") == 0)
            break;

        if (strncmp(command, "insert", 6) == 0)
        {
            int value;
            sscanf(command + 7, "%d", &value);
            insert(value);
        }

        else if (strncmp(command, "delete", 6) == 0)
        {
            int value;
            sscanf(command + 7, "%d", &value);
            delete (value);
        }
    }
    show_Heap();
}
// int main()
// {
//     // const char *op1 = "insert";
//     // const char *op2 = "delete";
//     char input[100];
//     int number;
//     // char op_in[10];
//     // char data_in[10];
//     // int data_in_index = 0;
//     while (scanf("%s", input) != EOF)
//     {
//         // // 取得資料
//         // for (int i = 0; input[i]; ++i)
//         //     if (input[i] >= '0' && input[i] <= '9')
//         //         data_in[data_in_index++] = input[i];
//         // data_in[data_in_index] = '\0';
//         // int data = atoi(data_in);

//         // // 取得指令
//         // strncpy(op_in, input, 6);
//         // op_in[6] = '\0';
//         // if (strcmp("\n", op1) == 0)
//         //     break;
//         // if (strncmp(op_in, op1, 6) == 0)
//         //     insert(data);
//         // else if (strncmp(op_in, op2, 6) == 0)
//         //     delete (data);
//         // else
//         //     break;

//         if (fgets(input, sizeof(input), stdin) == NULL)
//             break;
//         if (strcmp(input, "\n") == 0)
//             break;
//         if (sscanf(input, "insert %d", &number) == 1)
//             insert(number);
//         else if (sscanf(input, "delete %d", &number) == 1)
//             delete (number);
//         else
//             break;
//     }
//     // insert(10);
//     // insert(5);
//     // insert(15);
//     // insert(3);
//     // insert(1);
//     // insert(7);
//     // insert(155);
//     // insert(58);
//     // insert(88);
//     // delete (88);
//     // show_Heap();
// }
