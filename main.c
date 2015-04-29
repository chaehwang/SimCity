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

int main()  
{ 
  platform *test = malloc(sizeof(platform));
  *test = new_platform(5);
  printf("%d, %d", test->min_m, test->max_m);
  
  hi *x = malloc(sizeof(hi));
  x->importances = malloc(12 * sizeof(int));
  x->importances[3] = 5;
 // *x = y(3);
  printf("HERE: %d", x->importances[3]);
  
  
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

}

float *bruteforce (town t)
{
    float sum = malloc(sizeof(float));
    int num = malloc(sizeof(int));
    float opt = malloc(sizeof(float)*2);
    int *tri = malloc(sizeof(int)*(t.n*t.n*-t.n)/2);
    int *all = malloc(sizeof(int)*t.n*t.n);
    for (int edges = 1; edges < K_MAX; edges++)
    {
        for (int i=t.n-edges; i<t.n; i++)
        {
            tri[i]=1;
        }
        do
        {
            int counter = 0;
            for (int i=0; i<t.n;i++)
            {
                for(int j=0; j<=i;j++)
                {
                    if (i=j)
                    {
                        all[n*i+j] = 0;
                    }
                    else
                    {
                        all[n*i+j] = all[n*j+i] = tri[counter];
                    }
                }
            }
            road_construction rc;
            rc.n = t.n;
            rc.m = edges;
            rc.degrees = degrees(all, t.n)
            rc.roads = all;
            rc.optimality = times_to_optimality(t, times(traffic_dist(t, rc, t.n), t.n));
            
            if(connected rc)
            {
                if(rc.optimality<minopt)
                    opt[0]=rc.optimality;
                    
                sum += rc.optimality;
                num ++;
                opt[1] = sum/num;
            }
            next(tri, t.n);
        }
        while (!full(tri, t.n, edges))  
    }
    free(tri);
    free(sum);
    free(num);
    free(all);
    return opt;  
}

float *bruteforce (town t)
{
    int *tri = malloc((t.n*t.n-t.n)/2*sizeof(int));
    int *all = malloc((t.n*t.n)*sizeof(bool));
    float sum = malloc(sizeof(float));
    int num = malloc(sizeof(num));
    float opt = malloc(sizeof(float)*2);
    for (int edges = 1; edges < K_MAX; edges++)
    {
        for (int i=t.n-edges; i<t.n; i++)
        {
            tri[i]=1;
        }
        do
        {
            int counter = 0;
            for (int i=0; i<t.n;i++)
            {
                for(int j=0; j<=i;j++)
                {
                    if (i=j)
                    {
                        all[n*i+j] = 0;
                    }
                    else
                    {
                        all[n*i+j] = all[n*j+i] = tri[counter];
                    }
                }
            }
            road_construction rc;
            rc.n = t.n;
            rc.m = edges;
            rc.roads = all;
            rc.degrees = degree(all,t.n);
            rc.optimality = times_to_optimality(t, times(traffic_dist(t,rc,t.n),t.n));
            
            if(connected rc)
            {
                sum+= rc.optimality;
                num++;
                if (rc.optimality < minopt)
                    opt[0] = rc.optimality;
                opt[1] = sum/num;
            }  
            next(tri, t.n);
        }
        while (!full(tri, t.n, edges))
    }
    free(tri);
    free(sum);
    free(num);
    free(all);
    return opt
}

