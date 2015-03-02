#include "Db.h"

DbEntry_t *DbEntry_create(char *nickname, char *ip, uint16_t port) {
    DbEntry_t *target = calloc(1, sizeof(DbEntry_t));
    strcpy(target->nickname, nickname);
    strcpy(target->ip, ip);
    target->port = port;
    return target;
}


void DbEntry_free(DbEntry_t *target) {
    free(target);
}


void DbEntry_print(DbEntry_t *target) {
    printf("%s@%s:%d\n", target->nickname, target->ip, target->port);
}


Db_t *Db_create() {
    Db_t *target = calloc(1, sizeof(Db_t));
    LIST_INIT(&(target->head));
};


void Db_free(Db_t *target) {
    Db_reset(target);
    free(target);
}


int Db_insert(Db_t *target, DbEntry_t *record) {
    target->size++;
    LIST_INSERT_HEAD(&(target->head), record, pointers);
    return 0;
}


void Db_show(Db_t *target) {
    DbEntry_t *curr;
    puts("--- Userlist ---");
    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        DbEntry_print(curr);
    }
    puts("----------------");
}


DbEntry_t *Db_findById(Db_t *target, char *nickname) {
    DbEntry_t *curr;
    for (curr = target->head.lh_first; curr != NULL; curr = curr->pointers.le_next) {
        if (strcmp(curr->nickname, nickname) == 0) {
            return curr;
        }
    }
    return NULL;
}


void Db_deletebyId(Db_t *target, char *nickname) {
    DbEntry_t *item = Db_findById(target, nickname);
    if (item == NULL) {
        return;
    }
    LIST_REMOVE(item, pointers);
    target->size--;
}


void Db_reset(Db_t * target) {
    while(target->head.lh_first != NULL) {
        LIST_REMOVE(target->head.lh_first, pointers);
    }
}
