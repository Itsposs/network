
#include "optional.h"

#define BUF_SIZE 100


int main(int argc, char *argv[]) {
	int fd;
	char buf[BUF_SIZE];

	fd = open("data.txt", O_RDONLY);
	if (fd == -1)
		error_handling("open() error.");
	printf("file descriptor: %d \n", fd);

	if (read(fd, buf, sizeof(buf)) == -1)
		error_handling("read() error!");
	printf("file data: %s", buf);
	close(fd);
	return 0;
}
