#pragma once

#include <vector>
#include <map>
#include <algorithm>

class WeightedGraph {
private:
    std::vector<std::vector<std::pair<size_t, double>>> AdjacencyLists;
    std::map<std::pair<size_t, size_t>, double> Edges;
public:
    WeightedGraph() = default;

    explicit WeightedGraph(size_t vertexNum)
        : AdjacencyLists(vertexNum)
    {}

    void AddEdge(size_t u, size_t v, double weight) {
        if (u >= AdjacencyLists.size() || v >= AdjacencyLists.size()) {
            AdjacencyLists.resize(std::max(u + 1, v + 1));
        }
        AdjacencyLists[u].push_back({ v, weight });
        AdjacencyLists[v].push_back({ u, weight });
        Edges.insert({ { u, v }, weight });
        Edges.insert({ { v, u }, weight });
    }

    const std::vector<std::pair<size_t, double>>& GetAdjancencyListFor(size_t v) const {
        return AdjacencyLists[v];
    }
    
    const std::map<std::pair<size_t, size_t>, double>& GetEdges() const {
        return Edges;
    }
    
    double GetEdgeWeight(size_t from, size_t to) const {
        auto it = Edges.find({ from, to });
        if (it == Edges.end()) {
            return std::numeric_limits<double>::max();
        }
        return it->second;
    }

    WeightedGraph MstWithPrim() const;
    WeightedGraph MstWithKruskal() const;

    size_t GetVertexNum() const {
        return AdjacencyLists.size();
    }
};
