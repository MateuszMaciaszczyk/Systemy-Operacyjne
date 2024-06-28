#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

typedef struct
{
    struct sockaddr_in addr;
    char id[32];
    int active;
} client_t;

int server_socket;
client_t clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void signal_handler(int signum)
{
    close(server_socket);
    exit(0);
}

int find_client_index(struct sockaddr_in client_addr);
void handle_clients(int server_socket);
void send_to_all(char *message, char *sender_id);
void send_to_one(char *message, char *sender_id, char *receiver_id);
void list_clients(int server_socket, struct sockaddr_in client_addr);
void remove_client(struct sockaddr_in client_addr);
void ping_clients();
void add_client(struct sockaddr_in client_addr, char *client_id);

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <address> <port>\n", argv[0]);
        return -1;
    }

    char *address = argv[1];
    int port = atoi(argv[2]);

    signal(SIGINT, signal_handler);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1)
    {
        perror("Socket error");
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(address);
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind error");
        return -1;
    }

    pthread_t ping_thread;
    pthread_create(&ping_thread, NULL, (void *)ping_clients, NULL);

    handle_clients(server_socket);

    close(server_socket);
    return 0;
}

int find_client_index(struct sockaddr_in client_addr)
{
    int client_index = -1;
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].active &&
            clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr &&
            clients[i].addr.sin_port == client_addr.sin_port)
        {
            client_index = i;
            break;
        }
    }
    return client_index;
}

void handle_clients(int server_socket)
{
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    while (1)
    {
        int bytes_received = recvfrom(server_socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received <= 0)
        {
            continue;
        }
        buffer[bytes_received] = '\0';

        int client_index = find_client_index(client_addr);
        if (client_index == -1)
        {
            add_client(client_addr, buffer);
        }
        else
        {
            char *sender_id = clients[client_index].id;
            if (strncmp(buffer, "LIST", 4) == 0)
            {
                list_clients(server_socket, client_addr);
            }
            else if (strncmp(buffer, "2ALL", 4) == 0)
            {
                send_to_all(buffer + 5, sender_id);
            }
            else if (strncmp(buffer, "2ONE", 4) == 0)
            {
                char *receiver_id = strtok(buffer + 5, " ");
                char *message = strtok(NULL, "");
                send_to_one(message, sender_id, receiver_id);
            }
            else if (strncmp(buffer, "STOP", 4) == 0)
            {
                remove_client(client_addr);
            }
        }
    }
}

void add_client(struct sockaddr_in client_addr, char *client_id)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (!clients[i].active)
        {
            clients[i].addr = client_addr;
            strncpy(clients[i].id, client_id, sizeof(clients[i].id) - 1);
            clients[i].active = 1;
            printf("%s connected\n", client_id);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_to_all(char *message, char *sender_id)
{
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%s: %s", sender_id, message);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].active)
        {
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void send_to_one(char *message, char *sender_id, char *receiver_id)
{
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUFFER_SIZE];
    snprintf(buffer, sizeof(buffer), "%s: %s", sender_id, message);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].active && strcmp(clients[i].id, receiver_id) == 0)
        {
            sendto(server_socket, buffer, strlen(buffer), 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void list_clients(int server_socket, struct sockaddr_in client_addr)
{
    pthread_mutex_lock(&clients_mutex);
    char list[BUFFER_SIZE] = "Active clients:\n";
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].active)
        {
            strcat(list, clients[i].id);
            strcat(list, "\n");
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    sendto(server_socket, list, strlen(list), 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
}

void remove_client(struct sockaddr_in client_addr)
{
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (clients[i].active &&
            clients[i].addr.sin_addr.s_addr == client_addr.sin_addr.s_addr &&
            clients[i].addr.sin_port == client_addr.sin_port)
        {
            printf("%s disconnected\n", clients[i].id);
            clients[i].id[0] = '\0';
            clients[i].active = 0;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void ping_clients()
{
    while (1)
    {
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients[i].active)
            {
                if (sendto(server_socket, "ALIVE", 5, 0, (struct sockaddr *)&clients[i].addr, sizeof(clients[i].addr)) <= 0)
                {
                    printf("%s is not responding, removing client\n", clients[i].id);
                    clients[i].id[0] = '\0';
                    clients[i].active = 0;
                }
                else {
                    printf("ALIVE %s\n", clients[i].id);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
        sleep(10);
    }
}
