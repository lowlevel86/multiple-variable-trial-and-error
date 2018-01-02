#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mvtrialanderror.h"

//compile for windows:
//tcc -mwindows -o test.exe test.c mvtrialanderror.c

//compile for linux:
//tcc -lm -o test test.c mvtrialanderror.c

//execute directly with:
//tcc mvtrialanderror.c -run test.c

//run with:
//test > test.txt
//or
//./test > test.txt

int main(int argc, char **argv)
{
   int i, dimensions, layer;
   long long int layerSize;
   long long int layerSizeSum;
   long long int layerSizeSumDelta;
   double valueArray[32];
   
   for (dimensions=1; dimensions <= 32; dimensions++)
   {
      printf("dimensions:%i\n", dimensions);
      layer = 0;
      layerSize = 0;
      layerSizeSum = 0;
      layerSizeSumDelta = 0;
      
      while (1)
      {
         layerSizeSum += layerSize;
         layerSize = getLayerValuesCnt(dimensions, layer);
         layerSizeSumDelta += layerSize;
         
         //32bit overflow check
         if (layerSizeSumDelta > 0x100000000 - layerSizeSum)
         break;
         
         
         printf("\n");
         printf("layer %i size:%lld\n", layer, layerSize);
         printf("count %lld starting values:\n", layerSizeSum);
         
         generateValuesB(dimensions, layerSizeSum, &valueArray[0]);
         
         for (i=0; i < dimensions; i++)
         {
            printf("%.10f ", valueArray[i]);
         }
         printf("\n");
         
         
         printf("count %lld ending values:\n", layerSizeSumDelta-1);
         
         generateValuesB(dimensions, layerSizeSumDelta-1, &valueArray[0]);
         
         for (i=0; i < dimensions; i++)
         {
            printf("%.10f ", valueArray[i]);
         }
         printf("\n");
         
         
         layer++;
      }
      printf("\n\n");
   }
   
   return 0;
}
