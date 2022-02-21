#ifndef BMP_CURL_H
#define BMP_CURL_H

#include <curl/curl.h>
#include <curl/easy.h>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace lab_bmp {
struct Curl {
private:
    CURL *curl_handle{};
    CURLcode res = CURLE_OK;

public:
    Curl();

    Curl(Curl &&other_curl) = default;

    Curl &operator=(Curl &&other_curl) = default;

    Curl(const Curl &) = delete;

    Curl &operator=(const Curl &) = delete;

    std::string download_to_buf(const std::string &url);

    ~Curl();
};
}  // namespace lab_bmp
#endif  // BMP_CURL_H
