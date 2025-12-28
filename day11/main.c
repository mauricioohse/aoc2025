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

#define TEST 1
char *input = TEST ? "test.txt" : "input.txt";

#define printf if(TEST) printf


#define N_LETTERS 26

// a device is characterized by 3 letters name
// a 3 letter name map to an integer between 0 and N_LETTERS*N_LETTERS*N_LETTERS
typedef int name_t;

#define DEVICE_MAX_OUTPUTS 30

// a device has a name, and can point to up to 10 other device names.
// it can also store how many paths until the out
typedef struct {
    name_t name;
    name_t outputs[DEVICE_MAX_OUTPUTS];
    int    out_cnt;
    int path_cnt;
} device_t;

// there are only 26*26*26 possible device names
struct pool_names_t {
  device_t devices[N_LETTERS * N_LETTERS * N_LETTERS];
  int cnt;
} list;

// you can map each name to a index
// assumes *s has length 3
int name_to_idx(char *s)
{
    return (s[0] - 'a') * N_LETTERS * N_LETTERS + 
           (s[1] - 'a') * N_LETTERS + 
           (s[2] - 'a');
}

typedef struct { char s[4]; } name_str_t;

name_str_t idx_to_name(int d)
{
    name_str_t res;
    res.s[0] = (d / (N_LETTERS * N_LETTERS)) % N_LETTERS + 'a';
    res.s[1] = (d / N_LETTERS) % N_LETTERS + 'a';
    res.s[2] = d % N_LETTERS + 'a';
    res.s[3] = '\0';
    return res;
}

void print_device (device_t d)
{
    printf("%s: ", idx_to_name(d.name).s);
    for(int i = 0; i < d.out_cnt; i++)
    {
        printf("%s ", idx_to_name(d.outputs[i]).s);
    }
}

#define MAX_DFS_SIZE 1<<15
device_t dfs_data[MAX_DFS_SIZE] = {0};
int dfs_data_cnt = 0;


int SVR_IDX;
int OUT_IDX;
int DAC_IDX;
int FFT_IDX;

void print_name(name_t name)
{
    name_str_t str = idx_to_name(name);
    for( int i  = 0; i < (int)strlen(str.s); i++)
    {
        printf("%c", str.s[i]);
    }
}

// input is dfs_data
int run_dfs(name_t name, int crossed_fft, int crossed_dac)
{
    device_t *curr = &list.devices[name];

    if(name == DAC_IDX) crossed_dac = 1;
    if(name == FFT_IDX) crossed_fft = 1;

    // printf("Evaluating: ");
    // print_device(*curr);
    // printf("\n");
    printf("%s->", idx_to_name(name).s);

    if (name == OUT_IDX) {
      if (crossed_dac && crossed_fft) {
        curr->path_cnt = 1;
        printf("\n\t");
        return 1;
      } else {
        
        printf("\n\t");
        return 0;
      }
    } // finished

    if (curr->path_cnt != 0) {
        printf("\t");
        return curr->path_cnt > 0 ? curr->path_cnt : 0;
    } // already accounted for

    // run dfs on all outputs, return the sum of paths
    int sum = 0;
    for (int i  = 0 ; i < curr->out_cnt ; i++)
    {
        sum += run_dfs(curr->outputs[i], crossed_fft, crossed_dac);
    }

    if (sum <= 0)
      curr->path_cnt = -1;
    else
      curr->path_cnt = sum;

    if (sum!=0) printf("returning sum: name %s, path_cnt %d\n", idx_to_name(name).s, curr->path_cnt);
    return sum;
}


int main()
{
    SVR_IDX = name_to_idx("svr");
    OUT_IDX = name_to_idx("out");
    DAC_IDX = name_to_idx("dac");
    FFT_IDX = name_to_idx("fft");

    FILE *f = fopen(input, "r");
    char line[LINE_MAX_LEN] = {0};

    while (fgets(line, sizeof(line), f))
    {
        int device_processed = 0;
        device_t new_device = {0}; 
        char* tok = strtok(line, " :");
        do{
            // process line
            if (!device_processed)
            {
                // the first tok is always the device name
                new_device.name = name_to_idx(tok);
                device_processed = 1;
            }
            else {
                // other entries are the outputs of that device
                new_device.outputs[new_device.out_cnt++] = name_to_idx(tok);
                assert(new_device.out_cnt <= DEVICE_MAX_OUTPUTS &&
                       "INCREASE DEVICE_T OUTPUT SIZE!");
            }

        } while ((tok = strtok(NULL, " :")) != NULL);

        list.devices[new_device.name] = new_device;
        print_device(new_device);
        printf("\n");
    }

#undef printf
    printf("\nresult: %d\n", run_dfs(SVR_IDX, 0, 0));

    return 0;
}