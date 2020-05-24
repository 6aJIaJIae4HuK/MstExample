#include "graph.h"
#include "dsu.h"

WeightedGraph WeightedGraph::MstWithKruskal() const {
    Dsu dsu(GetVertexNum());
    using Edge = std::pair<size_t, size_t>;
    std::vector<std::pair<Edge, double>> edges(Edges.begin(), Edges.end());
    std::sort(edges.begin(), edges.end(), [](const auto& p1, const auto& p2) { return p1.second < p2.second; });
    WeightedGraph res(GetVertexNum());
    for (const auto& [edge, weight] : edges) {
        auto [u, v] = edge;
        if (dsu.TryUnion(u, v)) {
            res.AddEdge(u, v, weight);
        }
    }    
    return res;
}
