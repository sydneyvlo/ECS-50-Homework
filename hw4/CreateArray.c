#include <stdio.h>
#include <stdlib.h>

int main()
{
  int i, size, start, seed;
  char filename[80];
  FILE *fp;

  printf("This program creates a MIPS file that fills an area in RAM with\n");
  printf("values that do not exceed 0xFFFF.\n");
  printf("Start address of n hex: ");
  scanf("%x", &start);
  printf("Size of array (in decimal): ");
  scanf("%d", &size);
   printf("Seed for random number generator: ");
  scanf("%d", &seed);
  srand(seed);
  printf("Filename: ");
  scanf("%s", filename);

  fp = fopen(filename, "w");
  fprintf(fp, "# Start: $%x, Size: %d, Seed: %d\n", start, size, seed);
  fprintf(fp, "\t.DATA 0x%x\n", start); 


  for(i = 0; i < size; i++)
  {
    fprintf(fp, "\t.WORD\t%d  #%x\n", (rand() & 0xffff), start + i * 4);
  } // for i


  fclose(fp);
  return 0;
}

