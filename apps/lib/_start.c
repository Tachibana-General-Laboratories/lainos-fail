static char **environ;

extern int main(int, char **, char **);

extern void exit(int);

void _start(int argc, char *argv[], char *envp[]) {
	environ = envp;
	int r = main(argc, argv, envp);
	exit(r);
}

