#pragma once
#include <utility>
#include <algorithm>
#include "my_allocator.h"

template <typename T>
struct Node {
	T _item;
	Node* _next;

	Node():_next(nullptr) {}

	template<typename...Args>
	Node(Args&&...args):_item(std::forward<Args>(args)...), _next(nullptr) {}
	
	Node(const T& item): _next(nullptr), _item(item) {}

	Node(T&& item):_next(nullptr) {
		std::swap(_item, item);
	}

	~Node() {}

	T getItem() const {
		return _item;
	}

	Node<T>* getNext() {
		return _next;
	}

	void setNext(Node<T>* next) {
		_next = next;
	}
};


template <typename T, typename allocator = std::allocator<Node<T> > >
class Queue {
	Node<T>* _head;
	Node<T>* _tail;
	allocator _alloc;

public:

	Queue(): _head(nullptr), _tail(nullptr), _alloc() {}

	//контруктор копирования при совпадающих аллокаторах
	Queue(const Queue<T, allocator>& other): _head(nullptr), _tail(nullptr), _alloc() {
		//std::cout << "const_alloc!\n";
		std::for_each(other.begin(), other.end(), [this](auto it) {this->enqueue(it); });
	}

	//контруктор копирования при разных аллокаторах
	template <typename other_alloc>
	Queue(const Queue<T, other_alloc>& other): _head(nullptr), _tail(nullptr), _alloc() {
		//std::cout << "const_other_alloc!\n";
		std::for_each(other.begin(), other.end(), [this](auto it) {this->enqueue(it); });
	}
	
	//контруктор копирования перемещением при совпадающих аллокаторах
	Queue(Queue<T, allocator>&& other) noexcept :_head(nullptr), _tail(nullptr), _alloc()  {
		//std::cout << "&&_alloc!\n";
		std::swap(_head, other._head);
		std::swap(_tail, other._tail);
		std::swap(_alloc, other._alloc);
	}

	//контруктор копирования перемещением при разных аллокаторах
	template <typename other_alloc>
	Queue(Queue<T, other_alloc>&& other):_head(nullptr), _tail(nullptr), _alloc() {
		//std::cout << "&&_other_alloc!\n";
		std::for_each(other.begin(), other.end(), [this](auto it) {this->enqueue(std::move(it)); });
	}

	~Queue() {
		
		while (!isEmpty()) {
			Node<T>* temp = _head;
			if (_head == _tail) {_tail = _tail->getNext();}
			_head=_head->getNext();
			_alloc.destroy(temp);
			_alloc.deallocate(temp, 1);
		}
		
	}

	bool isEmpty() const {
		return _head == nullptr;
	}

	Node<T>* getHead() const {
		return _head;
	}

	template <typename...Args>
	void enqueue(Args&&...args) {
		Node<T>* node = _alloc.allocate(1);
		_alloc.construct(node, std::forward<Args>(args)...);
		if (isEmpty()) {
			_head = node;
			_tail = node;
		}
		else {
			_tail->setNext(node);
			_tail = node;
		}
	}

	T dequeue() {
		if (isEmpty()) return 0;
		
		T item = _head->getItem();
		Node<T>* temp_head = _head;
		if (_head == _tail) {_tail = _tail->getNext();}
		_head = _head->getNext();
		_alloc.destroy(temp_head);
		_alloc.deallocate(temp_head, 1);
		return item;
	}

	using value_type = T;

	class const_iterator :
		std::iterator<std::forward_iterator_tag, const value_type, std::ptrdiff_t, const value_type*, const value_type&> {
	public:
		Node<T>* _node;

		const_iterator() = default;
		const_iterator(const const_iterator& it) = default;
		const_iterator(Node<T>* n) {
			_node = n;
		}
		const_iterator& operator=(const const_iterator& it) = default;

		const_iterator& operator++() {
			_node = _node->getNext();
			return *this;
		}

		bool operator==(const const_iterator& it) const {
			if (_node == it._node) {
				return true;
			}
			return false;
		}

		bool operator!=(const const_iterator& it) const {
			return !(*this == it);
		}
		const value_type& operator*() const { return _node->_item; }
		const value_type* operator->() const { return &(_node->getItem()); }
	};

	const_iterator begin() const {
		if (this->isEmpty()) {
			return const_iterator();
		}
		return const_iterator(_head);
	}

	const_iterator end()   const {
		if (this->isEmpty()) {
			return const_iterator();
		}
		return const_iterator(nullptr);
	}
};





