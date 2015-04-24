#include <stdio.h>
#include <stdlib.h>
#include "platform.c"

int main()
{
  printf("Should return true: %d\n", test_connected1());
  printf("Should return false: %d\n", test_connected2());
  test_degrees();  
  
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
