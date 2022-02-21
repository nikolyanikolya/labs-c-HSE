#include <memory>
#include "curl.h"
#include "image.h"
namespace lab_bmp {
struct WrongInputFormat : std::runtime_error {
    WrongInputFormat() : std::runtime_error("Missing arguments") {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};

struct FileOpenException : std::runtime_error {
    explicit FileOpenException(const std::string &error)
        : std::runtime_error(error) {
    }
    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};

void open_file_reading(std::ifstream &file, const std::string &file_name) {
    try {
        file.open(file_name, std::ios_base::in | std::ios_base::binary);
    } catch (const std::ifstream::failure &) {
        std::string error = "Unable to open file \"";
        error = error + file_name + "\"";
        throw lab_bmp::FileOpenException(error);
    }
}

void open_file_writing(std::ofstream &file, const std::string &file_name) {
    try {
        file.open(file_name, std::ios_base::out | std::ios_base::binary);
    } catch (const std::ofstream::failure &) {
        std::string error = "Unable to open file \"";
        error = error + file_name + "\"";
        throw lab_bmp::FileOpenException(error);
    }
}

void check_number_arguments(int number) {
    if (number < 8) {
        throw lab_bmp::WrongInputFormat();
    }
}
}  // namespace lab_bmp

int main(int argc, char *argv[]) {
    try {
        lab_bmp::check_number_arguments(argc);

        std::string query(argv[1]);
        bool without_download = (query == "crop-rotate");
        lab_bmp::image initial_picture;
        if (without_download) {
            std::ifstream input_file;
            input_file.exceptions(std::ifstream::failbit |
                                  std::ifstream::badbit);
            lab_bmp::open_file_reading(input_file, argv[2]);
            initial_picture.read(input_file);
            input_file.close();
        } else {
            std::string url(argv[2]);
            lab_bmp::Curl curl;
            std::string buf = curl.download_to_buf(url);
            std::istringstream info(buf);
            initial_picture.read(info);
        }
        std::int32_t x = std::stoi(argv[4]);
        std::int32_t y = std::stoi(argv[5]);
        std::int32_t w = std::stoi(argv[6]);
        std::int32_t h = std::stoi(argv[7]);
        lab_bmp::image picture_aft_crop;
        picture_aft_crop.crop(initial_picture, x, y, w, h);
        lab_bmp::image picture_aft_rotate;
        picture_aft_rotate.rotate_clockwise(picture_aft_crop);
        std::ofstream output_file;
        output_file.exceptions(std::ofstream::failbit | std::ofstream::badbit);
        lab_bmp::open_file_writing(output_file, argv[3]);
        picture_aft_rotate.write(output_file);
        output_file.close();
    } catch (const std::bad_alloc &) {
        std::cerr << "Insufficient memory\n";
        return 1;
    } catch (const std::runtime_error &error) {
        std::cerr << error.what() << "\n";
        return 1;
    }
    return 0;
}