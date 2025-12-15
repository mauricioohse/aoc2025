#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


size_t count_digits(size_t n)
{
    int digits = 0;
    while(n > 0)
    {
        digits++;
        n /= 10;
    }
    return digits;
}

// size_t is_repeated_every(size_t n, size_t i, int digits)
// {


//     return 0;
// }

size_t is_repeated_twice(size_t n)
{
    int digits = count_digits(n);

    size_t left = n/(pow(10,digits/2));
    __int64 right = n - left*pow(10,digits/2);

    if ( (left - right) == 0) return 1;

    return 0;
}


int main(){

    FILE* f = fopen("input.txt", "r");

    char line[600] = {0};

    fgets(line,sizeof(line),f);
    size_t min = 0;
    size_t max = 0;
    size_t sum = 0;

    char* tok = strtok(line, ",");
    do
    {
        if(!sscanf(tok,"%llu-%llu", &min, &max))
        {
            printf("Invalid scanf!!\n");
            exit(1);
        }

        if(max < min)
        {
            printf("max %lld is less than min %lld, check\n", max, min);
        }

        size_t curr = min;
        while(curr <= max)
        {
            if(is_repeated_twice(curr))
            {
                sum += curr;
                printf("r:%lld\n", curr);
            }
            curr++;
        }

    } while ( (tok = strtok(NULL, ",")) );

        printf("p1: %lld\np2: %d", sum, 0);

    return 0;
}