#include "../include/RoutingEngine.h"

#include <limits>
#include <algorithm>

using namespace std;

RoutingEngine::RoutingEngine()
{
}

pair<unordered_map<int, int>, unordered_map<int, int>>
RoutingEngine::computeShortestInfo(const Graph& graph, int source) const
{
    unordered_map<int, int> distances;
    unordered_map<int, int> parent;

    const int INF = numeric_limits<int>::max();

    vector<int> vertices = graph.getVertices();

    for (int vertex : vertices)
    {
        distances[vertex] = INF;
        parent[vertex] = -1;
    }

    if (graph.hasVertex(source) == false)
    {
        return {distances, parent};
    }

    distances[source] = 0;

    MinHeap<pair<int, int>> minHeap(
        [](const pair<int, int>& a, const pair<int, int>& b)
        {
            return a.first < b.first;
        });

    minHeap.insert({0, source});

    while (!minHeap.empty())
    {
        pair<int, int> current = minHeap.extractMin();
        int currentDistance = current.first;
        int currentVertex = current.second;

        if (currentDistance != distances[currentVertex])
        {
            continue;
        }

        vector<int> neighbors = graph.getNeighbors(currentVertex);

        for (int neighbor : neighbors)
        {
            int weight = graph.getWeight(currentVertex, neighbor);

            if (weight <= 0)
            {
                continue;
            }

            int newDistance = currentDistance + weight;

            if (newDistance < distances[neighbor])
            {
                distances[neighbor] = newDistance;
                parent[neighbor] = currentVertex;
                minHeap.insert({newDistance, neighbor});
            }
        }
    }

    return {distances, parent};
}

unordered_map<int, int> RoutingEngine::dijkstra(const Graph& graph, int source) const
{
    if (!graph.hasVertex(source))
    {
        return {};
    }

    pair<unordered_map<int, int>, unordered_map<int, int>> result =
        computeShortestInfo(graph, source);

    return result.first;
}

vector<int> RoutingEngine::reconstructPath(const unordered_map<int, int>& parent,
                                          int source, int target) const
{
    vector<int> path;

    if (source == target)
    {
        path.push_back(source);
        return path;
    }

    if (parent.find(target) == parent.end())
    {
        return path;
    }

    int current = target;

    while (current != -1)
    {
        path.push_back(current);

        if (current == source)
        {
            break;
        }

        if (parent.find(current) == parent.end())
        {
            return {};
        }

        current = parent.at(current);
    }

    if (current != source)
    {
        return {};
    }

    reverse(path.begin(), path.end());
    return path;
}

vector<int> RoutingEngine::shortestPath(const Graph& graph, int source, int target) const
{
    if (!graph.hasVertex(source) || !graph.hasVertex(target))
    {
        return {};
    }

    pair<unordered_map<int, int>, unordered_map<int, int>> result =
        computeShortestInfo(graph, source);

    const unordered_map<int, int>& parent = result.second;

    return reconstructPath(parent, source, target);
}

int RoutingEngine::distanceTo(const Graph& graph, int source, int target) const
{
    if (!graph.hasVertex(source) || !graph.hasVertex(target))
    {
        return -1;
    }

    pair<unordered_map<int, int>, unordered_map<int, int>> result =
        computeShortestInfo(graph, source);

    const unordered_map<int, int>& distances = result.first;

    if (distances.find(target) == distances.end())
    {
        return -1;
    }

    if (distances.at(target) == numeric_limits<int>::max())
    {
        return -1;
    }

    return distances.at(target);
}

bool RoutingEngine::isReachable(const Graph& graph, int source, int target) const
{
    return distanceTo(graph, source, target) != -1;
}
