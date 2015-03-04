#include "Parser.h"

Msg_t * msg_new(char * cmd, char * payload) {
    Msg_t * target = calloc(1, sizeof(Msg_t));
    strcpy(target->command, cmd);
    strcpy(target->payload, payload);
    return target;
}


Msg_t * msg_fromString(char * raw) {
    Msg_t * target = calloc(1, sizeof(Msg_t));
    strncpy(target->command, raw, 5);
    strncpy(target->payload, raw+5, 1010);
    return target;
}

char * msg_toString(Msg_t * target) {
    char buffer[1015];
    strncpy(buffer, target->command, 5);
    strncpy(buffer + 5, target->payload,1010);
    return buffer;
}

void * msg_display(Msg_t * target) {
    printf("CMD=%s MSG=%s", target->command, target->payload);
}

void msg_free(Msg_t * target) {
    free(target);
}
