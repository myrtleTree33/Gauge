#include <stdio.h>
#include <stdlib.h>

#include "../core/Db.h"

/**
* This is a test driver for DB and DBEntry
*/
int main() {
    puts("====== Start of Test DB ==========");
    puts("test creation of DB entry");
    DbEntry_t * entry = DbEntry_create("jhtong", "127.0.0.1", 6500);
    DbEntry_t * entry2 = DbEntry_create("mary", "127.0.0.2", 7000);
    DbEntry_t * entry3 = DbEntry_create("john", "127.0.0.3", 8000);

    puts("test display of single DB entry");
    DbEntry_print(entry);

    puts("test creation of DB");
    Db_t * database = Db_create();

    puts("test DB insertion");
    Db_insert(database, entry);
    Db_insert(database, entry2);
    Db_insert(database, entry3);

    puts("test DB display");
    Db_show(database);

    puts("test DB nick retrieval");
    DbEntry_t * entryFound = Db_findById(database, "mary");
    DbEntry_print(entryFound);

    puts("Test serialize");
    char * stream = Db_serialize(database);

    puts("Test deserialize");
    Db_t * database2 = Db_create();
    Db_deserialize(database2, stream);
    puts("   Showing output after deserialization -->");
    Db_show(database2);

    puts("test delete by ID");
    Db_deletebyId(database, "mary");
    Db_show(database);

    puts("test DB reset");
    Db_reset(database);
    Db_show(database);

    puts("test DB free");
    Db_free(database);
    Db_free(database2);

    puts("====== End of Test DB ==========");
    return 0;
}