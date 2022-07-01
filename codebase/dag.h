#include "Node.h"
#include <unordered_map>
#include <chrono>
#include <queue>
#include <stack>

class dag
{

    public:
    int rows, cols, WIDTH, HEIGHT, size, density;
    bool term, hide;                                          // used to exit program
    int key;                                            // user input keyboard
    int lb,mb,rb,mx,my,evt;                             // user input mouse
    float cellSize;                                     // size of hitbox


    std::unordered_map<int, Node*> map;                 // graph (directed acyclic)

    std::queue<Node*> nodeQue;                          // queue used to generate dag
    std::vector<Node*> returnPath;

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

        FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

        populateMap(size/d);                    // populate random cells in graph with nodes
        yellowNode();
        generateDag();                          // connect nodes in graph as a dag
        
    }

    void draw(int waitTime = 20, bool hideNodes = false)
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

        FsSwapBuffers();
        FsSleep(waitTime);
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
            bfs(map.begin()->second);
        }

        if (key == FSKEY_H)
        {
            hide = !hide;
        }

        
	    evt=FsGetMouseEvent(lb,mb,rb,mx,my);        // mouse input
	    if(FSMOUSEEVENT_LBUTTONDOWN==evt)           // if left button clicked
        {
            Node* curr = findNode(mx,my);           // find node where the screen is clicked
            if(curr != nullptr)
            {
               curr->selected = !curr->selected;    // highlight all edges of said node
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

    void addNode(bool dest = false)
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
                        draw();

                    }

                }
                else if (!nodeQue.empty()){

                    nodeQue.back()->addNeighbor(node);              // nase case while queue isn't long enough to pick 4 neighbors
                    draw();

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
        return map[idxY*cols+idxX];
    }


    void topSort()
    {

    }

    void dijkstra()
    {

    }

    void bellmanFord()
    {

    }

    Node* dfs(Node* root)
    {
        
        if(root->nodeVec.size() == 0)
        {
            root->visited = true;

        }
        else
        {
            for(auto curr : root->nodeVec)
            {
                if (!std::get<0>(curr))
                Node* rslt = dfs(curr);
                return rslt;

            }

        }
        


    }

    void bfs(Node* root)
    {
        std::queue<Node*> q;

        q.push(root);
        while (!q.empty())
        {
            Node* frontNode = q.front();
            //draw();

            if (frontNode->dest)
                break;

            frontNode->visited = true;

            for (auto curr : frontNode->nodeVec)
            {
                if((!std::get<0>(curr)->visited)&&(std::get<0>(curr)!=nullptr))
                {
                    q.push(std::get<0>(curr));
                    std::get<0>(curr)->parent = frontNode;
                }
            }

            q.pop();

            
        }

        Node* backtrack = q.front();
        returnPath.push_back(backtrack);
        while(backtrack->parent!= nullptr)
        {
            backtrack = backtrack->parent;
            returnPath.push_back(backtrack);
        }

    }

    void drawReturn()
    {
        for (int i = returnPath.size()-1; i>0; --i)
        {
            returnPath[i]->path = true;
            returnPath[i]->drawNode();
            if(i==1)
                returnPath[0]->drawNode();
            glColor3ub(255, 87, 51);
            glBegin(GL_LINES);
            glVertex2f(returnPath[i]->locx,returnPath[i]->locy);
            glVertex2f(returnPath[i-1]->locx,returnPath[i-1]->locy);
            glEnd();
        }


    }
   
};