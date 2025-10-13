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

void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    char buffer[1024];
    ssize_t bytes_received;

    // Receive data from the client
    while ((bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        printf("Received from client %d: %s\n", client_socket, buffer);

        // Process data and send a response (example: echo back)
        send(client_socket, buffer, bytes_received, 0);
    }

    printf("Client %d disconnected.\n", client_socket);
    close(client_socket);
    free(arg); // Free the dynamically allocated client_socket pointer
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pthread_t tid;

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Prepare the sockaddr_in structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all interfaces
    server_addr.sin_port = htons(8989);      // Port number

    // Bind the socket
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_socket, 5) < 0) { // 5 is the backlog queue size
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port 8989...\n");
    	

    while (1) {
        // Accept a new connection
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            perror("Error accepting connection");
            continue;
        }

        printf("New connection accepted from %s:%d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        // Allocate memory for client_socket to pass to the thread
        int *new_sock = malloc(sizeof(int));
        *new_sock = client_socket;

        // Create a new thread to handle the client
        if (pthread_create(&tid, NULL, handle_client, (void *)new_sock) != 0) {
            perror("Error creating thread");
            close(client_socket);
            free(new_sock);
        }
        pthread_detach(tid);
    }
    close(server_socket);
    return 0;
}