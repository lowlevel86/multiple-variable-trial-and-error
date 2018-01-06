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
   int64_t layerSize;
   int64_t layerSizeSum;
   double valueArray[32];
   
   for (dimensions=1; dimensions <= 32; dimensions++)
   {
      printf("dimensions:%i\n", dimensions);
      
      layer = 0;
      layerSizeSum = sumCurrentAndPriorLayers(dimensions, layer);
      layerSize = layerSizeSum - 0;
      
      while (layerSizeSum <= 0x100000000)//32bit overflow check
      {
         printf("\n");
         printf("layer %i size:%lld\n", layer, layerSize);
         printf("count %lld, starting values:\n", layerSizeSum-layerSize);
         
         generateValues(dimensions, layerSizeSum-layerSize, &valueArray[0]);
         
         for (i=0; i < dimensions; i++)
         {
            printf("%.10f ", valueArray[i]);
         }
         printf("\n");
         
         
         printf("count %lld, ending values:\n", layerSizeSum-1);
         
         generateValues(dimensions, layerSizeSum-1, &valueArray[0]);
         
         for (i=0; i < dimensions; i++)
         {
            printf("%.10f ", valueArray[i]);
         }
         printf("\n");
         
         
         layer++;
         layerSizeSum = sumCurrentAndPriorLayers(dimensions, layer);
         layerSize = layerSizeSum - sumCurrentAndPriorLayers(dimensions, layer-1);
      }
      printf("\n\n");
   }
   
   return 0;
}
