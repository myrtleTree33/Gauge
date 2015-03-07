#ifndef DB_H
#define DB_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/queue.h>
#include <stdio.h>

/**
* This is a database class, consisting of multiple DB Entries
*/

typedef struct DbEntry {
    char nickname[12]; //TODO resize
    char ip[12]; // TODO resize
    int port;
    LIST_ENTRY(DbEntry) pointers;
} DbEntry_t;


typedef struct {
    int size;
    LIST_HEAD(listhead, DbEntry) head; // stores linked list of DB entries
} Db_t;

struct listhead *headp;


DbEntry_t *DbEntry_create(char *nickname, char *ip, uint16_t port);

void DbEntry_free(DbEntry_t *target);

void DbEntry_print(DbEntry_t *target);

Db_t *Db_create();

void Db_free(Db_t *target);

int Db_insert(Db_t *target, DbEntry_t *record);

void Db_show(Db_t *target);

DbEntry_t *Db_findById(Db_t *target, char *nickname);

void Db_deletebyId(Db_t *target, char *nickname);

void Db_reset(Db_t *target);

char *Db_serialize(Db_t *target);

void Db_deserialize(Db_t *target, char *stream);

void DbEntry_print_custom(DbEntry_t *target, void (*printFn)(char *message));

void Db_print_custom(Db_t *target, void (*printFn)(char *message));


#endif DB_H