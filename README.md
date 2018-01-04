# multiple-variable-trial-and-error
mvtrialanderror.c contains 3 main useful functions  

closerFurther() - checks if values are closer to their targets  
guessValues() - guesses and finds values based on input  
These functions are demonstrated in the graph and graph3D folders.  

![trial_and_error_graph](https://github.com/lowlevel86/multiple-variable-trial-and-error/blob/master/graph/graph.png "graph example")

![trial_and_error_graph](https://github.com/lowlevel86/multiple-variable-trial-and-error/blob/master/graph3D/graph3D.png "3d graph example")

generateValues() - given an input number can generate multiple values within ranges without hitting the same value combination twice, it can also start anywhere with about the same amount of computation  
This function is demonstrated in the generateValues folder.  

This graph shows an example of 3 dimensional output:  
![all_combinations](https://github.com/lowlevel86/multiple-variable-trial-and-error/blob/master/all_combinations.png "generateValues example")

Here is an example of generated 2 dimensional output:  
-1.000000 -1.000000  
1.000000 -1.000000  
-1.000000 1.000000  
1.000000 1.000000  
0.000000 0.000000  
0.000000 -1.000000  
-1.000000 0.000000  
1.000000 0.000000  
0.000000 1.000000  
-0.500000 -0.500000  
0.500000 -0.500000  
-0.500000 0.500000  
0.500000 0.500000  
-0.500000 -1.000000  
0.500000 -1.000000  
-1.000000 -0.500000  
0.000000 -0.500000  
1.000000 -0.500000  
-0.500000 0.000000  
0.500000 0.000000  
-1.000000 0.500000  
0.000000 0.500000  
1.000000 0.500000  
-0.500000 1.000000  
0.500000 1.000000  
-0.750000 -0.750000  
-0.250000 -0.750000  
0.250000 -0.750000  
0.750000 -0.750000  
-0.750000 -0.250000  
-0.250000 -0.250000  
0.250000 -0.250000  
0.750000 -0.250000  
-0.750000 0.250000  
-0.250000 0.250000  
0.250000 0.250000  
0.750000 0.250000  
-0.750000 0.750000  
-0.250000 0.750000  
0.250000 0.750000  
0.750000 0.750000  
-0.750000 -1.000000  
-0.250000 -1.000000  
0.250000 -1.000000  
0.750000 -1.000000  
-1.000000 -0.750000  
-0.500000 -0.750000  
0.000000 -0.750000  
0.500000 -0.750000  
1.000000 -0.750000  
-0.750000 -0.500000  
-0.250000 -0.500000  
0.250000 -0.500000  
0.750000 -0.500000  
-1.000000 -0.250000  
-0.500000 -0.250000  
0.000000 -0.250000  
0.500000 -0.250000  
1.000000 -0.250000  
-0.750000 0.000000  
-0.250000 0.000000  
0.250000 0.000000  
0.750000 0.000000  
-1.000000 0.250000  
-0.500000 0.250000  
0.000000 0.250000  
0.500000 0.250000  
1.000000 0.250000  
-0.750000 0.500000  
-0.250000 0.500000  
0.250000 0.500000  
0.750000 0.500000  
-1.000000 0.750000  
-0.500000 0.750000  
0.000000 0.750000  
0.500000 0.750000  
1.000000 0.750000  
-0.750000 1.000000  
-0.250000 1.000000  
0.250000 1.000000  
0.750000 1.000000  
-0.875000 -0.875000  
-0.625000 -0.875000  
-0.375000 -0.875000  
-0.125000 -0.875000  
0.125000 -0.875000  
0.375000 -0.875000  
0.625000 -0.875000  
0.875000 -0.875000  
-0.875000 -0.625000  
-0.625000 -0.625000  
-0.375000 -0.625000  
-0.125000 -0.625000  
0.125000 -0.625000  
0.375000 -0.625000  
0.625000 -0.625000  
0.875000 -0.625000  
-0.875000 -0.375000  
-0.625000 -0.375000  
-0.375000 -0.375000  
-0.125000 -0.375000  

This can be used to test functions in increasing detail which is demonstrated in the test3DCube folder.  
![test function](https://github.com/lowlevel86/multiple-variable-trial-and-error/blob/master/test3DCube/cube.png "test cube")
