#ifndef ROUTINGENGINE_H
#define ROUTINGENGINE_H

#include <vector>
#include <unordered_map>
#include <utility>
#include "Graph.h"
#include "MinHeap.h"

class RoutingEngine
{
public:
    RoutingEngine();

    std::unordered_map<int, int> dijkstra(const Graph& graph, int source) const;
    std::vector<int> shortestPath(const Graph& graph, int source, int target) const;
    int distanceTo(const Graph& graph, int source, int target) const;
    bool isReachable(const Graph& graph, int source, int target) const;
    std::vector<int> reconstructPath(const std::unordered_map<int, int>& parent,
                                    int source, int target) const;

private:
    std::pair<std::unordered_map<int, int>, std::unordered_map<int, int>>
    computeShortestInfo(const Graph& graph, int source) const;
};

#endif
