#include "../include/Graph.h"

#include <queue>
#include <stack>
#include <algorithm>

using namespace std;

Graph::Graph()
{
}

Graph::Graph(int vertexCount)
{
    for (int i = 0; i < vertexCount; ++i)
    {
        addVertex(i);
    }
}

void Graph::addVertex(int id)
{
    if (adjacency.find(id) == adjacency.end())
    {
        adjacency[id] = vector<Edge>();
    }
}

void Graph::addRoad(int from, int to, int weight)
{
    if (weight <= 0)
    {
        throw invalid_argument("Road weight must be positive.");
    }

    addVertex(from);
    addVertex(to);

    if (hasRoad(from, to))
    {
        updateRoadWeight(from, to, weight);
        return;
    }

    addUndirectedEdge(from, to, weight);
}

void Graph::removeRoad(int from, int to)
{
    if (adjacency.find(from) == adjacency.end() || adjacency.find(to) == adjacency.end())
    {
        return;
    }

    auto& fromList = adjacency[from];
    auto& toList = adjacency[to];

    fromList.erase(
        remove_if(fromList.begin(), fromList.end(),
                 [&](const Edge& e)
                 {
                     return e.to == to;
                 }),
        fromList.end());

    toList.erase(
        remove_if(toList.begin(), toList.end(),
                 [&](const Edge& e)
                 {
                     return e.to == from;
                 }),
        toList.end());
}

void Graph::addUndirectedEdge(int from, int to, int weight)
{
    adjacency[from].push_back(Edge(to, weight, false));
    adjacency[to].push_back(Edge(from, weight, false));
}

void Graph::blockRoad(int from, int to)
{
    if (!hasRoad(from, to))
    {
        return;
    }

    findAndUpdateEdge(from, to, true, true);
    findAndUpdateEdge(to, from, true, true);
}

void Graph::unblockRoad(int from, int to)
{
    if (!hasRoad(from, to))
    {
        return;
    }

    findAndUpdateEdge(from, to, true, false);
    findAndUpdateEdge(to, from, true, false);
}

void Graph::updateRoadWeight(int from, int to, int newWeight)
{
    if (newWeight <= 0)
    {
        throw invalid_argument("New road weight must be positive.");
    }

    if (!hasRoad(from, to))
    {
        return;
    }

    findAndUpdateWeight(from, to, newWeight);
    findAndUpdateWeight(to, from, newWeight);
}

bool Graph::hasVertex(int id) const
{
    return adjacency.find(id) != adjacency.end();
}

bool Graph::hasRoad(int from, int to) const
{
    if (adjacency.find(from) == adjacency.end())
    {
        return false;
    }

    for (const Edge& edge : adjacency.at(from))
    {
        if (edge.to == to)
        {
            return true;
        }
    }

    return false;
}

bool Graph::isBlocked(int from, int to) const
{
    if (!hasRoad(from, to))
    {
        return false;
    }

    for (const Edge& edge : adjacency.at(from))
    {
        if (edge.to == to)
        {
            return edge.blocked;
        }
    }

    return false;
}

int Graph::getWeight(int from, int to) const
{
    if (!hasRoad(from, to))
    {
        return -1;
    }

    for (const Edge& edge : adjacency.at(from))
    {
        if (edge.to == to)
        {
            return edge.weight;
        }
    }

    return -1;
}

vector<int> Graph::getVertices() const
{
    vector<int> vertices;

    for (const auto& pair : adjacency)
    {
        vertices.push_back(pair.first);
    }

    return vertices;
}

vector<int> Graph::getNeighbors(int vertex) const
{
    vector<int> neighbors;

    if (adjacency.find(vertex) == adjacency.end())
    {
        return neighbors;
    }

    for (const Edge& edge : adjacency.at(vertex))
    {
        if (!edge.blocked)
        {
            neighbors.push_back(edge.to);
        }
    }

    return neighbors;
}

bool Graph::isReachable(int start, int target) const
{
    if (adjacency.find(start) == adjacency.end() || adjacency.find(target) == adjacency.end())
    {
        return false;
    }

    if (start == target)
    {
        return true;
    }

    unordered_set<int> visited;
    queue<int> q;

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();

        for (const Edge& edge : adjacency.at(current))
        {
            if (edge.blocked)
            {
                continue;
            }

            if (visited.find(edge.to) == visited.end())
            {
                if (edge.to == target)
                {
                    return true;
                }

                visited.insert(edge.to);
                q.push(edge.to);
            }
        }
    }

    return false;
}

vector<int> Graph::DFS(int start) const
{
    vector<int> result;
    unordered_set<int> visited;
    stack<int> st;

    if (adjacency.find(start) == adjacency.end())
    {
        return result;
    }

    st.push(start);
    visited.insert(start);

    while (!st.empty())
    {
        int current = st.top();
        st.pop();
        result.push_back(current);

        for (const Edge& edge : adjacency.at(current))
        {
            if (edge.blocked)
            {
                continue;
            }

            if (visited.find(edge.to) == visited.end())
            {
                visited.insert(edge.to);
                st.push(edge.to);
            }
        }
    }

    return result;
}

vector<int> Graph::BFS(int start) const
{
    vector<int> result;
    unordered_set<int> visited;
    queue<int> q;

    if (adjacency.find(start) == adjacency.end())
    {
        return result;
    }

    q.push(start);
    visited.insert(start);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        result.push_back(current);

        for (const Edge& edge : adjacency.at(current))
        {
            if (edge.blocked)
            {
                continue;
            }

            if (visited.find(edge.to) == visited.end())
            {
                visited.insert(edge.to);
                q.push(edge.to);
            }
        }
    }

    return result;
}

void Graph::printGraph() const
{
    for (const auto& pair : adjacency)
    {
        cout << "Vertex " << pair.first << " -> ";

        for (const Edge& edge : pair.second)
        {
            cout << "(" << edge.to << ", " << edge.weight << ", "
                 << (edge.blocked ? "blocked" : "open") << ") ";
        }

        cout << endl;
    }
}

bool Graph::findAndUpdateEdge(int from, int to, bool updateBlocked, bool newBlockedValue)
{
    if (adjacency.find(from) == adjacency.end())
    {
        return false;
    }

    for (Edge& edge : adjacency[from])
    {
        if (edge.to == to)
        {
            if (updateBlocked)
            {
                edge.blocked = newBlockedValue;
            }
            return true;
        }
    }

    return false;
}

bool Graph::findAndUpdateWeight(int from, int to, int newWeight)
{
    if (adjacency.find(from) == adjacency.end())
    {
        return false;
    }

    for (Edge& edge : adjacency[from])
    {
        if (edge.to == to)
        {
            edge.weight = newWeight;
            return true;
        }
    }

    return false;
}
