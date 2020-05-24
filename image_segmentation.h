#pragma once

#include <png++/png.hpp>
#include <string>

png::image<png::rgb_pixel> ImageSegmentate(const png::image<png::rgb_pixel>& image);

inline void ImageSegmentate(const std::string& inPath, const std::string& outPath) {
    png::image<png::rgb_pixel> inImage(inPath);
    png::image<png::rgb_pixel> outImage = ImageSegmentate(inImage);
    outImage.write(outPath);
}
