#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#define LINE_MAX_LEN 140
#define LINE_QTD 201
#define MAX(a, b) (a > b ? a : b)
//#define printf if(0) printf

typedef struct {
    char grid[LINE_MAX_LEN][LINE_MAX_LEN];
    int grid_sum[LINE_MAX_LEN][LINE_MAX_LEN];
    int grid_size;
} grid_t;

grid_t grid = {0};

int main()
{

    FILE *f = fopen("test.txt", "r");

    char line[LINE_MAX_LEN] = {0};
    size_t part2 = 0;

    while (fgets(line, sizeof(line), f))
    {

        uint64_t largest_battery = get_largest_battery_from_line(line);

        // printf("for line %s found largest %lldd\n", line, largest_battery);
        part2 += largest_battery;
    }

#undef printf

    printf("part2: %llu\n", part2); // too low: 12760564484080 // too high: 5932020698053382612

    return 0;
}