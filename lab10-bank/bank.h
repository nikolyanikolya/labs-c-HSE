#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#ifndef BANK_H
#define BANK_H
namespace bank {
struct transaction;
struct user;

struct user_transactions_iterator {
private:
    size_t counter = 1;
    const user *const user_;

public:
    explicit user_transactions_iterator(const user *user_);

    user_transactions_iterator(size_t counter_, const user *user_);

    [[nodiscard]] bank::transaction wait_next_transaction();
};

struct transfer_error : std::runtime_error {
    explicit transfer_error(const std::string &s);
};

struct not_enough_funds_error : transfer_error {
    explicit not_enough_funds_error(int a, int b);
};

struct user {
private:
    mutable std::mutex m;
    const std::string id;
    int balance = 100;
    std::vector<transaction> history;
    mutable std::condition_variable cond_var;
    size_t counter = 1;

public:
    explicit user(std::string s);

    user &operator=(const user &user_) = delete;

    user &operator=(user &&user_) = delete;

    user(const user &user_) = delete;

    user(user &&user_) = delete;

    ~user() = default;

    user_transactions_iterator monitor() const;

    user_transactions_iterator snapshot_transactions(
        const std::function<void(const std::vector<transaction> &, int)>
            &my_func) const;

    [[nodiscard]] std::string name() const noexcept;

    [[nodiscard]] int balance_xts() const;

    void transfer(user &user_, int amount, const std::string &comment_);

    friend user_transactions_iterator;
};

struct ledger {
private:
    mutable std::mutex m;

    std::map<std::string, std::unique_ptr<user>> is_user_exists;

public:
    ledger() = default;

    user &get_or_create_user(const std::string &s);
};

struct transaction {
    const user *const counterparty;  // NOLINT
    const int balance_delta_xts;     // NOLINT
    const std::string comment;       // NOLINT

    transaction() = delete;

    transaction(const user *user_, int balance_delta, std::string comment_);
};

}  // namespace bank
#endif