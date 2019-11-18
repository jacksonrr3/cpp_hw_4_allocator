#define _ITERATOR_DEBUG_LEVEL 0

#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include "my_allocator.h"
#include "my_container.h"

struct hard {
	
	int i;
	double d;

	hard(int i, double d) : i(i), d(d) { std::cout << "Hard_construct" << std::endl; };

	hard(const hard&)= delete;
	   // { std::cout << "const hard&" << std::endl; };

	hard(hard&&) = delete;
	 //   { std::cout << "hard&&" << std::endl; };

	~hard() { std::cout << "Hard_destruct" << std::endl; };
};

int make_fac(std::size_t n) {
	if (n == 0) { return 1; }
	return n * make_fac(n-1);
}

double make_fib(size_t n) {
	if (n == 0) { return 0.0; }
	if (n == 1) { return 1.1; }
	return make_fib(n-1) + make_fib(n-2);
}


template <typename T>
void make_map(T& cont, size_t n) {
	//çàïîëíåíèå êîíòåéíåðà çíà÷åíèÿìè
	for (size_t i = 0; i < n; ++i) {
		cont.emplace(std::piecewise_construct,
			std::forward_as_tuple(i),
			std::forward_as_tuple(make_fac(i), make_fib(i)));
		std::cout << "---" << std::endl;
	}
}


template <typename T>
void make_cont(T& cont, size_t n) {
	
	for (size_t i = 0; i < n; ++i) {
		cont.enqueue(make_fac(i), make_fib(i));
		std::cout << "---" << std::endl;
	}
}



int main(int, char* []) {

	auto m_std = std::map<int, hard>{};
	auto m_all = std::map<int, hard, std::less<int>, my_allocator<std::pair<const int, hard>, 10>>{};
	auto q_std = Queue<hard>{};
	auto q_all = Queue<hard, my_allocator<Node<hard>, 10>>{};

	make_map(m_std, 11);
	make_map(m_all, 11);
	make_cont(q_std, 11);
	make_cont(q_all, 11);


	//âûâîä íà ýêðàí èç êîíòåéíåðà
	for (auto& i : m_all) {
		std::cout << i.second.i << " " << i.second.d << std::endl;
	}

	for (auto& i : q_all) {
		std::cout << i.i << " " << i.d << std::endl;
	}


	return 0;
}


