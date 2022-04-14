#pragma once
#include <cstdio>

template<class T>
class circular_buffer
{
public:
	explicit circular_buffer(const std::size_t iSize) 
	{ 
		_buffer = new T[iSize]; 
		_write_index = 0;
		_read_index = 0;
		_count_elements = iSize;
	};

	std::size_t size() const;
	void push(T element);
	T pop();
private:
	T *_buffer;
	std::size_t _write_index;
	std::size_t _read_index;
	std::size_t _count_elements;
};

