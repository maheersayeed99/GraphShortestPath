#include <iostream>
#include <string.h>
#include "dag.h"

int WIDTH = 1500;                       // Set Window Width
int HEIGHT = 600;                       // Set Window Height

int main (int argc, char* argv[])
{
    int y,d;
    srand(time(NULL));                  // seed chrono
    
    if(argc<2)
    {
        y = 20;                         // set default grid size to 20 rows
        d = 20;                         // set default density to 1/20
    }
    else
    {
        y = atoi(argv[1]);
        d = atoi(argv[2]);
    }

    dag newDag(y,2*y,WIDTH,HEIGHT,d);   // generate direct acyclic graph

    while(!newDag.terminate())
    {
        newDag.run();
        newDag.draw(20,newDag.hide);
    }

    return 1;
}