#pragma once
#include <utility>
#include "allocator.h"

//êëàññ, ðåàëèçóþùèé ÿ÷åéêó äÿë êîíòåéíåðà
template <typename T>
struct Node {
	T _item;
	Node* _next;

	Node() {
		_next = nullptr;
	}
	
	template<typename...Args>
	Node(Args&&...args):_item(std::forward<Args>(args)...) {
		_next = nullptr;
		//_item{std::forward<Args>(args)... };
	}
	/*
	Node(const T& item) {
		_next = nullptr;
		_item = item;
	}
	
	
	Node(T&& item) {
		_next = nullptr;
		_item = item;
	}
	
	Node( Node<T>* next, const T& item ) {
		_next = next;
		_item = item;
	}
	*/
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

	//friend class Queue;
};


template <typename T, typename allocator= std::allocator<Node<typename T> > >
class Queue {
	Node<T>* _head;
	Node<T>* _tail;
	allocator _alloc;

public:
	Queue():_alloc(), _head( nullptr), _tail (nullptr) {}

	~Queue() {
		//while (!isEmpty()) {
		//	Node<T>* temp_head = _head;
		//	_head = _head->getNext();
		//	_alloc.destroy(temp_head);
		//	_alloc.deallocate(temp_head, 1);
		//}
		//_alloc.~allocator();
	}

	bool isEmpty() const {
		return _head == nullptr;
	}

	Node<T>* getHead() const {
		return _head;
	}

	template <typename...Args>
	void enqueue(Args &&...args) {
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
		_head = _head->getNext();
		_alloc.destroy(temp_head);
		_alloc.deallocate(temp_head, 1);
		return item;
	}

	using value_type = T;

	class const_iterator: 
		std::iterator<std::forward_iterator_tag, const value_type, ptrdiff_t, const value_type*, const value_type&> {
	public:
		Node<T>* _node;

		const_iterator() = default;
		const_iterator(const const_iterator & it) = default;
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



