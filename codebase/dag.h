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
    int steps, numVisited;
    char* mode;

    std::unordered_map<int, Node*> map;                 // graph (directed acyclic)

    std::queue<Node*> nodeQue;                          // queue used to generate dag
    std::vector<Node*> returnPath;                      // vector used to draw return path after search is complete

    public:

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
    steps = 0;
    numVisited = 0;
    mode = "";

    FsOpenWindow(50,500,WIDTH,HEIGHT,1);    // Open window

    populateMap(size/d);                    // populate random cells in graph with nodes
    generateDag();                          // connect nodes in graph as a dag

    }




    // user input
    void run();
    // draw functions
    void draw(int waitTime = 20, bool hideNodes = false);
    void drawReturn();
    void drawMenu();
    // algorithms
    bool addNode(bool dest = false);
    void generateDag();
    void bellmanFord(Node* root);
    void dijkstra(Node* root);
    void bfs(Node* root);
    void dfs(Node* root);
    bool dfsRecurse(Node* root, bool found);
    // helper functions
    bool terminate();
    void clearMap();
    void cleanBoard();
    void populateMap(int n);
    void yellowNode();
    void printPath(std::vector<int> vec);
    Node* findNode(int x, int y);


};



