#include "mytest.h"
#include <functional>
#include <iostream>
#include <string>

namespace mytest {

test_to_run::test_to_run(std::string name_, std::function<void()> body)
    : name(std::move(name_)), body(std::move(body)) {
    get_vec().push_back(*this);
}

std::string test_to_run::get_name() const {
    return name;
}

std::function<void()> test_to_run::get_body() const {
    return body;
}

std::vector<test_to_run> &get_vec() {
    static std::vector<test_to_run> tests;
    return tests;
}

int &get_counter() {
    static int test_counter;
    return test_counter;
}

int &get_successful_counter() {
    static int test_successful_counter;
    return test_successful_counter;
}

bool &is_some_checks_failed() {
    static bool check_failed;
    return check_failed;
}

void run_tests() {
    for (auto const &test : get_vec()) {
        std::cerr << "Running"
                  << " "
                  << "\"" << test.get_name() << "\""
                  << "..." << std::endl;
        is_some_checks_failed() = false;
        get_counter()++;
        test.get_body()();
        if (!is_some_checks_failed()) {
            get_successful_counter()++;
        }
    }
    std::cerr << "===== Tests passed:"
              << " " << get_successful_counter() << "/" << get_counter()
              << " =====" << std::endl;
}

void do_check_2(bool expression,
                const std::string &my_message,
                const std::string &exp,
                std::string const &file,
                int line) {
    if (!expression) {
        std::cerr << "CHECK"
                  << "(" << exp << ")"
                  << " "
                  << "at"
                  << " " << file << ":" << line << " "
                  << "failed!" << std::endl
                  << " "
                  << " "
                  << " "
                  << " "
                  << "message"
                  << ":"
                  << " " << my_message << std::endl;
        is_some_checks_failed() = true;
    }
}

void do_check(bool expression,
              const std::string &exp,
              std::string const &file,
              int line) {
    if (!expression) {
        std::cerr << "CHECK"
                  << "(" << exp << ")"
                  << " "
                  << "at"
                  << " " << file << ":" << line << " "
                  << "failed!" << std::endl;
        is_some_checks_failed() = true;
    }
}

namespace {}
}  // namespace mytest

// Externally visible declarations and definitions.
// namespace mytest
