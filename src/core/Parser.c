#include "Parser.h"

Msg_t *msg_new(char *cmd, char *payload) {
    Msg_t *target = calloc(1, sizeof(Msg_t));
    strcpy(target->command, cmd);
    strcpy(target->payload, payload);
    return target;
}


/**
* @brief Unlike msg_fromString, this formats a user keyed string
* in the format similar to:
*
* /command some text here
*
* into a well-defined message to send over.
*
* If no command is given, then the whole chunk is intepreted as text to send user.
*
* Please note that if no payload is to be specified, include a space character at end.
*
* e.g. /JOIN would be "/JOIN ".
*
*/
Msg_t *msg_fromUserInput(char *inputString) {
    Msg_t *target = calloc(1, sizeof(Msg_t));

    if (*inputString == '/') {
        char *spacePtr = strchr(inputString, ' ');
        if (spacePtr == NULL) {
            strcpy(target->command, "");
            strcpy(target->payload, "");
            return target; // TODO: Fix this such that it only returns a command if no space found

        } else {
            printf("POS=%d", spacePtr - inputString);
            strncpy(target->command, inputString + 1, spacePtr - inputString); // get rid of leading slash
            strncpy(target->payload, spacePtr + 1, 900); // get the remainder of text to send
            target->command[spacePtr - inputString - 1] = '\0';
        }
    } else {
        strcpy(target->command, "send"); // assumes null char in place
        strncpy(target->payload, inputString, 1010); // assumes null char in place
    }

    return target;
}


/** Create a message from a string.  NOT USER INPUT.
*
*/
Msg_t *msg_fromString(char *raw) {
    Msg_t *target = calloc(1, sizeof(Msg_t));
    strncpy(target->command, raw, 5);
    strncpy(target->payload, raw + 5, 1010);
    return target;
}


/**
* Dump message as a string, for serializing.
*/
char *msg_toString(Msg_t *target) {
    char buffer[1015];
    strncpy(buffer, target->command, 5);
    strncpy(buffer + 5, target->payload, 1010);
    return buffer;
}


/**
* Display a message output to screen, for debugging purposes.
*/
void *msg_display(Msg_t *target) {
    printf("CMD=%s MSG=%s", target->command, target->payload);
}


/**
* Free message data.
*/
void msg_free(Msg_t *target) {
    free(target);
}
