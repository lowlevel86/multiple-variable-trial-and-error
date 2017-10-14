set terminal pngcairo size 540,400; set output "graph.png"

#set title "Graph Title"
#set xlabel "X"
#set ylabel "Y"

# sets background color
set object 1 rectangle from screen -0.1,-0.1 to screen 1.1,1.1 fillcolor rgb "#ffffff" behind

# changes border color
set border linecolor rgb "#555555"

# displays the x and y axis
set xzeroaxis linewidth 0.5 linecolor rgb "#555555" linetype 1
set yzeroaxis linewidth 0.5 linecolor rgb "#555555" linetype 1

# displays the x and y grid
set grid xtics linecolor rgb "#888888" linewidth 0.2 linetype 9
set grid ytics linecolor rgb "#888888" linewidth 0.2 linetype 9

# sets the axis range
#set xrange [-1.0:1.0]
#set yrange [-1.0:1.0]
#set size ratio 1.0

# moves the key out of the graph
set key outside vertical top right

plot\
"point.dat" title "Point Data" with points pointsize 0.6 pointtype 7 linecolor rgb "black",\
"line.dat" title "Approx." with lines linewidth 0.2 linecolor rgb "black"
