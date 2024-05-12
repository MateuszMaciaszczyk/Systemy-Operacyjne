#ifndef LABY_SYSOPY_CONFIG_H
#define LABY_SYSOPY_CONFIG_H

char SERVER_PATH[] = "/server";
char CLIENT_PATH[] = "/client";

typedef enum {
    INIT,
    MESG,
    END
} message_type_t;

typedef struct {
    message_type_t type;
    int id;
    char text[100];
} message_t;

#endif
