#ifndef image_H_
#define image_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace lab_bmp {
#pragma pack(push, 1)
struct BitMapInfoHeader {
    std::int32_t biSize = 0;
    std::int32_t biWidth = 0;
    std::int32_t biHeight = 0;
    std::int16_t biPlanes = 0;  // NOLINT
    std::int16_t biBitCount = 0;
    std::int32_t biCompression;
    std::int32_t biSizeimage = 0;
    std::int32_t biXPelsPerMeter = 0;  // NOLINT
    std::int32_t biYPelsPerMeter = 0;  // NOLINT
    std::int32_t biClrUsed = 0;
    std::int32_t biClrImportant = 0;  // NOLINT
};
#pragma pack(pop)
static_assert(sizeof(BitMapInfoHeader) == 40, "incorrect BitMapInfoHeader");
#pragma pack(push, 1)
struct BitMapFileHeader {
    std::int16_t bfType = 0;       // NOLINT
    std::int32_t bfSize = 0;       // NOLINT
    std::int16_t bfReserved1 = 0;  // NOLINT
    std::int16_t bfReserved2 = 0;  // NOLINT
    std::int32_t bfOffBits = 0;
};
#pragma pack(pop)
static_assert(sizeof(BitMapFileHeader) == 14, "incorrect BitMapFileHeader");

struct Pixel {             // cppcheck-suppress unusedStructMember
    char first_byte = 0;   // cppcheck-suppress unusedStructMember
    char second_byte = 0;  // cppcheck-suppress unusedStructMember
    char third_byte = 0;   // cppcheck-suppress unusedStructMember
};

struct image {
private:
    BitMapFileHeader fileHeader;
    BitMapInfoHeader infoHeader;
    std::vector<Pixel> pixels;
    std::int32_t align_bytes = 0;

public:
    image(const image &other_image) = default;

    image(image &&other_image) = default;

    image() = default;

    ~image() = default;

    image &operator=(const image &other_image) = default;

    image &operator=(image &&other_image) = default;

    void read(std::istream &input_file);

    void crop(const lab_bmp::image &other_image,
              std::int32_t x,
              std::int32_t y,
              std::int32_t w,
              std::int32_t h);

    void rotate_clockwise(const lab_bmp::image &other_image);

    void write(std::ostream &output_file);
};

}  // namespace lab_bmp

#endif  // image_H_
