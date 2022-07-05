# GraphShortestPath

## Description
This is a program that tests 4 separate pathfinding algoriths on a directed acyclic graph. The DAG is first marked with a start and end node. Every single connected neighbor in the DAG is accompanied with a value representing the cost to travel between those nodes. This value is the euclidean distance between the nodes. After this, the user can test the algorithms individually, while information is stored such as the distance travelled and the nodes visited.  

The first algorithm, depth-first-search uses a recurisive algorithm to find the marker. However, the distance is not taken into accound, and the number of steps is not minimized.  

The next algorithm, breadth-first-search, uses a queue based algorithm to find the marker. The distances are not used, but the number of steps is minimized.  

The next algorithm, dijkstra's algorithm, uses a priority queue based algorithm to find the marker. The distances are used this time, and the path with the smallest distance is picked as the shortest path solution. This solution cannot handle negative weights, but that does not concern this program as all the weights on the node edges are positive.  

The final algorithm is the bellman-ford algorithm. This algorithm can handle negative weights, but it is a less efficient algorithm as it has to make multiple passes over the DAG to correctly find the shortest path. This algorithm will visit the most nodes. 

## Running the Application
Cmake is used to build and run the program. Create a build directory outside the repository and enter
`cmake add ./GraphShortestPath`. After this, build the program with the command `cmake --build . --target gsp --config Release`. 'gsp' is the name of the target executable for this program.  
Once the project is built, run by opening the gsp.app file `open ./codebase/gsp.app --args rows density`. rows is an integer that represents the number of rows in the maze. The number of columns will be twice the number of rows. A default value of 20 is used for rows in this program. Density is an integer that represents the fraction of the node grid (1/density) that will have nodes in the generated directed acyclic graph. A default value of 20 is used for the density.
## Controls
`R` Generates a random new DAG and marks the start and end nodes green and red. The DAG generation algorithm is found in algorithms.cpp  
![DAG Generated](https://github.com/maheersayeed99/GraphShortestPath/images/Select.png)
`H` Hides everything except the return path.  
`LEFT CLICK` Highlights the paths to the neighbors of the node that is clicked.
`D` Finds the marker using depth first search. The recursive algorithm is found in algorithms.cpp   
`B` Finds the marker using breadth first search. The iterative queue based algorithm is found in algorithms.cpp  
`J` Finds the marker using Dijkstra's algorithm. The weights are the euclidean distances between the neighboring nodes. The priority queue based algorithm is found in algorithms.cpp  
`F` Finds the marker using the Bellman-Ford algorithm. The queue based algorithm is found in algorithms.cpp  
`ESC` Exits the program
## Menu
The number of steps until the marker is found.  
The total distance travelled until marker is found.  
The total number of nodes visited, including nodes that are visited more than once.  
The number of rows in the node map.  
The number of colums in the node map.  
The total number of nodes that are in the dag.  
