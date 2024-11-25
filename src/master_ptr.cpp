#include "master_ptr.h"
#include <iostream>

//check and delete if count reaches 0
template <typename T>
void master_ptr<T>::check_and_delete() {
    if (count && --(*count) == 0) {
        delete ptr;
        delete count;
    }
}

//constructor
template <typename T>
master_ptr<T>::master_ptr(T* p) : ptr(p), count(new int(1)) {
}

//copy constructor
template <typename T>
master_ptr<T>::master_ptr(const master_ptr<T>& other) : ptr(other.ptr), count(other.count) {
    (*count)++;
}

//destructor
template <typename T>
master_ptr<T>::~master_ptr() {
    check_and_delete();
}

//assignment operator
template <typename T>
master_ptr<T>& master_ptr<T>::operator=(const master_ptr<T>& other) {
    if (this != &other) {
        check_and_delete();  // Clean up current object
        ptr = other.ptr;
        count = other.count;
        (*count)++;
    }
    return *this;
}

//getter
template <typename T>
T* master_ptr<T>::get() const {
    return ptr;
}
