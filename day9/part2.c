#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdint.h>
#include <assert.h>

#define LINE_MAX_LEN 150
#define MAX(a, b) (a > b ? a : b)
#define RED_BEG "\033[31m"
#define RED_END "\033[0m"

#define printf if(0) printf

typedef int64_t i64;
typedef uint64_t u64;

#define TEST 0
char *input = TEST ? "test.txt" : "input.txt";

typedef struct
{
    int row;
    int col;
    char direction;
} point_t;

typedef struct {
    point_t pos1;
    point_t pos2;
    i64 area;
} square_t;

struct
{
    point_t points[500];
    int cnt;
} p = {0};

i64 calc_area(int idx_1, int idx_2)
{
    point_t p1 = p.points[idx_1];
    point_t p2 = p.points[idx_2];

    return ((i64)llabs((i64)p1.col - (i64)p2.col) + 1) * ((i64)llabs((i64)p2.row - (i64)p1.row) + 1);
}

i64 calc_area_points(point_t p1, point_t p2)
{

    return ((i64)llabs((i64)p1.col - (i64)p2.col) + 1) * ((i64)llabs((i64)p2.row - (i64)p1.row) + 1);
}

int get_point_hor_parity(int h_row,int col)
{
    // checks if there is any point with vertical direction in this collumn
    int parity = 0;
    for(int i = 0; i < p.cnt; i++ )
    {
        point_t point = p.points[i];
        point_t next_point = p.points[i+1%p.cnt];
        int in_same_col = point.col == col;
        int has_vertical_dir = (point.direction == 'v' || point.direction== '^');
        

        if(in_same_col && has_vertical_dir)
        {
            // now we check if we are in the edge - if yes end the edge is moving down, we increase parity, otherwise decrease it
            if (point.direction == 'v' && point.row < h_row && h_row < next_point.row)
            {
                parity += 1;
                printf("\t\tVertical down edge: h_row=%d, col=%d, point(%d,%d,'%c'), next_point(%d,%d,'%c'), partial_parity=+1\n", 
                       h_row, col, point.row, point.col, point.direction, next_point.row, next_point.col, next_point.direction);
            }
            if (point.direction == '^' && point.row > h_row && h_row > next_point.row)
            {
                parity -= 1;
                printf("\t\tVertical up edge: h_row=%d, col=%d, point(%d,%d,'%c'), next_point(%d,%d,'%c'), partial_parity=-1\n", 
                       h_row, col, point.row, point.col, point.direction, next_point.row, next_point.col, next_point.direction);
            }
        }

    }

    printf("\tget_point_hor_parity: h_row=%d, col=%d, parity=%d\n", h_row, col, parity);
    return parity;

}

int is_point_inside(point_t point)
{
    // one way to check is to do a horizontal and vertical sweep, starting on the left or top, and
    // adding/decreasing the parity everytime we hit an edge
    // an edge is determined by two consecutive points

    // horizontal sweep: checking for points from left to right that
    int parity_h = 0;
    int h_row = point.row;

    for (int col = 0; col <= point.col; col++)
    {
        parity_h+= get_point_hor_parity(h_row, col);
    }


    // vertical sweep: check for points from up to down.

    return parity_h;

}

// int get_vertical_parity(point_t point)
// {

// }

square_t max_square = {0};

void iterate_on_edges(point_t p1, point_t p2)
{
    // Find the bounds of the rectangle formed by p1 and p2
    int min_row = p1.row < p2.row ? p1.row : p2.row;
    int max_row = p1.row > p2.row ? p1.row : p2.row;
    int min_col = p1.col < p2.col ? p1.col : p2.col;
    int max_col = p1.col > p2.col ? p1.col : p2.col;

    printf("Iterating on edges of rectangle from (%d,%d) to (%d,%d):\n", 
           p1.row, p1.col, p2.row, p2.col);
    
    // Top edge (min_row, from min_col to max_col)
    for (int col = min_col; col <= max_col; col++) {
        //printf("Edge point: (%d,%d)\n", min_row, col);
    }
    
    // Bottom edge (max_row, from min_col to max_col)
    if (max_row != min_row) {
        for (int col = min_col; col <= max_col; col++) {
            //printf("Edge point: (%d,%d)\n", max_row, col);
        }
    }
    
    // Left edge (min_col, from min_row+1 to max_row-1)
    for (int row = min_row + 1; row < max_row; row++) {
        if( !is_point_inside((point_t){row,min_col, 0}) )
        {
            printf("Point (%d,%d) is not inside, returning early\n", row, min_col);
            return;
        }
    }
    
    // Right edge (max_col, from min_row+1 to max_row-1)
    if (max_col != min_col) {
        for (int row = min_row + 1; row < max_row; row++) {
            //printf("Edge point: (%d,%d)\n", row, max_col);
        }
    }

    // if we reached here, the whole square is inside!
    if (max_square.area < calc_area_points(p1, p2))
    {
        max_square.area = calc_area_points(p1, p2);
        max_square.pos1 = p1;
        max_square.pos2 = p2;
        printf("New max area inside: %lld from points (%d,%d) to (%d,%d)\n",
               max_square.area, p1.row, p1.col, p2.row, p2.col);
    }
}



char get_dir_if_there_is_a_point_or_get_dot(int row, int col)
{
    for(int i = 0; i < p.cnt; i++)
    {
        if (p.points[i].col == col && p.points[i].row == row)
            return p.points[i].direction;
    }

    return '.';
}

char grid[20][20] = {0};

void print_grid()
{
    for(int i =0 ;i< 9; i++)
    {
        for(int j =0 ;j< 14; j++)
            printf("%c ", grid[i][j]);

        printf("\n");
    }
}

char get_dir_between_points(point_t curr, point_t next)
{
    i64 delta_col = next.col - curr.col;
    i64 delta_row = next.row - curr.row;

    assert((curr.row != next.row || curr.col != next.col) && "The two points are equal!");
    assert((curr.row == next.row || curr.col == next.col) && "The two points are not in a line!");


    if (delta_col > 0) return '>';
    if (delta_col < 0) return '<';
    if (delta_row > 0) return 'v';
    if (delta_row < 0) return '^';

    assert(0 && "Reached invalid positions!");
    return -1;
}

void add_dir_to_points()
{
    for (int i = 0; i < p.cnt; i++)
    {
        
        point_t curr = p.points[i];
        int ni = (i+1)%p.cnt; // wraps on last i
        point_t next = p.points[ni]; 

        p.points[i].direction = get_dir_between_points(curr, next);
        printf("Point %d: (%d,%d) direction: %c\n", i, curr.row, curr.col, p.points[i].direction);

    }
}

int main()
{
    FILE *f = fopen(input, "r");

    char line[LINE_MAX_LEN] = {0};
    if(TEST) memset(grid,'.',sizeof(grid));


    while (fgets(line, sizeof(line), f))
    {
        sscanf(line, "%d,%d", &p.points[p.cnt].col, &p.points[p.cnt].row);
        if(TEST) grid[p.points[p.cnt].row][p.points[p.cnt].col] = p.cnt + '0';
        p.cnt++;
    }

    if(TEST) print_grid();

    add_dir_to_points();

    // lets verify it is working properly:
    for(int row = 0; row < 9; row++)
    {
        for (int col = 0; col < 14; col++)
        {

            char c = get_dir_if_there_is_a_point_or_get_dot(row,col);
            printf("%c ",c);
        }
        printf("\n");
    }
    #undef printf

    printf("Started iterating on edges!\n");
    for( int i = 0; i <  p.cnt; i++)
    {
        for( int j = i+1;  j < p.cnt; j++)
        {
            point_t p1 = p.points[i];
            point_t p2 = p.points[j];
            iterate_on_edges(p1,p2);
        }
        printf("progress: (%d/%d)\n", i, p.cnt);
    }

    printf("\nresult: %llu\n",
        max_square.area); // 2147379216, 4735131726 too low!
    printf("pos1: (%d,%d), pos2: (%d,%d)\n", 
        max_square.pos1.row, max_square.pos1.col, 
        max_square.pos2.row, max_square.pos2.col);



    return 0;
}