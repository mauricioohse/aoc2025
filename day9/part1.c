#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#define LINE_MAX_LEN 150
#define MAX(a, b) (a > b ? a : b)
#define RED_BEG "\033[31m"
#define RED_END "\033[0m"

// #define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

#define TEST 0
char *input = TEST ? "test.txt" : "input.txt";

typedef struct
{
    int row;
    int col;
} int2;

typedef struct {
    int2 pos1;
    int2 pos2;
    i64 area;
} square_t;

struct
{
    int2 points[500];
    int cnt;
} p = {0};

i64 calc_area(int idx_1, int idx_2)
{
    int2 p1 = p.points[idx_1];
    int2 p2 = p.points[idx_2];

    return ((i64)llabs((i64)p1.col - (i64)p2.col) + 1) * ((i64)llabs((i64)p2.row - (i64)p1.row) + 1);
}

int main()
{
    FILE *f = fopen(input, "r");

    char line[LINE_MAX_LEN] = {0};

    while (fgets(line, sizeof(line), f))
    {
        sscanf(line, "%d,%d", &p.points[p.cnt].col, &p.points[p.cnt].row);
        p.cnt++;
    }

    square_t max_square = {0};

    for( int i = 0; i <  p.cnt; i++)
    {
        for( int j = i+1;  j < p.cnt; j++)
        {
            i64 area = calc_area(i,j);
            if (area > max_square.area)
            {
                max_square.area =  area;
                max_square.pos1 = p.points[i];
                max_square.pos2 = p.points[j];
            printf("New max area: %llu from points (%d,%d) * (%d,%d)\n",
                   area, p.points[i].col, p.points[i].row, p.points[j].col, p.points[j].row);
            }
        }
    }

#undef printf
    printf("\nresult: %llu from points (%d,%d) * (%d,%d) UINT64_MAX: %llu\n",
           max_square.area, max_square.pos1.col, max_square.pos1.row, max_square.pos2.col, max_square.pos2.row, INT64_MAX); // 2147379216, 4735131726 too low!



    return 0;
}