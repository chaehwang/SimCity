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
  town test_town;
  test_town.n = 4;
  test_town.importances = malloc(test_town.n * sizeof(int));
  for (int i = 0; i < 4; i++)
    test_town.importances[i] = 1; 
  test_town.distances = malloc(test_town.n * sizeof(int));
  test_town.distances[1] = 3;
  test_town.distances[2] = 5;
  test_town.distances[3] = 7;
  test_town.distances[4] = 3;
  test_town.distances[6] = 3;
  test_town.distances[7] = 9;
  test_town.distances[8] = 5;
  test_town.distances[9] = 3;
  test_town.distances[11] = 5;
  test_town.distances[12] = 7;
  test_town.distances[13] = 9;
  test_town.distances[14] = 5;
  
  
  platform test = new_platform(2); 
  //printf("%d, %d", test.min_m, test.max_m);
  for (int i = 3; i <= 4; i++)
  {
    town sub_town;
    sub_town.n = i;
    sub_town.distances = malloc(i * i * sizeof(float));
    sub_town.importances = malloc(i * sizeof(int));
    for (int j = 0; j < i; j++)
    {
      sub_town.importances[i] = test_town.importances[i];
      for (int k = j + 1; k < i; k++)
        sub_town.distances[i * j + k] = sub_town.distances[i * k + j] = test_town.distances[4 * j + k];
    }
  }
  
  /* bool *arr = malloc(3 * sizeof(bool));
  arr[0] = 0; arr[1] = 0; arr[2] = 1;
  next(arr, 3);
  for (int i = 0; i < 3; i++)
    printf("%d ", arr[i]); */
  
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

