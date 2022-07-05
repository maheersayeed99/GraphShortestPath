#include "dag.h"

/////////// DAG GENERATION ALGORITHM //////////////////////////////////////////////////////////////


bool dag::addNode(bool dest)
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



void dag::generateDag()                                                  // populate dictionary nodes with neighbors to make graph
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


/////////// BELLMAN-FORD ALGORITHM //////////////////////////////////////////////////////////////

void dag::bellmanFord(Node* root)
{
    std::queue<Node*> q;

    for(auto[key,val]:map)                  // set all distances to infinity 
    {
        val->bfdist = 1000000;              // set all parents to null
        val->parent = nullptr;
    }
    bool done = false;                      // this boolean keeps track of whether search is complete
    Node* found;                            // when node is found this will be used to access it
    score = 0;
    steps = 0;
    numVisited = 0;

    while(!done)
    {
        done = true;                        // set boolean true and it will turn false if more updating is needed
        
        root->bfdist = 0;                   // set start node distance to 0
        q.push(root);

        while(!q.empty())                                                   // bellman ford loop
        {
            Node* frontNode = q.front();                                    // access front element
            frontNode->visited = true;
            numVisited++;

            if(frontNode->dest)                                             // if found
                found = frontNode;

            for(auto curr: frontNode->nodeVec)                              // iterate through all neighbors
            {
                Node* tempNode = std::get<0>(curr);
                
                float tempDist = frontNode->bfdist + std::get<1>(curr);     // distance
                if(tempDist<tempNode->bfdist)
                {
                    tempNode->bfdist = tempDist;                            // update distance if new path is faster
                    tempNode->parent = frontNode;                           // set parent for backtrack
                    tempNode->parentScore = std::get<1>(curr);              // set parent score
                    done = false;                                           // make boolean false if anything is changed in a pass
                }
                q.push(tempNode);                                           // add neighbor to queue

            }

            q.pop();                                                        // remove latest from queue
        }
        //std::cout<<"exited first loop"<<std::endl;

    }
    //std::cout<<"exited second loop"<<std::endl;

    while(found!=nullptr)                   // begin backtrack
    {
        returnPath.push_back(found);        // add to return path
        score += found->parentScore;        // update score
        found = found->parent;
        steps++;
    }
    steps--;
    std::cout<<score<<std::endl;
}

/////////// DIJKSTRA ALGORITHM //////////////////////////////////////////////////////////////
void dag::dijkstra(Node* root)
{
    //std::queue<Node*> q; 
    score = 0;
    steps = 0;
    numVisited = 0;
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
        numVisited++;

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
    
    while(found!=nullptr)                               // backtrack
    {
        returnPath.push_back(found);                    // add to drawing vector
        score += found->parentScore;                    // increment total score
        found = found->parent;                          // iterate found
        steps++;
    }
    steps--;

    std::cout<<"score = "<<score<<std::endl;

}

////////// DEPTH FIRST SEARCH ALGORITHM ///////////////////////////////

bool dag::dfsRecurse(Node* root, bool found = false)      // recursive function takes start node and a boolean
{
    root->visited = true;                       // mark node as visited
    numVisited++;
    if (root->dest)                                 // if target found
    {
        returnPath.push_back(root);                 // add destination to return path
        return true;                                // return true
    }
    else
    {
        for(int i = 0; i<root->nodeVec.size(); ++i)                                 // iterate through neighbor list
        {
            // This next codeblock is skipped if destination is found
            if ((!std::get<0>(root->nodeVec[i])->visited)&&(found == false)){        // if neighbor is already visited and search still going 
                std::get<0>(root->nodeVec[i])->parentScore = std::get<1>(root->nodeVec[i]);                  // set parent score appropriately
                found = dfsRecurse(std::get<0>(root->nodeVec[i]));                  // recurse until found is true
            }
            
            if(found && !root->path){                           // if node is found
                std::cout<<score<<std::endl;                    
                score+=std::get<0>(root->nodeVec[i])->parentScore;                       // increment score as backtracking is done
                steps++;
                returnPath.push_back(root);                     // add each returned node to return path
                root->path = true;                              // mark path nodes as visited to account for edge cases
            }
        }
        return found;                                           // return boolean, this will be true only if target is found
    }     
}

void dag::dfs(Node* root)            // separate function make just to keep track of score
{
    score = 0;
    steps = 0;
    numVisited = 0;
    dfsRecurse(root);
    std::cout<<score<<std::endl;
}



////////// BREADTH FIRST SEARCH ALGORITHM ///////////////////////////////

void dag::bfs(Node* root)
{
    std::queue<Node*> q;                            // Use a first in first out queue
    score = 0;                                      // Set score as 0
    steps = 0;
    numVisited = 0;
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
                numVisited++;
                //curr->returnPath = frontNode->returnPath;                   // different method of backtrack, keep track of every path
                //curr->returnPath.push_back(i);                              // at each level, increment the path to this node from the start
                curr->parentScore = std::get<1>(frontNode->nodeVec[i]);     // increment parent score
                curr->parent = frontNode;
                q.push(curr);                                               // push edited node to the queue
            }
        }

        q.pop();                        // delete the node at the front of the queue
    }
    
    Node* backtrack = q.front();
    while(backtrack!=nullptr)
    {
        returnPath.push_back(backtrack);
        score+= backtrack->parentScore;
        backtrack = backtrack->parent;
        steps++;
    }
    steps--;

    std::cout<<score<<std::endl;

    /*
    std::vector<int> currPath = q.front()->returnPath;        // access return path to destination node

    //returnPath.push_back(root);                               // add the start node to return path

    for(int i = 0; i< currPath.size(); ++i)                   // iterate backwards through return path
    {
        std::cout<<score<<std::endl;
        int location = currPath[i];
        
        score+= root->parentScore;                            // update score
        returnPath.push_back(root);                           // update node to be the neighbor

        root =  std::get<0>(root->nodeVec[location]);         // find neighbor of current node according to return path
    }

    score+= root->parentScore;
    returnPath.push_back(root);
    */

}
