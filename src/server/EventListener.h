#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

typedef struct {
    char name[30];
    int (*callback)(int connection, char * opr, char * payload);
} Event_t;

typedef struct {
    Event_t events[10];
    int size;
} EventListener_t;

void Event_fill(Event_t * event, char * name, int (*callback)(int connection, char * opr, char * payload));

EventListener_t *EventListener_create();
int EventListener_addEvent(EventListener_t * target, Event_t event);
void EventListener_free(EventListener_t * target);
void EventListener_yield(EventListener_t * target, int connection, char * payload);
int chunkify(char * string, char * opr, char * payload);

#endif EVENT_LISTENER_H