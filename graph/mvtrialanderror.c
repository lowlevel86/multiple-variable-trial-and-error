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

//return non-redundant combination pair from a single group
void retSingleGroupPairCombination(int groupSize, int combInc, int *selectedPair)
{
   int combCnt;
   int triangularRoot;
   int resetCombInc;
   
	//get the number of pair combinations the group can have
	combCnt = ((groupSize-1) * ((groupSize-1) + 1.0)) / 2.0; //get triangular number
   
	//get the triangular root of "combInc (reversed)" to help find the combination order
	triangularRoot = (int)((sqrt(8 * ((combCnt-1)-combInc) + 1.0) - 1.0) / 2.0);
	
	
	//find the first number of the combination pair
	selectedPair[0] = (int)((groupSize-1) - (triangularRoot+1));
	
	//use "resetCombInc" to subtract from combInc and put the combination in a sequential order
	resetCombInc = (selectedPair[0] * (selectedPair[0] + 1.0)) / 2.0;
	
	//find the second number of the combination pair
	selectedPair[1] = (int)((combInc - resetCombInc) % (triangularRoot+1) + selectedPair[0]+1);
}

//return non-redundant combination pair from 2 different groups and not from the same group
void retDualGroupPairCombination(int groupSize, int combInc, int *selectedPair)
{
	//find the combination pair
	selectedPair[0] = (int)(combInc / (groupSize+1));
	selectedPair[1] = (int)(combInc % (groupSize+1));
}

void retPyramidPairValueCombination(int iteration, float *selectedPairValue)
{
   float triRoot;
   int rev2Pow;
   int revRev2PowInt;
   float triNum, triNumLg, triNumSm;
   int pyramidLayerInc;
   int layerCombInc;
   int layerCombCnt;
   int layerCellCnt;
   float layerCellCntHalf;
   float layerCellCntFrac;
   int singleGroupCombSize;
   int dualGroupCombSize;
   int dualGroupCombInc;
   int singleGroupCombInc;
   int selectedPair[2];
   
	//get the triangular root of "iteration"
	triRoot = (sqrt(8 * iteration + 1.0) - 1.0) / 2.0;
	
	//used for finding the number of layers descended
	rev2Pow = (int)((float)log(triRoot) / (float)log(2));
	
	//used for finding the layer combination increment and count
	revRev2PowInt = 1 << rev2Pow;
	triNum = (revRev2PowInt * (revRev2PowInt + 1.0)) / 2.0;
	triNumLg = ((revRev2PowInt<<1) * ((revRev2PowInt<<1) + 1.0)) / 2.0;
	triNumSm = ((revRev2PowInt>>1) * ((revRev2PowInt>>1) + 1.0)) / 2.0;
	
	//find the number of layers descended
	//find the number of layer combinations horizontally progressed
	//find the number of layer combinations
	if (triNum < iteration)
   {
		pyramidLayerInc = rev2Pow+1;
		layerCombInc = iteration - triNum - 1;
		layerCombCnt = triNumLg - triNum;
   }
	else
   {
		pyramidLayerInc = rev2Pow;
		layerCombInc = iteration - triNumSm - 1;
		layerCombCnt = triNum - triNumSm;
	}
   
	//find the number of cells within a layer
	layerCellCnt = 1 << pyramidLayerInc;
	
   layerCellCntHalf = layerCellCnt / 2.0;
   layerCellCntFrac = 1.0 / layerCellCntHalf;
   
	//find the single group pair combination size for the pyramid layer
	singleGroupCombSize = ((layerCellCntHalf-1) * ((layerCellCntHalf-1) + 1.0)) / 2.0;
	
	//find the dual group pair combination size for the pyramid layer
	dualGroupCombSize = layerCombCnt - singleGroupCombSize;
	
   //find the single or dual group pair combination increment and selected pair values
	if (layerCombInc < dualGroupCombSize)
   {
		dualGroupCombInc = layerCombInc;
      retDualGroupPairCombination((int)layerCellCntHalf, dualGroupCombInc, &selectedPair[0]);
		selectedPairValue[0] = layerCellCntFrac * 2 * selectedPair[0] - (1.0 - layerCellCntFrac);
		selectedPairValue[1] = (float)(selectedPair[1]) * layerCellCntFrac * 2 - 1.0;
	}
   else
   {
		singleGroupCombInc = layerCombInc - dualGroupCombSize;
		retSingleGroupPairCombination((int)layerCellCntHalf, singleGroupCombInc, &selectedPair[0]);
		selectedPairValue[0] = layerCellCntFrac * 2 * selectedPair[0] - (1.0 - layerCellCntFrac);
		selectedPairValue[1] = layerCellCntFrac * 2 * selectedPair[1] - (1.0 - layerCellCntFrac);
	}
}

void retPyramidValue(int iteration, float *value)
{
   int rev2Pow;
   int layerCellCnt;
   
   rev2Pow = (int)((float)log(iteration) / (float)log(2));

	//find the number of cells within a layer
	layerCellCnt = 1 << rev2Pow;
	
	//return a value between -1 and 1 starting with 0 at the top
	//of the pyramid and becoming closer to 1 and -1 moving down
	//example output: 0, -0.5, 0.5, -0.75, -0.25, 0.25, 0.75
	*value = 1.0 / layerCellCnt * 2 * (iteration-layerCellCnt) - (1.0 - 1.0 / layerCellCnt);
}

void generateAlikeValues(float *topMostValueArray, float *bottomMostValueArray,
                         int valueArrayCnt, int iteration, float *valueArray)
{
   int i;
   float value, valueMod, sz;
   
   retPyramidValue(iteration, &value);
   
   for (i=0; i < valueArrayCnt; i++)
   {
      //change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
      sz = topMostValueArray[i] - bottomMostValueArray[i];
      valueMod = (value + 1.0) / 2.0 * sz + bottomMostValueArray[i];
      
      valueArray[i] = valueMod; 
   }
}

void generateBinCntValues(float *topMostValueArray, float *bottomMostValueArray,
                          int valueArrayCnt, int iteration, float *valueArray)
{
   int i;
   int binPossib;
   float pairValue[2], pairValueMod[2];
   float sz;
   int binNum;
	
	//number of binary possibilities in the "valueArrayCnt" number
	//3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
	binPossib = (1 << valueArrayCnt) - 2;
   
   retPyramidPairValueCombination((int)((iteration-1) / binPossib) + 1, &pairValue[0]);
	
	//find the binary possibilities
	binNum = (iteration-1) % binPossib + 1;
   for (i=0; i < valueArrayCnt; i++)
   {
      //change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
      sz = topMostValueArray[i] - bottomMostValueArray[i];
      pairValueMod[0] = (pairValue[0] + 1.0) / 2.0 * sz + bottomMostValueArray[i];
      pairValueMod[1] = (pairValue[1] + 1.0) / 2.0 * sz + bottomMostValueArray[i];
      
		valueArray[i] = pairValueMod[(binNum>>i)&0b1];
	}
}

void accountBinPossibilities(int valueArrayCnt, int iteration, int *binCombInc, int *alikeCombInc)
{
   int binPossib;
   int slowIteration;
   float triRoot;
   int rev2Pow;
   int revRev2PowInt;
   float triNum, triNumLg, triNumSm;
   float triNumBin, triNumLgBin, triNumSmBin;
   int pyramidLayerInc2;
   int topToLastLayerCombCnt2;
   int topToThisLayerCombCnt2;
   int layerCombInc2, layerCombCnt2;
   int layerCellCnt;
   float layerCellCntHalf;
	int topToLastLayerAlikeCnt;
   int alikeLayerInc;
   
	//number of binary possibilities in the "valueArrayCnt" number
	//3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
	binPossib = (1 << valueArrayCnt) - 2;
   
	//slow the iteration to account for the binary possibilities
	slowIteration = (int)((iteration-1) / binPossib) + 1;
	
	//get the triangular root of "slowIteration"
	triRoot = (sqrt(8 * slowIteration + 1.0) - 1.0) / 2.0;
	
	//used for finding the number of layers descended
	rev2Pow = (int)((float)log(triRoot) / (float)log(2));
	
   
	//used for finding the layer combination increment and count
	revRev2PowInt = 1 << rev2Pow;
	triNum = (revRev2PowInt * (revRev2PowInt + 1.0)) / 2.0;
	triNumLg = ((revRev2PowInt<<1) * ((revRev2PowInt<<1) + 1.0)) / 2.0;
	triNumSm = ((revRev2PowInt>>1) * ((revRev2PowInt>>1) + 1.0)) / 2.0;
	
	//account for binary possibilities
	triNumBin = triNum * binPossib;
	triNumLgBin = triNumLg * binPossib;
	triNumSmBin = triNumSm * binPossib;
	
   
	//find the number of layers descended
	//find the number of layer combinations horizontally progressed
	//find the number of layer combinations
	if ((triNumBin + (revRev2PowInt-1)) < iteration)
   {
		pyramidLayerInc2 = rev2Pow+1;
		topToLastLayerCombCnt2 = (triNumBin + (revRev2PowInt-1));
		topToThisLayerCombCnt2 = (triNumLgBin + (revRev2PowInt*2-1));
   }
	else
   {
		pyramidLayerInc2 = rev2Pow;
		topToLastLayerCombCnt2 = (triNumSmBin + ((int)(revRev2PowInt/2)-1));
		topToThisLayerCombCnt2 = (triNumBin + (revRev2PowInt-1));
	}
   
	layerCombInc2 = iteration - topToLastLayerCombCnt2 - 1;
	layerCombCnt2 = topToThisLayerCombCnt2 - topToLastLayerCombCnt2;
	
   //find the number of cells within the layer
	layerCellCnt = 1 << pyramidLayerInc2;
   layerCellCntHalf = layerCellCnt / 2.0;
   
	//find the top to last layer combination count for the alike values
	topToLastLayerAlikeCnt = layerCellCntHalf - 1;
	
	
	//find the iteration number for either the "alike" values or the "binary count" values
	if (layerCombInc2 < layerCombCnt2 - layerCellCntHalf)
   {
		*binCombInc = (int)(iteration - topToLastLayerAlikeCnt);
		*alikeCombInc = 0;
   }
	else
   {
		alikeLayerInc = layerCellCntHalf - (layerCombCnt2 - layerCombInc2);
		*alikeCombInc = topToLastLayerAlikeCnt + alikeLayerInc + 1;
		*binCombInc = 0;
	}
}

void generateValues(float *topMostValueArray, float *bottomMostValueArray,
                    int valueArrayCnt, int iteration, float *valueArray)
{
   int i;
   int binPossib;
   float pairValue[2], pairValueMod[2];
   float sz;
   int binNum;
   int binCombInc, alikeCombInc;
   
	//make the first 2 iterations equal the bottom most value and the top most value
	if (iteration <= 2)
   {
		//number of binary possibilities in the "valueArrayCnt" number
		//3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
      binPossib = (1 << valueArrayCnt) - 2;
      
		if (iteration == 1)
      {
         pairValue[0] = -1.0;
         pairValue[1] = -1.0;
      }
		if (iteration == 2)
      {
         pairValue[0] = 1.0;
         pairValue[1] = 1.0;
      }
		
		//find the binary possibilities
		if (binPossib)
      binNum = (iteration-1) % binPossib + 1;
		else
      binNum = 0;
      for (i=0; i < valueArrayCnt; i++)
      {
         //change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
         sz = topMostValueArray[i] - bottomMostValueArray[i];
         pairValueMod[0] = (pairValue[0] + 1.0) / 2.0 * sz + bottomMostValueArray[i];
         pairValueMod[1] = (pairValue[1] + 1.0) / 2.0 * sz + bottomMostValueArray[i];
         
         valueArray[i] = pairValueMod[(binNum>>i)&0b1];
      }
      
      return;
   }
   
	iteration -= 2;
	
	//find if the values should be alike or if they should be different and use binary possibilities
	if (valueArrayCnt > 1)
   {
      accountBinPossibilities(valueArrayCnt, iteration, &binCombInc, &alikeCombInc);
	}
   else
   {
      binCombInc = 0;
		alikeCombInc = iteration;
	}
   
	if (binCombInc)
   generateBinCntValues(&topMostValueArray[0], &bottomMostValueArray[0],
                        valueArrayCnt, binCombInc, &valueArray[0]);
	
	if (alikeCombInc)
   generateAlikeValues(&topMostValueArray[0], &bottomMostValueArray[0],
                       valueArrayCnt, alikeCombInc, &valueArray[0]);
}
