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

#define TEST 0
char * input = TEST? "test.txt" : "input.txt";
double max_empirical_dist = TEST ? 366 : 7821.5;

typedef struct {
    i64 p[3];
} int3;

#define MAX_POINTS 1000

struct {
    int3 points[MAX_POINTS];
    i64 point_to_circuit_map[MAX_POINTS]; // idx is point_idx, and value is circuit point belongs to
    i64 len;
    i64 circuit_count;
} p = {0};

typedef struct {
    i64 p1_idx;
    i64 p2_idx;
    double dist;
} pair_t;

struct {
    pair_t pairs[1000*1000];
    i64 cnt;
} pr = {0};

void add_pair(i64 idx1, i64 idx2, double dist)
{
    if (dist < 0) printf("panic!! dist is negative!!\n");
    if (dist > max_empirical_dist) return;
    pr.pairs[pr.cnt++] = (pair_t){.p1_idx = idx1, .p2_idx = idx2, .dist = dist};
}

double euclid_dist(int3 p1, int3 p2)
{
    i64 x = pow(p1.p[0] - p2.p[0],2);
    i64 y = pow(p1.p[1] - p2.p[1],2);
    i64 z = pow(p1.p[2] - p2.p[2],2);
   

    if (x < 0 || y < 0 || z < 0) {
        fprintf(stderr, "Error: Distance components must be non-negative\n");
        exit(1);
    }
    return sqrt((double)x + y + z);
}

void add_circuit(i64 p_id, i64 circuit_id)
{
    p.point_to_circuit_map[p_id] = circuit_id;
}

int get_circuit(int p_id)
{
    return p.point_to_circuit_map[p_id];
}

i64 get_next_available_circuit_id()
{
    // note: this can be a bottleneck if called too frequently
    i64 max = 0;
    for (i64 i = 0; i < MAX_POINTS; i++)
    {
        if (max < p.point_to_circuit_map[i])
            max = p.point_to_circuit_map[i];
    }

    return max + 1;
}

void merge_circuits(i64 c1, i64 c2)
{
    // we always merge c2 into c1, but that is arbitrary
    for (i64 i = 0; i < MAX_POINTS; i++)
    {
        if (p.point_to_circuit_map[i] == c2)
            p.point_to_circuit_map[i]= c1;
    }
}

int comp_pairs(const void* int1, const void* int2)
{
    pair_t* i1 = (pair_t*) int1;
    pair_t* i2 = (pair_t*) int2;

    if(i1->dist > i2->dist) return 1; // what was the reason to not return i1 - i2??
    if(i1->dist < i2->dist) return -1;
    return 0;
}

typedef struct {
    int cnt;
    int ckt_id;
} ckt_t;

int comp_ckt(const void* int1, const void* int2)
{
    ckt_t* i1 = (ckt_t*) int1;
    ckt_t* i2 = (ckt_t*) int2;

    if(i1->cnt > i2->cnt) return -1; // what was the reason to not return i1 - i2??
    if(i1->cnt < i2->cnt) return 1;
    return 0;
}

int main()
{

    FILE *f = fopen(input, "r");

    char line[LINE_MAX_LEN] = {0};
  
    while (fgets(line, sizeof(line), f))
    {
        sscanf(line, "%lld,%lld,%lld", &p.points[p.len].p[0],&p.points[p.len].p[1],&p.points[p.len].p[2]);
        p.len++;
    }

    for(i64 i = 0; i <  p.len; i++ )
    {

        for(i64 j = i+1; j < p.len; j++  )
        {
            double dist = euclid_dist(p.points[i], p.points[j]);
            add_pair(i, j, dist);
        }

    }

    printf("found %lld pairs with dist less than %f\n", pr.cnt, max_empirical_dist);

    qsort(pr.pairs, pr.cnt, sizeof(pr.pairs[0]), comp_pairs);

    // assert correct order
    for (i64 i = 0; i < pr.cnt-1; i++)
        if(pr.pairs[i].dist > pr.pairs[i+1].dist ) printf("ERROR AT DIST AFTER QSORT");

    // print at position 1000 to see the size I should actually use
    printf("dist at idx %d is %f\n",10, pr.pairs[10].dist);
    printf("dist at idx %d is %f\n",998, pr.pairs[998].dist);
    printf("dist at idx %d is %f\n",999, pr.pairs[999].dist);
    printf("dist at idx %d is %f\n",1000, pr.pairs[1000].dist);
    printf("dist at idx %d is %f\n",1001, pr.pairs[1001].dist);


    // add circuits to the 1000 pairs that has low dist!
    for( int i = 0;  i < 999; i++)
    {
        int p1_idx = pr.pairs[i].p1_idx;
        int p2_idx = pr.pairs[i].p2_idx;
        
        int p1_ckt = get_circuit(p1_idx);
        int p2_ckt = get_circuit(p2_idx);

        if(p1_ckt == 0 && p2_ckt == 0)
        {
            int new_ckt = get_next_available_circuit_id();
            add_circuit(p1_idx, new_ckt);
            add_circuit(p2_idx, new_ckt);
        }
        else if(p1_ckt != 0 && p2_ckt == 0)
        {
            add_circuit(p2_idx, p1_ckt);
        }
        else if(p1_ckt == 0 && p2_ckt != 0)
        {
            add_circuit(p1_idx, p2_ckt);
        }
        else
        {
            merge_circuits(p1_ckt,p2_ckt);
        }
    }

    // finally, we count the circuits
    ckt_t circuits[1000] = {0};

    for(int i = 0; i<1000; i++)
    {
        int p_ckt = p.point_to_circuit_map[i];
        circuits[p_ckt].cnt++;
        circuits[i].ckt_id=i;
    }

    qsort(circuits, 1000, sizeof(ckt_t), comp_ckt);

    printf("printing Circuits:\n");
    // int last_ckt = 0;
    for( int i = 0; circuits[i].cnt!=0 || i == 1; i++)
    {
        printf("c%d:(%d) ", circuits[i].ckt_id, circuits[i].cnt);
        // last_ckt = i;
    }
    printf("\n");

    //printing points that belong to 
    for (int i = 1; i <= 3 /*last_ckt*/; i++)
    {
        printf("Circuit %d: ", circuits[i].ckt_id);
        for(int j = 0;j<1000; j++ )
        {
            if(p.point_to_circuit_map[j]==circuits[i].ckt_id)
                printf("(%lld,%lld,%lld),", p.points[j].p[0],p.points[j].p[1],p.points[j].p[2]);
        }
        printf("\n");
    }

    i64 result = circuits[1].cnt *circuits[2].cnt*circuits[3].cnt;
    printf("result %lld = circuits[1]%d*circuits[2]%d*circuits[3]%d;\n honorable mention, points without circut: %d\n",
           result, circuits[0 + 1].cnt, circuits[1 + 1].cnt, circuits[2 + 1].cnt, circuits[0].cnt ); //128199 / 318459 too high!

    // now we sort the circuits and multiply the first three

    // // add circuits
    // i64 curr_ckt = p.point_to_circuit_map[i];
    // i64 other_ckt = p.point_to_circuit_map[j];
    // if (curr_ckt != 0 && other_ckt == 0)
    // {
    //     // curr point already has circuit, so we just add the other point to the same circuit
    //     p.points[min_idx].circuit = curr_ckt;
    // }
    // else if (curr_ckt == 0 && other_ckt == 0)
    // {
    //     // create a new circuit!
    //     p.points[i].circuit = p.circuit_count + 1; // circuit count starts at zero, but the first circuit is at 1
    //     p.points[min_idx].circuit = p.circuit_count + 1;
    //     ;
    //     p.circuit_count++;
    // }
    // else if (curr_ckt != 0 && other_ckt != 0)
    // {
    //     // merge the circuits, TBD
    //     printf("IMPLEMENT MERGING CIRCUITS\n");
    // }
    // else if (curr_ckt == 0 && other_ckt != 0)
    // {
    //     // take circuit from other
    //     p.points[i].circuit = other_ckt;
    // }
    


#undef printf
    // printf("\nresult: %llu\n", result); 

    return 0;
}