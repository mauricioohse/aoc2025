#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>

#define LINE_MAX_LEN 120
#define LINE_QTD 201
#define MAX(a, b) (a > b ? a : b)
#define printf if(0) printf

// problem:
// given a string of batter joltage
// ex:
//
/*
987654321111111 -> 98
811111111111119 -> 89
234234234234278 -> 78
818181911112111 -> 91

find the pair of battery joltage that makes the largest joltage

Solution:
> parse the array from left to right:
int tenth

*/
#define PACK_SIZE 12

typedef struct
{
    int batteries[PACK_SIZE];
    int count;
} pack_t;

pack_t pack = {0};

int set_n(int value, int idx)
{
    if(idx > pack.count)
    {
        printf("YOU BETRAYED ME");
        exit(-1);
    }

    if(pack.count == idx) pack.count++;
    pack.batteries[idx] = value;
    return value;
}

int clear_n_to_end(int idx)
{
    pack.count = idx;
    return idx;
}

int set_n_clear_rest(int value, int idx)
{
    set_n(value, idx);
    clear_n_to_end(idx+1);
    return value;
}

int get_n(int idx)
{
    if (pack.count <= idx)
        return 0;

    return pack.batteries[idx];
}

uint64_t print_pack_get_int_value()
{
    uint64_t ret = 0;
    for (int i = 0; i < pack.count; i++)
    {
        printf("%c", get_n(i));
        ret = ret * 10 + get_n(i) - '0';
    }
    printf("\n");

    return ret;
}

void clear_pack_after_idx_n(int n)
{
    printf("\t\tcleaning pack after idx %d with count %d : ", n, pack.count);
    print_pack_get_int_value();
    for (int i = n; n < PACK_SIZE; n++)
    {
        if (pack.batteries[i] != 0)
            pack.count--;
        pack.batteries[i] = 0;
    }
    printf("\t\t finished cleaning, now has count %d :", pack.count);
    print_pack_get_int_value();
}

uint64_t get_largest_battery_from_line(char *line)
{
    int curr_line_idx = 0;
    int max_line_idx = strlen(line);
    if (line[max_line_idx-1] == '\n')
        max_line_idx--;

    printf("Analyzing string %s\n", line);

    char c = *(line + curr_line_idx);
    while (isdigit(c = *(line + curr_line_idx)))
    {
        printf("\tchar %c ", c);

        // if we are near the end of the line, we dont need to check the firsts anymore:
        // i.e line 12345
        // pack_size of 3
        // we dont need to check pack_idx:0 when curr_line_idx is 4 or 5
        // if curr_line_idx-max_line_idx < pack_size
        // start index needs to be
        // curr_line_idx + pack_size - max_line_id
        //          3    +   3       -   5
        // curr_pack_idx = curr_line_idx + pack_size - max_line_idx

        int curr_pack_idx = MAX(0, curr_line_idx + PACK_SIZE - max_line_idx );

        printf(" curr_pack_idx starts at %d : MAX(0, %d)\n", curr_pack_idx, curr_line_idx + PACK_SIZE - max_line_idx );
        for (; curr_pack_idx < PACK_SIZE; curr_pack_idx++)
        {
            // check if c is greater than current stored in pack
            // if yes, add it to pack
            // if not, check next available pack space and do the same

            if (c > get_n(curr_pack_idx) || get_n(curr_pack_idx) == 0)
            {
                printf("at curr_pack_idx %d we found that %c is greater than pack.batteries[%d]: %c \n", curr_pack_idx, c, curr_pack_idx, pack.batteries[curr_pack_idx]);
                set_n_clear_rest(c,curr_pack_idx);
                break;                                     // we break and go to the next char
            }
        }

        printf("\tCurrent pack has count %d and is ", pack.count);
        print_pack_get_int_value();
        curr_line_idx++;
    }

    printf("Conclusion: string %s has pack ", line);

    uint64_t ret = print_pack_get_int_value();

    memset(&pack,0  ,sizeof(pack));
    //clear_pack_after_idx_n(0);

    return ret;
}

int main()
{

    FILE *f = fopen("input.txt", "r");

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