#include <stdio.h>
#include <stdlib.h>
#include <Parser.h>

#include "../core/Parser.h"

int main() {
    puts("====== Start of Test parser ==========");

    puts("Test Create");
    Msg_t * msg = msg_new("JOIN", "it is me"), *msg2, *msg3, *msg4;

    puts("Test toString");
    char * output = msg_toString(msg);

    puts("Test fromString");
    msg2 = msg_fromString(output);

    puts("Test Display");
    msg_display(msg);
    msg_display(msg2);

    puts("Test from user input");
    puts("");
    msg3 = msg_fromUserInput("/join i am here do you see me");
    msg_display(msg3);
    msg_free(msg3);
    puts("");

    puts("");
    msg4 = msg_fromUserInput("/x i am here do you see me");
    msg_display(msg4);
    msg_free(msg4);
    puts("");

    puts("");
    msg4 = msg_fromUserInput("/hello");
    msg_display(msg4);
    msg_free(msg4);
    puts("");

    puts("");
    msg4 = msg_fromUserInput("/hello ");
    msg_display(msg4);
    msg_free(msg4);
    puts("");

    puts("");
    msg4 = msg_fromUserInput("this is some body text without commands");
    msg_display(msg4);
    msg_free(msg4);
    puts("");

    puts("Test free");
    msg_free(msg);
    msg_free(msg2);




    return 0;
}
