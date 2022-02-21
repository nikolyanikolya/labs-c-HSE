#ifndef PTRS_SHARED_PTR_H
#define PTRS_SHARED_PTR_H

#include <algorithm>
#include <type_traits>

namespace lab_05::shared {
template <typename Y>
struct shared_ptr {
private:
    using T = typename std::remove_extent<Y>::type;
    T *ptr = nullptr;
    int *counter = new int(0);

public:
    explicit shared_ptr() = default;

    explicit shared_ptr(T *ptr_) : ptr(ptr_) {
        (ptr_ != nullptr) ? (*counter) = 1 : (*counter) = 0;
    }

    explicit shared_ptr(const shared_ptr &other) {
        delete ptr;
        ptr = other.ptr;
        delete counter;
        (*other.counter)++;
        counter = other.counter;
    }

    explicit shared_ptr(shared_ptr &&other) {
        delete ptr;
        delete counter;
        ptr = std::exchange(other.ptr, nullptr);
        counter = std::exchange(other.counter, nullptr);
    }

    void clear() {
        if (counter == nullptr || (*counter) <= 1) {
            delete ptr;
            delete counter;
        } else {
            (*counter)--;
        }
    }

    ~shared_ptr() {
        clear();
    }

    shared_ptr &operator=(const shared_ptr &other) {
        if (&other != this) {
            delete ptr;
            delete counter;
            ptr = other.ptr;
            counter = other.counter;
            (*counter)++;
        }
        return *this;
    }

    shared_ptr &operator=(shared_ptr &&other) {
        if (&other != this) {
            clear();
            ptr = std::exchange(other.ptr, nullptr);
            counter = std::exchange(other.counter, nullptr);
        }
        return *this;
    }

    explicit operator bool() const {
        return ptr != nullptr;
    }

    T *get() {
        return ptr;
    }

    const T *get() const {
        return ptr;
    }

    T &operator*() const {
        return *ptr;
    }

    T *operator->() const {
        return ptr;
    }

    bool operator==(const shared_ptr<T> &other) const {
        return (other.ptr == ptr);
    }

    bool operator!=(const shared_ptr<T> &other) const {
        return other.ptr != ptr;
    }

    template <typename T>
    friend void swap(shared_ptr<T> &ptr_1, shared_ptr<T> &ptr_2);
};

template <typename T>
void swap(shared_ptr<T> &ptr_1, shared_ptr<T> &ptr_2) {
    if (ptr_1 != ptr_2) {
        ptr_1.ptr = std::exchange(ptr_2.ptr, ptr_1.ptr);
        ptr_1.counter = std::exchange(ptr_2.counter, ptr_1.counter);
    }
}

}  // namespace lab_05::shared
#endif  // PTRS_SHARED_PTR_H
