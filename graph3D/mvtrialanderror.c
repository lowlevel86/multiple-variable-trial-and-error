//mvtrialanderror.c (multiple variable trial and error)
//this code contains 3 main functions: closerFurther, guessValues, and generateValues

//closerFurther() checks if values are closer to their targets 

//guessValues() guesses and finds values based on input

//generateValues() given an input number can generate multiple values within ranges without hitting the same value combination twice, it can also start anywhere with about the same amount of computation

#include <stdio.h>
#include <math.h>
#include "mvtrialanderror.h"
#define TRUE 1
#define FALSE 0
#define NEITHER -1


int64_t intPow(int64_t a, int64_t b)
{    
   return floor(pow(a, b) + 0.5);//round
}


void iniCloserFurther(double *lastValues, double *closestValues, int valueArrayCnt)
{
   int i;
   
   for (i=0; i < valueArrayCnt; i++)
   {
      lastValues[i] = INFINITY;
      closestValues[i] = INFINITY;
   }
}

//for finding if an array of values are closer to their target values
void closerFurther(double *valueArray, double *targetArray, double *lastValues, double *closestValues,
                   int valueArrayCnt, double tolerance,
                   int *valueIsCloserArray, int *valueIsMostClosestArray, double *valueToTargetSum)
{
   int i;
   
   *valueToTargetSum = 0.0;
   
   for (i=0; i < valueArrayCnt; i++)
   {
      *valueToTargetSum += fabs(targetArray[i] - valueArray[i]);
      
      if ((valueArray[i]+tolerance >= targetArray[i]) && (valueArray[i]-tolerance <= targetArray[i]))
      {
         valueIsCloserArray[i] = NEITHER;
         closestValues[i] = valueArray[i];
         valueIsMostClosestArray[i] = NEITHER;
      }
      else
      {
         if (fabs(targetArray[i]-valueArray[i]) < fabs(targetArray[i]-lastValues[i]))
         valueIsCloserArray[i] = TRUE;
         else
         valueIsCloserArray[i] = FALSE;
      
         if (fabs(targetArray[i]-valueArray[i]) < fabs(targetArray[i]-closestValues[i]))
         {
            closestValues[i] = valueArray[i];
            valueIsMostClosestArray[i] = TRUE;
         }
         else
         {
            valueIsMostClosestArray[i] = FALSE;
         }
      }
      
      lastValues[i] = valueArray[i];
   }
}

double keepMinCombToll;

void iniGuessValues(double *topMostValueArray, double *bottomMostValueArray, int valueArrayCnt,
                    double *baseValueArray, double *possibValueArray, double *valueArray)
{
   int i;
   double middleValue;
   
   //guess between the top and bottom most for the first set of values
   for (i=0; i < valueArrayCnt; i++)
   {
      middleValue = (topMostValueArray[i] - bottomMostValueArray[i]) / 2.0 + bottomMostValueArray[i];
      valueArray[i] = middleValue;
      baseValueArray[i] = middleValue;
      possibValueArray[i] = middleValue;
   }
   
   //initialize the minimum combination toll to infinity
   keepMinCombToll = INFINITY;
}

void guessValues(double *topMostValueArray, double *bottomMostValueArray,
                 int valueArrayCnt, double lastCombToll, uint32_t iteration,
                 double *baseValueArray, double *possibValueArray, double *valueArray)
{
   int64_t i;
   int64_t highLowCombCnt, highLowCombInc;
   int64_t combCnt, combInc;
   int64_t combIteration;
   int64_t guessHighLow;
   double valueFrac;
   
   //keep the set of values with the smallest toll
   if (lastCombToll < keepMinCombToll)
   {
      keepMinCombToll = lastCombToll;
      for (i=0; i < valueArrayCnt; i++)
      {
         possibValueArray[i] = valueArray[i];
      }
   }
   
   //timing variables
   highLowCombCnt = intPow(2, valueArrayCnt) - 1;
   highLowCombInc = (iteration-2) % highLowCombCnt;
   combCnt = highLowCombCnt * 2;
   combInc = (iteration-2) % combCnt;
   combIteration = (int64_t)((iteration-2) / combCnt);
   guessHighLow = (int64_t)(combInc / highLowCombCnt) % 2;
   
   //save the new base value
   if (highLowCombInc == 0)
   for (i=0; i < valueArrayCnt; i++)
   {
      baseValueArray[i] = possibValueArray[i];
   }
   
   //add or subtract away a fraction to the chosen values using an incrementing binary combination
   for (i=0; i < valueArrayCnt; i++)
   {
      //find a fraction to add to the value
      valueFrac = (topMostValueArray[i] - bottomMostValueArray[i]) / (double)(2<<combIteration);
      
      if (((highLowCombInc+1)>>i)&0b1)
      {
         if (guessHighLow)
         valueArray[i] = baseValueArray[i] + valueFrac;
         else
         valueArray[i] = baseValueArray[i] - valueFrac;
      }
      else
      {
         valueArray[i] = baseValueArray[i];
      }
   }
}

int64_t retMagnitudeAreaOffset(int dimensions, int layer, int64_t magnitudeAreaSz, int64_t inc)
{
   int64_t i;
   int64_t powOfTwoLayer;
   int64_t smEvenNum, smOddNum;
   int64_t skipCntFrac;
   int64_t offsetGain;
   int64_t variedInc;
   int64_t magnitudeNum;
   int64_t magnitudeFrac;
   int64_t magnitudeFracSum;
   int64_t oldMagnitudeNum;
   
   /*
   dimension=4 layer=3
   
   This is a "magnitude area".
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1 |
   |                 |                  magnitudeNum
   magnitudeNum      magnitudeNum
   
   To reproduce the above use this code:
   int i = 0, incOffset = 0, keepIncOffset = 0, incOffsetDiffSum = 0;
   for (i=0; i < 604; i++)
   {
      incOffset = retMagnitudeAreaOffset(4, 3, 604, i);
      if (incOffset - keepIncOffset == 2)
      {
         printf("%i\n", incOffsetDiffSum);
         incOffsetDiffSum = 0;
      }
      incOffsetDiffSum += 1;
      keepIncOffset = incOffset;
   }
   printf("%i\n", incOffsetDiffSum);

   magnitudeFrac:
   1+1+1+1+9+1+1+1+1+9+1+1+1+1+9+1+1+1+1+9+1+1+1+1+81 = 137
   1+1+1+1+9 = 13
   1 = 1
   
   skipCntFrac == number of commas == number of skipped numbers
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81, = 25
   1,1,1,1,9, = 5
   1, = 1
   */
   
   //initialize
   powOfTwoLayer = intPow(2, layer);
   smEvenNum = powOfTwoLayer / 2;
   smOddNum = smEvenNum + 1;
   
   skipCntFrac = intPow(smOddNum, dimensions-1) / smOddNum;
   
   offsetGain = 0;
   variedInc = inc;
   
   magnitudeNum = intPow(powOfTwoLayer+1, dimensions-2);
   magnitudeFrac = (magnitudeAreaSz + magnitudeNum) / smOddNum;
   
   //find the increment offset
   for (i=0; i < dimensions-1; i++)
   {
      //sum up "skipCnt" within "inc" using "turbulentFrac" ignoring the remainder
      offsetGain += (variedInc + magnitudeNum) / magnitudeFrac * skipCntFrac;
      
      //if "inc" is within a "magnitudeNum" then return "inc" + "skipCnt"
      if (variedInc % magnitudeFrac >= magnitudeFrac - magnitudeNum)
      return variedInc + offsetGain;
      
      skipCntFrac /= smOddNum;

      magnitudeFracSum = variedInc / magnitudeFrac * magnitudeFrac;
      offsetGain += magnitudeFracSum;
      variedInc -= magnitudeFracSum; //added back later
      
      oldMagnitudeNum = magnitudeNum;
      magnitudeNum /= powOfTwoLayer+1;
      magnitudeFrac = (magnitudeFrac - oldMagnitudeNum + magnitudeNum) / smOddNum;
   }
}

int64_t retTurbulentAreaOffset(int dimensions, int layer, int64_t turbulentAreaSz, int64_t turbulentEndNum, int64_t inc)
{
   int64_t i;
   int64_t powOfTwoLayer;
   int64_t smEvenNum;
   int64_t skipCntFrac;
   int64_t offsetGain;
   int64_t variedInc;
   int64_t turbulentNum;
   int64_t turbulentFrac;
   int64_t turbulentFracSum;
   int64_t oldTurbulentNum;
   int64_t magnitudeNum;
   int64_t oldMagnitudeNum;
   
   /*
   turbulentEndNum                      turbulentNum
   |                                    |
   91,1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,91,
      |              |                 |
      turbulentNum   turbulentNum      turbulentEndNum
   
   To reproduce the above use this code:
   int i = 0, incOffset = 0, keepIncOffset = 0, incOffsetDiffSum = 0;
   for (i=0; i < 665; i++)
   {
      incOffset = retTurbulentAreaOffset(4, 3, 665, 91, i);
      if (incOffset - keepIncOffset == 2)
      {
         printf("%i\n", incOffsetDiffSum);
         incOffsetDiffSum = 0;
      }
      incOffsetDiffSum += 1;
      keepIncOffset = incOffset;
   }
   printf("%i\n", incOffsetDiffSum);
   
   turbulentFrac:
   1+1+1+11+1+1+1+11+1+1+1+11+1+1+1+101 = 146
   1+1+1+11 = 14
   1 = 1
   
   skipCntFrac == number of commas == number of skipped numbers
   1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101, = 16
   1,1,1,11, = 4
   1, = 1
   */
   
   //initialize
   powOfTwoLayer = intPow(2, layer);
   smEvenNum = powOfTwoLayer / 2;
   
   skipCntFrac = intPow(smEvenNum, dimensions-1) / smEvenNum;
   
   offsetGain = 0;
   
   if (inc >= turbulentEndNum)
      variedInc = inc - turbulentEndNum;
   else
      return inc;
   
   magnitudeNum = intPow(powOfTwoLayer+1, dimensions-2);
   turbulentNum = turbulentEndNum + (turbulentEndNum - magnitudeNum);
   turbulentFrac = (turbulentAreaSz - turbulentEndNum*2 + turbulentNum) / smEvenNum;
   
   //find the increment offset
   for (i=0; i < dimensions-1; i++)
   {
      //sum up "skipCnt" within "inc" using "turbulentFrac" ignoring the remainder
      offsetGain += (variedInc + turbulentNum) / turbulentFrac * skipCntFrac;
      
      //if "inc" is within a "turbulentNum" then return "inc" + "skipCnt"
      if (variedInc % turbulentFrac >= turbulentFrac - turbulentNum)
      return variedInc + offsetGain + turbulentEndNum;
      
      skipCntFrac /= smEvenNum;

      turbulentFracSum = variedInc / turbulentFrac * turbulentFrac;
      offsetGain += turbulentFracSum;
      variedInc -= turbulentFracSum; //added back later
      
      oldMagnitudeNum = magnitudeNum;
      magnitudeNum /= powOfTwoLayer+1;
      oldTurbulentNum = turbulentNum;
      turbulentNum -= oldMagnitudeNum + magnitudeNum;
      turbulentFrac = (turbulentFrac - oldTurbulentNum + turbulentNum) / smEvenNum;
   }
}

void retAreaSzsAndOffset(int dimensions, int layer,
                         int64_t *magnitudeAreaSz, int64_t *turbulentAreaSz,
                         int64_t *turbulentEndNum, int64_t *areaSzOffset)
{
   int64_t i;
   int64_t powOfTwoLayer;
   int64_t smEvenNum, smOddNum;
   int64_t powOfSmEvenNum;
   int64_t powOfSmOddNum;
   
   int64_t magnitudeNum;
   int64_t magnitudeNumSum;
   int64_t magnitudeSingleNumSum;
   int64_t magnitudeMultiplyOperand;

   int64_t turbulentNum;
   int64_t turbulentNumSum;
   int64_t turbulentSingleNumSum;
   int64_t turbulentMultiplyOperand;
   
   int64_t turbulentAreaSkipCnt;
   
   /*
   formula for finding the number of skips in the magnitude area
   D3_L2 = 3*3
   D3_L3 = 5*5
   D3_L4 = 9*9
   
   D4_L2 = 3*3*3
   D4_L3 = 5*5*5
   D4_L4 = 9*9*9
   
   D5_L2 = 3*3*3*3
   D5_L3 = 5*5*5*5
   D5_L4 = 9*9*9*9

   skipCnt = pow(pow(2, layer-1)+1, dimensions-1)
   
   
   formula for finding the sum of the singles and clusters in the magnitude area
       singles + clusters
   D3_L2 = 2*3 + ( 5*2 )
   D3_L3 = 4*5 + ( 9*4 )
   D3_L4 = 8*9 + ( 17*8 )
   
   D4_L2 = 2*3*3 + ( 5*2*3 + 25*2 )
   D4_L3 = 4*5*5 + ( 9*4*5 + 81*4 )
   D4_L4 = 8*9*9 + ( 17*8*9 + 289*8 )
   
   D5_L2 = 2*3*3*3 + ( 5*2*3*3 + 25*2*3 + 125*2 )
   D5_L3 = 4*5*5*5 + ( 9*4*5*5 + 81*4*5 + 729*4 )
   D5_L4 = 8*9*9*9 + ( 17*8*9*9 + 289*8*9 +4913*8 )
   
   if dimension == 5 and layer == 4
   magnitudeMultiplyOperand == 8*9*9*9, 8*9*9, 8*9, 8
   
   
   dimension=3 layer=3
   This is a "magnitude area".
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1
   |       |
   single  cluster
   
   dimension=4 layer=3
   This is a "magnitude area".
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,81,
   1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1,9,1,1,1,1 |
   |                 |                  magnitudeNum
   magnitudeNum      magnitudeNum
   */
   
   /*
   formula for finding the number of skips in the turbulent area
   D3_L2 = 2*2
   D3_L3 = 4*4
   D3_L4 = 8*8

   D4_L2 = 2*2*2
   D4_L3 = 4*4*4
   D4_L4 = 8*8*8
   
   D5_L2 = 2*2*2*2
   D5_L3 = 4*4*4*4
   D5_L4 = 8*8*8*8
   
   skipCnt = pow(pow(2, layer-1), dimensions-1)
   
   
   formula for finding the sum of the singles and clusters in the turbulent area
       singles + clusters + endNum*2
   D3_L2 = 1*2 + (  7*1 ) +  6*2
   D3_L3 = 3*4 + ( 11*3 ) + 10*2
   D3_L4 = 7*8 + ( 19*7 ) + 18*2
   
   D4_L2 = 1*2*2 + ( 7*1*2 + 37*1 ) + 31*2
   D4_L3 = 3*4*4 + ( 11*3*4 + 101*3 ) + 91*2
   D4_L4 = 7*8*8 + ( 19*7*8 + 325*7 ) + 307*2
   
   D5_L2 = 1*2*2*2 + ( 7*1*2*2 + 37*1*2 + 187*1 ) + 156*2
   D5_L3 = 3*4*4*4 + ( 11*3*4*4 + 101*3*4 + 911*3 ) + 820*2
   D5_L4 = 7*8*8*8 + ( 19*7*8*8 + 325*7*8 + 5527*7 ) + 5220*2
   
   if dimension == 5 and layer == 4
   turbulentMultiplyOperand == 7*8*8*8, 7*8*8, 7*8, 7
   
   
   dimension=3 layer=3
   This is a "turbulent area".
   10,1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,10,
   |      |          |
   endNum single     cluster
   
   dimension=4 layer=3
   This is a "turbulent area".
   
   turbulentEndNum                      turbulentNum
   |                                    |
   91,1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,101,
      1,1,1,11,1,1,1,11,1,1,1,11,1,1,1,91,
      |              |                 |
      turbulentNum   turbulentNum      turbulentEndNum
   */
   
   //initialize
   powOfTwoLayer = intPow(2, layer);
   smEvenNum = powOfTwoLayer / 2;
   smOddNum = smEvenNum + 1;
   powOfSmEvenNum = intPow(smEvenNum, dimensions-2);
   powOfSmOddNum = intPow(smOddNum, dimensions-2);
   
   magnitudeNum = powOfTwoLayer+1;
   magnitudeNumSum = 0;
   magnitudeSingleNumSum = powOfSmOddNum * smEvenNum;
   magnitudeMultiplyOperand = magnitudeSingleNumSum;
   
   turbulentNum = powOfTwoLayer+3;
   turbulentNumSum = 0;
   turbulentSingleNumSum = powOfSmEvenNum * (smEvenNum-1);
   turbulentMultiplyOperand = turbulentSingleNumSum;
   *turbulentEndNum = 1;
   
   for (i=0; i < dimensions-2; i++)
   {
      *turbulentEndNum += magnitudeNum;
      turbulentNum = *turbulentEndNum + (*turbulentEndNum - magnitudeNum);
      turbulentMultiplyOperand /= smEvenNum;
      turbulentNumSum += turbulentNum * turbulentMultiplyOperand;
      magnitudeMultiplyOperand /= smOddNum;
      magnitudeNumSum += magnitudeNum * magnitudeMultiplyOperand;
      magnitudeNum *= powOfTwoLayer + 1;
   }
   
   *magnitudeAreaSz = magnitudeSingleNumSum + magnitudeNumSum;
   *turbulentAreaSz = turbulentSingleNumSum + turbulentNumSum;

   if (dimensions > 1)
   *turbulentAreaSz += *turbulentEndNum * 2;
   
   turbulentAreaSkipCnt = powOfSmEvenNum * smEvenNum;
   *areaSzOffset = *turbulentAreaSz + turbulentAreaSkipCnt;
}

int64_t retOddEvenGroupSkipInc(int dimensions, int layer, int64_t inc)
{
   int64_t magnitudeAreaSz, turbulentAreaSz;
   int64_t turbulentEndNum;
   int64_t areaSzOffset;
   int64_t incRemainder;
   int64_t incOffset;
   
   retAreaSzsAndOffset(dimensions, layer,
                       &magnitudeAreaSz, &turbulentAreaSz, &turbulentEndNum, &areaSzOffset);
   
   incRemainder = inc % (magnitudeAreaSz + turbulentAreaSz);

   if (incRemainder < magnitudeAreaSz)
   {
      incOffset = retMagnitudeAreaOffset(dimensions, layer, magnitudeAreaSz, incRemainder);
   }
   else
   {
      incOffset = retTurbulentAreaOffset(dimensions, layer, turbulentAreaSz, turbulentEndNum, incRemainder-magnitudeAreaSz);
      incOffset += areaSzOffset;
   }
   
   incOffset += inc / (magnitudeAreaSz + turbulentAreaSz) * (areaSzOffset + areaSzOffset);
   
   return incOffset;
}

int64_t retBaseNthDigit(int64_t base, int64_t num, int64_t digitLoc)
{
   int64_t digit;
   int64_t upperDigits;
   double placeValue1;
   double placeValue2;
   
   placeValue1 = intPow(base, digitLoc);
   placeValue2 = placeValue1 * base;
   
   //use integer division to remove smaller digits
   upperDigits = num / (int64_t)placeValue1;
   
   //subtract away the larger digits
   digit = upperDigits - num / (int64_t)placeValue2 * base;
   
   return digit;
}

void retSingleGroupComb(int dimensions, int layer, int64_t inc, double *valueArray)
{
   int64_t i;
   int64_t digit;
   int64_t groupSize;
   
   groupSize = intPow(2, layer-1);
   
   //find all combinations within the group
   for (i=0; i < dimensions; i++)
   {
      digit = retBaseNthDigit(groupSize, inc, i);
      
      //convert the combination number into a fraction
      valueArray[i] = 2.0 / groupSize * digit - (1.0 - 1.0 / groupSize);
   }
}

void retOddEvenGroupComb(int dimensions, int layer, int64_t inc, double *valueArray)
{
   int64_t i;
   int64_t digit;
   int64_t skipInc;
   int64_t oddSideGroupSize;
   
   oddSideGroupSize = intPow(2, layer)+1;
   
   //find all combinations for both the odd group and the even group
   
   //skip over numbers that were used before
   skipInc = retOddEvenGroupSkipInc(dimensions, layer, inc);
   
   for (i=0; i < dimensions; i++)
   {
      digit = retBaseNthDigit(oddSideGroupSize, skipInc, i);
      
      //convert the combination number into a fraction
      valueArray[i] = 2.0 / (oddSideGroupSize-1) * digit - 1.0;
   }
}

int64_t sumCurrentAndPriorLayers(int dimensions, int layer)
{
   return intPow(intPow(2, layer)+1, dimensions);
}

void generateValues(int valueArrayCnt, uint32_t inc, double *valueArray)
{
   int64_t i;
   int64_t firstCombCnt;
   int layer;
   int64_t singleGroupCombCnt;
   int64_t layerValuesInc;
   int64_t pastLayersSum;
   
   
   //find the top and bottom most value combination
   firstCombCnt = intPow(2, valueArrayCnt);
   
   if (inc < firstCombCnt)
   {
      for (i=0; i < valueArrayCnt; i++)
      {
         valueArray[i] = retBaseNthDigit(2, inc, i) * 2 - 1;
      }
      
      return;
   }
   
   
   //find the layer
   layer = log(pow(inc, 1.0/valueArrayCnt)-1) / log(2) + 1;
   
   //correct layer for rounding errors
   if (intPow(intPow(2, layer)+1, valueArrayCnt) == inc)
   layer++;
   
   pastLayersSum = intPow(intPow(2, layer-1)+1, valueArrayCnt);
   
   layerValuesInc = inc - pastLayersSum;
   singleGroupCombCnt = intPow(firstCombCnt, layer-1);
   
   
   if (layerValuesInc < singleGroupCombCnt)
   retSingleGroupComb(valueArrayCnt, layer, layerValuesInc, &valueArray[0]);
   else
   retOddEvenGroupComb(valueArrayCnt, layer, layerValuesInc-singleGroupCombCnt, &valueArray[0]);
}

void generateRangedValues(double *topMostValueArray, double *bottomMostValueArray,
                          int valueArrayCnt, uint32_t inc, double *valueArray)
{
   int64_t i;
   
   generateValues(valueArrayCnt, inc, &valueArray[0]);
   
   //change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
   for (i=0; i < valueArrayCnt; i++)
   {
      //newValue = (value + 1) / 2 * size + bottomMostValue
      valueArray[i] = (valueArray[i] + 1.0) / 2.0 *
                      (topMostValueArray[i] - bottomMostValueArray[i]) + bottomMostValueArray[i];
   }
}
