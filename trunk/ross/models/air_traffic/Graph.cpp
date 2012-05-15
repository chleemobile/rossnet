#include "Graph.hpp"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <assert.h>

using namespace std;


Graph::Graph(int nodes)
{
    num_node = nodes;
}

Graph::~Graph()
{

}

void Graph::create_graph(const char *path)
{
    cout<<path<<endl;
	ifstream infile;
	string line;
	infile.open(path, ifstream::in);
    
    adjMatrix = new float*[num_node];

    for (int i=0; i<num_node; i++) {
        adjMatrix[i] = new float[num_node];
    }
    
	if(infile.is_open())
	{
		int i=0;
		while(getline(infile, line))
		{
			istringstream linestream(line);
			string item;
			int j=0;
			while (getline (linestream, item, ','))
			{
                int weight = -1;
                if(item !="")
                {
                    weight = atoi(item.c_str());
                }
				adjMatrix[i][j] = weight;
				j++;
			}
			i++;
		}
	}
	else
	{
		cout << "Error opening file";
        assert(false);
	}
}

deque<int> Graph::get_shortest_path(int source, int dest)
{
    /*
     Indexing is one off
     I added 1 in debug mode
     */
    
    //cout<<source+1<<","<<dest+1<<endl;
    float dist[num_node];   	
	int prev[num_node]; 
	bool marked[num_node];
	bool route_found = false;
	int closest;
    
	//use a big number as the infinity flag
	for(int i=0;i<num_node;i++){
		dist[i] = 32767;
		marked[i]= false;
		prev[i] = -1;
	}
	dist[source] = 0;
    
	for(int i=0; i < num_node; i++){
		//get the closest unmarked node
		closest = closest_node(dist, marked, source);
		marked[closest] = true;	
		//if the next closest is the dest, you are done
		if(closest == dest){break;}
		for(int j = 0; j < num_node; j++){
			//find the minimal adjacent link
			if(!marked[j] && adjMatrix[closest][j] > 0){
				//mark it up if necessary
				float other_route = dist[closest]+adjMatrix[closest][j];
				if(other_route < dist[j]){
					dist[j] = other_route; 
					prev[j] = closest;
				}
			}		
		}
	}


    deque<int> ret;
    int v = dest;
    while(1){
		if(prev[v] == -1){break;}
        //cout<<prev[v]+1<<endl;
        ret.push_back(prev[v]);
        v=prev[v];
	}
    ret.push_front(dest);
    
//    cout<<""<<endl;

    return ret;
}

int Graph::closest_node(float* dist, bool* marked, int source)
{
	int min_dist=32767;
	int closest_node;
    
	for(int i=0; i<num_node; i++){
		if(!marked[i] && min_dist >= dist[i]){
			min_dist = dist[i];
			closest_node = i;
		}	
	}
	return closest_node;
}
