#include "image.h"
#include <memory>
namespace lab_bmp {
struct WrongBmpFormat : std::runtime_error {
    explicit WrongBmpFormat(const std::string &s)
        : std::runtime_error("Invalid BMP: " + s) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};
struct WrongArguments : std::runtime_error {
    WrongArguments()
        : std::runtime_error("The requested area is not a subimage") {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};
void check_arguments(std::int32_t x,
                     std::int32_t y,
                     std::int32_t w,
                     std::int32_t h,
                     std::int32_t picture_width,
                     std::int32_t picture_height) {
    if (!(0 <= x && 0 < w && x + w <= picture_width && 0 <= y && 0 < h &&
          y + h <= picture_height)) {
        throw lab_bmp::WrongArguments();
    }
}
}  // namespace lab_bmp

std::int32_t to_four(std::int32_t x) {
    return x + (4 - x % 4) % 4;
}

void lab_bmp::image::read(std::istream &input_file) {
    input_file.read(reinterpret_cast<char *>(&fileHeader),
                    sizeof(BitMapFileHeader));
    input_file.read(reinterpret_cast<char *>(&infoHeader),
                    sizeof(BitMapInfoHeader));
    if (infoHeader.biSize != 40) {
        std::string s =
            "expected version 3 and header size 40, but header size is ";
        s += std::to_string(infoHeader.biSize);
        throw lab_bmp::WrongBmpFormat(s);
    }
    if (infoHeader.biHeight <= 0) {
        std::string s = "expected positive biHeight, got ";
        s += std::to_string(infoHeader.biHeight);
        throw lab_bmp::WrongBmpFormat(s);
    }
    if (infoHeader.biBitCount != 24) {
        std::string s = "expected 24 bits per pixel, got ";
        s += std::to_string(infoHeader.biBitCount);
        throw lab_bmp::WrongBmpFormat(s);
    }
    if (infoHeader.biCompression != 0) {
        throw lab_bmp::WrongBmpFormat("compression is unsupported");
    }
    if (infoHeader.biClrUsed != 0) {
        throw lab_bmp::WrongBmpFormat("color palette is unsupported");
    }
    pixels.resize(infoHeader.biWidth * infoHeader.biHeight);
    Pixel fictitious_pixel;
    align_bytes = to_four(3 * infoHeader.biWidth) - 3 * infoHeader.biWidth;
    for (int i = 0; i < infoHeader.biHeight; i++) {
        input_file.read(reinterpret_cast<char *>(pixels.data()) +
                            i * 3 * infoHeader.biWidth,
                        3 * infoHeader.biWidth);
        input_file.read(reinterpret_cast<char *>(&fictitious_pixel),
                        align_bytes);
    }
}

void lab_bmp::image::crop(const lab_bmp::image &other_image,
                          std::int32_t x,
                          std::int32_t y,
                          std::int32_t w,
                          std::int32_t h) {
    infoHeader = other_image.infoHeader;
    fileHeader = other_image.fileHeader;
    auto other_W = infoHeader.biWidth;
    auto other_H = infoHeader.biHeight;
    check_arguments(x, y, w, h, other_W, other_H);
    align_bytes = to_four(3 * w) - 3 * w;
    pixels.resize(w * h);
    size_t index = 0;
    for (std::int32_t l = 0; l < other_H; l++) {
        for (std::int32_t j = 0; j < other_W; j++) {
            if (j >= x && j <= x + w - 1 && l >= other_H - y - h &&
                l <= other_H - y - 1) {
                pixels[index++] = other_image.pixels[l * other_W + j];
            }
        }
    }
    infoHeader.biWidth = w;
    infoHeader.biHeight = h;
    infoHeader.biSizeimage = (3 * w + align_bytes) * h;
    fileHeader.bfSize = infoHeader.biSizeimage + fileHeader.bfOffBits;
}

void lab_bmp::image::rotate_clockwise(const lab_bmp::image &other_image) {
    infoHeader = other_image.infoHeader;
    fileHeader = other_image.fileHeader;
    auto W = infoHeader.biWidth;
    auto H = infoHeader.biHeight;
    align_bytes = to_four(3 * H) - 3 * H;
    pixels.resize(H * W);
    size_t index = 0;
    for (std::int32_t j = W - 1; j >= 0; j--) {
        for (std::int32_t l = 0; l < H; l++) {
            pixels[index++] = other_image.pixels[l * W + j];
        }
    }
    auto t = infoHeader.biHeight;
    infoHeader.biHeight = infoHeader.biWidth;
    infoHeader.biWidth = t;
}

void lab_bmp::image::write(std::ostream &output_file) {
    output_file.write(reinterpret_cast<char *>(&fileHeader),
                      sizeof(BitMapFileHeader));
    output_file.write(reinterpret_cast<char *>(&infoHeader),
                      sizeof(BitMapInfoHeader));
    Pixel fictitious_pixel;
    for (int i = 0; i < infoHeader.biHeight; i++) {
        output_file.write(reinterpret_cast<char *>(pixels.data()) +
                              i * 3 * infoHeader.biWidth,
                          3 * infoHeader.biWidth);
        output_file.write(reinterpret_cast<char *>(&fictitious_pixel),
                          align_bytes);
    }
}
