#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]) {
    char buf[0x100];
    int listen_fd, comm_fd;
    struct sockaddr_in servaddr;

    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(51423);

    int i = bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    listen(listen_fd, 5);
    	
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    while (1) {
        memset(buf, 0, sizeof(buf));
        read(comm_fd, buf, 0x100);
        printf("Echoing back %s", buf);
        write(comm_fd, buf, strlen(buf) + 1);
    }
    return 0;
}