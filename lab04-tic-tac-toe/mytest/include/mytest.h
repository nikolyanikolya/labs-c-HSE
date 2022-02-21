#ifndef MYTEST_H_
#define MYTEST_H_

#include <functional>
#include <string>
#include <vector>

namespace mytest {
class test_to_run {
private:
    std::string name;
    std::function<void()> body;

public:
    test_to_run(std::string name_, std::function<void(void)> body);

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] std::function<void()> get_body() const;
};

std::vector<test_to_run> &get_vec();

void run_tests();

void do_check(bool expression,
              const std::string &exp,
              std::string const &file,
              int line);

void do_check_2(bool expression,
                const std::string &my_message,
                const std::string &exp,
                std::string const &file,
                int line);

int &get_counter();

bool &is_some_checks_failed();

int &get_successful_counter();
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CONCAT_2_IMPL(a, b) a##b
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CONCAT_2(a, b) CONCAT_2_IMPL(a, b)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE(class_name, name)                        \
    static void class_name();                                              \
    static mytest::test_to_run CONCAT_2(test, __LINE__)(name, class_name); \
    static void class_name()
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE2(line, name) \
    MYTEST_INTERNAL_TEST_CASE(mytest_test_class_##line, name)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define MYTEST_INTERNAL_TEST_CASE1(line, name) \
    MYTEST_INTERNAL_TEST_CASE2(line, name)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define TEST_CASE(name) MYTEST_INTERNAL_TEST_CASE1(__LINE__, name)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK(expr) mytest::do_check(expr, #expr, __FILE__, __LINE__)
// NOLINTNEXTLINE: cppcoreguidelines-macro-usage
#define CHECK_MESSAGE(expr, message) \
    mytest::do_check_2(expr, message, #expr, __FILE__, __LINE__)

}  // namespace mytest

#endif  // MYTEST_H_
