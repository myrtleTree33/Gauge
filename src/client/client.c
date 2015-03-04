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


GtkBuilder *builder;

void onDelete(GtkWidget * widget, GdkEvent * event, gpointer data) {
    g_print("delete event occured\n");
}


void onSubmit(GtkWidget * widget, GdkEvent * event, gpointer data) {
    gchar buffer[200];
    GtkWidget * txtPrompt = GTK_WIDGET (gtk_builder_get_object(builder, "txtPrompt"));
    gchar * input = gtk_entry_get_text(GTK_ENTRY(txtPrompt));
    strcpy(buffer, input);
    gtk_entry_set_text(GTK_ENTRY(txtPrompt), "");
    g_print(buffer);
    g_print("Submit clicked\n");
}

void gtkTextViewAppend(GtkWidget *textview, gchar *text)
{
    GtkTextBuffer *tbuffer;
    GtkTextIter itr;

    tbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_end_iter(tbuffer, &itr);
    gtk_text_buffer_insert(tbuffer, &itr, text, -1);
}


void display(gchar * input) {
    GtkWidget * viewMain = gtk_builder_get_object(builder, "viewMain");
    gtkTextViewAppend(viewMain, input);
}


void *threadFn_ui(void *varargp) {
//    gtk_init(&argc, &argv);
    gtk_init(NULL, NULL);
    GtkWidget *window;

    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "/home/joel/repos/IdeaProjects/Gauge/src/UI/client.glade", NULL);

    window = GTK_WIDGET (gtk_builder_get_object(builder, "window1"));
    gtk_builder_connect_signals(builder, NULL);

    display("i am here!");
    display("haha");

    gtk_widget_show(window);
    gtk_main();
}



int main(int argc, const char *argv[]) {

    pthread_t thread_ui;
    pthread_create(&thread_ui, NULL, threadFn_ui, NULL);

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
