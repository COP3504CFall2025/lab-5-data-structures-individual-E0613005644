#pragma once

#include <cstddef>
#include <stdexcept>
#include "Interfaces.hpp"
#include <utility>

template <typename T>
class ABDQ : public DequeInterface<T> {
private:
    T* data_;                 // underlying dynamic array
    std::size_t capacity_;    // total allocated capacity
    std::size_t size_;        // number of stored elements
    std::size_t front_;       // index of front element
    std::size_t back_;        // index after the last element (circular)

    static constexpr std::size_t SCALE_FACTOR = 2;

public:
    // Big 5
    ABDQ() : capacity_(4), size_(0), front_(0), back_(0), data_(new T[capacity_]) {}
    explicit ABDQ(std::size_t capacity) : capacity_(capacity), size_(0), front_(0), back_(0), data_(new T[capacity_]) {}
    ABDQ(const ABDQ& other){
        this->data_ = new T[other.capacity_];
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->front_ = 0; //making index 0 the first element
        this->back_ = other.size_;
        for(size_t i=0; i < other.size_; ++i){
            data_[i] = other.data_[(other.front_+i)%other.capacity_];
        }
    }
    ABDQ(ABDQ&& other) noexcept{
        this->data_ = other.data_;
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->front_ = other.front_;
        this->back_ = other.back_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        other.front_ = 0;
        other.back_ = 0;
    }
    ABDQ& operator=(const ABDQ& other){
        if(this==&other){
            return *this;
        }
        delete[] data_;
        this->data_ = new T[other.capacity_];
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->front_ = 0; //trying to make the array start where the first element is at index 0
        this->back_ = other.size_;
        for(size_t i=0; i < other.size_; ++i){
            data_[i] = other.data_[(other.front_+i)%other.capacity_];
        }
        return *this;
    }
    ABDQ& operator=(ABDQ&& other) noexcept{
        if(this==&other){
            return *this;
        }
        delete[] data_;
        this->data_ = other.data_;
        this->capacity_ = other.capacity_;
        this->size_ = other.size_;
        this->front_ = other.front_;
        this->back_ = other.back_;
        other.data_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        other.front_ = 0;
        other.back_ = 0;
        return *this;
    }
    ~ABDQ() override{
        delete[] data_;
        data_ = nullptr;
    }

    // Insertion
    //imagine a clock, and when you push front you move ccw
    void pushFront(const T& item) override{
        if(size_ == capacity_){
            size_t new_capacity_ = capacity_ * SCALE_FACTOR;
            T* new_data_ = new T[new_capacity_];
            for(size_t i=0;i<size_;i++){
                new_data_[i] = data_[(front_+i)%capacity_]; //starts from the actual front and then adds the i index, but %capacity makes it wrap around when you hit the end of data_
            }
            delete[] data_;
            data_ = new_data_;
            capacity_ = new_capacity_;
            front_ = 0;
            back_ = size_;
        }
        if(front_ == 0) {
            front_ = capacity_-1; //we have to insert one behind the front, that's actually the front of the list or if you use a circle the last element is right behind it
        }else{
            front_ = front_ -1;
        }
        data_[front_] = item;
        size_++;
    }
    void pushBack(const T& item) override{
        if(size_ == capacity_){
            size_t new_capacity_ = capacity_ * SCALE_FACTOR;
            T* new_data_ = new T[new_capacity_];
            for(size_t i=0;i<size_;i++){
                new_data_[i] = data_[(front_+i)%capacity_];
            }
            delete[] data_;
            data_ = new_data_;
            capacity_ = new_capacity_;
            front_ = 0;
            back_ = size_;

        }
        data_[back_] = item;
        back_ = (back_+1) % capacity_; //bc back is the index where the NEXT element will be inserted at the back, it's one step forward in a "circular" array
        size_++;
    }

    // Deletion
    T popFront() override{
        if(size_ == 0){
            throw std::out_of_range("Empty queue");
        }else{
            T item = data_[front_];
            front_ = (front_+1)%capacity_; //move the front_ index forward in "circular" array (clockwise)
            size_--;
            return item;
        }
    }
    T popBack() override{
        if(size_ == 0){
            throw std::out_of_range("Empty queue");
        }else{
            if(back_==0){ //move back first because back_ points to the NEXT free slot, not the last-filled one. You have to move back. If it is zero then we have to go to capacity-1
                back_ = capacity_-1;
            }else{
                back_ = back_-1;
            }
            T item = data_[back_];
            size_--;
            return item;
        }
    }

    // Access
    const T& front() const override{
        if(size_ == 0){
            throw std::out_of_range("Empty queue");
        }else{
            return data_[front_];
        }
    }
    const T& back() const override{
        if(size_ == 0){
            throw std::out_of_range("Empty queue");
        }else{
            size_t position = 0;
            if(back_==0){
                position = capacity_-1;
            }else{
                position = back_-1;
            }
            return data_[position];
        }
    }

    // Getters
    std::size_t getSize() const noexcept override{
        return size_;
    }

};
