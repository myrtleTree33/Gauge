#include "Db.h"


/**
* Creates a DB Entry instance.
*/
DbEntry_t *DbEntry_create(char *nickname, char *ip, uint16_t port) {
    DbEntry_t *target = calloc(1, sizeof(DbEntry_t));
    strcpy(target->nickname, nickname);
    strcpy(target->ip, ip);
    target->port = port;
    return target;
}


/**
* Free memory from DB Entry
*/
void DbEntry_free(DbEntry_t *target) {
    free(target);
}


/**
* Print a DB entry
*/
void DbEntry_print(DbEntry_t *target) {
    printf("%s@%s:%d\n", target->nickname, target->ip, target->port);
}


/**
* Print using a custom print handler.  Useful for output to GTK3 screen.
*/
void DbEntry_print_custom(DbEntry_t *target, void (*printFn)(char *message)) {
    printFn(target->nickname);
    printFn("@");
    printFn(target->ip);
    printFn("\n");
}


/**
* Print using a custom print handler.  Useful for output to GTK3 screen.
*/
void Db_print_custom(Db_t *target, void (*printFn)(char *message)) {
    DbEntry_t *curr;
    printFn("--- Userlist ---\n");
    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        DbEntry_print_custom(curr, printFn);
    }
    printFn("----------------\n");

}


/**
* Creates a new DB.  A Database is a linked list of DBEntries.  A linked list is needed
* to reduce complexity of dealing with logins and logouts, empty records, handling a flexible size
* of records, etc.
*/
Db_t *Db_create() {
    Db_t *target = calloc(1, sizeof(Db_t));
    LIST_INIT(&(target->head));
}


/**
* Frees a DB.
*/
void Db_free(Db_t *target) {
    Db_reset(target);
    free(target);
}


/**
* Insert a new DB entry, into the DB.
*/
int Db_insert(Db_t *target, DbEntry_t *record) {
    target->size++;
    LIST_INSERT_HEAD(&(target->head), record, pointers);
    return 0;
}


/**
* Show contents of DB.  For custom printing to GTK window, use Db_print_custom().
*/
void Db_show(Db_t *target) {
    DbEntry_t *curr;
    puts("--- Userlist ---");
    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        DbEntry_print(curr);
    }
    puts("----------------");
}


/**
* Find DB Entry by nickname.
*/
DbEntry_t *Db_findById(Db_t *target, char *nickname) {
    DbEntry_t *curr;
    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        if (strcmp(curr->nickname, nickname) == 0) {
            return curr;
        }
    }
    return NULL;
}


/**
* Delete DB Entry by nickname.
*/
void Db_deletebyId(Db_t *target, char *nickname) {
    DbEntry_t *item = Db_findById(target, nickname);
    if (item == NULL) {
        return;
    }
    LIST_REMOVE(item, pointers);
    target->size--;
}


/**
* Reset all records in DB.
*/
void Db_reset(Db_t *target) {
    while (target->head.lh_first != NULL) {
        LIST_REMOVE(target->head.lh_first, pointers);
    }
}


/**
* Serialize a DB, for transmission over TCP / UDP.
*
* This is done by first encoding the DB length as first element of stream, and encoding the rest of DB data.
*
*/
char *Db_serialize(Db_t *target) {
    char *buffer = calloc(1, target->size * (sizeof(DbEntry_t) + sizeof(int))); // needed for spaces
    DbEntry_t *curr;
    unsigned long i = 0;
    unsigned long offset = sizeof(int);
    memcpy(buffer, &(target->size), offset);

    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        printf("Copied.\n");
//        strncpy(buffer + offset + i * (sizeof(DbEntry_t)), curr->nickname, sizeof(curr->nickname));
//        strncpy(buffer + offset + i * (sizeof(DbEntry_t)) + sizeof(curr->nickname), curr->ip, sizeof(curr->ip));
//        memcpy(buffer + offset + i * (sizeof(DbEntry_t)) + sizeof(curr->nickname) + sizeof(curr->ip),
//                curr->port, 4);
        memcpy(buffer + offset + i * sizeof(DbEntry_t), curr, sizeof(DbEntry_t));

        i++;
    }
    return buffer;
}


/**
* Deserialize and reconstruct a DB from stream sent via TCP or UDP.
*/
void Db_deserialize(Db_t *target, char *stream) {
    int size = 0;
    unsigned long i = 0;
    unsigned long offset = sizeof(int);
    char nickname[12], ip[12];
    int port;

    memcpy(&size, stream, sizeof(int));
    printf("Size=%d\n", size);

    // clear target first
    Db_reset(target);

    // populate with fields
    target->size = size;

    for (i = 0; i < size; i++) {
        //TODO Get rid of 20 magic numbers
        strncpy(nickname, stream + offset + i * sizeof(DbEntry_t), 12);
        strncpy(ip, stream + offset + i * sizeof(DbEntry_t) + 12, 12);
        memcpy(&port, stream + offset + i * sizeof(DbEntry_t) + 12 + 12, 4);
        Db_insert(target, DbEntry_create(nickname, ip, port));
    }
}
