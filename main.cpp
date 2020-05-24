#include "graph.h"
#include "image_segmentation.h"

#include <iostream>
#if __has_include(<filesystem>)
#  include <filesystem>
#else
#  include <experimental/filesystem>
   namespace std {
     namespace filesystem = experimental::filesystem;
   }
#endif

namespace fs = std::filesystem;

int main(int argc, const char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <imageToSegmentate>"  << std::endl;
        return 1;
    }

    WeightedGraph graph;
    graph.AddEdge(0, 1, 3);
    graph.AddEdge(0, 2, 4);
    graph.AddEdge(0, 4, 1);
    graph.AddEdge(1, 2, 5);
    graph.AddEdge(2, 3, 2);
    graph.AddEdge(2, 4, 6);
    graph.AddEdge(3, 4, 7);

    WeightedGraph mst1 = graph.MstWithPrim();
    WeightedGraph mst2 = graph.MstWithKruskal();

    std::string inputPath = argv[1];
    std::string fileName = fs::path(inputPath).filename();
    std::string newFileName = "segmentated_" + fileName;
    std::string outputPath = fs::path(inputPath).replace_filename(newFileName);
    ImageSegmentate(inputPath, outputPath);

    return 0;
}
