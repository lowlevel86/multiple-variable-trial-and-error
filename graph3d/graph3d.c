#include <stdio.h>
#include <math.h>
#include "mvtrialanderror.h"
#define TRUE 1
#define FALSE 0

//compile for windows:
//tcc -mwindows -o graph3d.exe graph3d.c mvtrialanderror.c

//compile for linux:
//tcc -lm -o graph3d mvtrialanderror.c graph3d.c

//execute directly with:
//tcc mvtrialanderror.c -run graph3d.c

//view graph:
//gnuplot -p graph3dWxt.gnu

//draw graph to png:
//gnuplot graph3d.gnu


void zPokesTrigon(float xT1, float yT1, float zT1,
                  float xT2, float yT2, float zT2,
                  float xT3, float yT3, float zT3,
                  int pokeCnt, float *xPokeLocs, float *yPokeLocs, float *zPokeLocsRet)
{
   int i;
   float xE4, yE4, zE4;
   float xQ1toQ2, zQ1toQ2;
   float xQ2toQ3, zQ2toQ3;
   float xQ3toQ4, zQ3toQ4;
   float xQ4toQ1, zQ4toQ1;
   float zPokeLoc1, zPokeLoc2;
   
   //add extra point to convert the trigon into a quad
   xE4 = xT1 - xT2 + xT3;
   yE4 = yT1 - yT2 + yT3;
   zE4 = zT1 - zT2 + zT3;
   
	for (i=0; i < pokeCnt; i++)
   {
      //extend or contract 2 parallel edges of the quad to the same y-plane as "yPokeLocs[i]"
      xQ1toQ2 = xT1 - (yT1 - yPokeLocs[i]) / (yT2 - yT1) * (xT2 - xT1);
      zQ1toQ2 = zT1 - (yT1 - yPokeLocs[i]) / (yT2 - yT1) * (zT2 - zT1);
      
      xQ2toQ3 = xT2 - (yT2 - yPokeLocs[i]) / (yT3 - yT2) * (xT3 - xT2);
      zQ2toQ3 = zT2 - (yT2 - yPokeLocs[i]) / (yT3 - yT2) * (zT3 - zT2);
      
      xQ3toQ4 = xT3 - (yT3 - yPokeLocs[i]) / (yE4 - yT3) * (xE4 - xT3);
      zQ3toQ4 = zT3 - (yT3 - yPokeLocs[i]) / (yE4 - yT3) * (zE4 - zT3);
      
      xQ4toQ1 = xE4 - (yE4 - yPokeLocs[i]) / (yT1 - yE4) * (xT1 - xE4);
      zQ4toQ1 = zE4 - (yE4 - yPokeLocs[i]) / (yT1 - yE4) * (zT1 - zE4);
      
      //extend or contract a horizontal line to the same x-plane as "xPokeLocs[i]"
      zPokeLoc1 = zQ1toQ2 - (xQ1toQ2 - xPokeLocs[i]) / (xQ3toQ4 - xQ1toQ2) * (zQ3toQ4 - zQ1toQ2);
      zPokeLoc2 = zQ2toQ3 - (xQ2toQ3 - xPokeLocs[i]) / (xQ4toQ1 - xQ2toQ3) * (zQ4toQ1 - zQ2toQ3);
      
      if (isnormal(zPokeLoc1))
      zPokeLocsRet[i] = zPokeLoc1;
      
      if (isnormal(zPokeLoc2))
      zPokeLocsRet[i] = zPokeLoc2;
	}
}


// rotate using degrees
void degRot(float *horiP, float *vertP, float degrees)
{
   float h, v;
   float hUc;
   float vUc;
   float hLine1, vLine1;
   float hLine2, vLine2;
   
   if (degrees != degrees) // check if NaN
   return;
   
   hUc = cos(degrees * (M_PI * 2.0 / 360.0));
   vUc = sin(degrees * (M_PI * 2.0 / 360.0));
   
   hLine1 = hUc;
   vLine1 = vUc;
   hLine2 = -vUc;
   vLine2 = hUc;

   h = *vertP * hLine2 + *horiP * vLine2;
   v = *horiP * vLine1 + *vertP * hLine1;
   *horiP = h;
   *vertP = v;
}


float maxValueInArray(float *valueArray, int valueArrayCnt)
{
   int i;
   float retValue;
   
   retValue = valueArray[0];
   
   for (i=1; i < valueArrayCnt; i++)
   {
      if (valueArray[i] > retValue)
      retValue = valueArray[i];
   }
   
   return retValue;
}


float minValueInArray(float *valueArray, int valueArrayCnt)
{
   int i;
   float retValue;
   
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
   int i;
   float xPoints[] = {-1.53, 10.41, -7.34, 4.55, -2.60, 2.80, 5.71, -4.74, -4.85, -0.42};
   float yPoints[] = {9.70, -7.52, -11.88, -8.25, -10.68, 4.27, -1.47, 1.92, -4.65, -3.56};
   float zPoints[] = {-1.86, 5.91, -2.18, 2.69, 0.83, 1.77, 2.53, -1.33, -1.92, -0.09};
   int pointCnt = sizeof(xPoints)/sizeof(float);
   float *xPokePts = xPoints;
   float *yPokePts = yPoints;
   float zPokePtsRet[sizeof(xPoints)/sizeof(float)];
   float xPointMax, xPointMin;
   float yPointMax, yPointMin;
   float zPointMax, zPointMin;
   float xCenter, yCenter, zCenter;
   
   float triangleSz;
   float xT1, yT1, zT1;
   float xT2, yT2, zT2;
   float xT3, yT3, zT3;
   float xT1Chg, yT1Chg, zT1Chg;
   float xT2Chg, yT2Chg, zT2Chg;
   float xT3Chg, yT3Chg, zT3Chg;
   
   float topMostValueArray[3];
   float bottomMostValueArray[3];
   
   int valueArrayCnt = 3;
   float zTriBaseValueArray[3];
   float zTriPossibValueArray[3];
   float valueArray[3];
   
   float lastValues[sizeof(xPoints)/sizeof(float)];
   float closestValues[sizeof(xPoints)/sizeof(float)];
   float tolerance = 0.001;
   int rubberBandValues = TRUE;
   int valueIsCloserArray[sizeof(xPoints)/sizeof(float)];
   int valueIsMostClosestArray[sizeof(xPoints)/sizeof(float)];
   float valueToTargetSum;
   
   FILE *ptDataFilePtr;
   FILE *lineDataFilePtr;
   
   
   //find the min and max x, y, and z values
   xPointMax = maxValueInArray(&xPoints[0], pointCnt);
   xPointMin = minValueInArray(&xPoints[0], pointCnt);
   yPointMax = maxValueInArray(&yPoints[0], pointCnt);
   yPointMin = minValueInArray(&yPoints[0], pointCnt);
   zPointMax = maxValueInArray(&zPoints[0], pointCnt);
   zPointMin = minValueInArray(&zPoints[0], pointCnt);
   
   //write point data to file
   ptDataFilePtr = fopen("point.dat", "wb");
   
   if (!ptDataFilePtr)
   return 0;
   
   for (i=0; i < pointCnt; i++)
   {
      fprintf(ptDataFilePtr, "%f %f %f\n\n", xPoints[i], yPoints[i], zPoints[i]);
   }
   
   fclose(ptDataFilePtr);
   
   
   lineDataFilePtr = fopen("line.dat", "wb");
   
   if (!lineDataFilePtr)
   return 0;
   
   
   //find the center of the point data
   xCenter = (xPointMax - xPointMin) / 2 + xPointMin;
   yCenter = (yPointMax - yPointMin) / 2 + yPointMin;
   zCenter = (zPointMax - zPointMin) / 2 + zPointMin;
   
   //create a triangle with 120 degree angles in the center of the points
   if (fabs(xPointMax-xCenter) > fabs(yPointMax-yCenter))
   triangleSz = fabs(xPointMax-xCenter);
   else
   triangleSz = fabs(yPointMax-yCenter);
   
   xT1 = 0.0;
   yT1 = triangleSz;
   zT1 = 0.0;
   
   xT2 = -0.866 * triangleSz;
   yT2 = -0.5 * triangleSz;
   zT2 = 0.0;
   
   xT3 = 0.866 * triangleSz;
   yT3 = -0.5 * triangleSz;
   zT3 = 0.0;
   
   
   //initialize the top most and bottom most values
   topMostValueArray[0] = 360;
   bottomMostValueArray[0] = 0;
   topMostValueArray[1] = 360;
   bottomMostValueArray[1] = 0;
   topMostValueArray[2] = zPointMax;
   bottomMostValueArray[2] = zPointMin;
   
   iniGuessValues(&topMostValueArray[0], &bottomMostValueArray[0], valueArrayCnt,
                  &zTriBaseValueArray[0], &zTriPossibValueArray[0], &valueArray[0]);
   
   iniCloserFurther(&lastValues[0], &closestValues[0], pointCnt);
   
   xT1Chg = xT1;
   yT1Chg = yT1;
   zT1Chg = zT1;
   xT2Chg = xT2;
   yT2Chg = yT2;
   zT2Chg = zT2;
   xT3Chg = xT3;
   yT3Chg = yT3;
   zT3Chg = zT3;
   
   
   //find a triangle that best approximates the point data slope
   for (i=1; i < 300; i++)
   {
      zPokesTrigon(xT1Chg, yT1Chg, zT1Chg,
                   xT2Chg, yT2Chg, zT2Chg,
                   xT3Chg, yT3Chg, zT3Chg,
                   pointCnt, &xPokePts[0], &yPokePts[0], &zPokePtsRet[0]);
      
      closerFurther(&zPokePtsRet[0], &zPoints[0], &lastValues[0], &closestValues[0],
                    pointCnt, tolerance, rubberBandValues,
                    &valueIsCloserArray[0], &valueIsMostClosestArray[0], &valueToTargetSum);
      
      printf("valueToTargetSum: %f\n", valueToTargetSum);
      
      guessValues(&topMostValueArray[0], &bottomMostValueArray[0],
                  valueArrayCnt, valueToTargetSum, i,
                  &zTriBaseValueArray[0], &zTriPossibValueArray[0], &valueArray[0]);
      
      xT1Chg = xT1;
      yT1Chg = yT1;
      zT1Chg = zT1;
      xT2Chg = xT2;
      yT2Chg = yT2;
      zT2Chg = zT2;
      xT3Chg = xT3;
      yT3Chg = yT3;
      zT3Chg = zT3;
      
      //change rotation of triangle
      degRot(&xT1Chg, &zT1Chg, valueArray[0]);
      degRot(&xT2Chg, &zT2Chg, valueArray[0]);
      degRot(&xT3Chg, &zT3Chg, valueArray[0]);
      
      degRot(&yT1Chg, &zT1Chg, valueArray[1]);
      degRot(&yT2Chg, &zT2Chg, valueArray[1]);
      degRot(&yT3Chg, &zT3Chg, valueArray[1]);
      
      //change location of triangle
      zT1Chg += valueArray[2];
      zT2Chg += valueArray[2];
      zT3Chg += valueArray[2];
      
      //write line data to file
      fprintf(lineDataFilePtr, "%f %f %f\n", xT1Chg, yT1Chg, zT1Chg);
      fprintf(lineDataFilePtr, "%f %f %f\n\n\n", xT2Chg, yT2Chg, zT2Chg);
      fprintf(lineDataFilePtr, "%f %f %f\n", xT2Chg, yT2Chg, zT2Chg);
      fprintf(lineDataFilePtr, "%f %f %f\n\n\n", xT3Chg, yT3Chg, zT3Chg);
      fprintf(lineDataFilePtr, "%f %f %f\n", xT3Chg, yT3Chg, zT3Chg);
      fprintf(lineDataFilePtr, "%f %f %f\n\n\n", xT1Chg, yT1Chg, zT1Chg);
   }
   
   fclose(lineDataFilePtr);
   
   return 0;
}
