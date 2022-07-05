#include "Node.h"
#include <unordered_map>
#include <chrono>
#include <queue>
#include <stack>
#include <ysglfontdata.h>

class dag
{

    public:
    int rows, cols, WIDTH, HEIGHT, size, density;
    bool term, hide;                                    // used to exit program
    int key;                                            // user input keyboard
    int lb,mb,rb,mx,my,evt;                             // user input mouse
    float cellSize;                                     // size of hitbox
    float score;

    std::unordered_map<int, Node*> map;                 // graph (directed acyclic)

    std::queue<Node*> nodeQue;                          // queue used to generate dag
    std::vector<Node*> returnPath;                      // vector used to draw return path after search is complete

    dag(int y, int x, int width, int height, int d)
    {
        rows = y;
        cols = x;
        size = x*y;
        WIDTH = width;
        HEIGHT = height;
        density = d;

        if (cols>rows*width/height){            // sizing cells to window
            cellSize = width/cols;
        }
        else{
            cellSize = height/rows;
        }
        term = false;
        hide = false;

        score = 0;

        FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

        populateMap(size/d);                    // populate random cells in graph with nodes
        generateDag();                          // connect nodes in graph as a dag
        
    }

    void draw(int waitTime = 20, bool hideNodes = false)
    {
        
        glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        drawReturn();

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

        FsSwapBuffers();
        FsSleep(waitTime);
    }

    void drawReturn()                                                           // Draws return path
    {
        for (int i = returnPath.size()-1; i>0; --i)                             
        {
            returnPath[i]->path = true;
            returnPath[i]->drawNode();                                          // draw the node itself
            if(i==1)
                returnPath[0]->drawNode();
            glColor3ub(255, 87, 51);
            glLineWidth(5);
            glBegin(GL_LINES);
            glVertex2f(returnPath[i]->locx,returnPath[i]->locy);
            glVertex2f(returnPath[i-1]->locx,returnPath[i-1]->locy);
            glEnd();
            glColor3ub(82,183,223);

            glRasterPos2i((returnPath[i]->locx+returnPath[i-1]->locx)/2,(returnPath[i]->locy+returnPath[i-1]->locy)/2);                  // Mode
		    char str[256];
            sprintf(str,"%f", returnPath[i]->parentScore);
		    YsGlDrawFontBitmap12x16(str);
        }

    }

    void run()
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
            cleanBoard();
            bfs(map.begin()->second);
        }

        if (key == FSKEY_D)
        {
            cleanBoard();
            dfs(map.begin()->second);
        }

        if (key == FSKEY_J)
        {
            cleanBoard();
            dijkstra(map.begin()->second);
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

    bool terminate()                    // exit program
    {
        return term;
    }

    void clearMap()                     // clear dictionary
    {
        returnPath.clear();
        map.clear();
    }

    void cleanBoard()
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



    void populateMap(int n)             // add n nodes to dictionary
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

    bool addNode(bool dest = false)
    {
        int randX = rand()%cols;                                    // pick random cell to put node
        int randY = rand()%rows;
        int randIdx = randY*cols+randX;                             // make dictionary index from random row and col

        Node* curr = map[randIdx];                                  // find what occupies the dictionary at that slot
        //std::cout<<"ENTR x: "<<randX<<"  y: "<<randY<<std::endl;

        while(curr != nullptr){                                     // if slot is already occupied
            //randIdx += pow(count,2);
            //count++;
            randIdx = rand()%size;                                  // probe until an empty slot is found
            curr = map[randIdx];                                    // update curr
        }

        map[randIdx] = new Node(randIdx%cols,randIdx/cols,cellSize);    // when curr is a null pointer, populate it with a new node
        map[randIdx]->dest = dest;                                      // set destination node
    }

    void yellowNode()                                   // change color of all nodes
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

    void generateDag()                                                  // populate dictionary nodes with neighbors to make graph
    {
        int randPick;
        int randNum;
        for(auto [key, node] : map)                             // iterate through map
        {
            if(node!=nullptr){                                  // ensure location isn't empty
            
                randNum = (rand()%3)+1;                         // generate a randum number of parent nodes for current node

                if(nodeQue.size()>4)                            // make sure that queue size stays 4 so that one node does not get too many enighbors
                    nodeQue.pop();

                if (nodeQue.size()==4)
                {
                    for(int i = 0; i< randNum; ++i)
                    {
                        Node* curr = nodeQue.front();               // this block takes the last node in teh queue and makes the current node its neighbor
                        nodeQue.back()->addNeighbor(node);          // then it moves the front node to the end of the queue
                        nodeQue.pop();                              // it does this a randNum amount of times
                        nodeQue.push(curr);
                        //draw();

                    }

                }
                else if (!nodeQue.empty()){

                    nodeQue.back()->addNeighbor(node);              // nase case while queue isn't long enough to pick 4 neighbors
                    //draw();

                }

                nodeQue.push(node);                                 // add current node to the back
            }

        }

    }

    Node* findNode(int x, int y)                        // find node from dictionary based on mouse input
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


    void topSort()
    {

    }

    void bellmanFord()
    {

    }

/////////// DIJKSTRA ALGORITHM //////////////////////////////////////////////////////////////
    void dijkstra(Node* root)
    {
        //std::queue<Node*> q; 
        score = 0;
        auto cmp = [](Node* node1, Node* node2) { return node1->dijkdist > node2->dijkdist;};   // initialize priority queue
        std::priority_queue<Node*,std::vector<Node*>, decltype(cmp)> priorQ(cmp);               // queue uses dijkstra distance

        Node* found;                                    // initialize found node

        for(auto [key,val] : map)                       // fill priority queue
        {
            if(val == root)
                val->dijkdist = 0;                      // start node distance is 0
            else
                val->dijkdist  =10000000;               // all other nodes distance is infinity
                val->parent = nullptr;                  // set parent node as nullpointer
            priorQ.push(val);
        }

        while (!priorQ.empty())                         // while priority queue is not empty
        {
            Node* frontNode = priorQ.top();             // access first element (call it frontNode)
            frontNode->visited = true;                                           // mark as visited

            if (frontNode->dest)                        // if node is found
                {
                    std::cout<<"found!!!"<<std::endl;
                    found = frontNode;
                    break;
                }
            
            for(auto currNode : frontNode->nodeVec)                                 // iterate through list of neighbors
            {
                Node* tempNode  = std::get<0>(currNode);                            // isolate node from tuple
                if(!tempNode->visited)
                {
                    float tempDist = std::get<1>(currNode) + frontNode->dijkdist;       // distance is the distance of parent node + cost to new node
                    if(tempDist < tempNode->dijkdist)                                   // if this distance is less than existing distance
                    {                    
                        tempNode->parent = frontNode;                                   // set parent as frontnode
                        //frontNode->parentScore = std::get<1>(currNode);
                        tempNode->parentScore = std::get<1>(currNode);                  // adjust score
                        tempNode->dijkdist = tempDist;                                  // update distance (now it will move up in priority queue)
                        priorQ.push(tempNode);
                    }
                }
            }

            priorQ.pop();                                                           // pop current smalles distance

        }
        
        while(found!=nullptr)
        {
            returnPath.push_back(found);
            score += found->parentScore;
            if (found == nullptr)
                std::cout<<"wow"<<std::endl;
            else
                std::cout<<"entry"<<std::endl;

            found = found->parent;
            
        }

        std::cout<<"score = "<<score<<std::endl;

    }

////////// DEPTH FIRST SEARCH ALGORITHM ///////////////////////////////
    
    bool dfsRecurse(Node* root, bool found = false)      // recursive function takes start node and a boolean
    {
        if (root->dest)                                 // if target found
        {
            returnPath.push_back(root);                 // add destination to return path
            root->visited = true;                       // mark node as visited
            return true;                                // return true
        }
        else
        {
            for(int i = 0; i<root->nodeVec.size(); ++i)                                 // iterate through neighbor list
            {
                // This next codeblock is skipped if destination is found
                if ((!std::get<0>(root->nodeVec[i])->visited)&&(found == false)){        // if neighbor is already visited and search still going 
                    root->parentScore = std::get<1>(root->nodeVec[i]);                  // set parent score appropriately
                    found = dfsRecurse(std::get<0>(root->nodeVec[i]));                  // recurse until found is true
                }
                
                if(found && !root->path){                           // if node is found
                    std::cout<<score<<std::endl;                    
                    score+=root->parentScore;                       // increment score as backtracking is done
                    returnPath.push_back(root);                     // add each returned node to return path
                    root->path = true;                              // mark path nodes as visited to account for edge cases
                }
            }
            return found;                                           // return boolean, this will be true only if target is found
        }     
    }

    void dfs(Node* root)            // separate function make just to keep track of score
    {
        score = 0;
        dfsRecurse(root);
        std::cout<<score<<std::endl;
    }
    


////////// BREADTH FIRST SEARCH ALGORITHM ///////////////////////////////

    void bfs(Node* root)
    {
        std::queue<Node*> q;                            // Use a first in first out queue
        score = 0;                                      // Set score as 0
        q.push(root);                                   // Add starting node to queue

        while (!q.empty())
        {
            Node* frontNode = q.front();                // isolate node at the front of the queue

            if (frontNode->dest)                        // if destination is found, break out of loop
                break;

            //std::cout<<frontNode->nodeVec.size()<< "  This the number of neighbors"<<std::endl;
            for(int i = 0; i<frontNode->nodeVec.size();++i)
            {
                Node* curr = std::get<0>(frontNode->nodeVec[i]);
                // if node is not visited
                if(curr->visited==false)
                {
                    curr->visited = true;                                       // mark node as visited
                    curr->returnPath = frontNode->returnPath;                   // different method of backtrack, keep track of every path
                    curr->returnPath.push_back(i);                              // at each level, increment the path to this node from the start
                    curr->parentScore = std::get<1>(frontNode->nodeVec[i]);     // increment parent score
                    q.push(curr);                                               // push edited node to the queue
                }
            }

            q.pop();                        // delete the node at the front of the queue
        }

        std::vector<int> currPath = q.front()->returnPath;        // access return path to destination node

        returnPath.push_back(root);                               // add the start node to return path

        for(int i = 0; i< currPath.size(); ++i)                   // iterate backwards through return path
        {
            std::cout<<score<<std::endl;
            int location = currPath[i];
            root =  std::get<0>(root->nodeVec[location]);         // find neighbor of current node according to return path
            score+= root->parentScore;                            // update score
            returnPath.push_back(root);                           // update node to be the neighbor
        }

        std::cout<<score<<std::endl;

    }

    void printPath(std::vector<int> vec)                            // helper function to print the path
    {
        for(int i = 0; i<vec.size(); ++i)
        {
            std::cout<<vec[i]<<std::endl;
        }
    }

};