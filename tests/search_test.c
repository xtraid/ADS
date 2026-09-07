#include "b_search.h"
#include <stdlib.h>
#include <stdio.h>

static void unsorted_initialization (int *arr, int n){
  if(!arr)
    return;
  for (int i = 0; i<n; i++)
    arr[i] = n - i;
}

int main (void){
  const int n = 10;
  int *unsorted = malloc(sizeof(int) * n);

  if (!unsorted){
    printf("malloc failed\n");
    return -2;
  }

  unsorted_initialization(unsorted, n);



  printf("correctness tests:\n");

  int expected_check = linear_search(unsorted, n, 5);
  if(expected_check != 5)
    printf("Error: function returned an index different fron 4 : arr[%d] = %d\n", expected_check, unsorted[expected_check]);
  else
    printf("in bound key: pass\n");

  int out_of_bound_key = linear_search(unsorted, n, n+1);
  if(out_of_bound_key != -1)
    printf("Error: called an out of bound index got:  %d\n", out_of_bound_key);
  else
    printf("out of bound key: pass\n");








  free(unsorted);
  return 0;
}
