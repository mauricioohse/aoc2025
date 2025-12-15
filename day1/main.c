#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    char rot;
    int value;
} instruction_t ;

int count = 0;
int dial = 50;

int zero_count = 0;
int zero_crosses = 0;

void add_to_dial(char rot, int value)
{
    int start = dial;
    
    if (rot == 'R')
    {
        dial = (dial + value) % 100;
        zero_crosses += (start + value) / 100;
    }
    else
    {
        dial = ((dial - value) % 100 + 100) % 100;
        if (start == 0)
            zero_crosses += value / 100;
        else if (start <= value)
            zero_crosses += 1 + (value - start) / 100;
    }

    if (dial == 0)
        zero_count++;
}

int main(){

    FILE* f = fopen("input.txt", "r");

    char line[100] = {0};
    char rot;
    int value;
    while (fgets(line,100,f))
    {
        sscanf(line, "%c%d", &rot, &value);
        add_to_dial(rot, value);
        //printf("%d: %c%d - dial: %d zero_crosses: %d\n", count, rot, value,dial, zero_crosses);

        count++;


    }

    printf("p1: %d\np2: %d", zero_count, zero_crosses);

    return 0;
}