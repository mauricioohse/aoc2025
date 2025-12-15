#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N_NUMS 10000
#define ITERATIONS 10000

long long nums[N_NUMS];

int count_div(long long n) {
    int c = 0;
    do {
        n /= 10;
        c++;
    } while (n > 0);
    return c;
}

int count_log(long long n) {
    if (n == 0) return 1;
    return (int)log10((double)n) + 1;
}

int count_opt(long long n) {
    if (n < 10000000000LL) {
        if (n < 100000) {
            if (n < 100) return (n < 10) ? 1 : 2;
            return (n < 1000) ? 3 : (n < 10000) ? 4 : 5;
        }
        if (n < 10000000) return (n < 1000000) ? 6 : 7;
        return (n < 100000000) ? 8 : (n < 1000000000) ? 9 : 10;
    }
    if (n < 1000000000000000LL) {
        if (n < 1000000000000LL) return (n < 100000000000LL) ? 11 : 12;
        return (n < 10000000000000LL) ? 13 : (n < 100000000000000LL) ? 14 : 15;
    }
    if (n < 100000000000000000LL) return (n < 10000000000000000LL) ? 16 : 17;
    return (n < 1000000000000000000LL) ? 18 : 19;
}

int main() {
    srand((unsigned)time(NULL));
    for (int i = 0; i < N_NUMS; i++) {
        // construct larger random numbers (rand is usually 15 bits)
        nums[i] = (long long)rand() | ((long long)rand() << 15) | ((long long)rand() << 30) | ((long long)rand() << 45);
        if (nums[i] < 0) nums[i] = -nums[i]; 
    }

    volatile int sink = 0; // prevent optimization
    clock_t start, end;

    start = clock();
    for (int k = 0; k < ITERATIONS; k++) {
        for (int i = 0; i < N_NUMS; i++) {
            sink += count_div(nums[i]);
        }
    }
    end = clock();
    printf("div10: %.4f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int k = 0; k < ITERATIONS; k++) {
        for (int i = 0; i < N_NUMS; i++) {
            sink += count_log(nums[i]);
        }
    }
    end = clock();
    printf("log10: %.4f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    start = clock();
    for (int k = 0; k < ITERATIONS; k++) {
        for (int i = 0; i < N_NUMS; i++) {
            sink += count_opt(nums[i]);
        }
    }
    end = clock();
    printf("optim: %.4f sec\n", (double)(end - start) / CLOCKS_PER_SEC);

    printf("sink: %d\n", sink); // use result
    return 0;
}

