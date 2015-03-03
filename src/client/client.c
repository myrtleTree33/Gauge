#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>
#include <gtk/gtk.h>

#include "../core/Socket.h"

void *thread_prompt(void *ptr) {
    puts("hello world");
}


int main(int argc, const char *argv[]) {
    gtk_init(&argc, &argv);
    GtkBuilder *builder;
    GtkWidget *window;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/joel/repos/IdeaProjects/Gauge/src/UI/client.glade", NULL);

    window = GTK_WIDGET (gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);
    g_object_unref(G_OBJECT (builder));

    gtk_widget_show (window);
    gtk_main ();

//    while(1) {};



//    pthread_t inc

    // host-related stuff
    char nickname[20];

    int sock, recvBytes;
    char sendData[1024], recvData[1024];
    struct hostent *host;
    struct sockaddr_in serverAddr;

    host = gethostbyname("127.0.0.1");

    // create client socket fd
    sock = Socket(AF_INET, SOCK_STREAM, 0);

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr = *((struct in_addr *) host->h_addr);
    bzero(&(serverAddr.sin_zero), 8);

    //connect to server at port 5000
    Connect(sock, (struct sockaddr *) &serverAddr, sizeof(struct sockaddr));

    printf("\n I am conneted to (%s , %d)",
            inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));

    puts("");
    // retrieve nickname ----------------
    printf("Enter your nickname: ");
    scanf("%s", nickname);
    //-----------------------------------


    puts("== Welcome to the Gauge Chat Client ==");
    puts("/nick - set username /chat - chat user /help - show help menu");

    while (1) {
        //Input data from user through Standard Input device
        printf("\nTYPE ANY TEXT TO SEND (q or Q to quit) : ");
        gets(sendData);

        if (strcmp(sendData, "q") != 0 && strcmp(sendData, "Q") != 0) {
            //send data to server
            send(sock, sendData, strlen(sendData), 0);

            //get reply from server
            recvBytes = recv(sock, recvData, 1024, 0);
            recvData[recvBytes] = '\0';

            //process data. Here, we simply print it
            printf("\n%s ", recvData);
        }
        else {//send q or Q to server and close connection
            send(sock, sendData, strlen(sendData), 0);
            close(sock);
            break;
        }

    }
    return 0;
}
