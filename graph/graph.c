#include <stdio.h>
#include "mvtrialanderror.h"
#define TRUE 1
#define FALSE 0

//compile for windows:
//tcc -mwindows -o graph.exe graph.c mvtrialanderror.c

//compile for linux:
//tcc -lm -o graph mvtrialanderror.c graph.c

//execute directly with:
//tcc mvtrialanderror.c -run graph.c

//view graph:
//gnuplot -p graphWxt.gnu

//draw graph to png:
//gnuplot graph.gnu


void yCutsLine(double x1Line, double x2Line, double y1Line, double y2Line,
               int cutCnt, double *xCutLocs, double *yCutLocsRet)
{
   int i;
   
	for (i=0; i < cutCnt; i++)
   {
		yCutLocsRet[i] = y1Line - (x1Line - xCutLocs[i]) / (x1Line - x2Line) * (y1Line - y2Line);
	}
}

double maxValueInArray(double *valueArray, int valueArrayCnt)
{
   int i;
   double retValue;
   
   retValue = valueArray[0];
   
   for (i=1; i < valueArrayCnt; i++)
   {
      if (valueArray[i] > retValue)
      retValue = valueArray[i];
   }
   
   return retValue;
}

double minValueInArray(double *valueArray, int valueArrayCnt)
{
   int i;
   double retValue;
   
   retValue = valueArray[0];
   
   for (i=1; i < valueArrayCnt; i++)
   {
      if (valueArray[i] < retValue)
      retValue = valueArray[i];
   }
   
   return retValue;
}

int main()
{
   unsigned int i;
   double xPoints[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0};
   double yPoints[] = {749.0, 614.0, 518.0, 412.0, 398.0, 347.0, 291.0, 238.0, 338.0};
   int pointCnt = sizeof(xPoints)/sizeof(double);
   double *xCutPts = xPoints;
   double yCutPtsRet[sizeof(xPoints)/sizeof(double)];
   double xPointMax;
   double xPointMin;
   double topMostValue;
   double bottomMostValue;
   
   double topMostValueArray[2];
   double bottomMostValueArray[2];
   double middleValueArray[2];
   
   int lineValueArrayCnt = 2;
   double lineBaseValueArray[2];
   double linePossibValueArray[2];
   double lineValueArray[2];
   
   double lastValues[sizeof(xPoints)/sizeof(double)];
   double closestValues[sizeof(xPoints)/sizeof(double)];
   double tolerance = 0.001;
   int rubberBandValues = TRUE;
   int valueIsCloserArray[sizeof(xPoints)/sizeof(double)];
   int valueIsMostClosestArray[sizeof(xPoints)/sizeof(double)];
   double valueToTargetSum;
   
   FILE *ptDataFilePtr;
   FILE *lineDataFilePtr;
   
   
   //find the min and max x and y values
   xPointMax = maxValueInArray(&xPoints[0], pointCnt);
   xPointMin = minValueInArray(&xPoints[0], pointCnt);
   topMostValue = maxValueInArray(&yPoints[0], pointCnt);
   bottomMostValue = minValueInArray(&yPoints[0], pointCnt);
   
   //write point data to file
   ptDataFilePtr = fopen("point.dat", "wb");
   
   if (!ptDataFilePtr)
   return 0;
   
   for (i=0; i < pointCnt; i++)
   {
      fprintf(ptDataFilePtr, "%f %f\n\n", xPoints[i], yPoints[i]);
   }
   
   fclose(ptDataFilePtr);
   
   
   lineDataFilePtr = fopen("line.dat", "wb");
   
   if (!lineDataFilePtr)
   return 0;
   
   //initialize the top most and bottom most values
   topMostValueArray[0] = topMostValue;
   bottomMostValueArray[0] = bottomMostValue;
   topMostValueArray[1] = topMostValue;
   bottomMostValueArray[1] = bottomMostValue;
   
   iniGuessValues(&topMostValueArray[0], &bottomMostValueArray[0], lineValueArrayCnt,
                  &lineBaseValueArray[0], &linePossibValueArray[0], &lineValueArray[0]);
   
   iniCloserFurther(&lastValues[0], &closestValues[0], pointCnt);
   
   
   //find a line that best approximates the point data
   for (i=1; i < 50; i++)
   {
      yCutsLine(xPointMin, xPointMax, lineValueArray[0], lineValueArray[1],
                pointCnt, &xCutPts[0], &yCutPtsRet[0]);
      
      closerFurther(&yCutPtsRet[0], &yPoints[0], &lastValues[0], &closestValues[0],
                    pointCnt, tolerance, rubberBandValues,
                    &valueIsCloserArray[0], &valueIsMostClosestArray[0], &valueToTargetSum);
      
      printf("valueToTargetSum: %f\n", valueToTargetSum);
      
      guessValues(&topMostValueArray[0], &bottomMostValueArray[0],
                  lineValueArrayCnt, valueToTargetSum, i,
                  &lineBaseValueArray[0], &linePossibValueArray[0], &lineValueArray[0]);
      
      //write line data to file
      fprintf(lineDataFilePtr, "%f %f\n", xPointMin, lineValueArray[0]);
      fprintf(lineDataFilePtr, "%f %f\n\n\n", xPointMax, lineValueArray[1]);
   }
   
   fclose(lineDataFilePtr);
   
   return 0;
}
