#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// given a town and the number of roads to construct, brute forces
// the best optimality value that can be obtained, as well as the
// average optimality for all road constructions that are connected
float *bruteforce (town t, int edges)
{
    // tri is the upper triangle of the road constrction matrix,
    // where edges are to be permuted through. all is the entire
    // matrix, which can be calculated by symmetry after tri
    int tri_length = (t.n * t.n - t.n)/2;
    bool *tri = calloc(tri_length, sizeof(bool));
    bool *all = calloc(t.n * t.n, sizeof(bool));
    long double sum = 0;
    int num = 0;
    // opt[0] is the best optimality, opt[1] is the average optimality
    float *opt = calloc(2, sizeof(float));  
    opt[0] = INFTY; 
    // start with the first permutation, with all edges in the
    // last entries of tri
    for (int i = tri_length - edges; i < tri_length; i++)
        tri[i] = 1;
    
    // go through every permutation
    while(true)
    {
        int counter = 0;
        for (int i = 1; i < t.n; i++)
        {
            for(int j = 0; j < i; j++)
            {
              all[t.n * i + j] = all[t.n * j + i] = tri[counter];
              counter++;
            }
        }
        road_construction rc;
        rc.n = t.n;
        rc.m = edges;
        rc.roads = calloc(t.n * t.n, sizeof(bool));
        for (int i = 0; i < t.n * t.n; i++)
          rc.roads[i] = all[i];
        // if this permutation is not connected, move on to the next one
        if (!connected(rc))
        {
          free(rc.roads);
          if (full(tri, tri_length, edges))
            break;
          next(tri, tri_length);
          continue;
        }
         
        // otherwise, calculate optimality
        rc.degree = degree(all,t.n);
        float *td = traffic_dist(t, rc, rc.n);
        float *times_matrix = times(td, rc.n);
        rc.optimality = times_to_optimality(t, times_matrix);
        free(rc.degree);
        free(td);
        free(times_matrix);      
        
        // update sum of total optimalities and number of constructions
        sum+= rc.optimality;
        num++;
        // keep track of best optimality
        if (rc.optimality < opt[0])
          opt[0] = rc.optimality;
          

        free(rc.roads);  
        if (full(tri, tri_length, edges))
          break;
        next(tri, tri_length);
    }
    // calculate average optimality of connected road constructions
    opt[1] = sum/num;
    free(tri);
    free(all); 
    return opt;
}
 
// apply bruteforce for every possibility of number of edges
void bruteforce_compare(town t, float *optimalities)
{
    int max_edges = min((t.n * t.n - t.n)/2, t.n * MAX_K/2);
    for (int i = t.n - 1; i <= max_edges; i++)
    {
      float *bf = bruteforce(t,i);
      printf("%d edges: MIN: %f AVERAGE: %f CHI: %f\n", i, 
        bf[0], bf[1], (optimalities[i - t.n + 1] - bf[0])/(bf[1] - bf[0]));
      free(bf);
    }
}
