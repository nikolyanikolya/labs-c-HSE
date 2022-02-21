#include "employees.h"
#include <sstream>
#include <string>
#include <type_traits>
#include "doctest.h"

std::string to_string(const employees::Employee &e) {
    std::stringstream s;
    static_cast<std::ostream &>(s) << e;
    return s.str();
}

std::unique_ptr<employees::Employee> from_string(const std::string &str) {
    std::stringstream s;
    s << str;
    return employees::Employee::read_from(static_cast<std::istream &>(s));
}

TEST_CASE("read_from empty") {
    CHECK(from_string("") == nullptr);
    CHECK(from_string("    ") == nullptr);
}

TEST_CASE("Developer") {
    const std::string serialized = "Developer Ivan~Ivanov 100000 ivanivanov";

    auto check_employee = [&](const employees::Employee &e) {
        CHECK(e.full_name == "Ivan Ivanov");
        CHECK(e.base_salary_usd_per_year == 100'000);
        CHECK(e.salary_usd_per_year() == 101'000);
        CHECK(to_string(e) == serialized);
    };

    auto check_developer = [&](const employees::Developer &d) {
        check_employee(d);
        CHECK(d.salary_usd_per_year() == 101'000);
        CHECK(to_string(d) == serialized);

        CHECK(d.github_account == "ivanivanov");
    };

    SUBCASE("Getters") {
        employees::Developer d("Ivan Ivanov", 100'000, "ivanivanov");
        check_developer(d);
    }

    SUBCASE("Read") {
        auto e = from_string(serialized);
        check_employee(*e);

        auto *dptr = dynamic_cast<employees::Developer *>(e.get());
        REQUIRE(dptr);
        check_developer(*dptr);
    }
}

TEST_CASE("Manager") {
    const std::string serialized = "Manager Ivan~Ivanov 100000 proj";

    auto check_employee = [&](const employees::Employee &e) {
        CHECK(e.full_name == "Ivan Ivanov");
        CHECK(e.base_salary_usd_per_year == 100'000);
        CHECK(e.salary_usd_per_year() == 100'400);
        CHECK(to_string(e) == serialized);
    };

    auto check_manager = [&](const employees::Manager &m) {
        check_employee(m);
        CHECK(m.salary_usd_per_year() == 100'400);
        CHECK(to_string(m) == serialized);

        CHECK(m.project_name == "proj");
    };

    SUBCASE("Getters") {
        employees::Manager m("Ivan Ivanov", 100'000, "proj");
        check_manager(m);
    }

    SUBCASE("Read") {
        auto e = from_string(serialized);
        check_employee(*e);

        auto *mptr = dynamic_cast<employees::Manager *>(e.get());
        REQUIRE(mptr);
        check_manager(*mptr);
    }
}

TEST_CASE("LeadDeveloper") {
    const std::string serialized =
        "LeadDeveloper Ivan~Ivanov 100000 ivanivanov proj";

    auto check_employee = [&](const employees::Employee &e) {
        CHECK(e.full_name == "Ivan Ivanov");
        CHECK(e.base_salary_usd_per_year == 100'000);
        CHECK(e.salary_usd_per_year() == 101'820);
        CHECK(to_string(e) == serialized);
    };

    auto check_developer = [&](const employees::Developer &d) {
        check_employee(d);
        CHECK(d.salary_usd_per_year() == 101'820);
        CHECK(to_string(d) == serialized);

        CHECK(d.github_account == "ivanivanov");
    };

    auto check_manager = [&](const employees::Manager &m) {
        check_employee(m);
        CHECK(m.salary_usd_per_year() == 101'820);
        CHECK(to_string(m) == serialized);

        CHECK(m.project_name == "proj");
    };

    auto check_lead_developer = [&](const employees::LeadDeveloper &ld) {
        check_employee(ld);
        check_developer(ld);
        check_manager(ld);
        CHECK(ld.salary_usd_per_year() == 101'820);
        CHECK(to_string(ld) == serialized);
    };

    SUBCASE("Getters") {
        employees::LeadDeveloper ld("Ivan Ivanov", 100'000, "ivanivanov",
                                    "proj");
        check_lead_developer(ld);
    }

    SUBCASE("Read") {
        auto e = from_string(serialized);
        check_employee(*e);

        auto *dptr = dynamic_cast<employees::Developer *>(e.get());
        REQUIRE(dptr);
        check_developer(*dptr);

        auto *mptr = dynamic_cast<employees::Manager *>(e.get());
        REQUIRE(mptr);
        check_manager(*mptr);

        auto *ldptr = dynamic_cast<employees::LeadDeveloper *>(e.get());
        REQUIRE(ldptr);
        check_lead_developer(*ldptr);
    }
}

TEST_CASE("Employee is abstract") {
    CHECK(std::is_abstract_v<employees::Employee>);
}

TEST_CASE("No default constructors") {
    CHECK(!std::is_default_constructible_v<employees::Employee>);
    CHECK(!std::is_default_constructible_v<employees::Developer>);
    CHECK(!std::is_default_constructible_v<employees::Manager>);
    CHECK(!std::is_default_constructible_v<employees::LeadDeveloper>);
}

TEST_CASE("No copies or moves") {
    CHECK(!std::is_copy_constructible_v<employees::Developer>);
    CHECK(!std::is_copy_constructible_v<employees::Manager>);
    CHECK(!std::is_copy_constructible_v<employees::Developer>);
    CHECK(!std::is_copy_constructible_v<employees::LeadDeveloper>);
    CHECK(!std::is_copy_assignable_v<employees::Developer>);
    CHECK(!std::is_copy_assignable_v<employees::Manager>);
    CHECK(!std::is_copy_assignable_v<employees::Developer>);
    CHECK(!std::is_copy_assignable_v<employees::LeadDeveloper>);

    CHECK(!std::is_move_constructible_v<employees::Developer>);
    CHECK(!std::is_move_constructible_v<employees::Manager>);
    CHECK(!std::is_move_constructible_v<employees::Developer>);
    CHECK(!std::is_move_constructible_v<employees::LeadDeveloper>);
    CHECK(!std::is_move_assignable_v<employees::Developer>);
    CHECK(!std::is_move_assignable_v<employees::Manager>);
    CHECK(!std::is_move_assignable_v<employees::Developer>);
    CHECK(!std::is_move_assignable_v<employees::LeadDeveloper>);
}
