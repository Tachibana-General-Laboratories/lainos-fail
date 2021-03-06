// init: The initial user-level program

#include <sys/types.h>
#include <sys/stat.h>
#include <user.h>
#include <sys/fcntl.h>

char *argv[] = { "sh", 0 };
char *envp[] = {"PATH=/bin:", "USER=lain", 0};

int
main(void) {
	int pid, wpid;

	if (open("/dev/console", O_RDWR) < 0) {
		mknod("/dev/console", 1, 1);
		open("/dev/console", O_RDWR);
	}

	dup(0);  // stdout
	dup(0);  // stderr

	for (;;) {
		printf(1, "init: starting sh\n");
		pid = fork();

		if (pid < 0) {
			printf(1, "init: fork failed\n");
			exit();
		}

		if (pid == 0) {
			execve("sh", argv, envp);
			printf(1, "init: exec sh failed\n");
			exit();
		}

		while ((wpid = wait()) >= 0 && wpid != pid) {
			printf(1, "zombie!\n");
		}
	}
}
