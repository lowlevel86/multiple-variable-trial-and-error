set terminal pngcairo transparent enhanced font "arial,10" fontscale 1.0 size 500,400; set output "graph3D.png"

#set title "Graph Title"
#set xlabel "X"
#set ylabel "Y"
#set zlabel "Z"

# sets background color
set object 1 rectangle from screen -0.1,-0.1 to screen 1.1,1.1 fillcolor rgb "#ffffff" behind

# allows rendering of polygons with hidden line removal
set hidden3d back offset 0 trianglepattern 3 undefined 1 altdiagonal bentover

# displays borders 0x7F = 0b1111111
set border 0x7F linecolor rgb "#555555"

# displays the x, y and z axis
set xzeroaxis linewidth 0.5 linetype 1
set yzeroaxis linewidth 0.5 linetype 2
set zzeroaxis linewidth 0.5 linetype 3

# displays the x, y and z grid
set grid xtics linecolor rgb "#888888" linewidth 0.2 linetype 9
set grid ytics linecolor rgb "#888888" linewidth 0.2 linetype 9
set grid ztics linecolor rgb "#888888" linewidth 0.2 linetype 9

# moves the x, y grid to 0
set xyplane at 0

# makes the x, y, and z axis proportional
set view equal xyz

# moves the key out of the graph
set key outside vertical bottom right

# hides the key
set key off

set size 1.5,1.5
set origin -0.25,-0.32

splot\
"line.dat" title "line" with lines linewidth 0.2 linecolor rgb "#404040",\
"point.dat" title "point" with points pointsize 0.6 pointtype 7 linecolor rgb "black"
