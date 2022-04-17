#include "circular_buffer.h"

int main(int argc, char* argv[])
{
	printf("path %s", argv[0]);
	circular_buffer<int> que(10);

	for (std::size_t i = 0; i < 10; ++i)
	{
		que.push(i);
	}

	int len = que.size()+1;
	for (std::size_t i = 0; i < len; ++i)
	{
		printf("element [%ld] = %d\n", i, que.pop());
	}

	return 0;
}