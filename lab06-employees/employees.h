#ifndef EMPLOYEE_H_
#define EMPLOYEE_H_

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace employees {
struct Employee;

struct Base {
    std::vector<std::unique_ptr<Employee>> data;

    void load(const std::string &file_name);

    void save(const std::string &type, const std::string &file_name);

    void list_devs();
};

struct Employee {
    Employee();

    Employee &operator=(const Employee &) = delete;

    Employee(const Employee &) = delete;

    Employee &operator=(Employee &&) noexcept = delete;

    Employee(Employee &&) noexcept = delete;

    Employee(std::istream &is);

    std::string full_name;
    std::size_t base_salary_usd_per_year = 0;

    virtual ~Employee() = default;

    virtual void printTo(std::ostream &os) const;

    virtual void stream_writing(std::ostream &os) const = 0;

    friend std::ostream &operator<<(std::ostream &os, const Employee &e);

    [[nodiscard]] virtual std::size_t salary_usd_per_year() const = 0;

    static std::unique_ptr<employees::Employee> read_from(std::istream &in);

protected:
    explicit Employee(std::string full_name_,
                      std::size_t base_salary_usd_per_year_);
};

struct Developer : virtual Employee {
    Developer &operator=(const Developer &) = delete;

    Developer(const Developer &) = delete;

    Developer &operator=(Developer &&) noexcept = delete;

    Developer(Developer &&) noexcept = delete;

    std::string github_account;

    void printTo(std::ostream &os) const override;

    void stream_writing(std::ostream &os) const override;

    [[nodiscard]] std::size_t salary_usd_per_year() const override;

    explicit Developer(std::istream &is);

    explicit Developer(std::string full_name_,
                       std::size_t base_salary_usd_per_year_,
                       std::string github_account_);

    explicit Developer(std::string github_account_);

    static std::unique_ptr<Developer> read_from(std::istream &in);

    ~Developer() override = default;
};

struct Manager : virtual Employee {
    Manager &operator=(const Manager &) = delete;

    Manager(const Manager &) = delete;

    Manager &operator=(Manager &&) noexcept = delete;

    Manager(Manager &&) noexcept = delete;

    std::string project_name;

    explicit Manager(std::istream &is);

    explicit Manager(std::string full_name_,
                     std::size_t base_salary_usd_per_year_,
                     std::string project_name_);

    explicit Manager(std::string project_name_);

    void printTo(std::ostream &os) const override;

    void stream_writing(std::ostream &os) const override;

    static std::unique_ptr<Manager> read_from(std::istream &in);

    [[nodiscard]] std::size_t salary_usd_per_year() const override;

    ~Manager() override = default;
};

struct LeadDeveloper final : Developer, Manager {
    LeadDeveloper &operator=(const LeadDeveloper &) = delete;

    LeadDeveloper(const LeadDeveloper &) = delete;

    LeadDeveloper &operator=(LeadDeveloper &&) noexcept = delete;

    LeadDeveloper(LeadDeveloper &&) noexcept = delete;

    explicit LeadDeveloper(std::istream &is);

    explicit LeadDeveloper(const std::string &full_name_,
                           std::size_t base_salary_usd_per_year_,
                           std::string github_account,
                           std::string project_name_);

    void printTo(std::ostream &os) const final;

    void stream_writing(std::ostream &os) const override;

    static std::unique_ptr<LeadDeveloper> read_from(std::istream &in);

    [[nodiscard]] std::size_t salary_usd_per_year() const final;

    ~LeadDeveloper() final = default;
};

}  // namespace employees

#endif  // EMPLOYEE_H_