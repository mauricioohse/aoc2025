#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef unsigned long long u64;

int main(void) {
    LARGE_INTEGER frequency;
    LARGE_INTEGER start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    FILE* f = fopen("input.txt", "rb");
    if (!f) return 1;
    fseek(f, 0, SEEK_END);
    long sz = ftell(f);
    rewind(f);
    char* buf = malloc(sz + 1);
    if (!buf) { fclose(f); return 1; }
    fread(buf, 1, sz, f);
    buf[sz] = 0;
    fclose(f);

    u64 pows[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
    u64 sum = 0;
    char* p = buf;

    while (*p) {
        if (*p < '0' || *p > '9') { p++; continue; }
        u64 min = 0, max = 0;
        while (*p >= '0' && *p <= '9') min = min * 10 + (*p++ - '0');
        while (*p && *p != '-' && (*p < '0' || *p > '9')) p++;
        if (*p == '-') p++;
        while (*p >= '0' && *p <= '9') max = max * 10 + (*p++ - '0');

        for (int l = 1; l <= 9; ++l) {
            u64 mul = pows[l] + 1;
            u64 lo = pows[l-1] * mul;
            u64 hi = (pows[l] - 1) * mul;
            
            u64 a = (lo > min) ? lo : min;
            u64 b = (hi < max) ? hi : max;
            
            if (a > b) continue;
            
            u64 k1 = (a + mul - 1) / mul;
            u64 k2 = b / mul;
            
            if (k1 <= k2) {
                u64 cnt = k2 - k1 + 1;
                sum += (k1 + k2) * cnt / 2 * mul;
            }
        }
    }

    QueryPerformanceCounter(&end);
    double interval = (double)(end.QuadPart - start.QuadPart) * 1000000.0 / frequency.QuadPart;

    printf("%llu\n", sum);
    printf("Execution time: %.2f microseconds\n", interval);
    free(buf);
    return 0;
}
