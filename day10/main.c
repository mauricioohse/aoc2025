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
//#define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

#define TEST 0
const int LIGHT_SIZE = TEST ? 12 : 12;
char *input = TEST ? "test.txt" : "input.txt";

typedef u64 light_t; // light will be used in bits

light_t target_light = 0;

typedef struct {
    u64 items[50];
    int cnt;
} array_t;

array_t buttons = {0};

light_t parse_target_light(char *s)
{
    s++; // skips first [

    light_t result = {0};
    int curr_bit = 0;

    while(*s!= '\0')
    {
        if (*s == '#') result+= 1 << curr_bit;
        curr_bit++;
        s++;
    }
    return result;
}

light_t parse_button(char *s)
{
    s++;
    light_t result = {0};

    while(*s!= '\0')
    {
        if (isdigit(*s)) result+= 1 << (*s-'0');
        s++;
    }

    return result;
}


void print_bits(int n)
{
    int size = LIGHT_SIZE;
    while(size--)
    {
        printf("%d", n & 1);
        n = n >> 1;
    }
    printf(" ");
}

void arr_append(array_t *arr, light_t button)
{
    arr->items[arr->cnt++]=button;
}

void arr_reset(array_t*arr)
{
    arr->cnt = 0;
}

light_t arr_get(array_t *arr, int idx)
{
    if(idx >= arr->cnt) assert(0 && "INDEX OUT OF BOUNDS FOR ARRAY");
    return arr->items[idx];
}

void arr_print_b(array_t *arr)
{
    printf("[");
    for (int i =0; i < arr->cnt; i++)
        print_bits(arr->items[i]);
    printf("]\n");
}

int presses = 0;

typedef struct {
    light_t curr_light;
    int buttons_pressed[20];
    int button_cnt;
    int presses;
} bfs_data_t;

#define BFS_DATA_MAX 5000000

bfs_data_t bfs_data[BFS_DATA_MAX] = {0}; //
int bfs_data_count = 0;


void bfs_append(light_t light)
{
    if(bfs_data_count>BFS_DATA_MAX-1) assert(0&&"INCREASE BFS DATA SIZE!");
    bfs_data[bfs_data_count].curr_light=light;
    bfs_data[bfs_data_count++].presses = 0;
}

bfs_data_t next_bfs_data[BFS_DATA_MAX] = {0};
int next_bfs_cnt = 0;

void clean_bfs_data()
{
    memset(bfs_data, 0, sizeof(bfs_data));
    bfs_data_count = 0;
    memset(next_bfs_data, 0, sizeof(next_bfs_data));
    next_bfs_cnt = 0;
}

// bfs_data_t create_bfs_data(light_t next_light, bfs_data_t data)
// {
//     return ((bfs_data_t){next_light,data. ,data.presses+1});
// }

void add_to_next_bfs_data(bfs_data_t data)
{
    if(next_bfs_cnt>BFS_DATA_MAX-1) assert(0&&"INCREASE BFS DATA SIZE!");
    next_bfs_data[next_bfs_cnt++]=data;
}

void swap_next_bfs_data_to_current()
{
    memcpy(bfs_data, next_bfs_data, sizeof(bfs_data));
    bfs_data_count = next_bfs_cnt;

    memset(next_bfs_data,0, sizeof(next_bfs_data));
    next_bfs_cnt = 0;
}

int count_bits(u64 n)
{
    int cnt = 0;
    while (n != 0)
    {
        cnt += n & 1;
        n = n >> 1;
    }

    return cnt;

}

int button_advances_to_target(light_t button, light_t curr_light)
{
    curr_light ^=button;
    // if (count_bits(curr_light^target_light) <= count_bits(target_light)) return curr_light;
    // return 999999;
    return curr_light;
}

void print_bfs_data(bfs_data_t d)
{
    printf("Printing final bfs data:\n\t final light: ");
    print_bits(d.curr_light);
    printf("buttons pressed: ");
    for (int i  = 0; i< d.button_cnt; i++)
    {
        print_bits(d.buttons_pressed[i]);
    }
    printf("\n");

}

// runs one iteration of bfs
// all bfs data is stored in bfs_data
// the result is added to global presses variable
int run_bfs_once()
{
    for( int i = 0; i < bfs_data_count; i++) 
    {
        bfs_data_t curr = bfs_data[i];   
        for (int b = 0; b < buttons.cnt; b ++)
        {
            light_t button = buttons.items[b];
                light_t next_light = button_advances_to_target(button, curr.curr_light);
            if(next_light!=999999)
            {
                // bfs_data_t new_bfs_data = create_bfs_data(next_light, curr);
                // create next bfs data
                bfs_data_t new_bfs_data; 
                memcpy(&new_bfs_data, &curr, sizeof(curr));
                new_bfs_data.buttons_pressed[new_bfs_data.button_cnt++]=button;
                new_bfs_data.curr_light = next_light;
                new_bfs_data.presses++;
                if(new_bfs_data.curr_light == target_light)
                {
                    presses += new_bfs_data.presses;
                    print_bfs_data(new_bfs_data);
                    return 1;
                }
                add_to_next_bfs_data(new_bfs_data);
            }
                
        }
    }

    swap_next_bfs_data_to_current();
    return 0;

}

int main()
{
    FILE *f = fopen(input, "r");

    char line[LINE_MAX_LEN] = {0};
    clean_bfs_data();


    while (fgets(line, sizeof(line), f))
    {
        arr_reset(&buttons);

        char* tok = strtok(line, " ");
        do{
            if(*tok == '[')
            {
                target_light = parse_target_light(tok);
            }
            else if ( *tok=='(' )
            {
                light_t new_button = parse_button(tok);
                arr_append(&buttons, new_button);
            }
            else if ( *tok =='{')
            {
                // do nothing on purpose
            }

        } while ((tok = strtok(NULL, " ")) != NULL);

        printf("Target light: ");
        print_bits(target_light);
        printf("\n\t");
        arr_print_b(&buttons);
        printf("\n");

        // now, we do the BFS process until it finds a solution!
        bfs_append((light_t)0);
        while(run_bfs_once() == 0);

        printf("Presses needed: %d\n", presses);
        clean_bfs_data();


    }



#undef printf
    printf("\nresult: %d\n", presses);

    return 0;
}