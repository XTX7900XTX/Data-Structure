#include <stdio.h>
#include <stdlib.h>
#include <math.h>
int smmh[1000] = {0};
int point = 2;

void show()
{
    for (int i = 2; i < point; i++)
    {
        printf("%d ", smmh[i]);
    }
    printf("\n");
}

void insert(int key)
{
    smmh[point] = key;
    int curr = point;
    point++;

    // ver sib
    int par = curr / 2;
    int sib_l = par * 2;
    if (smmh[curr] < smmh[sib_l])
    {
        int tmp = smmh[curr];
        smmh[curr] = smmh[sib_l];
        smmh[sib_l] = tmp;
        curr = sib_l;
    }
    // ver par and par-sib
    while (curr / 4 > 0)
    {
        int grand = curr / 4;
        int par_l = grand * 2;
        int par_r = grand * 2 + 1;

        if (smmh[par_l] > smmh[curr])
        {
            int tmp = smmh[par_l];
            smmh[par_l] = smmh[curr];
            smmh[curr] = tmp;
            curr = par_l;
        }
        else if (smmh[par_r] < smmh[curr])
        {
            int tmp = smmh[par_r];
            smmh[par_r] = smmh[curr];
            smmh[curr] = tmp;
            curr = par_r;
        }
        else
        {
            break;
        }
    }
}

int delete_min()
{
    int ans = smmh[2];
    smmh[2] = smmh[--point];
    int curr = 2;
    // 和四個子比較，找最小交換
    // 和sib比較，若較小則交換
    int smallest = 2;
    while (curr < point)
    {
        int child_s1 = curr / 2 * 4;
        int child_s2 = (curr / 2 * 2 + 1) * 2;

        printf("%d %d %d\n", smmh[smallest], smmh[child_s1], smmh[child_s2]);
        if (child_s1 < point && smmh[child_s1] < smmh[smallest])
            smallest = child_s1;
        if (child_s2 < point && smmh[child_s2] < smmh[smallest])
            smallest = child_s2;
        if (smmh[smallest] < smmh[curr])
        {
            int tmp = smmh[curr];
            smmh[curr] = smmh[smallest];
            smmh[smallest] = tmp;
            curr = smallest;
        }
        else
        {
            break;
        }
    }
    int sib_r = curr + 1;
    if (smmh[sib_r] < smmh[curr])
    {
        int tmp = smmh[curr];
        smmh[curr] = smmh[sib_r];
        smmh[sib_r] = tmp;
        curr = sib_r;
    }
    return ans;
}

int delete_max()
{
    int ans = smmh[3];
    smmh[3] = smmh[--point];
    {
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


    // insert(4);
    // insert(80);
    // insert(8);
    // insert(60);
    // insert(6);
    // insert(40);
    // insert(12);
    // insert(20);
    // insert(10);
    // insert(16);
    // insert(14);
    // insert(30);

    // insert(2);
    // insert(50);

    // delete_min();
    delete_min();
    printf("\n...END\n");
    show();
}