#include <stdio.h>
#include <stdlib.h>

#include "../core/Parser.h"

int main() {
    puts("====== Start of Test parser ==========");

    puts("Test Create");
    Msg_t * msg = msg_new("JOIN", "it is me"), *msg2;

    puts("Test toString");
    char * output = msg_toString(msg);

    puts("Test fromString");
    msg2 = msg_fromString(output);

    puts("Test Display");
    msg_display(msg);
    msg_display(msg2);

    puts("Test free");
    msg_free(msg);
    msg_free(msg2);


    return 0;
}
