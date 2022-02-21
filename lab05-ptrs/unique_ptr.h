#ifndef PTRS_UNIQUE_PTR_H
#define PTRS_UNIQUE_PTR_H

#include <algorithm>
#include <type_traits>

namespace lab_05::unique {
template <typename Y>
struct default_deleter {
    void operator()(Y *ptr) {
        delete ptr;
    }
};

template <typename Y>
struct default_deleter<Y[]> {
    void operator()(Y *ptr) {
        delete[] ptr;
    }
};

template <typename Y, typename Deleter = default_deleter<Y>>
struct unique_ptr {
private:
    using T = typename std::remove_extent<Y>::type;
    T *ptr;
    Deleter deleter;

public:
    unique_ptr() : ptr(nullptr) {
    }

    explicit unique_ptr(T *ptr_, Deleter deleter_ = Deleter())
        : ptr(ptr_), deleter(std::move(deleter_)) {
    }

    unique_ptr(const unique_ptr &other) = delete;

    explicit unique_ptr(unique_ptr &&other)
        : ptr(std::exchange(other.ptr, nullptr)) {
    }

    unique_ptr &operator=(const unique_ptr &other) = delete;

    unique_ptr &operator=(
        unique_ptr &&other) {  // cppcheck-suppress operatorEqToSelf
        if (this != &other) {
            delete ptr;
            ptr = other.ptr;
            other.ptr = nullptr;
        }
        return *this;
    }

    ~unique_ptr() {
        deleter(ptr);
    }

    explicit operator bool() const {
        return (ptr);
    }

    T *get() const {
        return ptr;
    }

    T &operator*() const {
        return *ptr;
    }

    T *operator->() const {
        return ptr;
    }

    T &operator[](std::size_t index) const {
        return *(ptr + index);
    }

    void reset() {
        delete ptr;
        ptr = nullptr;
    }

    void reset(T *ptr_) {
        delete ptr;
        ptr = ptr_;
    }

    T *release() {
        T *ptr_ = ptr;
        ptr = nullptr;
        return ptr_;
    }

    bool operator==(const unique_ptr<T, Deleter> &other) const {
        return (other.ptr == ptr);
    }

    bool operator!=(const unique_ptr<T, Deleter> &other) const {
        return other.ptr != ptr;
    }
};

template <typename T, typename Deleter = default_deleter<T>>
void swap(unique_ptr<T, Deleter> &ptr_1, unique_ptr<T, Deleter> &ptr_2) {
    unique_ptr<T, Deleter> t(std::move(ptr_2));
    ptr_2 = std::move(ptr_1);
    ptr_1 = std::move(t);
}

}  // namespace lab_05::unique
#endif  // PTRS_UNIQUE_PTR_H
