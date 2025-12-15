#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#define LINE_MAX_LEN 120
#define LINE_QTD 201


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



int get_largest_battery_from_line(char* line)
{
    int tenth = -1;
    int unit = -1;
    char *p = line;

    while (isdigit(*p))
    {
        if(*p > tenth && isdigit(*(p+1)))
        {
            tenth = *p;
            unit = -1;
            p++;
            continue;
        }

        if(*p > unit)
        {
            unit = *p;
        }

        p++;

    }


    tenth -= '0';
    unit -= '0';

    if (tenth < 0 || unit <0) printf("error: tenth %d, unit %d", tenth, unit);
    return tenth*10 + unit;

}

int main(){

    FILE* f = fopen("input.txt", "r");

    char line[LINE_MAX_LEN] = {0};
    size_t part1 = 0;


    while(fgets(line,sizeof(line),f))
    {

        int largest_battery = get_largest_battery_from_line(line);

        printf("for pack %s found largest %d\n", line, largest_battery);
        part1+=largest_battery;

    } 

        printf("part1: %llu\n", part1); // too low: 25305969709 //

    return 0;
}