#include "circular_buffer.h"

template <class T>
std::size_t circular_buffer<T>::size() const 
{
	return (_write_index + _count_elements - _read_index) % _count_elements;
}

template <class T>
void circular_buffer<T>::push(T element)
{
	_buffer[_write_index++] = element;
	if (_write_index == _count_elements)
	{
		_write_index = 0;
	}
}

template <class T>
T circular_buffer<T>::pop()
{	
	if (_read_index == _count_elements)
		_read_index = 0;

	return _buffer[_read_index++];
}