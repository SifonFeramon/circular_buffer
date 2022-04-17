#pragma once
#include <cstdio>
#include <mutex>

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
		_isfull = false;
	};

	std::size_t size() const;
	void push(T element);
	T pop();
	T& operator[](const std::size_t index);
	const T& operator[](const std::size_t index) const;
	~circular_buffer() { delete[] _buffer; }
private:
	T *_buffer;
	std::size_t _write_index;
	std::size_t _read_index;
	std::size_t _count_elements;
	std::mutex _lock;
	bool _isfull;
};

template <class T>
std::size_t circular_buffer<T>::size() const
{
	if (_isfull)
		return _count_elements;

	return (_write_index + _count_elements - _read_index) % _count_elements;
}

template <class T>
void circular_buffer<T>::push(T element)
{
	std::lock_guard<std::mutex> lock(_lock);

	if (_isfull && _write_index == _read_index)
	{
		_read_index++;
	}

	_buffer[_write_index++] = element;

	if (_write_index == _count_elements)
		_write_index = 0;

	if (_write_index == _read_index)
	{
		_isfull = true;
	}
}

template <class T>
T circular_buffer<T>::pop()
{
	std::lock_guard<std::mutex> lock(_lock);

	if (_read_index == _count_elements)
		_read_index = 0;

	if (!_isfull && _read_index == _write_index)
		return T();

	if (_isfull)
		_isfull = !_isfull;

	return _buffer[_read_index++];
}

template <class T>
T& circular_buffer<T>::operator[](const std::size_t index)
{
	std::lock_guard<std::mutex> lock(_lock);
	if (_read_index == _write_index)
		return T();

	return _buffer[(_read_index + index) % _count_elements];
}

template<class T>
const T& circular_buffer<T>::operator[](const std::size_t index) const
{
	std::lock_guard<std::mutex> lock(_lock);
	if (_read_index == _write_index)
		return T();

	return _buffer[(_read_index + index) % _count_elements];
}

