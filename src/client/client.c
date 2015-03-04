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
#include "../core/Db.h"
#include "../core/Parser.h"


GtkBuilder *builder;
char * nickname[12];

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


void display(char * input) {
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

    display("== Welcome to the Gauge Chat Client ==\n");
    display("Welcome, ");
    display(nickname);
    display("!!\n");
    display("/nick - set username /chat - chat user /help - show help menu\n");

    gtk_widget_show(window);
    gtk_main();
}


void *threadFn_join(void * varargp) {
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

    // make payload and send
    Msg_t * msgSend = msg_new("JOIN", nickname);
    char * raw = msg_toString(msgSend);
    send(sock, raw, 1024, 0);


//    while (1) {
//        //Input data from user through Standard Input device
//        printf("\nTYPE ANY TEXT TO SEND (q or Q to quit) : ");
//        gets(sendData);
//
//        if (strcmp(sendData, "q") != 0 && strcmp(sendData, "Q") != 0) {
//            //send data to server
//            send(sock, sendData, strlen(sendData), 0);
//
//            //get reply from server
//            recvBytes = recv(sock, recvData, 1024, 0);
//            recvData[recvBytes] = '\0';
//
//            //process data. Here, we simply print it
//            printf("\n%s ", recvData);
//        }
//        else {//send q or Q to server and close connection
//            send(sock, sendData, strlen(sendData), 0);
//            close(sock);
//            break;
//        }
//
//    }

}


int main(int argc, const char *argv[]) {
    // retrieve nickname ----------------
    printf("Enter your nickname: ");
    scanf("%s", nickname);
    //------------name-----------------------

    // host-related stuff
    pthread_t thread_ui, thread_server;
    pthread_create(&thread_ui, NULL, threadFn_ui, NULL);

    sleep(2);

    pthread_create(&thread_server, NULL, threadFn_join, NULL);

    while(1) {}

    return 0;
}
