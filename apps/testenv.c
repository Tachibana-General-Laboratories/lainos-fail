#include <sys/types.h>
#include <sys/stat.h>
#include <user.h>

#define alloca(size) __builtin_alloca (size)

#define _STEP(function, message...) \
({ \
    printf(1, "" #function " " message "\n", function); \
})

int _all() {
	printf(1, "All envp\n");

	int i = 0;
	do
		printf(1, "    %s %d\n", environ[i], &environ[i]);
	while(environ[i++]);
}

int
main(int argc, char *argv[]) {

	_all();

	printf(1, "getenv PATH: '%s'\n", getenv("PATH"));

	_STEP(setenv("yyy", "666", 0), "%d");
	_STEP(getenv("yyy"), "%s");

	_all();

	_STEP(setenv("xxx", "666", 0), "%d");
	_STEP(getenv("xxx"), "%s");

	_all();

	_STEP(setenv("xxx", "777", 0), "%d");
	_STEP(getenv("xxx"), "%s");

	_STEP(setenv("xxx", "888", 1), "%d");
	_STEP(getenv("xxx"), "%s");

	_STEP(setenv("zzz", "666", 0), "%d");
	_STEP(getenv("zzz"), "%s");

	_all();
	_STEP(unsetenv("xxx"), "%d");
	_STEP(getenv("xxx"), "%s");

	_all();

	_STEP(setenv("xxx", "999", 1), "%d");
	_STEP(getenv("xxx"), "%s");

	_all();



	printf(1, "puttenv qwerty: '%d'\n", putenv("qwerty=dwarf"));
	printf(1, "getenv qwerty: '%s'\n", getenv("qwerty"));

	_all();

	printf(1, "***NEW envp\n");
	exec("env", 0);



	return 0;
}
