#include "employees.h"
int main() {
    employees::Base b;
    std::string function;
    std::string file_name;
    std::cout << "> ";
    while (std::cin >> function) {
        if (function == "load") {
            std::cin >> file_name;
            b.load(file_name);
            std::cout << "> ";
        } else if (function == "save") {
            std::string type;
            std::cin >> type;
            std::cin >> file_name;
            b.save(type, file_name);
            std::cout << "> ";
        } else {
            b.list_devs();
            std::cout << "> ";
        }
    }
}
