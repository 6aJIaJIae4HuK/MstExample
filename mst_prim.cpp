#include "graph.h"

#include <set>

WeightedGraph WeightedGraph::MstWithPrim() const {
    static constexpr double INF = std::numeric_limits<double>::max();
    static constexpr size_t NO_VERTEX = static_cast<size_t>(-1);

    std::vector<double> dist(GetVertexNum(), INF);
    std::vector<size_t> parent(GetVertexNum(), NO_VERTEX);
    std::vector<char> used(GetVertexNum(), false);
    dist[0] = 0;

    std::set<std::pair<double, size_t>> q;
    for (size_t i = 0; i < GetVertexNum(); ++i) {
        q.insert({ dist[i], i });
    }

    while (!q.empty()) {
        size_t u = q.begin()->second;
        used[u] = true;
        q.erase(q.begin());
        for (auto [adj, w] : AdjacencyLists[u]) {
            if (!used[adj] && w < dist[adj]) {
                q.erase({ dist[adj], adj });
                dist[adj] = w;
                q.insert({ dist[adj], adj });
                parent[adj] = u;
            }
        }
    }

    WeightedGraph res(GetVertexNum());
    for (size_t i = 0; i < GetVertexNum(); ++i) {
        if (parent[i] == NO_VERTEX) {
            continue;
        }
        res.AddEdge(i, parent[i], Edges.at({ i, parent[i] }));
    }
    return res;
}
