#include "Node.h"
#include <unordered_map>
#include <chrono>
#include <queue>
#include <stack>

class dag
{

    public:
    int rows, cols, WIDTH, HEIGHT, size, density;
    bool term, hide;                                    // used to exit program
    int key;                                            // user input keyboard
    int lb,mb,rb,mx,my,evt;                             // user input mouse
    float cellSize;                                     // size of hitbox
    float score;

    int* temp;


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

        score = 0;

        FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

        populateMap(size/d);                    // populate random cells in graph with nodes
        //yellowNode();
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

    void drawReturn()
    {
        for (int i = returnPath.size()-1; i>0; --i)
        {
            returnPath[i]->path = true;
            returnPath[i]->drawNode();
            if(i==1)
                returnPath[0]->drawNode();
            glColor3ub(255, 87, 51);
            glLineWidth(5);
            glBegin(GL_LINES);
            glVertex2f(returnPath[i]->locx,returnPath[i]->locy);
            glVertex2f(returnPath[i-1]->locx,returnPath[i-1]->locy);
            glEnd();
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

    
    bool dfsRecurse(Node* root, float currScore, bool rslt = false)
    {
        if (root->dest)
        {
            score+=currScore;
            returnPath.push_back(root);
            return true;
        }
        else
        {
            for(int i = 0; i<root->nodeVec.size(); ++i)
            //for(auto curr : root->nodeVec)
            {
                if ((!std::get<0>(root->nodeVec[i])->visited)&&(rslt == false)){
                    std::get<0>(root->nodeVec[i])->visited = true;
                    rslt = dfsRecurse(std::get<0>(root->nodeVec[i]),std::get<1>(root->nodeVec[i]));
                }

                if(rslt)
                    score+=currScore;
                    returnPath.push_back(root);
                    //root->path = true;


            }
            return rslt;

        }        

    }

    void dfs(Node* root)
    {
        score = 0;
        dfsRecurse(root,score);
        std::cout<<score<<std::endl;
    }
    
    void bfs(Node* root)
    {
        std::queue<Node*> q;
        score = 0;

        //root->returnPath.push_back(0);
        q.push(root);

        //root->visited = true;
        while (!q.empty())
        {
            Node* frontNode = q.front();
            //draw();

            if (frontNode->dest)
            {
                printPath(frontNode->returnPath);
                std::cout<<std::endl;
                break;
            }

            if(frontNode!=nullptr)    
                frontNode->visited = true;

            //for (auto curr : frontNode->nodeVec)
            for(int i = 0; i<frontNode->nodeVec.size();++i)
            {
                // if node is not visited                       and         node is not null pointer
                if((!std::get<0>(frontNode->nodeVec[i])->visited)&&(std::get<0>(frontNode->nodeVec[i])!=nullptr))
                {
                    // set curr as the current neighbor node
                    Node* curr = std::get<0>(frontNode->nodeVec[i]);
                    // add it to the queue 
                    

                    //curr->returnPath = frontNode->returnPath + 

                    // change neighbors return path to path of parent
                    curr->returnPath = frontNode->returnPath;
                    // add current index to current neighbors return path
                    curr->returnPath.push_back(i);
                    //frontNode->nodeVec[i]
                    curr->parent = frontNode;
                    curr->parentScore = std::get<1>(frontNode->nodeVec[i]);
                    //std::get<0>(curr)->parent = std::make_tuple(frontNode,std::get<1>(c                                                     urr));
                    q.push(curr);
                }
            }

            q.pop();

            
        }

        std::vector<int> currPath = q.front()->returnPath;

        //Node* backtrack = q.front();                // node that is found (has return path)
        Node* beginning = map.begin()->second;      // beginning node

        printPath(currPath);
        std::cout<<std::endl;
        printPath(beginning->returnPath);


        returnPath.push_back(beginning);
        for(int i = 0; i< currPath.size(); ++i)
        {
            int location = currPath[i];
            //returnPath.push_back(std::get<0>(beginning->nodeVec[location]));
            
            beginning =  std::get<0>(beginning->nodeVec[location]);
            returnPath.push_back(beginning);
            
        }

    }

    void printPath(std::vector<int> vec)
    {
        for(int i = 0; i<vec.size(); ++i)
        {
            std::cout<<vec[i]<<std::endl;
        }
    }

    
   
};