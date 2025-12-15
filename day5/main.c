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
            break;

        if (!range_finished)
        {
            sscanf(line, "%lld-%lld", &r.ranges[r.cnt].min, &r.ranges[r.cnt].max);
            r.cnt++;
            printf("%lld-%lld\n",r.ranges[r.cnt-1].min, r.ranges[r.cnt-1].max);
        }

    }

    qsort(&r.ranges,r.cnt,sizeof(range_t), comp_range_t);
    qsort(&n.nums,n.cnt,sizeof(i64), comp_i64);

    // clear overlapping ranges and calculate result
    i64 result = 0;
    for (size_t i = 0; i < r.cnt; i++)
    {
        if (r.ranges[i].min <= r.ranges[i].max)
        {
            if (r.ranges[i].max >= r.ranges[i + 1].min)
            {
                r.ranges[i + 1].min = r.ranges[i].max + 1;
            }

            result += r.ranges[i].max - r.ranges[i].min + 1;
        }
        else
        {
            // if we are here, that means that this line minimum range is higher than the max!
            // this can only happen if the previous range contains this range
            // in this case, if the next range has overlap, we need to use our minimum without the +1 
            // so we are just propagating the last minimum range forward!
            if (r.ranges[i].min >= r.ranges[i + 1].min)
            {
                r.ranges[i + 1].min = r.ranges[i].min;
            }
            // an alternate way to do this without being this hacky is to change the ranges to not be 
            // inclusive with the max value. that would automatically solve this issue, but requires 
            // more changes then this hacky way
        }
        // printf("%lld-%lld, result:=%lld\n", r.ranges[i].min, r.ranges[i].max, result);
    }

    // asserting all is good after the cleaning!
    for (size_t i = 0; i < r.cnt-1; i++)
    {
        if(r.ranges[i].max > r.ranges[i+1].min)
        {
            printf("r.ranges[i %lld+1].max %lld > %lld r.ranges[i].min!!!\n", i, r.ranges[i].min, r.ranges[i + 1].min);
        }
    }





#undef printf
     printf("result: %llu\n", result);  // 346568678915976 too high

    return 0;
}