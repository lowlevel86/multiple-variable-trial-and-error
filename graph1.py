import matplotlib.pyplot as plt
from mvtrialanderror import *

def xCutsLine(x1Line, x2Line, y1Line, y2Line, xCutLocs):
	xLine = []
	yLine = []
	
	for cutLoc in xCutLocs:
		xLine.append(cutLoc)
		yLine.append(y1Line - float(x1Line - cutLoc) / float(x1Line - x2Line) * (y1Line - y2Line))
	
	return xLine, yLine

true = 1
false = 0
valueToTargetSum = 0
xPoints = [1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0]
yPoints = [749.0, 614.0, 518.0, 412.0, 398.0, 347.0, 291.0, 238.0, 338.0]
topMostValue = max(yPoints)
bottomMostValue = min(yPoints)
xValues = [min(xPoints), max(xPoints)]
yValues = []

for iteration in range(1, 50):
	
	yValues = guessValues(topMostValue, bottomMostValue, len(xValues), valueToTargetSum, iteration)
	
	xLinePts, yLinePts = xCutsLine(xValues[0], xValues[1], yValues[0], yValues[1], xPoints)
	
	if iteration == 1:
		closerFurtherIni(yLinePts)
	
	closerFurtherRetData = closerFurther(yLinePts, yPoints, len(yLinePts), 0.001, true)
	#valueIsCloserArray = closerFurtherRetData[0]
	#valueIsMostClosestArray = closerFurtherRetData[1]
	#numOfValuesWithinTolerance = closerFurtherRetData[2]
	valueToTargetSum = closerFurtherRetData[3]
	
	print("valueToTargetSum: %f" % (valueToTargetSum))
	
	#plt.clf()
	plt.plot(xPoints, yPoints, 'ro')
	plt.plot([xValues[0], xValues[1]], [yValues[0], yValues[1]])
	plt.draw()

plt.savefig('graph.png')
