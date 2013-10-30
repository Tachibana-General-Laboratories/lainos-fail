#include <sys/types.h>
#include <sys/stat.h>
#include <user.h>

int
main(int argc, char *argv[], char *envp[]) {
	int i;

	for (i = 0; envp[i]; i++) {
		printf(1, "%s\n", envp[i]);
	}

	return 0;
}
