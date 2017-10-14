//mvtrialanderror.c (multiple variable trial and error)
//this code contains 3 main functions: closerFurther, guessValues, and generateValues

//closerFurther() checks if values are closer to their targets 

//guessValues() guesses and finds values based on input

//generateValues() can generate multiple values within a range without hitting the same value combination twice, it can also start where it left off without having to start from the first iteration

#include <stdio.h>
#include <math.h>
#define TRUE 1
#define FALSE 0
#define NEITHER -1

void iniCloserFurther(float *lastValues, float *closestValues, int valueArrayCnt)
{
   int i;
   
   for (i=0; i < valueArrayCnt; i++)
   {
      lastValues[i] = INFINITY;
      closestValues[i] = INFINITY;
   }
}

//this function is a swiss army knife for finding if an array of values are closer to their target values
void closerFurther(float *valueArray, float *targetArray, float *lastValues, float *closestValues,
                   int valueArrayCnt, float tolerance, int rubberBandValues,
                   int *valueIsCloserArray, int *valueIsMostClosestArray, float *valueToTargetSum)
{
   int i;
   
	*valueToTargetSum = 0.0;
   
   for (i=0; i < valueArrayCnt; i++)
   {
      //rubberBandValues = the further a value is the more weight it has
		if (rubberBandValues)
      *valueToTargetSum += pow(fabs(targetArray[i] - valueArray[i]), 2);
		else
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

float keepMinCombToll;

void iniGuessValues(float *topMostValueArray, float *bottomMostValueArray, int valueArrayCnt,
                    float *baseValueArray, float *possibValueArray, float *valueArray)
{
   int i;
   float middleValue;
   
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

void guessValues(float *topMostValueArray, float *bottomMostValueArray,
                 int valueArrayCnt, float lastCombToll, int iteration,
                 float *baseValueArray, float *possibValueArray, float *valueArray)
{
   int i;
   int highLowCombCnt, highLowCombInc;
	int combCnt, combInc;
	int combIteration;
	int guessHighLow;
   float valueFrac;
   
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
	highLowCombCnt = (1 << valueArrayCnt) - 1;
	highLowCombInc = (iteration-2) % highLowCombCnt;
	combCnt = highLowCombCnt * 2;
	combInc = (iteration-2) % combCnt;
	combIteration = (int)((iteration-2) / combCnt);
	guessHighLow = (int)(combInc / highLowCombCnt) % 2;
	
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
      valueFrac = (topMostValueArray[i] - bottomMostValueArray[i]) / (float)(2<<combIteration);
      
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

int retBaseNthDigit(int base, int num, int digitLoc)
{
   int digit;
   int upperDigits;
   float placeValue1;
   float placeValue2;
   
   placeValue1 = pow(base, digitLoc);
   placeValue2 = placeValue1 * base;
   
   //use integer division to remove smaller digits
   upperDigits = num / (int)placeValue1;
   
   //subtract away the larger digits
   digit = upperDigits - num / (int)placeValue2 * base;
   
   return digit;
}

void retSingleGroupComb(int evenGroupSize, int combInc, int valueArrayCnt, float *valueArray)
{
   int i, digit;
   
   //find all combinations for all numbers within a group
   for (i=0; i < valueArrayCnt; i++)
   {
      digit = retBaseNthDigit(evenGroupSize, combInc, i);
      
      //convert the combination number into a fraction
      valueArray[i] = 2.0 / evenGroupSize * digit - (1.0 - 1.0 / evenGroupSize);
   }
}

void retOddEvenGroupComb(int evenGroupSize, int inc, int valueArrayCnt, float *valueArray)
{
   int i, digit;
   int combInc;
   int evenGroupNum;
   int oddGroupNum;
   float fracNums[2];
   
   //make room for all combinations except numbers with same digits
   combInc = inc / ((1 << valueArrayCnt) - 2);
   
   //find the combination numbers between two groups
	oddGroupNum = combInc / evenGroupSize;
	evenGroupNum = combInc % evenGroupSize;
   
   //convert the odd and even combination numbers into fractions
   fracNums[0] = 2.0 / evenGroupSize * oddGroupNum - 1.0;
   fracNums[1] = 2.0 / evenGroupSize * evenGroupNum - (1.0 - 1.0 / evenGroupSize);
   
   //skip over numbers with same digits
   inc = inc % ((1 << valueArrayCnt) - 2);
   inc++;
   
   //find all possibilities both fraction numbers can be positioned 
   for (i=0; i < valueArrayCnt; i++)
   {
      digit = retBaseNthDigit(2, inc, i);
      valueArray[i] = fracNums[digit];
   }
}

void generateValuesA(int valueArrayCnt, float *valueArray, int iteration)
{
   int binPossib;
   float mersenneNum;
   float dimensionMultiplier;
   float expNum, triangleNumA, triangleNumB;
   
   int layer, layerCombCntSum;
   float singGroupCombCntSum;
   float doubGroupCombCntSum;
   float singGroupCombCnt;
   float doubGroupCombCnt;
   
   float smSingGroupCombCnt;
   float smDoubGroupCombCnt;
   float lgSingGroupCombCnt;
   float lgDoubGroupCombCnt;
   float exLgSingGroupCombCnt;
   float exLgDoubGroupCombCnt;
   int singGroupSize;
   
   //number of binary possibilities in "valueArrayCnt"
   //3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111
   binPossib = pow(2, valueArrayCnt);
   
   //variables to help calculate layer sums and sizes relative to "valueArrayCnt"
   mersenneNum = pow(2, valueArrayCnt-1) - 1;
   dimensionMultiplier = mersenneNum / 3;
   
   //find the approximate layer the iteration is in
   //    [] layer 1
   //  [][][] layer 2
   //[][][][][] layer 3...
   layer = (int)log(iteration * 1.0 / binPossib) / log(binPossib) + 2;
   
   //find the sum of all possible combinations within the past layers using the approximate layer
   singGroupCombCntSum = (pow(binPossib, layer-1) - binPossib) / (binPossib-1) + 1;
   
   expNum = pow(2, layer-1);
   triangleNumA = (expNum * (expNum - 1.0)) / 2.0;
   triangleNumB = ((expNum+1) * ((expNum+1) - 1.0)) / 2.0;
   
   doubGroupCombCntSum = triangleNumA * triangleNumB /
                         (triangleNumB / (4 + 16.0 / expNum)) * dimensionMultiplier;
   
   layerCombCntSum = singGroupCombCntSum + doubGroupCombCntSum;
   
   
   //find the exact layer the iteration is in
   smSingGroupCombCnt = pow(binPossib, layer-2);
   smDoubGroupCombCnt = (pow(2, layer-2) * (pow(2, layer-2) + 1.0)) / 2.0 * 4 * mersenneNum;
   
   lgSingGroupCombCnt = pow(binPossib, layer-1);
   lgDoubGroupCombCnt = (pow(2, layer-1) * (pow(2, layer-1) + 1.0)) / 2.0 * 4 * mersenneNum;
   
   exLgSingGroupCombCnt = pow(binPossib, layer);
   exLgDoubGroupCombCnt = (pow(2, layer) * (pow(2, layer) + 1.0)) / 2.0 * 4 * mersenneNum;
   
   if (iteration > layerCombCntSum - (smSingGroupCombCnt + smDoubGroupCombCnt))
   if (iteration <= layerCombCntSum)
   layer -= 1;
   
   if (iteration > layerCombCntSum + (lgSingGroupCombCnt + lgDoubGroupCombCnt))
   if (iteration <= layerCombCntSum + (lgSingGroupCombCnt + lgDoubGroupCombCnt) + (exLgSingGroupCombCnt + exLgDoubGroupCombCnt))
   layer += 1;
   
   if (iteration > layerCombCntSum + (lgSingGroupCombCnt + lgDoubGroupCombCnt) + (exLgSingGroupCombCnt + exLgDoubGroupCombCnt))
   layer += 2;
   
   
   //find the sum of all possible combinations within the past layers
   singGroupCombCntSum = (pow(binPossib, layer-1) - binPossib) / (binPossib-1) + 1;
   
   expNum = pow(2, layer-1);
   triangleNumA = (expNum * (expNum - 1.0)) / 2.0;
   triangleNumB = ((expNum+1) * ((expNum+1) - 1.0)) / 2.0;
   
   doubGroupCombCntSum = triangleNumA * triangleNumB /
                         (triangleNumB / (4 + 16.0 / expNum)) * dimensionMultiplier;
   
   layerCombCntSum = singGroupCombCntSum + doubGroupCombCntSum;
   
   
   //find which group the iteration is in 
   singGroupCombCnt = pow(binPossib, layer-1);
   doubGroupCombCnt = (pow(2, layer-1) * (pow(2, layer-1) + 1.0)) / 2.0 * 4 * mersenneNum;
   
   singGroupSize = pow(2, layer-1);
   
   if (iteration <= layerCombCntSum + singGroupCombCnt)
   retSingleGroupComb(singGroupSize, iteration-1 - layerCombCntSum, valueArrayCnt, &valueArray[0]);
   else
   retOddEvenGroupComb(singGroupSize, iteration-1 - layerCombCntSum - singGroupCombCnt, valueArrayCnt, &valueArray[0]);
}

void generateValuesB(int valueArrayCnt, float *valueArray, int iteration)
{
   int i;
   int firstNumCombCnt;
   
   //add the top and bottom most value combination to the beginning
   firstNumCombCnt = 1 << valueArrayCnt;
   
   if (iteration <= firstNumCombCnt)
   {
      for (i=0; i < valueArrayCnt; i++)
      {
         valueArray[i] = retBaseNthDigit(2, iteration-1, i) * 2 - 1;
      }
      
      return;
   }
   
   iteration -= firstNumCombCnt;
   
   generateValuesA(valueArrayCnt, &valueArray[0], iteration);
}

void generateValuesC(float *topMostValueArray, float *bottomMostValueArray,
                     int valueArrayCnt, float *valueArray, int iteration)
{
   int i;
   
   generateValuesB(valueArrayCnt, &valueArray[0], iteration);
   
   //change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
   for (i=0; i < valueArrayCnt; i++)
   {
      //newValue = (value + 1) / 2 * size + bottomMostValue
      valueArray[i] = (valueArray[i] + 1.0) / 2.0 *
                      (topMostValueArray[i] - bottomMostValueArray[i]) + bottomMostValueArray[i];
   }
}
