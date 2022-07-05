#include "dag.h"


void dag::run()
{
    FsPollDevice();
    key = FsInkey();                            // keyboard input

    if(key == FSKEY_ESC)                        // terminate program with ESC
    {
        term = true;
        std::cout<<term<<std::endl;
    }

    if(key == FSKEY_Y)
    {
        yellowNode();
    }

    if (key == FSKEY_R)                         // generate new dag with R
    {
        clearMap();
        populateMap(size/density);
        generateDag();
    }

    if (key == FSKEY_B)
    {
        mode = "BFS";
        cleanBoard();
        bfs(map.begin()->second);
    }

    if (key == FSKEY_D)
    {
        mode = "DFS";
        cleanBoard();
        dfs(map.begin()->second);
    }

    if (key == FSKEY_J)
    {
        mode = "DIJK";
        cleanBoard();
        dijkstra(map.begin()->second);
    }

    if (key == FSKEY_F)
    {
        mode = "B-FORD";
        cleanBoard();
        bellmanFord(map.begin()->second);
    }

    if (key == FSKEY_H)
    {
        hide = !hide;
    }

    if (key == FSKEY_G)
    {
        std::string temporary = "1234";

        for (auto curr : temporary)
        {
            std::vector<int> one = {1,2,3};
            std::vector<int> two = one;
            two.push_back(4);
            int intarr[4] = {1,2,3,4};
            int newInt = stoi(temporary);
            int newInt2 = atoi(&curr);
            int newInt3 = 5;
            char tempchar = newInt3;
            std::cout<<two[3]<<std::endl;
        }
    }

    
    evt=FsGetMouseEvent(lb,mb,rb,mx,my);        // mouse input
    if(FSMOUSEEVENT_LBUTTONDOWN==evt)           // if left button clicked
    {
        std::cout<<mx<<"   "<<my<<std::endl;
        Node* curr = findNode(mx,my);           // find node where the screen is clicked
        if(curr != nullptr)
        {
            curr->selected = !curr->selected;    // highlight all edges of said node
        }
        else{
            delete curr;
        }
    }
}



void dag::draw(int waitTime, bool hideNodes)
{
    
    glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    if (!hideNodes)
    {
        for(auto [key, val] : map)
        {
            if(val!=nullptr)
                val->drawNode();
        }                                // draw all nodes as circles
        for(auto [key, val] : map)
        {
            if(val!=nullptr)
                val->drawEdge();                                // draw all edges as lines
        }
    }

    drawReturn();

    drawMenu();

    FsSwapBuffers();
    FsSleep(waitTime);
}

void dag::drawReturn()                                                           // Draws return path
{
    for (int i = returnPath.size()-1; i>0; --i)                             
    {
        returnPath[i]->path = true;
        returnPath[i]->drawNode();                                          // draw the node itself
        if(i==1)
            returnPath[0]->drawNode();
        glColor3ub(253, 127, 57);
        glLineWidth(1);
        glBegin(GL_LINES);
        glVertex2f(returnPath[i]->locx,returnPath[i]->locy);
        glVertex2f(returnPath[i-1]->locx,returnPath[i-1]->locy);
        glEnd();
        glColor3ub(82,183,223);

        glRasterPos2i((returnPath[i]->locx+returnPath[i-1]->locx)/2,(returnPath[i]->locy+returnPath[i-1]->locy)/2);                  // Mode
        char str[256];
        sprintf(str,"%.0f", returnPath[i-1]->parentScore);
        YsGlDrawFontBitmap12x16(str);
    }

}

void dag::drawMenu()
{
    glColor3ub(82,183,223);
    char str[256];
    int fontY = 30;


    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"Mode: %s", mode);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Steps
    sprintf(str,"Stops: %d", steps);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Distance
    sprintf(str,"Distance: %.2f", score);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Distance
    sprintf(str,"Visisted: %d", numVisited);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Distance
    sprintf(str,"Rows: %d", rows);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Distance
    sprintf(str,"Columns: %d", cols);
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Distance
    sprintf(str,"Total Nodes: %d", map.size());
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    fontY = HEIGHT/2;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"key   action");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;


    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"R     reset");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"H     hide");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"D     dfs");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"L     bfs");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"J     dijkstra");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"F     bellman-ford");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"CLICK show neighbors");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

    glRasterPos2i(cols*cellSize+5,fontY);                  // Mode
    sprintf(str,"ESC   exit");
	YsGlDrawFontBitmap12x16(str);
    fontY+=30;

}


bool dag::terminate()                    // exit program
{
    return term;
}

void dag::clearMap()                     // clear dictionary
{
    returnPath.clear();
    map.clear();
}

void dag::cleanBoard()
{
    returnPath.clear();
    for(auto [key, node] : map)
        {
            if(node!=nullptr)
                node->path = false;
                node->visited = false;
                node->selected = false;
                node->returnPath.clear();
                node->parentScore = 0;
        }
}



void dag::populateMap(int n)             // add n nodes to dictionary
{
    int randDest = rand()%n;
    for (int i = 0; i<n; ++i)
    {
        if (i == randDest)
            addNode(true);
        else    
            addNode();
    }   
    map.begin()->second->start = true;      // color start node a different color
}

void dag::yellowNode()                                   // change color of all nodes
{
    for(auto [key, node] : map)
    {
        if (node!=nullptr)
        {
            node->yellow = !node->yellow;
            draw();
        }
    }
}

Node* dag::findNode(int x, int y)                        // find node from dictionary based on mouse input
{
    int idxX = x/cellSize;
    int idxY = y/cellSize;
    //std::cout<<idxX<<"   "<<idxY<<std::endl;
    if(map[idxY*cols+idxX]!=nullptr)
        return map[idxY*cols+idxX];
    else{
        map.erase(idxY*cols+idxX);

    }
}

void dag::printPath(std::vector<int> vec)                            // helper function to print the path
{
    for(int i = 0; i<vec.size(); ++i)
    {
        std::cout<<vec[i]<<std::endl;
    }
}