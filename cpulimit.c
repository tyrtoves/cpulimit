#include<stdio.h>
#include<fcntl.h>
#include<signal.h>
#include<unistd.h>
#include<stdlib.h>

void signal_handler(int sig) {
	exit(0);
}


struct sigaction act = {
	.sa_handler = signal_handler,
};

void cpu_limit(int argc, char **argv) {

	int limit = atoi(argv[1]) * 10000;
	
	int PID = fork();
	
	if (PID < 0) {
		perror("FORK");
		exit(0);
	}
	else if (PID == 0) {
		sigaction(SIGCHLD, &act, NULL);
		
		while(1) {
			usleep(limit);	
			kill(PID, SIGSTOP);
			usleep(1000000 - limit);
			kill(PID, SIGCONT);
		}
		
	}
	else {
		if(execvp(argv[2], argv + 2) == -1) {
			perror("EXEC");
			exit(0);
		}				
	}
}

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Too few argaments \n");
		exit(0);
	}
	
	cpu_limit(argc, argv);
	
	return 0;
}
