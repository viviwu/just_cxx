// terminal.c (Client Application)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10
#define SERVER_PORT 8889

int tracker_fd;
int client_id;
char client_ips[MAX_CLIENTS][16];
int num_clients = 0;

int server_fd;

void start_server() {
    struct sockaddr_in server_addr;

    // Create server socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);

    // Bind socket to address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);
}


void connect_to_tracker() {
    struct sockaddr_in tracker_addr;

    // Connect to the Tracker
    tracker_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (tracker_fd < 0) {
        perror("socket");
        exit(1);
    }

    memset(&tracker_addr, 0, sizeof(tracker_addr));
    tracker_addr.sin_family = AF_INET;
    tracker_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    tracker_addr.sin_port = htons(8888);

    if (connect(tracker_fd, (struct sockaddr *)&tracker_addr, sizeof(tracker_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    printf("Connected to Tracker.\n");
}

void disconnect_from_tracker() {
    close(tracker_fd);
    printf("Disconnected from Tracker.\n");
}

void query_other_clients() {
    char buffer[BUFFER_SIZE];
    int bytes_read;

    // Send a request to the Tracker for client information
    if (send(tracker_fd, "GET_CLIENTS", strlen("GET_CLIENTS"), 0) < 0) {
        perror("send");
        return;
    }

    // Receive client information
    memset(buffer, 0, BUFFER_SIZE);
    bytes_read = recv(tracker_fd, buffer, BUFFER_SIZE, 0);
    if (bytes_read < 0) {
        perror("recv");
        return;
    } else if (bytes_read == 0) {
        printf("Connection closed by the Tracker.\n");
        return;
    }

    // Parse client information
    char *token = strtok(buffer, " ");
    num_clients = 0;
    while (token != NULL) {
        strcpy(client_ips[num_clients], token);
        num_clients++;
        token = strtok(NULL, " ");
    }

    printf("Other accessible clients:\n");
    for (int i = 0; i < num_clients; i++) {
        printf("%s\n", client_ips[i]);
    }
}

void connect_to_client(char *ip) {
    int client_fd;
    struct sockaddr_in client_addr;

    // Create a socket for the client connection
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("socket");
        return;
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(ip);
    client_addr.sin_port = htons(8888);

    // Connect to the client
    if (connect(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
        perror("connect");
        close(client_fd);
        return;
    }

    printf("Connected to %s\n", ip);

    // TODO: Implement functionality for sending data, files, and disconnecting
    close(client_fd);
}

int main() {
    int choice;
    char ip[16];

    connect_to_tracker();

    while (1) {
        printf("\nClient Menu:\n");
        printf("1. Connect to the Tracker (already connected)\n");
        printf("2. Query information about other accessible terminals\n");
        printf("3. Connect to other known terminals\n");
        printf("4. Send information to other terminals\n");
        printf("5. Send file data to other terminals\n");
        printf("6. Disconnect from other terminals\n");
        printf("7. Disconnect from the Tracker\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Already connected to the Tracker.\n");
                break;
            case 2:
                query_other_clients();
                break;
            case 3:
                printf("Enter the IP of the client to connect to: ");
                scanf("%s", ip);
                connect_to_client(ip);
                break;
            case 4:
                // TODO: Implement sending information to other clients
                break;
            case 5:
                // TODO: Implement sending file data to other clients
                break;
            case 6:
                // TODO: Implement disconnecting from other clients
                break;
            case 7:
                disconnect_from_tracker();
                break;
            case 8:
                disconnect_from_tracker();
                exit(0);
            default:
                printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}