#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#define printf if(0) printf

size_t substr_repeats_n_times(char* str, char* substr, int times )
{
    char* curr = substr;
    int count = 0;

    while(1)
    {
        if(*str != *curr) return 0;
        str++;
        curr++;
        if(*curr == '\0')
        {
            count++;
            curr = substr;
            if(count == times) return *str == '\0';
        }
    }

    return 0;
}


int main(){

    FILE* f = fopen("input.txt", "r");

    char line[600] = {0};

    fgets(line,sizeof(line),f);
    char min[50];
    char max[50];
    size_t sum = 0;

    char* tok = strtok(line, ",");
    do
    {
        // tok is %s-%s, now separate again
        char* c = tok;
        while(1)
        {
            if(*c == '-') 
            {
                *c = ' ';
                break;
            }
            c++;
        }

        sscanf(tok,"%s %s", min, max);

        printf("Analyzing range %s - %s\n", min, max);

        if(atoll(max) < atoll(min))
        {
            printf("max %s is less than min %s, check\n", max, min);
        }

        size_t curr = atoll(min);
        size_t max_ll = atoll(max);
        while(curr <= max_ll)
        {
            char curr_str[50];
            sprintf(curr_str, "%llu", (unsigned long long)curr);
            int digits = strlen(curr_str);
            char substr[50];
            strcpy(substr,curr_str);
            for( int i = 1; i < digits; i++)
            {
                if (digits % i != 0) continue;
                if(substr_repeats_n_times(curr_str, substr+digits-i, (digits)/i))
                {
                    sum += curr;
                    printf("\tcurr %llu substr %s is repeated %d times!\n", (unsigned long long)curr, substr+digits-i, digits/i);
                    break;
                }
            }

            curr++;
        }

    } while ( (tok = strtok(NULL, ",")) );
#undef printf
        printf("%llu", (unsigned long long)sum); // too low: 25305969709 //

    return 0;
}