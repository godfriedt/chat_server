#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>


int main(int argc, char* argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;
    char buffer[1024];
    char message[1024];

    printf("[DEBUG] Creating socket...\n");
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("[ERROR] socket");
        return 1;
    }
    printf("[DEBUG] Socket created successfully (fd=%d)\n", sockfd);

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51423);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("[DEBUG] Attempting to connect to 127.0.0.1:51423...\n");
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("[ERROR] connect");
        close(sockfd);
        return 1;
    }

    printf("✅ Connected successfully to the server!\n");
    printf("Type messages below. Press Ctrl+C to quit.\n\n");

    while (1) {
        printf("You: ");
        fflush(stdout);

        if (!fgets(message, sizeof(message), stdin)) {
            printf("\n[DEBUG] EOF reached or input error.\n");
            break;
        }

        // Send message
        ssize_t sent = write(sockfd, message, strlen(message));
        if (sent < 0) {
            perror("[ERROR] write");
            break;
        }
        printf("[DEBUG] Sent %zd bytes\n", sent);

        // Receive echo
        memset(buffer, 0, sizeof(buffer));
        ssize_t received = read(sockfd, buffer, sizeof(buffer) - 1);
        if (received <= 0) {
            printf("[DEBUG] Server closed connection or read error (%zd)\n", received);
            break;
        }

        printf("Server: %s\n", buffer);
    }

    printf("[DEBUG] Closing socket.\n");
    close(sockfd);
    return 0;
}
