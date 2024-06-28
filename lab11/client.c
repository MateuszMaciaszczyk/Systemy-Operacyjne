#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int server_socket;
struct sockaddr_in server_addr;
char client_id[32];

void *receive_messages(void *arg);
void signal_handler(int signum);

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: %s <client_id> <server_address> <server_port>\n", argv[0]);
        return -1;
    }

    strcpy(client_id, argv[1]);
    char *server_address = argv[2];
    int server_port = atoi(argv[3]);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("Socket error");
        return -1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_address);
    server_addr.sin_port = htons(server_port);

    sendto(server_socket, client_id, strlen(client_id), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    signal(SIGINT, signal_handler);

    pthread_t receive_thread;
    pthread_create(&receive_thread, NULL, receive_messages, NULL);

    char buffer[BUFFER_SIZE];
    while (1)
    {
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

        if (strcmp(buffer, "STOP") == 0)
        {
            break;
        }
    }

    close(server_socket);
    return 0;
}

void *receive_messages(void *arg)
{
    char buffer[BUFFER_SIZE];
    while (1)
    {
        struct sockaddr_in from_addr;
        socklen_t from_len = sizeof(from_addr);
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&from_addr, &from_len);
        if (bytes_received <= 0)
        {
            printf("Disconnected from server\n");
            exit(1);
        }
        buffer[bytes_received] = '\0';
        if (strcmp(buffer, "ALIVE") != 0) // Ignore "ALIVE" messages
        {
            printf("%s\n", buffer);
        }
    }
    return NULL;
}

void signal_handler(int signum)
{
    sendto(server_socket, "STOP", 4, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    close(server_socket);
    exit(0);
}
