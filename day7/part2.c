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

//#define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

struct {
    char map[LINE_MAX_LEN][LINE_MAX_LEN];
    i64 beam_cnt[LINE_MAX_LEN]; // one per col
    i64 col_len;
    i64 row_len;
} m = {0};



int is_beam_starter(char c)
{
    return (c == 'S' || c == '|');
}

int is_valid(i64 row, i64 col)
{
    return (row >= 0 && row < m.row_len && col >= 0 && col < m.col_len);
}

void try_set_beam(i64 row, i64 col)
{
    if (is_valid(row,col)) //&& m.map[row][col]=='.' )
    {
        m.map[row][col] = '|';
    }
}

int main()
{

    FILE *f = fopen("input.txt", "r");

    char line[LINE_MAX_LEN] = {0};
  
    while (fgets(line, sizeof(line), f))
    {
        m.col_len = strcspn(line, "\n\r");
        memcpy(&m.map[m.row_len][0],line, m.col_len);
        char fmt[100];
        snprintf(fmt, sizeof(fmt),"%%%llds\n",m.col_len);
        printf(fmt, &m.map[m.row_len][0]);
        m.row_len++;
    }

/*
for each row and col
    if is_beam_or_s(m[r][c])
        char *down =  &m[r+1][c]
        if(down is '.')
            draw in down
    
        if(down is '^')
            draw_split_down
*/

    for (i64 row = 0; row < m.row_len; row++)
        for (i64 col = 0; col < m.col_len; col++)
        {
            char *curr = &m.map[row][col];
            if (is_beam_starter(*curr))
            {
                if (*curr == 'S') m.beam_cnt[col]++; 
                if (!is_valid(row + 1, col))
                    continue;

                char *down = &m.map[row + 1][col];
                if (*down == '.')
                    try_set_beam(row + 1, col);

                if (*down == '^')
                {
                    try_set_beam(row + 1, col + 1);
                    try_set_beam(row + 1, col - 1);
                    while(m.beam_cnt[col] > 0)
                    {
                        m.beam_cnt[col]--;
                        m.beam_cnt[col + 1]++;
                        m.beam_cnt[col - 1]++;
                    }
                }
            }
        }

    printf("after beams row_len = %lld col_len %lld\n\n", m.row_len, m.col_len);
    for (i64 row = 0; row < m.row_len; row++)
    {
        for (i64 col = 0; col < m.col_len; col++)
        {
            printf("%c", m.map[row][col]);
        }
        printf("\n");
    }

    size_t result = 0;
    for(i64 i = 0; i < m.col_len; i++)
        result += m.beam_cnt[i];


#undef printf
    printf("\nresult: %llu\n", result);  // too low 1638

    return 0;
}