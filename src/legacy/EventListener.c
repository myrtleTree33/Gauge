#include "EventListener.h"


#include <stdlib.h>
#include <string.h>
#include <stdio.h>


EventListener_t *EventListener_create() {
    EventListener_t * eventListener = calloc(1, sizeof(EventListener_t));
    return eventListener;
}

void Event_fill(Event_t * event, char * name, int (*callback)(int connection, char * opr, char * payload)) {
    strcpy(event->name, name);
    event->callback = callback;
}

int EventListener_addEvent(EventListener_t * target, Event_t event) {
    // cannot allocate and return error
    // does not check for duplicate events.
    if (target->size > 30) {
        return -1;
    }
    target->events[target->size] = event;
    target->size++;
    return 0;
}


void EventListener_free(EventListener_t * target) {
    free(target);
}

int chunkify(char * string, char * opr, char * payload) {
    char tmp[200];
    int idx = -1;
    strcpy(tmp, string);
    idx = (int) (strchr(tmp, ' ') - tmp);
    if (idx < 0) {
        return -1;  // error
    }

    strncpy(opr, tmp, idx);
    strncpy(payload, tmp+idx + 1, 200 - idx);
}

void EventListener_yield(EventListener_t * target, int connection, char * message) {
    int i = 0;
    char opr[10], payload[200];
    chunkify(message, opr, payload);
    for (i = 0; i < target->size; i++) {
        if (strcmp(opr, (target->events[i].name)) == 0) {
            target->events[i].callback(connection, opr, payload);
            break;
        }
    }
}
