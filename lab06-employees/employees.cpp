#include "employees.h"
#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

const std::string d = "Developer";
const std::string ld = "LeadDeveloper";
const std::string m = "Manager";
namespace employees {
void Base::load(const std::string &file_name) {
    data.clear();
    std::ifstream fin;
    fin.open(file_name);
    while (auto e = employees::Employee::read_from(fin)) {
        data.push_back(std::move(e));
    }
    fin.close();
}

void Base::save(const std::string &type, const std::string &file_name) {
    std::ofstream f_out;
    f_out.open(file_name);
    for (auto &employee : data) {
        if (type == "all") {
            f_out << *employee << "\n";
        } else if (type == "developers") {
            if (dynamic_cast<Developer *>(employee.get()) != nullptr) {
                f_out << *employee << "\n";
            }
        } else if (type == "managers") {
            if (dynamic_cast<Manager *>(employee.get()) != nullptr) {
                f_out << *employee << "\n";
            }
        } else if (type == "leads") {
            if (dynamic_cast<LeadDeveloper *>(employee.get()) != nullptr) {
                f_out << *employee << "\n";
            }
        }
    }
    f_out.close();
}

void Base::list_devs() {
    std::vector<Developer *> developers;
    for (const auto &employee : data) {
        auto *intermediate = dynamic_cast<Developer *>(employee.get());
        if (intermediate != nullptr) {
            developers.push_back(intermediate);
        }
    }
    std::cout << developers.size() << " developer(s) found:\n";
    int counter = 1;
    for (const auto &employee : developers) {
        std::cout << counter++ << ". " << employee->full_name << " aka @"
                  << employee->github_account << "\n";
    }
}

Employee::Employee() {
    assert(0);
}

Employee::Employee(std::istream &is) {
    is >> full_name >> base_salary_usd_per_year;
    std::replace(full_name.begin(), full_name.end(), '~', ' ');
}

std::unique_ptr<Employee> Employee::read_from(std::istream &in) {
    std::string profession;
    if (!(in >> profession)) {
        return nullptr;
    }
    if (profession == d) {
        return Developer::read_from(in);
    } else if (profession == m) {
        return Manager::read_from(in);
    } else if (profession == ld) {
        return LeadDeveloper::read_from(in);
    }
    return nullptr;
}

void Employee::printTo(std::ostream &os) const {
    std::string s = full_name;
    std::replace(s.begin(), s.end(), ' ', '~');
    os << s << " " << base_salary_usd_per_year << " ";
}

Employee::Employee(std::string full_name_,
                   std::size_t base_salary_usd_per_year_)
    : full_name(std::move(full_name_)),
      base_salary_usd_per_year(base_salary_usd_per_year_) {
}

std::ostream &operator<<(std::ostream &os, const Employee &employee) {
    employee.printTo(os);
    return os;
}

Developer::Developer(std::istream &is) : Employee(is) {
    is >> github_account;
}

Developer::Developer(std::string full_name_,
                     std::size_t base_salary_usd_per_year_,
                     std::string github_account_)
    : Employee(std::move(full_name_), base_salary_usd_per_year_),
      github_account(std::move(github_account_)) {
}

Developer::Developer(std::string github_account_)
    : Employee(), github_account(std::move(github_account_)) {
}

std::size_t Developer::salary_usd_per_year() const {
    return base_salary_usd_per_year + github_account.size() * 100;
}

void Developer::stream_writing(std::ostream &os) const {
    Employee::printTo(os);
    os << github_account;
}

void Developer::printTo(std::ostream &os) const {
    os << d << " ";
    stream_writing(os);
}

std::unique_ptr<Developer> Developer::read_from(std::istream &in) {
    return std::make_unique<Developer>(in);
}

Manager::Manager(std::istream &is) : Employee(is) {
    is >> project_name;
}

Manager::Manager(std::string full_name_,
                 std::size_t base_salary_usd_per_year_,
                 std::string project_name_)
    : Employee(std::move(full_name_), base_salary_usd_per_year_),
      project_name(std::move(project_name_)) {
}

Manager::Manager(std::string project_name_)
    : Employee(), project_name(std::move(project_name_)) {
}

void Manager::stream_writing(std::ostream &os) const {
    Employee::printTo(os);
    os << project_name;
}

void Manager::printTo(std::ostream &os) const {
    os << m << " ";
    stream_writing(os);
}

[[nodiscard]] std::size_t Manager::salary_usd_per_year() const {
    return base_salary_usd_per_year + project_name.size() * 100;
}

std::unique_ptr<Manager> Manager::read_from(std::istream &in) {
    return std::make_unique<Manager>(in);
}

LeadDeveloper::LeadDeveloper(std::istream &is)
    : Employee(is), Developer(is), Manager(is) {
}

LeadDeveloper::LeadDeveloper(const std::string &full_name_,
                             std::size_t base_salary_usd_per_year_,
                             std::string github_account_,
                             std::string project_name_)
    : Employee(full_name_, base_salary_usd_per_year_),
      Developer(std::move(github_account_)),
      Manager(std::move(project_name_)) {
}

[[nodiscard]] std::size_t LeadDeveloper::salary_usd_per_year() const {
    return base_salary_usd_per_year + github_account.size() * 150 +
           project_name.size() * 80;
}

std::unique_ptr<LeadDeveloper> LeadDeveloper::read_from(std::istream &in) {
    return std::make_unique<LeadDeveloper>(in);
}

void LeadDeveloper::stream_writing(std::ostream &os) const {
    Developer::stream_writing(os);
    os << " " << project_name;
}

void LeadDeveloper::printTo(std::ostream &os) const {
    os << ld << " ";
    stream_writing(os);
}
}  // namespace employees