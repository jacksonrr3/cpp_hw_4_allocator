#pragma once
#include <list>

template<typename T, int N>
class my_allocator {

	//вспомогательная структура одного блока аллоцированных данных  
	template <typename U>
	struct alloc_part {
		std::size_t _size = 0;		// количество занятых участков
		U* _array;			// указатель на область памяти

		alloc_part(size_t n) {		//конструктор, вызов malloc
			_array = reinterpret_cast<U*>(std::malloc(n * sizeof(U)));  
		}
		
		~alloc_part() {    		//деструктор, освобождение выделенного блока памяти
			std::free(_array);
		}				
	};

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

	my_allocator() :_data() {}

	~my_allocator() {};

	T* allocate(std::size_t n) {
		if (n > N) { throw std::bad_alloc(); }
		if (_data.empty() || size_less_n(n)) {
			_data.emplace_back(N);
		}
		return alloc_t(n);
	}

	void deallocate([[maybe_unused]] T* p, [[maybe_unused]] std::size_t n) const {}

	template<typename U, typename ...Args>
	void construct(U* p, Args&&...args) const {
		new(p) U(std::forward<Args>(args)...);
	};

	template <typename U>
	void destroy(U* p) const {
		p->~U();
	}

private:

	bool size_less_n(std::size_t n) {
		return _data.back()._size + n > N;
	}

	T* alloc_t(std::size_t n) {
		T* ret = (_data.back()._array) + _data.back()._size;
		_data.back()._size += n;
		return ret;
	}

	std::list<alloc_part<T>> _data;  //контейнер для хранения объектов аллоцированных блоков памяти
};
