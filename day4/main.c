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

#define printf if(0) printf

typedef struct {
    char cells[LINE_MAX_LEN][LINE_MAX_LEN];
    int adj_rolls[LINE_MAX_LEN][LINE_MAX_LEN];
    int size;
} grid_t;

grid_t g = {0};
size_t total_removed = 0;

int is_valid_coord(int i, int j)
{
    return (i >= 0  && i < LINE_MAX_LEN && j >= 0 && j < LINE_MAX_LEN );
}

void print_adj_rolls_painted(int ii, int jj)
{
    for (int i = 0; i < g.size; i++)
    {
        for (int j = 0; j < g.size; j++)
        {
            int cnt = g.adj_rolls[i][j];
            if( i == ii && j == jj) printf(RED_BEG);
            printf("%d", cnt);
            if( i == ii && j == jj) printf(RED_END);

        }
        printf("\n");
    }
}

void add_adj_rolls(int i, int j)
{
    if(is_valid_coord(i,j))
    {
        g.adj_rolls[i][j]++;
        // print_adj_rolls_painted(i,j);
        // printf("\n");
    }
}

void add_cell(int i, int j, char c)
{

    g.cells[i][j] = c;
    g.size = MAX(i,j)+1;

    if (c == '@') 
    {
        // printf("\n");
        int dirsx[] = {-1,0,1};
        int dirsy[] = {-1,0,1};

        for(size_t n = 0; n < 3; n++ )
        for(size_t m = 0; m < 3; m++ )
            if (n != 1 || m != 1)
                add_adj_rolls(i+dirsy[n], j+dirsx[m]);
    }

}

void reset_adj_rolls()
{
    for (int i = 0; i < g.size; i++)
        for (int j = 0; j < g.size; j++)
            g.adj_rolls[i][j] = 0;
}

void recalc_adj_rolls()
{
    for (int i = 0; i < g.size; i++)
        for (int j = 0; j < g.size; j++)
        {
            char c = g.cells[i][j];

            if (c == '@')
            {
                // printf("\n");
                int dirsx[] = {-1, 0, 1};
                int dirsy[] = {-1, 0, 1};

                for (size_t n = 0; n < 3; n++)
                    for (size_t m = 0; m < 3; m++)
                        if (n != 1 || m != 1)
                            add_adj_rolls(i + dirsy[n], j + dirsx[m]);
            }
        }
}

int remove_accessible_rolls()
{
    int removed = 0;
    printf("printing accessible locations:\n");
    for (int i = 0; i < g.size; i++)
    {
        for (int j = 0; j < g.size; j++)
        {
            int cnt = g.adj_rolls[i][j];
            printf("%d", cnt);
            if (cnt < 4 && g.cells[i][j]=='@')
            {
                removed++;
                g.cells[i][j] = '.';
            }
        }
        printf("\n");
    }
    total_removed += removed;
    return removed;
}

int main()
{

    FILE *f = fopen("input.txt", "r");

    char line[LINE_MAX_LEN] = {0};
    int i = 0;

    printf("printing input:\n");

    while (fgets(line, sizeof(line), f))
    {
        int j = 0;
        char *c = line;
        while (isalnum(*c) || *c == '.' || *c == '@')
        {
            printf("%c", *c);
            add_cell(i,j,*c);
            c++;
            j++;
        }
        printf("\n");
        i++;

    }

    while(remove_accessible_rolls())
    {
        reset_adj_rolls();
        recalc_adj_rolls();
    }

#undef printf
    printf("result: %llu, g.size %d\n", total_removed, g.size); // too low: 12760564484080 // too high: 5932020698053382612

    return 0;
}