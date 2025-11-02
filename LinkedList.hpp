#pragma once
#include <iostream>
using namespace std;



template <typename T>
class LinkedList {
private:
	// Stores pointers to first and last nodes and count
	struct Node {
		T data;
		Node* prev;
		Node* next;
	};

	Node* head;
	Node* tail;
	unsigned int count;


public:
	// Behaviors
	void printForward() const{
		if(count==0){
			std::cout<<"empty list"<<endl;
		}else{
			Node* current = head;
			while(current!=nullptr){
				std::cout<<current->data<<" "<<std::endl;
				current = current->next;
			}
		}
	}
	void printReverse() const{
		if(count==0){
			std::cout<<"empty list"<<endl;
		}else{
			Node* current = tail;
			while(current!=nullptr){
				std::cout<<current->data<<" "<<std::endl;
				current = current->prev;
			}
		}
	}

	// Accessors
	[[nodiscard]] unsigned int getCount() const { return count;}
	Node* getHead() { return head; }
	const Node* getHead() const { return head; }
	Node* getTail() { return tail; }
	const Node* getTail() const { return tail; }

	// Insertion
	void addHead(const T& data){
		Node* newNode = new Node;
		newNode->data = data;
		if(count==0){
			head = newNode;
			tail = newNode;
			count++;
			newNode->next = nullptr;
			newNode->prev = nullptr;
		}else{
			newNode->next = head;
			head->prev = newNode;
			head = newNode;
			newNode->prev = nullptr;
			count++;
		}
	}
	void addTail(const T& data){
		Node* newNode = new Node;
		newNode->data = data;
		if(count==0){
			head = newNode;
			tail = newNode;
			count++;
			newNode->next = nullptr;
			newNode->prev = nullptr;
		}else{
			newNode->prev = tail;
			tail->next = newNode;
			tail = newNode;
			newNode->next = nullptr;
			count++;
		}
	}

	// Removal
	bool removeHead(){
		if(count==0){
			return false;
		}else if(head==tail){
			delete head;
			head = nullptr;
			tail = nullptr;
			count = 0;
		}else{
			Node* temp = head;
			head = head->next;
			head->prev = nullptr;
			delete temp;
			temp = nullptr;
			count--;
		}
		return true;
	}
	bool removeTail(){
		if(count==0){
			return false;
		}else if(tail==head){
			delete tail;
			tail = nullptr;
			head = nullptr;
			count = 0;
		}else{
			Node* temp = tail;
			tail = tail->prev;
			tail->next = nullptr;
			delete temp;
			temp = nullptr;
			count--;
		}
		return true;
	}
	void clear() {
		Node* current = this->head;
		while(current!=nullptr) {
			Node* nextNode  = current->next;
			delete current;
			current = nextNode;
		}
		head = nullptr;
		tail = nullptr;
		count = 0;
	}

	// Operators
	LinkedList<T>& operator=(LinkedList<T>&& other) noexcept{
		if(this==&other){
			return *this;
		}
		clear();
		this->head = other.head;
		this->tail = other.tail;
		this->count = other.count;
		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;
		return *this;
	}
	LinkedList<T>& operator=(const LinkedList<T>& rhs){
		if(this==&rhs){
			return *this;
		}
		clear();
		Node* current = rhs.head;
		while(current!=nullptr){
			addTail(current->data);
			current = current->next;
		}
		return *this;
	}

	// Construction/Destruction
	LinkedList() {
		head = nullptr;
		tail = nullptr;
		count = 0;
	}
	LinkedList(const LinkedList<T>& list){
		this->head = nullptr;
		this->tail = nullptr;
		this->count = 0;
		Node* current = list.head;
		while(current!=nullptr){
			addTail(current->data);
			current = current->next;
		}
	}
	LinkedList(LinkedList<T>&& other) noexcept{
		this->head = other.head;
		this->tail = other.tail;
		this->count = other.count;
		other.head = nullptr;
		other.tail = nullptr;
		other.count = 0;
	}
	~LinkedList(){
		clear();
	}

};


