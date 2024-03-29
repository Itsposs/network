
#include "optional.h"

int main(int argc, char *argv[]) {
	int fd;
	char buf[] = "Let's do!\n";

	fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
	if (fd == -1)
		error_handling("open() error.");
	printf("file descriptor: %d \n", fd);

	if (write(fd, buf, sizeof(buf)) == -1)
		error_handling("write() error.");
	close(fd);
	return 0;
}
