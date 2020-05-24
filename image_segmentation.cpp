#include "image_segmentation.h"
#include "graph.h"
#include "dsu.h"

#include <cmath>
#include <iostream>

double GetIntensivity(const png::rgb_pixel& p) {
    return 0.2126 * p.red + 0.7152 * p.green + 0.0722 * p.blue;
}

double GetDistBetweenPixels(const png::rgb_pixel& p1, const png::rgb_pixel& p2) {
    return std::fabs(GetIntensivity(p1) - GetIntensivity(p2));
}

void Dfs(size_t v, const WeightedGraph& graph, std::vector<char>& used, std::vector<size_t>& component) {
    used[v] = true;
    component.push_back(v);

    for (const auto& [adj, _] : graph.GetAdjancencyListFor(v)) {
        if (!used[adj]) {
            Dfs(adj, graph, used, component);
        }
    }
}

WeightedGraph GraphSegmentate(const WeightedGraph& graph, double c, size_t minSize) {
    std::vector<double> ths(graph.GetVertexNum(), c);
    Dsu dsu(graph.GetVertexNum());
    using Edge = std::pair<size_t, size_t>;
    std::vector<std::pair<Edge, double>> edges(graph.GetEdges().begin(), graph.GetEdges().end());
    std::sort(edges.begin(), edges.end(), [](const auto& p1, const auto& p2) { return p1.second < p2.second; });
    
    WeightedGraph res(graph.GetVertexNum());
    double prev = 0;
    for (const auto& [edge, weight] : edges) {
        size_t u = dsu.GetRootIndex(edge.first);
        size_t v = dsu.GetRootIndex(edge.second);
        if (u != v) {
            if (weight <= std::min(ths[u], ths[v])) {
                dsu.TryUnion(u, v);
                u = dsu.GetRootIndex(u);
                ths[u] = weight + (c / dsu.GetSetSize(u));
            }
        }
    }
    
    for (const auto& [edge, weight] : edges) {
        size_t u = dsu.GetRootIndex(edge.first);
        size_t v = dsu.GetRootIndex(edge.second);
        if (u != v && (dsu.GetSetSize(u) < minSize || dsu.GetSetSize(v) < minSize)) {
            dsu.TryUnion(u, v);
        }
    }
    
    for (const auto& [edge, weight] : edges) {
        auto [u, v] = edge;
        if (dsu.GetRootIndex(u) == dsu.GetRootIndex(v)) {
            res.AddEdge(u, v, weight);
        }
    }
    
    return res;
}

png::image<png::rgb_pixel> ImageSegmentate(const png::image<png::rgb_pixel>& image) {
    WeightedGraph graph(image.get_width() * image.get_height());

    auto getPixelInd = [&](size_t row, size_t col) {
        return row * image.get_width() + col;
    };

    for (size_t row = 0; row < image.get_height(); ++row) {
        for (size_t col = 0; col < image.get_width(); ++col) {

            static constexpr int dRow[] = { -1, 1, 0, 0 };
            static constexpr int dCol[] = { 0, 0, -1, 1 };

            for (size_t dir = 0; dir < 4; ++dir) {
                size_t newRow = row + dRow[dir];
                size_t newCol = col + dCol[dir];
                if (!(0 <= newRow && newRow < image.get_height() && 0 <= newCol && newCol < image.get_width())) {
                    continue;
                }

                double weight = GetDistBetweenPixels(image.get_pixel(col, row), image.get_pixel(newCol, newRow));
                graph.AddEdge(getPixelInd(row, col), getPixelInd(newRow, newCol), weight);
            }
        }
    }
    WeightedGraph mst = GraphSegmentate(graph, 300, 200);
    png::image<png::rgb_pixel> resImage(image.get_width(), image.get_height());

    std::vector<char> used(image.get_width() * image.get_height());
    size_t cnt = 0;
    for (size_t row = 0; row < image.get_height(); ++row) {
        for (size_t col = 0; col < image.get_width(); ++col) {
            if (used[getPixelInd(row, col)]) {
                continue;
            }

            std::vector<size_t> component;
            Dfs(getPixelInd(row, col), mst, used, component);
            ++cnt;
            for (size_t v : component) {
                size_t vRow = v / image.get_width();
                size_t vCol = v % image.get_width();
                resImage.set_pixel(vCol, vRow, image.get_pixel(col, row));
            }
        }
    }
    std::cout << cnt << " components" << std::endl;

    return resImage;
}
