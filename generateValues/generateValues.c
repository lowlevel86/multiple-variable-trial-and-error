#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mvtrialanderror.h"

//compile for windows:
//tcc -mwindows -o generateValues.exe generateValues.c mvtrialanderror.c

//compile for linux:
//tcc -lm -o generateValues generateValues.c mvtrialanderror.c

//execute directly with:
//tcc mvtrialanderror.c -run generateValues.c

int main(int argc, char **argv)
{
   unsigned int i, j, argi = 4;
   unsigned int iBgn, iEnd;
   unsigned int valueArrayCnt;
   double topMostValueArray[32];
   double bottomMostValueArray[32];
   double valueArray[32];
   
   if (argc < 4)
   {
      printf("usage:\n");
      printf("generateValues [dimensions] [iteration start] [iteration end] optional:[min] [max] [min] [max]...\n\n");
      printf("example 1:\n");
      printf("generateValues 2 0 100\n\n");
      printf("example 2:\n");
      printf("generateValues 3 8 100 -10 10 0 10 0 360\n\n");
      
      return 0;
   }
   
   //number of dimensions
   valueArrayCnt = atoi(argv[1]);
   
   //find the min and max value ranges
   for (i=0; i < valueArrayCnt; i++)
   {
      if (argi < argc)
      bottomMostValueArray[i] = atof(argv[argi]);
      else
      bottomMostValueArray[i] = -1;
      
      argi++;
      
      if (argi < argc)
      topMostValueArray[i] = atof(argv[argi]);
      else
      topMostValueArray[i] = 1;
      
      argi++;
   }
   
   iBgn = strtoul(argv[2], NULL, 0);
   iEnd = strtoul(argv[3], NULL, 0);
   
   for (i=iBgn; i < iEnd; i++)
   {
      generateRangedValues(&topMostValueArray[0], &bottomMostValueArray[0],
                           valueArrayCnt, i, &valueArray[0]);
      
      for (j=0; j < valueArrayCnt; j++)
      {
         printf("%f ", valueArray[j]);
      }
      printf("\n");
   }
   
   generateRangedValues(&topMostValueArray[0], &bottomMostValueArray[0],
                        valueArrayCnt, iEnd, &valueArray[0]);
   
   for (j=0; j < valueArrayCnt; j++)
   {
      printf("%f ", valueArray[j]);
   }
   printf("\n");
   
   return 0;
}
