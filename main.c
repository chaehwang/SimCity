#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.c"
#include "platform.c"

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
    float sum = 0;
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
void bruteforce_all(town t)
{
    int max_edges = min((t.n * t.n - t.n)/2, t.n * MAX_K/2);
    for (int i = t.n - 1; i <= max_edges; i++)
    {
      float *bf = bruteforce(t,i);
      printf("%d edges: MIN: %f AVERAGE: %f\n", i, bf[0], bf[1]);
      free(bf);
    }
}

int main()  
{ 
  /*town test_town;
  test_town.n = 6;
  test_town.importances = malloc(test_town.n * sizeof(int));
  for (int i = 0; i < 6; i++)
    test_town.importances[i] = 1; 
  test_town.distances = malloc(test_town.n * test_town.n * sizeof(int));
  int dist[36] = 
    {0,1,3,2,1,1,
     1,0,9,4,3,1,
     3,9,0,2,3,2,
     2,4,2,0,6,1,
     1,3,3,6,0,3,
     1,1,2,1,3,0};
  test_town.distances = dist;
  
  road_construction r;
  r.n = 6; r.m = 8; 
  int r_degree[6] = {3,3,3,2,3,2};
  r.degree = r_degree;
  bool r_roads[36] =
    {0,1,1,0,1,0,
     1,0,1,1,0,0,
     1,1,0,0,1,0,
     0,1,0,0,0,1,
     1,0,1,0,0,1,
     0,0,0,1,1,0};
  r.roads = r_roads; */
  
  // create base platform of two nodes connected to each other
  platform *cur_p = new_platform(2);
  cur_p->optimal_constructions[0].degree[0] = 1;
  cur_p->optimal_constructions[0].degree[1] = 1;
  cur_p->optimal_constructions[0].roads[0] = 0;
  cur_p->optimal_constructions[0].roads[1] = 1;
  cur_p->optimal_constructions[0].roads[2] = 1;
  cur_p->optimal_constructions[0].roads[3] = 0;
  
  int n;
  printf("Number of buildings: ");
  scanf("%d", &n);
  town t;
  t.n = n;
  int manual = 0;
  t.distances = calloc(n * n, sizeof(int));
  t.importances = calloc(n, sizeof(int)); 
  printf("Manually Input or Template Town? 1 = Manual, 0 = Template: ");
    scanf("%d", &manual);
  // template will put all importances and distances equal to 1
  if (manual)
  {
    printf("\nImportance array of %d values, separated by spaces: \n", t.n);
    for (int i = 0; i < t.n; i++)
      scanf("%d", &t.importances[i]);
    printf("\nDistance matrix, separated by spaces: \n");
    for (int i = 0; i < t.n; i++)
    {
      for (int j = 0; j <= i; j++)
        printf("%d ", t.distances[t.n * i + j]); 
    
      for (int j = i + 1; j < t.n; j++)
      {
        scanf("%d", &t.distances[t.n * i + j]);
        t.distances[t.n * j + i] = t.distances[t.n * i + j]; 
      }
    }
    printf("\n");
  } 
  else
  {
    for (int i = 0; i < t.n; i++)
      t.importances[i] = 1;
    for (int i = 0; i < t.n; i++)
      for (int j = 0; j < i; j++)
        t.distances[t.n * i + j] = t.distances[t.n * j + i] = 1;
  }
  
  // HEURISTIC START
  time_t start_t, end_t;
  time(&start_t);
  
  /* first sort buildings by importance and reindex the buildings so
     our heuristic can build based on optimizing constructions for
     most important buildings first (the buildings will be reindexed
     back to their original indices after the heuristic is finished
     before printing the road constructions to a separate file) */
  int *ranks = make_rank(t.importances, t.n);
  int *sorted_importances = importance_sort(ranks, t.importances, t.n);
  int *sorted_distances = distance_sort(ranks, t.distances, t.n);
  free(t.importances);
  free(t.distances);
  t.importances = sorted_importances;
  t.distances = sorted_distances; 
  
  // apply heuristic to repeatedly extend the platform from base case 2 nodes
  // up to n nodes
  for (int i = 3; i <= n; i++) 
  {
    platform *new_p = new_platform(i);
    // create subtown of the first i buildings
    town sub_town; 
    sub_town.n = i;
    sub_town.distances = calloc(i * i, sizeof(float));
    sub_town.importances = calloc(i, sizeof(int));
    for (int j = 0; j < i; j++)
    {
      sub_town.importances[j] = t.importances[j];
      for (int k = j + 1; k < i; k++)
        sub_town.distances[i * j + k] = sub_town.distances[i * k + j] 
          = t.distances[n * j + k];
    } 
    extend_platform(sub_town, cur_p, new_p);
    free(sub_town.distances);
    free(sub_town.importances);
    int num_constructions = cur_p->max_m - cur_p->min_m + 1;
    for (int j = 0; j < num_constructions; j++)
    {
      free(cur_p->optimal_constructions[j].degree);
      free(cur_p->optimal_constructions[j].roads);
    }
    free(cur_p->optimal_constructions);
    free(cur_p);
    cur_p = new_p;    
  }  
  
  // print out adjacency matrix for each m 
  printf("\nHeuristic Results: \n");
  int num_rc = cur_p->max_m - cur_p->min_m + 1;
  
  FILE *f = fopen("Heuristic Optimal Road Constructions.txt", "w");
  
  for (int i = 0; i < num_rc; i++)
  {
    printf("%d edges: %f\n", i + cur_p->min_m, 
      cur_p->optimal_constructions[i].optimality);

   
    // prints out to a text file
    if(f == NULL)
    {
      printf("Error in opening file!\n");
      exit(1);
    }
    
    /* reindex the buildings back to their original indices before the
       heuristic was applied, and then print out the corresponding optimal
       road construtions found */
    bool *inverted = 
      reverse_matrix_sort(ranks, cur_p->optimal_constructions[i].roads, t.n);
    free(cur_p->optimal_constructions[i].roads);
    cur_p->optimal_constructions[i].roads = inverted; 
      
    fprintf(f, "%d edges: %f\n", i + cur_p->min_m, 
      cur_p->optimal_constructions[i].optimality);
    fprintf(f, "\n");
    for(int k = 0; k < n; k++)
    {
      for(int j = 0; j < n; j++)
        fprintf(f, "%d ", (cur_p->optimal_constructions[i].roads[n*k+j]));
      fprintf(f, "\n");
    }
    fprintf(f, "\n"); 
  }
  fclose(f);
  // Heuristic end 
  time(&end_t);
  int diff_t = difftime(end_t, start_t);
  printf("Heuristic Time: %d seconds\n\n", diff_t);
  
  int num_constructions = cur_p->max_m - cur_p->min_m + 1;
  for (int j = 0; j < num_constructions; j++)
  {
    free(cur_p->optimal_constructions[j].degree);
    free(cur_p->optimal_constructions[j].roads);
  }
  free(cur_p->optimal_constructions);
  free(cur_p);
 
  int bf = 0;
  printf("Bruteforce Search? 1 = Yes, 0 = No: ");
    scanf("%d", &bf);
  if (bf)
  {
    printf("Brute Force Results: \n\n");
    time_t start_t, end_t;
    time(&start_t);
    bruteforce_all(t);
    time(&end_t);
    double diff_t = difftime(end_t, start_t);
    printf("\nBrute Force Time: %f seconds\n", diff_t);
  }
  
  free(ranks);
  free(t.distances);
  free(t.importances);
}
