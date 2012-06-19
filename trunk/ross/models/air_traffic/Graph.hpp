#include <deque>

class Graph
{
    
public:
    Graph(int nodes);
    virtual ~Graph();
    
    void create_graph(const char *path);
    std::deque<int> get_shortest_path(int from, int to);
	void print_adjmatrix();
    
//private:
    float** adjMatrix;
    int num_node;
    int closest_node(float* dist, bool* marked, int source);
};
