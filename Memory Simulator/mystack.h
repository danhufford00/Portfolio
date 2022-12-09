#include <cstdlib>
#include <stdio.h>

#ifndef MYSTACK_H
#define MYSTACK_H

class mystack{
	public:
		mystack();
		mystack(const mystack&);
		~mystack();
		mystack& operator=(const mystack&);
		size_t capacity() const;
		size_t size() const;
		bool empty() const;
		void clear();
		void reserve(size_t);
		const char& top() const;
		void push(char);
		void pop();

	private:
		char* stack_array;//dynamically allocates an array of characters
		size_t stackCapacity;//number of elements in the array
		size_t stackSize;//number of values in the stack aray (less than or equal to capacity)
};

#endif
