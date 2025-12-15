#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#define LINE_MAX_LEN 5000
#define MAX(a, b) (a > b ? a : b)
#define RED_BEG "\033[31m"
#define RED_END "\033[0m"

//#define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

struct {
    i64 nums[5][3000];
    i64 r_cnt;
    i64 c_cnt;
} n = {0};

struct {
    char ops[3000];
    i64 cnt;
} o;


int main()
{

    FILE *f = fopen("input.txt", "r");

    char line[LINE_MAX_LEN] = {0};
    i64 max = 0;
  

    while (fgets(line, sizeof(line), f))
    {
        char* t = strtok(line, " ");
        if (isdigit(*t))
        {
            // get all numbers in line
            do
            {
                n.nums[n.r_cnt][n.c_cnt++] = atoll(t);
                printf("%lld ",n.nums[n.r_cnt][n.c_cnt-1]);

            } while ((t = strtok(NULL, " ")) != NULL);
            n.r_cnt++;
            max = n.c_cnt; 
            n.c_cnt = 0;
            printf("\n");
        }
        else
        {
            // get ops
            do
            {
                o.ops[o.cnt++] = *t;
                printf("%c ",o.ops[o.cnt-1]);

            } while ((t = strtok(NULL, " ")) != NULL);
            printf("\n");

        }
    }

    n.c_cnt = max;

    i64 total_result = 0;
    for (i64 ci = 0; ci < n.c_cnt; ci++)
    {
        i64 partial_result = 0;
        if (o.ops[ci] == '*') partial_result = 1;

        for (i64 ri = 0; ri < n.r_cnt; ri++)
        {
            // row iteration
            switch (o.ops[ci])
            {
            case '*':
                if (n.nums[ri][ci] != 0)
                    partial_result *= n.nums[ri][ci];
                break;
            case '+':
                if (n.nums[ri][ci] != 0)
                    partial_result += n.nums[ri][ci];
                break;
            default:
                printf("ERROOOO");
                break;
            }

        }
        total_result += partial_result;
        printf("%lld ", partial_result);
    }

#undef printf
    printf("\nresult: %llu\n", total_result); 

    return 0;
}