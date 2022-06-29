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



    Node(int x, int y, float cellSize)
    {
        idxX = x;
        idxY = y;
        diameter = cellSize;
        radius  = cellSize/2;


        locx = idxX*diameter+radius;
        locy = idxY*diameter+radius;


        resolution = 20;

        visited = false;
    }

    void addNeighbor(Node* root)
    {
        int weight = rand() %10;
        nodeVec.push_back(std::make_tuple(root,weight));
    }

    void drawNode()
    {
        //glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
        //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        glColor3ub(82,183,223);

        glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
        //glVertex2f(locx+radius, locy+radius); // center of circle
        glVertex2f(locx,locy); // center of circle
        for (int i = 0; i <= resolution; i++){
            glVertex2f((locx + (radius * cos(i * twicePi / resolution))), (locy + (radius * sin(i * twicePi / resolution))));
            }
        glEnd();


        //FsSwapBuffers();
        //FsSleep(10);

    }

    void drawEdge()
    {
        glColor3ub(250,100,200);
        glLineWidth(5);
        glBegin(GL_LINES);
        for(auto curr : nodeVec)
        {
            glVertex2f(locx,locy);
            glVertex2f(std::get<0>(curr)->locx,std::get<0>(curr)->locy);
        }
        glEnd();

    }

    void draw()
    {
        drawNode();
        //drawEdge();
    }

};