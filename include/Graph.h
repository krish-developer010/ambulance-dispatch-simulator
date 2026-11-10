#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <stdexcept>
#include <iostream>

class Graph
{
public:
    struct Edge
    {
        int to;
        int weight;
        bool blocked;

        Edge(int destination = -1, int roadWeight = 0, bool isBlocked = false)
            : to(destination), weight(roadWeight), blocked(isBlocked)
        {
        }
    };

    Graph();
    Graph(int vertexCount);

    void addVertex(int id);
    void addRoad(int from, int to, int weight);
    void removeRoad(int from, int to);
    void blockRoad(int from, int to);
    void unblockRoad(int from, int to);
    void updateRoadWeight(int from, int to, int newWeight);

    bool hasVertex(int id) const;
    bool hasRoad(int from, int to) const;
    bool isBlocked(int from, int to) const;
    int getWeight(int from, int to) const;

    std::vector<int> getVertices() const;
    std::vector<int> getNeighbors(int vertex) const;

    bool isReachable(int start, int target) const;
    std::vector<int> DFS(int start) const;
    std::vector<int> BFS(int start) const;

    void printGraph() const;

private:
    std::unordered_map<int, std::vector<Edge>> adjacency;
    void addUndirectedEdge(int from, int to, int weight);
    bool findAndUpdateEdge(int from, int to, bool updateBlocked, bool newBlockedValue);
    bool findAndUpdateWeight(int from, int to, int newWeight);
};

#endif
