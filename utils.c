#include <string.h>

void
memwrite(void *address, int value)
{
	memcpy(address, &value, sizeof(value));
}