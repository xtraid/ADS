#include "b_search.h"




int linear_search (int *arr, int n, int key){
  if (!arr)
    return -2;
  for (int i = 0; i<n; i++){
    if (arr[i] == key)
      return i;
  }
  return -1;
}


int b_search (int * arr, int s, int n, int key){
  if (!arr)
    return -2;
  if (s > n)
    return -1;
  int m = (s+n) / 2;
  if (arr[m] == key)
    return m;
  if (arr[m] > key)
    return b_search(arr, s, m-1, key);
  else 
    return b_search(arr, m+1, n, key);
}

