//
// main.c
// CPU
//
// Created by Макс on 24.03.14.
// Copyright (c) 2014 Максим Никитин. All rights reserved.
//

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

void CPUlimit(int argc, char **argv) {
    
	int limit = atoi(argv[1]) * 10000;
    
	int PID;
    
	if ((PID = fork()) < 0) {
		perror("Sorry! fork was failed");
		exit(0);
	}
	else if (PID == 0) {
		sigaction(SIGCHLD, &act, NULL);
        
		while(1) {
			usleep(limit);
			if(kill(PID, SIGSTOP) == -1){
                perror("Sorry! kill1 was failed");
                exit(0);
            }
			usleep(1000000 - limit);
			if(kill(PID, SIGCONT) == -1){
                perror("Sorry! kill2 was failed");
                exit(0);
            }
		}
        
	}
	else {
		if(execvp(argv[2], argv + 2) == -1) {
			perror("execvp was failed");
			exit(0);
		}
	}
}

int main(int argc, char **argv) {
    
	if (argc < 3) {
		printf("Too few argaments \n");
		exit(0);
	}
    
	CPUlimit(argc, argv);
    
	return 0;
}
