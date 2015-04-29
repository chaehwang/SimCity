#include <stdio.h>
#include <stdlib.h>
#include "platform.c"

typedef struct {
  int n;
  int *importances;
} hi; 

int y(int t)
{ 
  return 3;  
}
  
float *bruteforce (town t, int edges)
{
    bool *tri = malloc((t.n*t.n-t.n)/2*sizeof(bool));
    bool *all = malloc((t.n*t.n)*sizeof(bool));
    float sum = 0;
    int num = 0;
    float *opt = malloc(sizeof(float)*2);
    opt[0] = 100;

    for (int i=(t.n*t.n-t.n)/2-edges; i<(t.n*t.n-t.n)/2; i++)
    {
        tri[i]=1;
    }
    while(true)
    {
        int counter = 0;
        for (int i=0; i<t.n;i++)
        {
            for(int j=0; j<=i;j++)
            {
                if (i==j)
                {
                    all[t.n*i+j] = 0;
                }
                else
                {
                    all[t.n*i+j] = all[t.n*j+i] = tri[counter];
                    counter++;
                }
            }
        }
        road_construction rc;
        rc.n = t.n;
        rc.m = edges;
        rc.roads = malloc(t.n * t.n * sizeof(bool));
        for (int i = 0; i < t.n*t.n; i++)
          rc.roads[i] = all[i];
        //rc.roads = all;
        rc.degree = degree(all,t.n);
        // TODO: free times and traffic_dist
        rc.optimality = times_to_optimality(t, times(traffic_dist(t,rc,t.n),t.n));
        
        if (connected(rc))
        {
            sum+= rc.optimality;
            num++;
            if (rc.optimality < opt[0])
                opt[0] = rc.optimality;
            //printf("%f ", rc.optimality);

        }
        /*for (int i = 0; i < (t.n*t.n-t.n)/2; i++)
            printf("%d", tri[i]);
        printf(" ");*/  
        if (full(tri, (t.n*t.n-t.n)/2, edges))
            break;
        next(tri, (t.n*t.n-t.n)/2);
    }
    opt[1] = sum/num;  

    //free(tri);
    //free(all);
    return opt;
}


void test_bruteforce()
{
    town t;
    t.n = 6;
    float dist[36] = {0,1,3,2,1,1,1,0,9,4,3,1,3,9,0,2,3,2,2,4,2,0,6,1,1,3,3,6,0,3,1,1,2,1,3,0};
    t.distances = dist;
    int importances[6] = {1,1,1,1,1,1};
    t.importances = importances;

    printf("MIN: %f\nAVERAGE: %f\n", bruteforce(t,6)[0], bruteforce(t,6)[1]);
}

int main()  
{ 
 
  //test_distance();
  town test_town;
  test_town.n = 6;
  test_town.importances = malloc(test_town.n * sizeof(int));
  for (int i = 0; i < 6; i++)
    test_town.importances[i] = 1; 
  test_town.distances = malloc(test_town.n * test_town.n * sizeof(int));
  test_town.distances[0] = 0;
  test_town.distances[1] = 1;
  test_town.distances[2] = 3;
  test_town.distances[3] = 2;
  test_town.distances[4] = 1;
  test_town.distances[5] = 1;
  test_town.distances[6] = 1;
  test_town.distances[7] = 0;
  test_town.distances[8] = 9;
  test_town.distances[9] = 4;
  test_town.distances[10] = 3;
  test_town.distances[11] = 1; 
  test_town.distances[12] = 3;
  test_town.distances[13] = 9;
  test_town.distances[14] = 0;
  test_town.distances[15] = 2;
  test_town.distances[16] = 3;
  test_town.distances[17] = 2;
  test_town.distances[18] = 2;
  test_town.distances[19] = 4;
  test_town.distances[20] = 2;
  test_town.distances[21] = 0;
  test_town.distances[22] = 6;
  test_town.distances[23] = 1;
  test_town.distances[24] = 1; 
  test_town.distances[25] = 3;
  test_town.distances[26] = 3;
  test_town.distances[27] = 6;
  test_town.distances[28] = 0;
  test_town.distances[29] = 3;
  test_town.distances[30] = 1;
  test_town.distances[31] = 1;
  test_town.distances[32] = 2;
  test_town.distances[33] = 1;
  test_town.distances[34] = 3;
  test_town.distances[35] = 0;
  
  road_construction r;
  r.n = 6; r.m = 8; r.degree = malloc(6 * sizeof(int));
  r.degree[0] = 3;
  r.degree[1] = 3;
  r.degree[2] = 3;
  r.degree[3] = 2;
  r.degree[4] = 3;
  r.degree[5] = 2;
  r.roads = malloc(36 * sizeof(bool));
  r.roads[0] = 0;
  r.roads[1] = 1;
  r.roads[2] = 1;
  r.roads[3] = 0;
  r.roads[4] = 1;
  r.roads[5] = 0;
  r.roads[6] = 1;
  r.roads[7] = 0;
  r.roads[8] = 1;
  r.roads[9] = 1;
  r.roads[10] = 0;
  r.roads[11] = 0;
  r.roads[12] = 1;
  r.roads[13] = 1;
  r.roads[14] = 0;
  r.roads[15] = 0;
  r.roads[16] = 1;
  r.roads[17] = 0;
  r.roads[18] = 0;
  r.roads[19] = 1;
  r.roads[20] = 0;
  r.roads[21] = 0;
  r.roads[22] = 0;
  r.roads[23] = 1;
  r.roads[24] = 1;
  r.roads[25] = 0;
  r.roads[26] = 1;
  r.roads[27] = 0;
  r.roads[28] = 0;
  r.roads[29] = 1;
  r.roads[30] = 0;
  r.roads[31] = 0;
  r.roads[32] = 0;
  r.roads[33] = 1;
  r.roads[34] = 1;
  r.roads[35] = 0;
  
    
  for (int i = 0; i < 6; i++)
        { 
          printf("\n"); 
          for (int j = 0; j < 6; j++)
            printf("%d ", r.roads[6 * i + j]);
        }
  
  printf("\n Degrees: ");
        for (int i = 0; i < 6; i++)
          printf("%d ", r.degree[i]);
  
  float *td = traffic_dist(test_town, r, 6);
        float *times_matrix = times(td, 6);
        printf("Ans: %f\n", times_to_optimality(test_town, times_matrix));
        
        printf("\n TD Matrix: ");
        for (int i = 0; i < 6; i++)
        { 
          printf("\n"); 
          for (int j = 0; j < 6; j++)
            printf("%f ", td[6 * i + j]);
        }
        printf("\n");
        
  printf("Ans2: %f\n", times_to_optimality(test_town, times(traffic_dist(test_town,r,6),6)));
  
  
  
  //platform *test = new_platform(5);
  //printf("%d, %d", test->min_m, test->max_m);
  
  
  platform *cur_p = new_platform(2); 
  cur_p->optimal_constructions[0].degree[0] = 1; 
  cur_p->optimal_constructions[0].degree[1] = 1;
  cur_p->optimal_constructions[0].roads[0] = 0;
  cur_p->optimal_constructions[0].roads[1] = 1;
  cur_p->optimal_constructions[0].roads[2] = 1;
  cur_p->optimal_constructions[0].roads[3] = 0;
  //printf("%d, %d", test.min_m, test.max_m);
       
    /*platform *new_p = new_platform(3);
    town sub_town;
    sub_town.n = 3;
    sub_town.distances = malloc(3 * 3 * sizeof(float));
    sub_town.importances = malloc(3 * sizeof(int));
    for (int j = 0; j < 3; j++)
    {
      sub_town.importances[j] = test_town.importances[j];
      for (int k = 0; k < 3; k++)
        sub_town.distances[3 * j + k] = sub_town.distances[3 * k + j] = test_town.distances[4 * j + k];
    }
    extend_platform(sub_town, cur_p, new_p);
     */
   
  for (int i = 3; i <= 6; i++) 
  {
    platform *new_p = new_platform(i);
    town sub_town; 
    sub_town.n = i;
    sub_town.distances = malloc(i * i * sizeof(float));
    sub_town.importances = malloc(i * sizeof(int));
    for (int j = 0; j < i; j++)
    {
      sub_town.importances[j] = test_town.importances[j];
      for (int k = j + 1; k < i; k++)
        sub_town.distances[i * j + k] = sub_town.distances[i * k + j] = test_town.distances[4 * j + k];
    } 
    extend_platform(sub_town, cur_p, new_p);
    free(cur_p->optimal_constructions);
    free(cur_p);
    cur_p = new_p;    
  }  
      
 
  printf("5 edges: %f\n", cur_p->optimal_constructions[0].optimality);
  printf("6 edges: %f\n", cur_p->optimal_constructions[1].optimality);
  printf("7 edges: %f\n", cur_p->optimal_constructions[2].optimality);
  printf("8 edges: %f\n", cur_p->optimal_constructions[3].optimality);
  printf("9 edges: %f\n", cur_p->optimal_constructions[4].optimality);
  printf("10 edges: %f\n", cur_p->optimal_constructions[5].optimality);
  printf("12 edges: %f\n", cur_p->optimal_constructions[6].optimality);  
   
  /* bool *arr = malloc(3 * sizeof(bool));
  arr[0] = 0; arr[1] = 0; arr[2] = 1;
  next(arr, 3);
  for (int i = 0; i < 3; i++)
    printf("%d ", arr[i]); */
  /*
  hi *x = malloc(sizeof(hi)); 
  x->importances = malloc(12 * sizeof(int));
  x->importances[3] = 5;
 // *x = y(3);
  printf("HERE: %d", x->importances[3]); */
  
  /* int *test = malloc(12 * sizeof(road_construction)); */
  
  /* if (test[0] == NULL)
    printf("hi"); */ 
  /* test[5] = 10;
  test[3] = 8;
  for (int i = 0; i < 12; i++)
  { 
    printf("%d\n", test[i]);
  } 
  printf("%d\n", sizeof(int)); */
  test_bruteforce();
  //test_time();

}
 

