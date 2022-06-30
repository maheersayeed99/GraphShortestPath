#include "Node.h"
#include <unordered_map>
#include <chrono>
#include <queue>

class dag
{


    public:
    int rows, cols, WIDTH, HEIGHT, size, density;
    bool term;
    int key;
    int lb,mb,rb,mx,my,evt;
    float cellSize;


    std::unordered_map<int, Node*> map;
    std::vector<int> availNodes;


    std::queue<Node*> nodeQue;

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
        FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

        std::cout<<size<<std::endl;

        populateMap(size/d);
        yellowNode();
        generateDag();
        

        std::cout<<"map size: "<<map.size()<<std::endl;

    }

    void draw()
    {
        glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        for(auto [key, val] : map)
        {
            if(val!=nullptr)
                val->drawNode();
        }
        for(auto [key, val] : map)
        {
            if(val!=nullptr)
                val->drawEdge();
        }

        FsSwapBuffers();
        FsSleep(250);
    }

    void run()
    {
        FsPollDevice();
        key = FsInkey();

        if(key == FSKEY_ESC)
        {
            term = true;
            std::cout<<term<<std::endl;
        }

        if(key == FSKEY_Y)
        {
            
            yellowNode();

        }

        if (key == FSKEY_R)
        {
            clearMap();
            populateMap(size/density);
            generateDag();
        }

        
	    evt=FsGetMouseEvent(lb,mb,rb,mx,my);
	    if(FSMOUSEEVENT_LBUTTONDOWN==evt)
        {
            Node* curr = findNode(mx,my);
            //std::cout<<"found"<<std::endl;
            if(curr != nullptr)
            {
               curr->selected = !curr->selected;
            }
            //std::cout<<"null2"<<std::endl;
	    }
        //std::cout<<"null3"<<std::endl;

    }

    bool terminate()
    {
        return term;
    }

    void clearMap()
    {
        map.clear();
    }

    void populateMap(int n)
    {
        for (int i = 0; i<n; ++i)
        {
            addNode();
        }
    }

    void addNode()
    {
        int randX = rand()%cols;
        int randY = rand()%rows;
        int randIdx = randY*cols+randX;

        Node* curr = map[randIdx];

        std::cout<<"ENTR x: "<<randX<<"  y: "<<randY<<std::endl;

        //int count = 1;
        while(curr != nullptr){
            //randIdx += pow(count,2);
            //count++;
            randIdx = rand()%size;
            curr = map[randIdx];
        }

        std::cout<<"EXIT x: "<<randIdx%cols<<"  y: "<<randIdx/cols<<" index: "<< randIdx<<std::endl;

        map[randIdx] = new Node(randIdx%cols,randIdx/cols,cellSize);
        //availNodes.push_back(randIdx);

    }

    void yellowNode()
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

    void generateDag()
    {
        int randPick;
        int randNum;
        for(auto [key, node] : map)
        {
            if(node!=nullptr){
            
                randNum = (rand()%4)+1;

                if(nodeQue.size()>4)
                    nodeQue.pop();

                if (nodeQue.size()==4)
                {
                    for(int i = 0; i< 4; ++i)
                    {
                        Node* curr = nodeQue.front();
                        nodeQue.back()->addNeighbor(node);
                        nodeQue.pop();
                        nodeQue.push(curr);
                        draw();

                    }

                }
                else if (!nodeQue.empty()){

                    nodeQue.back()->addNeighbor(node);
                    draw();

                }
                

                nodeQue.push(node);
            }

        }

    }

    Node* findNode(int x, int y)
    {
        int idxX = x/cellSize;
        int idxY = y/cellSize;
        std::cout<<idxX<<"   "<<idxY<<std::endl;
        return map[idxY*cols+idxX];
    }

    bool isValidDag(Node* root)
    {
        return true;
    }

   
};