#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>

class Graph {
protected:
    int n;
    std::vector<std::vector<int>> adj;

public:
    Graph() : n(0) {}
    virtual ~Graph() {}

    virtual bool hasCycle() const = 0;
    virtual void addEdge(int u, int v) = 0;
    virtual int getNodeCount() const { return n; }
};

#endif