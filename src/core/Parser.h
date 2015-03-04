#ifndef PARSER_H
#define PARSER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    char command[5];
    char payload[1010];
} Msg_t;

Msg_t *msg_new(char *cmd, char *payload);

Msg_t * msg_fromUserInput(char * inputString);

Msg_t *msg_fromString(char *raw);

char * msg_toString(Msg_t * target);

void * msg_display(Msg_t * target);

void msg_free(Msg_t * target);

#endif;