
#ifndef _OPTIONAL_H_
#define _OPTIONAL_H_

#include <stdio.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <sys/select.h>
#include <sys/socket.h>


void error_handling(char *message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


#endif
