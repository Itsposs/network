
#include "optional.h"

int main(int argc, char *argv[]) {
	int sock;
	int snd_buf, rcv_buf, state;
	socklen_t len;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	len = sizeof(snd_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_SNDBUF, 
			(void*)&snd_buf, &len);
	if (state) 
		error_handling("getsockopt() error.");

	len = sizeof(rcv_buf);
	state = getsockopt(sock, SOL_SOCKET, SO_RCVBUF, 
			(void*)&rcv_buf, &len);
	if (state) 
		error_handling("getsockopt() error.");

	printf("Input buffer size: %d \n", rcv_buf);
	printf("Output buffer size: %d \n", snd_buf);
	return 0;
}
