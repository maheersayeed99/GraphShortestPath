#include <iostream>
#include <vector>
#include "fssimplewindow.h"
#include <tuple>
#include <math.h>


class Node
{
    public:
    std::vector<std::tuple<Node*, float> > nodeVec;         // vector of neighbors with weights
    Node* parent = nullptr;                                 // parent node for backtrack
    float parentScore = 0;                                  // parent score for measuring distance
    std::vector<int> returnPath;

    int idxX,idxY;                                          // map index
    float radius, diameter, locx, locy, drawRadius;         // node position variables for drawing
    float twicePi = 3.14*2;
    int resolution;
    bool visited, yellow, selected, start, path, dest;      // booleans used for drawing
    
    float dijkdist;             // distance used for dijkstra
    float bfdist;               // distance used for bellman ford


    Node(int x, int y, float cellSize)
    {
        idxX = x;
        idxY = y;
        diameter = cellSize;
        radius  = cellSize/2;

        locx = idxX*diameter+radius;
        locy = idxY*diameter+radius;

        resolution = 10;

        visited = false;
        path = false;
        yellow = false;
        selected = false;
        start = false;
        dest = false;
    }

    void addNeighbor(Node* root)
    {
        float weight = sqrt(pow(locy-root->locy,2) + pow(locx-root->locx, 2));    // weight of the edge is the euclidean distance b/w nodes
        nodeVec.push_back(std::make_tuple(root,weight));
    }

    void drawNode()
    {
        drawRadius = radius * (nodeVec.size()+1)/3;

        glColor3ub(100,100,100);                    // gray by default

        if(visited)
            glColor3ub(255, 195, 0);                // yellow
        if(path)
            glColor3ub(253, 127, 57);                // orange
        if(start)
            glColor3ub(218, 247, 166);              // green
        if(dest)
            glColor3ub(199, 0, 57);                 // red
        if(yellow)
            glColor3ub(250, 250, 250);              // white

            

        glBegin(GL_TRIANGLE_FAN);                           //BEGIN CIRCLE
        glVertex2f(locx,locy);                              // center of circle
        for (int i = 0; i <= resolution; i++){
            glVertex2f((locx + (drawRadius * cos(i * twicePi / resolution))), (locy + (drawRadius * sin(i * twicePi / resolution))));
            }
        glEnd();

    }

    void drawEdge()
    {  
            
        
        for(auto curr : nodeVec)                        // iterate through neighbor list
        {
            glLineWidth(1);                             // set width as 3
            glColor3ub(100,100,100);                    // gray by default

            if(std::get<0>(curr)->visited)
            {
                glColor3ub(255, 195, 0);                // yellow if visited
            }
            if(selected)
            {
                glLineWidth(3);
                glColor3ub(250,250,250);                // white if selected
            }
            
            glBegin(GL_LINES);
             
            glVertex2f(locx,locy);
            glVertex2f(std::get<0>(curr)->locx,std::get<0>(curr)->locy);
            glEnd();
        }
        

    }

};