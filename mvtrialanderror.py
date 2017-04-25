#mvtrialanderror.py (multiple variable trial and error)
#this code contains 3 main functions: closerFurther, guessValues, and generateValues

#closerFurther() checks if values are closer to their targets 

#guessValues() guesses and finds values based on input

#generateValues() can generate multiple values within a range without hitting the same value combination twice, it can also start where it left off without having to recalculate its place

import math


#static variables used inside closerFurther()#
keepLastValues = []
keepClosestValues = []
#end of static variables used inside closerFurther()#

def closerFurtherIni(valueArray):
	global keepLastValues
	global keepClosestValues
	keepLastValues = list(valueArray)
	keepClosestValues = list(valueArray)

#this function is a swiss army knife for finding if an array of values are closer to their target values
def closerFurther(valueArray, targetArray, valueArrayCnt, tolerance, rubberBandValues):
	global keepLastValues
	global keepClosestValues
	valueIsCloserArray = []
	valueIsMostClosestArray = []
	true = 1
	false = 0
	neither = -1
	
	numOfValuesWithinTolerance = 0
	valueToTargetSum = 0.0
	
	for i in range(0, valueArrayCnt):
		#rubberBandValues = the further a value is the more weight it has
		if (rubberBandValues):
			valueToTargetSum += math.fabs(targetArray[i] - valueArray[i]) ** 2
		else:
			valueToTargetSum += math.fabs(targetArray[i] - valueArray[i])
		
		if (valueArray[i]+tolerance >= targetArray[i]) and (valueArray[i]-tolerance <= targetArray[i]):
			valueIsCloserArray.append(neither)
			keepClosestValues[i] = valueArray[i]
			valueIsMostClosestArray.append(neither)
			numOfValuesWithinTolerance += 1
		else:
			if math.fabs(targetArray[i]-valueArray[i]) < math.fabs(targetArray[i]-keepLastValues[i]):
				valueIsCloserArray.append(true)
			else:
				valueIsCloserArray.append(false)
		
			if math.fabs(targetArray[i]-valueArray[i]) < math.fabs(targetArray[i]-keepClosestValues[i]):
				keepClosestValues[i] = valueArray[i]
				valueIsMostClosestArray.append(true)
			else:
				valueIsMostClosestArray.append(false)
		
		keepLastValues[i] = valueArray[i]
	
	return valueIsCloserArray, valueIsMostClosestArray, numOfValuesWithinTolerance, valueToTargetSum


#static variables used inside guessValues()#
keepMinCombToll = 0
keepValueArray = []
keepBaseValueArray = []
keepPossibValueArray = []
#end of static variables used inside guessValues()#

def guessValues(topMostValue, bottomMostValue, valueArrayCnt, lastCombToll, iteration):
	global keepMinCombToll
	global keepValueArray
	global keepBaseValueArray
	global keepPossibValueArray
	
	#guess between the top and bottom most for the first set of values 
	if iteration == 1:
		keepValueArray = [(topMostValue - bottomMostValue) / 2.0 + bottomMostValue]*valueArrayCnt
		keepBaseValueArray = list(keepValueArray)
		keepPossibValueArray = list(keepValueArray)
		return keepValueArray
	
	#keep the set of values with the smallest toll
	if iteration == 2:
		keepMinCombToll = lastCombToll
	else:
		if lastCombToll < keepMinCombToll:
			keepMinCombToll = lastCombToll
			keepPossibValueArray = list(keepValueArray)
	
	#timing variables
	highLowCombCnt = 2 ** valueArrayCnt - 1
	highLowCombInc = (iteration-2) % highLowCombCnt
	combCnt = highLowCombCnt * 2
	combInc = (iteration-2) % combCnt
	combIteration = int((iteration-2) / combCnt)
	guessHighLow = int(combInc / highLowCombCnt) % 2
	
	#find a fraction to add to one or many values
	valueFrac = (topMostValue - bottomMostValue) / float(2<<combIteration)
	
	#save the new base value
	if highLowCombInc == 0:
		keepBaseValueArray = list(keepPossibValueArray)
	
	#add or subtract away a fraction to the chosen values using an incrementing binary combination
	for i in range(0, valueArrayCnt):
		if ((highLowCombInc+1)>>i)&0b1:
			if guessHighLow:
				keepValueArray[i] = keepBaseValueArray[i] + valueFrac
			else:
				keepValueArray[i] = keepBaseValueArray[i] - valueFrac
		else:
			keepValueArray[i] = keepBaseValueArray[i]
	
	return keepValueArray


#return non-redundant combination pair from a single group
def retSingleGroupPairCombination(groupSize, combInc):
	selectedPair = []
	
	#get the number of pair combinations the group can have
	combCnt = ((groupSize-1) * ((groupSize-1) + 1.0)) / 2.0 #get triangular number
	
	#get the triangular root of "combInc (reversed)" to help find the combination order
	triangularRoot = int((math.sqrt(8 * ((combCnt-1)-combInc) + 1.0) - 1.0) / 2.0)
	
	
	#find the first number of the combination pair
	selectedPair.append(int((groupSize-1) - (triangularRoot+1)))
	
	#use "resetCombInc" to subtract from combInc and put the combination in a sequential order
	resetCombInc = (selectedPair[0] * (selectedPair[0] + 1.0)) / 2.0
	
	#find the second number of the combination pair
	selectedPair.append(int((combInc - resetCombInc) % (triangularRoot+1) + selectedPair[0]+1))
	
	return selectedPair


#return non-redundant combination pair from 2 different groups and not from the same group
def retDualGroupPairCombination(groupSize, combInc):
	selectedPair = []
	
	#find the combination pair
	selectedPair.append(int(combInc / (groupSize+1)))
	selectedPair.append(int(combInc % (groupSize+1)))
	
	return selectedPair


def retPyramidPairValueCombination(iteration):
	selectedPairValue = []
	
	#get the triangular root of "iteration"
	triRoot = (math.sqrt(8 * iteration + 1.0) - 1.0) / 2.0
	
	#used for finding the number of layers descended
	rev2Pow = int(math.log(triRoot) / math.log(2))
	
	#used for finding the layer combination increment and count
	revRev2PowInt = 2 ** rev2Pow
	triNum = (revRev2PowInt * (revRev2PowInt + 1.0)) / 2.0
	triNumLg = (revRev2PowInt*2 * (revRev2PowInt*2 + 1.0)) / 2.0
	triNumSm = (int(revRev2PowInt/2) * (int(revRev2PowInt/2) + 1.0)) / 2.0
	
	#find the number of layers descended
	#find the number of layer combinations horizontally progressed
	#find the number of layer combinations
	if (triNum < iteration):
		pyramidLayerInc = rev2Pow+1
		layerCombInc = iteration - triNum - 1
		layerCombCnt = triNumLg - triNum
	else:
		pyramidLayerInc = rev2Pow
		layerCombInc = iteration - triNumSm - 1
		layerCombCnt = triNum - triNumSm
	
	#find the number of cells within a layer
	layerCellCnt = 2 ** (pyramidLayerInc-1)
	
	#find the single group pair combination size for the pyramid layer
	singleGroupCombSize = ((layerCellCnt-1) * ((layerCellCnt-1) + 1.0)) / 2.0
	
	#find the dual group pair combination size for the pyramid layer
	dualGroupCombSize = layerCombCnt - singleGroupCombSize
	
	#find the single or dual group pair combination increment and selected pair values
	if layerCombInc < dualGroupCombSize:
		dualGroupCombInc = layerCombInc
		selectedPair = retDualGroupPairCombination(layerCellCnt, dualGroupCombInc)
		selectedPairValue.append(1.0 / layerCellCnt * 2 * selectedPair[0] - (1.0 - 1.0 / layerCellCnt))
		selectedPairValue.append(float(selectedPair[1]) / layerCellCnt * 2 - 1.0)
	else:
		singleGroupCombInc = layerCombInc - dualGroupCombSize
		selectedPair = retSingleGroupPairCombination(layerCellCnt, singleGroupCombInc)
		selectedPairValue.append(1.0 / layerCellCnt * 2 * selectedPair[0] - (1.0 - 1.0 / layerCellCnt))
		selectedPairValue.append(1.0 / layerCellCnt * 2 * selectedPair[1] - (1.0 - 1.0 / layerCellCnt))
	
	return selectedPairValue


def retPyramidValue(iteration):
	
	rev2Pow = int(math.log(iteration) / math.log(2))

	#find the number of cells within a layer
	layerCellCnt = 2 ** rev2Pow
	
	#return a value between -1 and 1 starting with 0 at the top
	#of the pyramid and becoming closer to 1 and -1 moving down
	#example output: 0, -0.5, 0.5, -0.75, -0.25, 0.25, 0.75
	return 1.0 / layerCellCnt * 2 * (iteration-layerCellCnt) - (1.0 - 1.0 / layerCellCnt)


def generateAlikeValues(topMostValue, bottomMostValue, valueArrayCnt, iteration):
	valueArray = []
	
	value = retPyramidValue(iteration)
	
	#change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
	sz = topMostValue - bottomMostValue
	value = (value + 1.0) / 2.0 * sz + bottomMostValue
	
	for i in range(0, valueArrayCnt):
		valueArray.append(value)
	
	return valueArray


def generateBinCntValues(topMostValue, bottomMostValue, valueArrayCnt, iteration):
	valueArray = []
	
	#number of binary possibilities in the "valueArrayCnt" number
	#3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
	binPossib = 2 ** valueArrayCnt - 2
	
	pairValue = retPyramidPairValueCombination(int((iteration-1) / binPossib) + 1)
	
	#change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
	sz = topMostValue - bottomMostValue
	pairValue[0] = (pairValue[0] + 1.0) / 2.0 * sz + bottomMostValue
	pairValue[1] = (pairValue[1] + 1.0) / 2.0 * sz + bottomMostValue
	
	#find the binary possibilities
	binNum = (iteration-1) % binPossib + 1
	for i in range(0, valueArrayCnt):
		valueArray.append(pairValue[(binNum>>i)&0b1])
	
	return valueArray


def accountBinPossibilities(valueArrayCnt, iteration):
	
	#number of binary possibilities in the "valueArrayCnt" number
	#3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
	binPossib = 2 ** valueArrayCnt - 2
	
	#slow the iteration to account for the binary possibilities
	slowIteration = int((iteration-1) / binPossib) + 1
	
	#get the triangular root of "slowIteration"
	triRoot = (math.sqrt(8 * slowIteration + 1.0) - 1.0) / 2.0
	
	#used for finding the number of layers descended
	rev2Pow = int(math.log(triRoot) / math.log(2))
	
	
	#used for finding the layer combination increment and count
	revRev2PowInt = 2 ** rev2Pow
	triNum = (revRev2PowInt * (revRev2PowInt + 1.0)) / 2.0
	triNumLg = (revRev2PowInt*2 * (revRev2PowInt*2 + 1.0)) / 2.0
	triNumSm = (int(revRev2PowInt/2) * (int(revRev2PowInt/2) + 1.0)) / 2.0
	
	#account for binary possibilities
	triNumBin = triNum * binPossib
	triNumLgBin = triNumLg * binPossib
	triNumSmBin = triNumSm * binPossib
	
	
	#find the number of layers descended
	#find the number of layer combinations horizontally progressed
	#find the number of layer combinations
	if ((triNumBin + (revRev2PowInt-1)) < iteration):
		pyramidLayerInc2 = rev2Pow+1
		topToLastLayerCombCnt2 = (triNumBin + (revRev2PowInt-1))
		topToThisLayerCombCnt2 = (triNumLgBin + (revRev2PowInt*2-1))
	else:
		pyramidLayerInc2 = rev2Pow
		topToLastLayerCombCnt2 = (triNumSmBin + (int(revRev2PowInt/2)-1))
		topToThisLayerCombCnt2 = (triNumBin + (revRev2PowInt-1))
	
	layerCombInc2 = iteration - topToLastLayerCombCnt2 - 1
	layerCombCnt2 = topToThisLayerCombCnt2 - topToLastLayerCombCnt2
	
	#find the number of cells within the layer
	layerCellCnt = 2 ** (pyramidLayerInc2-1)
	
	#find the top to last layer combination count for the alike values
	topToLastLayerAlikeCnt = layerCellCnt - 1
	
	
	#find the iteration number for either the "alike" values or the "binary count" values
	if layerCombInc2 < layerCombCnt2 - layerCellCnt:
		binCombInc = int(iteration - topToLastLayerAlikeCnt)
		alikeCombInc = 0
	else:
		alikeLayerInc = layerCellCnt - (layerCombCnt2 - layerCombInc2)
		alikeCombInc = topToLastLayerAlikeCnt + alikeLayerInc + 1
		binCombInc = 0
	
	return binCombInc, alikeCombInc


def generateValues(topMostValue, bottomMostValue, valueArrayCnt, iteration):
	valueArray = []
	
	#make the first 2 iterations equal the bottom most value and the top most value
	if iteration <= 2:
		#number of binary possibilities in the "valueArrayCnt" number
		#3 is equal to 8... 000, 100, 010, 110, 001, 101, 011, 111 minus 000 and 111
		binPossib = 2 ** valueArrayCnt - 2
		
		if iteration == 1:
			pairValue = [-1, -1]
		
		if iteration == 2:
			pairValue = [1, 1]
		
		#change numbers going from 1 to -1 into numbers going from the "top most" to "bottom most"
		sz = topMostValue - bottomMostValue
		pairValue[0] = (pairValue[0] + 1.0) / 2.0 * sz + bottomMostValue
		pairValue[1] = (pairValue[1] + 1.0) / 2.0 * sz + bottomMostValue
		
		#find the binary possibilities
		if binPossib:
			binNum = (iteration-1) % binPossib + 1
		else:
			binNum = 0
		for i in range(0, valueArrayCnt):
			valueArray.append(pairValue[(binNum>>i)&0b1])
	
		return valueArray
	
	iteration -= 2
	
	#find if the values should be alike or if they should be different and use binary possibilities
	if valueArrayCnt > 1:
		binCombInc, alikeCombInc = accountBinPossibilities(valueArrayCnt, iteration)
	else:
		binCombInc = 0
		alikeCombInc = iteration
	
	if binCombInc:
		valueArray = generateBinCntValues(topMostValue, bottomMostValue, valueArrayCnt, binCombInc)
	
	if alikeCombInc:
		valueArray = generateAlikeValues(topMostValue, bottomMostValue, valueArrayCnt, alikeCombInc)
	
	return valueArray

