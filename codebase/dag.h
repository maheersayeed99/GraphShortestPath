#include "Node.h"
#include <unordered_map>
#include <chrono>

class dag
{


    public:
    int rows, cols, WIDTH, HEIGHT, size;
    bool term;
    int key;
    float cellSize;


    std::unordered_map<int, Node*> map;

    dag(int y, int x, int width, int height)
    {
        rows = y;
        cols = x;
        size = x*y;
        WIDTH = width;
        HEIGHT = height;

        if (cols>rows*width/height){            // sizing cells to window
            cellSize = width/cols;
        }
        else{
            cellSize = height/rows;
        }


        term = false;
        FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

        std::cout<<size<<std::endl;

        populateMap(size/3);

        std::cout<<"map size: "<<map.size()<<std::endl;

    }

    void draw()
    {
        glClearColor(0.0,0.0,0.0,0.0);                          // Set background as black
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        for(auto [key, val] : map)
        {
            val->draw();
        }

        FsSwapBuffers();
        FsSleep(10);
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

    }

    bool terminate()
    {
        return term;
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


    }

    void generateNeighbors()
    {
        for(auto [key, node] : map)
        {
            
        }

    }

    void generateDag(int n)
    {
        int tempRow, tempCol;
        for (int i = 0; i<n; ++i)
        {
            tempRow = rand()%100;
            tempCol = rand()%100;
            Node* curr = new Node(tempRow, tempCol, cellSize);
            map[tempCol*cols+rows] = curr;
        }

    }

    bool isValidDag(Node* root)
    {
        return true;
    }

   
};