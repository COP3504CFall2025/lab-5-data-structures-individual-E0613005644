#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"

// Technically bad, but size_t isn't likely to conflict with any client code.
using std::size_t;

template<typename T>
class ABS : public StackInterface<T> {
public:
    // Big 5 + Parameterized Constructor
    ABS() :
        capacity_(1), curr_size_(0), array_(new T[capacity_]) {}
    explicit ABS(const size_t capacity) :
        capacity_(capacity), curr_size_(0), array_(new T[capacity_]) {}
    ABS(const ABS& other){
        this->array_ = new T[other.capacity_];
        this->capacity_ = other.capacity_;
        this->curr_size_ = other.curr_size_;
        for(size_t i = 0; i < other.curr_size_; i++){
            array_[i] = other.array_[i];
        }
    }
    ABS& operator=(const ABS& rhs){
        if(this==&rhs){
            return *this;
        }
        delete[] this->array_;
        this->array_ = new T[rhs.capacity_];
        this->capacity_ = rhs.capacity_;
        this->curr_size_ = rhs.curr_size_;
        for(size_t i=0;i<rhs.curr_size_;i++){
            array_[i] = rhs.array_[i];
        }
        return *this;
    }
    ABS(ABS&& other) noexcept{
        this->array_ = other.array_;
        this->capacity_ = other.capacity_;
        this->curr_size_ = other.curr_size_;
        other.array_ = nullptr;
        other.capacity_ = 0;
        other.curr_size_ = 0;
    }
    ABS& operator=(ABS&& rhs) noexcept{
        if(this==&rhs){
            return *this;
        }
        delete[] this->array_;
        this->array_ = rhs.array_;
        this->capacity_ = rhs.capacity_;
        this->curr_size_ = rhs.curr_size_;
        rhs.array_ = nullptr;
        rhs.capacity_ = 0;
        rhs.curr_size_ = 0;
        return *this;
    }
    ~ABS() noexcept override{
        delete[] array_;
        array_ = nullptr;
    }

    // Get the number of items in the ABS
    [[nodiscard]] size_t getSize() const noexcept override{
        return curr_size_;
    }

    // Get the max size of the ABS
    [[nodiscard]] size_t getMaxCapacity() const noexcept{
        return capacity_;
    }

    // Return underlying data for the stack
    [[nodiscard]] T* getData() const noexcept{
        return array_;
    }

    // Push item onto the stack
    void push(const T& data) override{
        if(curr_size_ == capacity_) {
            size_t new_capacity_ = capacity_ * scale_factor_;
            T* new_Array = new T[new_capacity_];
            for(size_t i=0; i<curr_size_;i++){
                new_Array[i] = array_[i];
            }
            delete[] array_;
            array_ = new_Array;
            capacity_ = new_capacity_;
        }
        array_[curr_size_] = data;
        curr_size_++;
    }

    T peek() const override{
        if(curr_size_==0){
            throw std::runtime_error("Empty ABS");
        }else{
            return array_[curr_size_-1];
        }
    }

    T pop() override{
        if(curr_size_ == 0) {
            throw std::runtime_error("Empty ABS");
        }else{
			T value = array_[curr_size_-1];
            curr_size_--;
			if(curr_size_*2 <= capacity_ && capacity_>2){ //if my current number of elements is half or less of my capacity, shrink and capacity>2 ensures that you don't shrink below 2
				size_t new_capacity_ = capacity_ /scale_factor_;
				T* new_array_ = new T[new_capacity_];
				for(size_t i=0; i<curr_size_;i++){
					new_array_[i] = array_[i];
				}
			delete[] array_;
			array_ = new_array_;
			capacity_ = new_capacity_;
			}
            return array_[value]; //for my sanity, this returns the element that was just removed
        }
    }

private:
    size_t capacity_;
    size_t curr_size_;
    T* array_;
    static constexpr size_t scale_factor_ = 2;
};
