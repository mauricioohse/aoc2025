#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

#define LINE_MAX_LEN 250
#define MAX(a, b) (a > b ? a : b)
#define RED_BEG "\033[31m"
#define RED_END "\033[0m"

// #pragma GCC diagnostic ignored "-Wformat"

typedef int64_t i64;
typedef uint64_t u64;

#define TEST 0
char *input = TEST ? "test.txt" : "input.txt";

// #define printf if(TEST) printf



int main()
{
    FILE *f = fopen(input, "r");
    char line[LINE_MAX_LEN] = {0};
    int grid_width = 0;
    int grid_height = 0;
    int gift_count[5];
    int sum_of_fits = 0;

    while (fgets(line, sizeof(line), f))
    {
        if(sscanf(line, "%dx%d: %d %d %d %d %d",
                        &grid_height, &grid_width,
                        &gift_count[0],
                        &gift_count[1],
                        &gift_count[2],
                        &gift_count[3],
                        &gift_count[4]
                    ) == 7) // seven matches
        {
            int total = 0;

            for (int i = 0; i < 5; i++)
              total += gift_count[i];

            int gift_min_area = total * 9;

            int grid_area = grid_height * grid_width;

            int fits = gift_min_area <= grid_area;

            sum_of_fits += fits;

            printf("grid %dx%d has area %d and gift requires %d, therefore %s fit. total fits: %d",
                    grid_height, grid_width, grid_area, gift_min_area,
                    fits ? "does" : "doesnt", 
                    sum_of_fits);

            printf("\n");
        }
    }

#undef printf
    // printf("\nresult: %d\n", run_dfs(SVR_IDX, 0, 0));

    return 0;
}