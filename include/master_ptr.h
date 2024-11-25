#pragma once
#include "Action.h"


template <typename T>
class master_ptr {
private:
    T* ptr;
    int* count;

    void check_and_delete() {
        if (count && --(*count) == 0) {
            delete ptr;
            delete count;
        }
    }

public:
    explicit master_ptr(T* p = nullptr) : ptr(p), count(p ? new int(1) : nullptr) {}
    
    master_ptr(const master_ptr<T>& other) : ptr(other.ptr), count(other.count) {
        if (count) {
            (*count)++;
        }
    }
    
    ~master_ptr() {
        check_and_delete();
    }
    
    master_ptr<T>& operator=(const master_ptr<T>& other) {
        if (this != &other) {
            check_and_delete();
            ptr = other.ptr;
            count = other.count;
            if (count) {
                (*count)++;
            }
        }
        return *this;
    }
    
    T* get() const {
        return ptr;
    }
};
