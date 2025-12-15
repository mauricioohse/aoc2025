#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#define LINE_MAX_LEN 140
#define LINE_QTD 201
#define MAX(a, b) (a > b ? a : b)
#define RED_BEG "\033[31m"
#define RED_END "\033[0m"

//#define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

typedef struct
{
    i64 min;
    i64 max;
} range_t;

struct {
    range_t ranges[200];
    size_t cnt;
} r = {0};

struct
{
    i64 nums[1200];
    size_t cnt;
} n;

int comp_range_t(const void* ele1, const void* ele2)
{
    range_t* r1 = (range_t*) ele1;
    range_t* r2 = (range_t*) ele2;
    if(r1->min > r2->min ) return 1;
    if(r1->min < r2->min ) return -1;
    return 0;
}

int comp_i64(const void* ele1,const void* ele2)
{
    i64* r1 = (i64*) ele1;
    i64* r2 = (i64*) ele2;
    if(*r1 > *r2 ) return 1;
    if(*r1 < *r2 ) return -1;
    return 0;
}

int main()
{

    FILE *f = fopen("input.txt", "r");

    char line[LINE_MAX_LEN] = {0};

    printf("printing input:\n");
    int range_finished = 0;

    while (fgets(line, sizeof(line), f))
    {
        if (*line == '\n')
            range_finished = 1;

        if (!range_finished)
        {
            sscanf(line, "%lld-%lld", &r.ranges[r.cnt].min, &r.ranges[r.cnt].max);
            r.cnt++;
            printf("%lld-%lld\n",r.ranges[r.cnt-1].min, r.ranges[r.cnt-1].max);
        }
        else
        {
            sscanf(line, "%lld", &n.nums[n.cnt++]);
            printf("%lld\n",n.nums[n.cnt-1]);
        }
    }

    qsort(&r.ranges,r.cnt,sizeof(range_t), comp_range_t);
    qsort(&n.nums,n.cnt,sizeof(i64), comp_i64);
    
    // asserting all is good after the sort!
    for (size_t i = 0; i < r.cnt-1; i++)
    {
        if(r.ranges[i].min > r.ranges[i+1].min)
        {
            printf("r.ranges[i %lldd].min %lld >  %lld r.ranges[i+1].min!!!", i, r.ranges[i].min, r.ranges[i + 1].min);
        }
    }

    for (size_t i = 0; i < r.cnt - 1; i++)
    {
        if (n.nums[i] > n.nums[i + 1])
        {
            printf("n.nums[i %lldd] %lld > %lld n.nums[i+1]!!!\n", i, n.nums[i], n.nums[i + 1]);
        }
    }


    // double index approach
    size_t ni = 0; // num idx
    size_t ri = 0; // range idx
    size_t result = 0; 

    while(ni < n.cnt && ri < r.cnt)
    {
        i64 curr_n = n.nums[ni];
        i64 r_min = r.ranges[ri].min;
        i64 r_max = r.ranges[ri].max;
        if (curr_n > r_max)
            ri++;
        else if (curr_n >= r_min && curr_n <= r_max)
        {
            // printf("Found match: n=%lld in range [%lld-%lld]\n", curr_n, r_min, r_max);
            result++;
            ni++;
        }
        else if (curr_n < r_min)
            ni++;
    }

#undef printf
     printf("result: %llu\n", result);  // 692 too high

    return 0;
}