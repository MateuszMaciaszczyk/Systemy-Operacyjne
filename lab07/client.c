#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include "config.h"

int main() {
    char client_path[100];
    sprintf(client_path, "%s%d", CLIENT_PATH, getpid());
    struct mq_attr attributes = {
            .mq_flags = 0,
            .mq_maxmsg = 10,
            .mq_msgsize = sizeof(message_t)
    };

    mqd_t client = mq_open(client_path, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes);
    if(client < 0)
        perror("mq_open client");

    mqd_t server = mq_open(SERVER_PATH, O_RDWR, S_IRUSR | S_IWUSR, NULL);
    if(server < 0)
        perror("mq_open server");

    message_t message;
    message.type = INIT;
    strcpy(message.text, client_path);
    mq_send(server, (char*)&message, sizeof(message), 0);
    mq_receive(client, (char*)&message, sizeof(message), NULL);

    printf("Connected as client %d\n", message.id);
    int id = message.id;

    if (fork() == 0) {
        while (true) {
            mq_receive(client, (char*)&message, sizeof(message), NULL);
            if (message.type == MESG) {
                printf("Client %d: %s\n", message.id, message.text);
            }
            else if (message.type == END) {
                return 0;
            }
        }
    }
    else {
        message.id = id;
        message.type = MESG;
        while (true) {
            printf("Enter message ('end' to end):\n");
            fgets(message.text, 100, stdin);
            if (strcmp(message.text, "end\n") == 0) {
                message.type = END;
                mq_send(server, (char*)&message, sizeof(message), 0);
                break;
            }
            mq_send(server, (char*)&message, sizeof(message), 0);
        }
    }

    printf("Client closed\n");
    mq_close(client);
    mq_close(server);
    mq_unlink(client_path);
    return 0;
}