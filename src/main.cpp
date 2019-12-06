#include <iostream>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include "my_allocator.h"
#include "my_container.h"
#include "any_allocator.h"

struct hard {

	int i;
	double d;

	hard(int i, double d) : i(i), d(d) {};

	hard(const hard&) = delete;
	// { std::cout << "const hard&" << std::endl; };

	hard(hard&&) = delete;
	//   { std::cout << "hard&&" << std::endl; };

	~hard() {};
};

int make_fac(std::size_t n) {
	if (n == 0) { return 1; }
	return n * make_fac(n - 1);
}

double make_fib(size_t n) {
	if (n == 0) { return 0.0; }
	if (n == 1) { return 1.1; }
	return make_fib(n - 1) + make_fib(n - 2);
}

template <typename T>
void make_map(T& cont, size_t n) {

	for (size_t i = 0; i < n; ++i) {
		cont.emplace(std::piecewise_construct,
			std::forward_as_tuple(i),
			std::forward_as_tuple(make_fac(i), make_fib(i)));
	}
}

template <typename T>
void make_cont(T& cont, size_t n) {

	for (size_t i = 0; i < n; ++i) {
		cont.enqueue(make_fac(i), make_fib(i));
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

	for (auto& i : m_all) {
		std::cout << i.second.i << " " << i.second.d << std::endl;
	}

	for (auto& i : q_all) {
		std::cout << i.i << " " << i.d << std::endl;
	}

	
	//дополнительный контейнер для проверки конструкторов копирования
	//так как у структуры Hard удалены конструкторы копирования, используем int
	auto q_all_1 = Queue<int, my_allocator<Node<int>, 10>>{};
	for (size_t i = 0; i < 11; ++i) {
		q_all_1.enqueue(i);
	}

	//тест работы конструктора копирования при совпадающих аллокаторах контейнера
	Queue<int, my_allocator<Node<int>, 10>> q_all_2(q_all_1);
	/*for (auto& i : q_all_2) {
		std::cout << i << std::endl;   
	}*/

	//тест работы конструктора копирования при отличающихся аллокаторах контейнера
	Queue<int, logging_allocator<Node<int>>> q_all_3(q_all_1);
	/*for (auto& i : q_all_2) {
		std::cout << i << std::endl;
	}*/

	//тест работы конструктора копирования перемещением при совпадающих аллокаторах контейнера
	Queue<int, my_allocator<Node<int>, 10>> q_all_4(std::move(q_all_2));
	/*for (auto& i : q_all_4) {
		std::cout << i << std::endl;
	}*/

	//тест работы конструктора копирования перемещением при отличающихся аллокаторах контейнера
	Queue<int, my_allocator<Node<int>, 10>> q_all_5(std::move(q_all_3));
	/*for (auto& i : q_all_5) {
		std::cout << i << std::endl;
	}*/

	return 0;
}
