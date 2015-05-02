#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.c"
#include "platform.c"
#include "bruteforce.c"

int main()  
{ 
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
  // template will put all importances and distances equal to 1
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
        fprintf(f, "%d ", (cur_p->optimal_constructions[i].roads[n * k + j]));
      fprintf(f, "\n");
    }
    fprintf(f, "\n"); 
  }
  fclose(f);
  // HEURISTIC END 
  time(&end_t);
  int diff_t = difftime(end_t, start_t);
  printf("Heuristic Time: %d seconds\n\n", diff_t);
 
  int bf = 0;
  printf("Bruteforce Search? 1 = Yes, 0 = No: ");
    scanf("%d", &bf);
  if (bf)
  { 
    printf("Brute Force Results: \n\n");
    time_t start_t, end_t;
    int num_constructions = cur_p->max_m - cur_p->min_m + 1;
    float *heuristic_optimalities = malloc(num_constructions * sizeof(float));
    for (int i = 0; i < num_constructions; i++)
      heuristic_optimalities[i] = cur_p->optimal_constructions[i].optimality;
    time(&start_t);
    bruteforce_compare(t, heuristic_optimalities);
    time(&end_t);
    free(heuristic_optimalities);
    int diff_t = difftime(end_t, start_t);
    printf("\nBrute Force Time: %d seconds\n", diff_t);
  }
  
  int num_constructions = cur_p->max_m - cur_p->min_m + 1;
  for (int j = 0; j < num_constructions; j++)
  {
    free(cur_p->optimal_constructions[j].degree);
    free(cur_p->optimal_constructions[j].roads);
  }
  free(cur_p->optimal_constructions);
  free(cur_p);
  free(ranks);
  free(t.distances);
  free(t.importances);
}
