#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mvtrialanderror.h"

//compile for windows:
//tcc -mwindows -o genValues.exe genValues.c mvtrialanderror.c

//compile for linux:
//tcc -lm -o genValues genValues.c mvtrialanderror.c

//execute directly with:
//tcc mvtrialanderror.c -run genValues.c

int main(int argc, char **argv)
{
   int i, j, argi = 4;
   int valueArrayCnt;
   float topMostValueArray[32];
   float bottomMostValueArray[32];
   float valueArray[32];
   
   if (argc < 4)
   {
      printf("usage:\n");
      printf("genValues [dimensions] [iteration start] [iteration end] optional:[max] [min] [max] [min]...\n\n");
      printf("example 1:\n");
      printf("genValues 2 1 100\n\n");
      printf("example 2:\n");
      printf("genValues 3 9 100 10 -10 10 0 360 0\n\n");
      
      return 0;
   }
   
   //number of dimensions
   valueArrayCnt = atoi(argv[1]);
   
   //find the max and min value ranges
   for (i=0; i < valueArrayCnt; i++)
   {
      if (argi < argc)
      topMostValueArray[i] = atof(argv[argi]);
      else
      topMostValueArray[i] = 1;
      
      argi++;
      
      if (argi < argc)
      bottomMostValueArray[i] = atof(argv[argi]);
      else
      bottomMostValueArray[i] = -1;
      
      argi++;
   }
   
   for (i=atoi(argv[2]); i <= atoi(argv[3]); i++)
   {
      generateValuesC(&topMostValueArray[0], &bottomMostValueArray[0],
                      valueArrayCnt, &valueArray[0], i);
      
      for (j=0; j < valueArrayCnt; j++)
      {
         printf("%f ", valueArray[j]);
      }
      printf("\n");
   }
   
   return 0;
}
