#include <stdio.h>

int main()
{
  int i, ar[10] = {5, 8, 12, 2, 1, 7};
  sort(ar, 6);
  for(i = 0; i < 6; i++)
  	printf("%d\n", ar[i]);
  	
  return 0;
} // main


