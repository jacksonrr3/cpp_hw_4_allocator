#pragma once
#include <list>
//#include <array>
//#include <utility>

template <typename T>
class alloc_part {
	std::size_t _size = 0;
	T* _array;
public:
	alloc_part(size_t n){
		_array = reinterpret_cast<T*>(std::malloc(n * sizeof(T)));
	}

	~alloc_part() { std::free(_array); }

	template<typename T, int N>
	friend class my_allocator;
};

template<typename T, int N>
class my_allocator {
public:
	using value_type = T;
	using pointer = T*;
	using const_pointer = const T*;
	using reference = T&;
	using const_reference = const T&;

	template<typename U>
	struct rebind {
		using other = my_allocator<U, N>;
	};

	my_allocator():_data(){
		std::cout << "Constract allocator!" << std::endl;
	}

	~my_allocator() {
		std::cout << "Destruct allocator!" << std::endl;
	};

	T* allocate(std::size_t n)  {
		std::cout << "allocate" << "[n = " << n << "]" << std::endl;

		if (n > N) { throw std::bad_alloc(); }
		if (_data.empty() || size_less_n(n)) {
			//T* temp = reinterpret_cast<T*>(std::malloc(N * sizeof(T)));
			_data.emplace_back(N);
			std::cout << "allocate_new_place" << "[n = " << n << "]" << std::endl;
		}
		//T* ret = (_data.back()._array)+ _data.back()._size;
		//++(_data.back()._size);
		
		return alloc_t();
	}

	void deallocate(T* p, std::size_t n) const {
		std::cout << "deallocate" << "[n = " << n << "]" << std::endl;
	}

	template<typename U, typename ...Args>
	void construct(U* p, Args&&...args) const {
		std::cout << "construct" << std::endl;
		new(p) U(std::forward<Args>(args)...);
	};

	template <typename U>
	void destroy(U* p) const {
		std::cout << "destruct" << std::endl;
		p->~U();
	}

private:

	bool size_less_n(std::size_t n) {
		return _data.back()._size + n >= N;
	}

	T* alloc_t() {
		T* ret = (_data.back()._array) + _data.back()._size;
		++(_data.back()._size);
		return ret;
	}

	std::list<alloc_part<T>> _data;
};
