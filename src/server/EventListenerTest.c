#include <stdlib.h>
#include <stdio.h>

#include "EventListener.h"

int callbackA(int connection, char * opr, char * payload) {
    printf("Callback executed!!");
    printf("OPR=%s PAYLOAD=%s", opr, payload);
    return 0;
}

int main() {
    EventListener_t * daemon = NULL;
    char opr[10], payload[200], msg[] = "COMM this is something";
    Event_t event;

    // test chunkify works
    chunkify(msg, opr, payload);
    printf("OPR:%s PAYLOAD:%s", opr, payload);

    // test Event_fill works
    Event_fill(&event, "COMM", callbackA);

    // test creation of daemon
    daemon = EventListener_create();

    // test addition of listener
    EventListener_addEvent(daemon, event);

    // test yield and execution of callback
    EventListener_yield(daemon, NULL, msg);

}