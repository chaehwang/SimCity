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

