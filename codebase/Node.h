#include <iostream>
#include <vector>
#include "fssimplewindow.h"
#include <tuple>
#include <math.h>


class Node
{
    public:
    std::vector<std::tuple<Node*, int> > nodeVec;
    int idxX,idxY;
    float radius;
    float diameter;
    float twicePi = 3.14159*2;
    float locx,locy;

    int resolution;
    float drawRadius;
    int idx;
    
    bool visited;
    bool yellow;
    bool selected;

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
        yellow = false;
        selected = false;
    }

    void addNeighbor(Node* root)
    {
        int weight = sqrt(pow(locy-root->locy,2) + pow(locx-root->locx, 2));
        nodeVec.push_back(std::make_tuple(root,weight));
    }

    void drawNode()
    {
        drawRadius = radius * (nodeVec.size()+1)/3;
        //glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
        //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        if(yellow)
            glColor3ub(200,0,200);
        else
            glColor3ub(82,183,223);

        glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
        //glVertex2f(locx+radius, locy+radius); // center of circle
        glVertex2f(locx,locy); // center of circle
        for (int i = 0; i <= resolution; i++){
            glVertex2f((locx + (drawRadius * cos(i * twicePi / resolution))), (locy + (drawRadius * sin(i * twicePi / resolution))));
            }
        glEnd();


        //FsSwapBuffers();
        //FsSleep(10);

    }

    void drawEdge()
    {
        if(selected)
            glColor3ub(250,100,200);
        else    
            glColor3ub(0,0,200);
        glLineWidth(1);
        glBegin(GL_LINES);
        for(auto curr : nodeVec)
        {
            glVertex2f(locx,locy);
            glVertex2f(std::get<0>(curr)->locx,std::get<0>(curr)->locy);

            //glVertex2f(std::get<0>(curr)->locx,std::get<0>(curr)->locy);
            //glVertex2f(std::get<0>(curr)->locx+5,std::get<0>(curr)->locy+5);

        }
        glEnd();

    }

    void draw()
    {
        drawNode();
        //drawEdge();
    }

};