#include "mytest.h"

int main() {
    mytest::run_tests();
    if (mytest::get_counter() == mytest::get_successful_counter()) {
        return 0;
    }
    return 1;
}
