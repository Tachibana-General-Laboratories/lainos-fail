#include <stddef.h>
#include <stdint.h>

#define _sizeof(x) printf(1, "sizeof(" #x ") -> %d\n", sizeof( x ) )

void printf(int, const char *, ...);

int main(int argc, char *argv[]) {

	//#include <stddef.h>
	_sizeof(ptrdiff_t);
	_sizeof(size_t);
	_sizeof(wchar_t);

	//#include <stdint.h>
	_sizeof(int8_t);
	_sizeof(int16_t);
	_sizeof(int32_t);
	_sizeof(int64_t);
	_sizeof(uint8_t);
	_sizeof(uint16_t);
	_sizeof(uint32_t);
	_sizeof(uint64_t);

	exit();
}
