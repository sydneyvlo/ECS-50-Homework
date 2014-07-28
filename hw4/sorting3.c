#include <stdio.h>
#include <string.h>

/*
void sort(char ar[][25], int n)
{
  char temp[25];
  int i, j;
	
  for(i = 1; i < n; i++)
  {
    j = i;
    strcpy(temp, ar[i]);

    while(j > 0 && strcmp(temp, ar[j - 1]) < 0)
    {
      strcpy(ar[j], ar[j - 1]);
      j--;
    }
	  
    strcpy(ar[j], temp);
  }  // for i
}  // sort()
*/

int main()
{
  int i;
  char ar[][25] = {"Tommorrow,", "this", "will", "be", "part", "of", "the", 
		"unchangeable", "past", "but", "fortunately,", "it", "can", "still",
		"be", "changed", "today.", " by ", "Ashleigh", "Brilliant"};
	

  sort(ar, 20);
  for(i = 0; i < 20; i++)
    printf("%s\n", ar[i]);
	
  return 0;
} // main


