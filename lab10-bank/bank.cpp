#include "bank.h"

std::string bank::user::name() const noexcept {
    return id;
}

int bank::user::balance_xts() const {
    return balance;
}

bank::user::user(std::string s) : id(std::move(s)) {
    history.emplace_back(nullptr, balance, "Initial deposit for " + id);
}

bank::user &bank::ledger::get_or_create_user(const std::string &s) {
    std::unique_lock l(m);
    if (is_user_exists.find(s) == is_user_exists.end()) {
        is_user_exists.insert(
            std::map<std::string, std::unique_ptr<user>>::value_type(
                s, std::make_unique<user>(s)));
    }
    return *is_user_exists[s];
}

bank::user_transactions_iterator bank::user::monitor() const {
    // std::unique_lock l(m);
    user_transactions_iterator it(this);
    return it;
}

bank::transaction bank::user_transactions_iterator::wait_next_transaction() {
    std::unique_lock l(user_->m);

    user_->cond_var.wait(l, [&]() { return counter != user_->history.size(); });
    return user_->history[counter++];
}

bank::user_transactions_iterator bank::user::snapshot_transactions(
    const std::function<void(const std::vector<transaction> &, int)> &my_func)
    const {
    std::unique_lock l(m);
    my_func(history, balance);
    return {counter, this};
}

bank::user_transactions_iterator::user_transactions_iterator(const user *user_)
    : user_(user_) {
}

bank::user_transactions_iterator::user_transactions_iterator(size_t counter_,
                                                             const user *user_)
    : counter(counter_), user_(user_) {
}

bank::transaction::transaction(const user *user_,
                               int balance_delta,
                               std::string comment_)
    : counterparty(user_),
      balance_delta_xts(balance_delta),
      comment(std::move(comment_)) {
}

void bank::user::transfer(user &user_,
                          int amount,
                          const std::string &comment_) {
    std::scoped_lock l(m, user_.m);
    if (amount > balance) {
        throw not_enough_funds_error(balance, amount);
    }
    counter++;
    user_.counter++;
    user_.balance += amount;
    user_.history.emplace_back(this, amount, comment_);
    history.emplace_back(&user_, -amount, comment_);
    balance -= amount;
    cond_var.notify_all();
    user_.cond_var.notify_all();
}

bank::transfer_error::transfer_error(const std::string &s)
    : std::runtime_error(s) {
}

bank::not_enough_funds_error::not_enough_funds_error(int a, int b)
    : transfer_error("Not enough funds: " + std::to_string(a) +
                     " XTS available, " + std::to_string(b) +
                     " XTS requested") {
}