#include <iostream>
#include <string.h>
#include "dag.h"

int WIDTH = 1200;
int HEIGHT = 600;

int main (int argc, char* argv[])
{
    srand(time(NULL));
    int y = atoi(argv[1]);

    dag newDag(y,2*y,WIDTH,HEIGHT);
    newDag.map[0] = new Node(0,0,newDag.cellSize);

    newDag.map[1] = new Node(1,0,newDag.cellSize);

    newDag.map[0]->addNeighbor(newDag.map[1]);


    //Node* newNode = new Node(50,50);

    std::cout<<7/3<<std::endl;
    std::cout<<8/3<<std::endl;
    
    


    while(!newDag.terminate())
    {

        newDag.run();
        //newNode->draw();
        newDag.draw();
    }
    



    /*
    Node* curr = new Node(x,16);

    std::unordered_map<int,Node*> map1;

    map1[50] = new Node(1,2);

    if (map1[2] == nullptr){
        std::cout<<"empty"<<std::endl;
    }
    else{
        std::cout<<"not empty"<<std::endl;
    }


    std::cout<< map1[50]->locx <<std::endl;
    */

    return 1;
}