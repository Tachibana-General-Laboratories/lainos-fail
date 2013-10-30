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

	printf(1, "0\n");
	//char mema[80*25*2];
	ushort *mem = malloc(80*25*sizeof(ushort));
	//char mema[80*25+ 80*23 + 10];

	printf(1, "1\n");

	//memset(mem, 0, sizeof(mem));
	printf(1, "2\n");

	for(int x=0; x<80*25;x++) {
		mem[x] = ('f' & 0xff) | (0xef << 8);
	}
	printf(1, "3\n");

	if (open("/dev/cga", O_RDWR) < 0) {
		mknod("/dev/cga", 2, 1);
		pid = open("/dev/cga", O_RDWR);

		write(pid, mem, 80*25*sizeof(ushort));
	}
	if (open("/dev/vbe", O_RDWR) < 0) {
		mknod("/dev/vbe", 3, 1);
		pid = open("/dev/vbe", O_RDWR);

        //while(1)
            write(pid, mem, 80*25*sizeof(ushort));
	}

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
