#include "curl.h"

namespace lab_bmp {
struct WrongServer : std::runtime_error {
    explicit WrongServer(const std::string &error) : std::runtime_error(error) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};

struct CurlErrorQuery : std::runtime_error {
    explicit CurlErrorQuery(const std::string &error)
        : std::runtime_error(error) {
    }

    [[nodiscard]] const char *what() const noexcept override {
        return runtime_error::what();
    }
};
}  // namespace lab_bmp
static bool any_exceptions = false;

static size_t write_data(char *ptr,
                         size_t size,
                         size_t nmemb,
                         std::string *stream) noexcept {
    size_t written = size * nmemb;
    try {
        (stream)->append(ptr, size * nmemb);
    } catch (const std::bad_alloc &) {
        any_exceptions = true;
        written = 0;
    }
    return written;
}

lab_bmp::Curl::Curl() {
    curl_global_init(CURL_GLOBAL_ALL);
    curl_handle = curl_easy_init();
}

std::string lab_bmp::Curl::download_to_buf(const std::string &url) {
    std::string buf;
    curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, &buf);

    res = curl_easy_perform(curl_handle);
    if (any_exceptions) {
        throw std::bad_alloc();
    }
    if (res != CURLE_OK) {
        std::string error = "libcurl error: ";
        error += curl_easy_strerror(res);
        throw lab_bmp::CurlErrorQuery(error);
    }
    long response_code = 0;
    curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &response_code);
    if (response_code != 200) {
        std::string error = "Server responded ";
        error += std::to_string(response_code);
        if (response_code == 404) {
            error += ": page not found";
        }
        throw lab_bmp::WrongServer(error);
    }
    return buf;
}

lab_bmp::Curl::~Curl() {
    curl_easy_cleanup(curl_handle);
    curl_global_cleanup();
}