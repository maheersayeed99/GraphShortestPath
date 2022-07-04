#include <iostream>
#include <vector>
#include "fssimplewindow.h"
#include <tuple>
#include <math.h>


class Node
{
    public:
    std::vector<std::tuple<Node*, float> > nodeVec;       // vector of neighbors with weights
    Node* parent = nullptr;
    float parentScore = 0;
    std::vector<int> returnPath;
    //std::string returnPath;

    //std::vector<std::tuple<Node*, float> > parentTup;

    int idxX,idxY;
    float radius, diameter, locx, locy;
    float twicePi = 3.14*2;

    int resolution;
    float drawRadius;
    int idx;
    
    bool visited, yellow, selected, start, path, dest;

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

        glColor3ub(100,100,100);

        if(visited)
            glColor3ub(255, 195, 0);
        if(path)
            glColor3ub(255, 87, 51);
        if(start)
            glColor3ub(218, 247, 166);
        if(dest)
            glColor3ub(199, 0, 57);

            

        glBegin(GL_TRIANGLE_FAN);                           //BEGIN CIRCLE
        glVertex2f(locx,locy);                              // center of circle
        for (int i = 0; i <= resolution; i++){
            glVertex2f((locx + (drawRadius * cos(i * twicePi / resolution))), (locy + (drawRadius * sin(i * twicePi / resolution))));
            }
        glEnd();

    }

    void drawEdge()
    {  
            
        glLineWidth(3);
        
        for(auto curr : nodeVec)
        {
            glLineWidth(3);
            glColor3ub(100,100,100);

            if(std::get<0>(curr)->visited)
            {
                glColor3ub(255, 195, 0);
            }
            if(selected)
            {
                glLineWidth(3);
                glColor3ub(250,250,250);
            }
            
            //if((path||start)&&(std::get<0>(curr)->path))
            //    glColor3ub(255, 87, 51);
            glBegin(GL_LINES);
             
            glVertex2f(locx,locy);
            glVertex2f(std::get<0>(curr)->locx,std::get<0>(curr)->locy);
            glEnd();


        }
        

    }

};