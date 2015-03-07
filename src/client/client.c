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
#include <Parser.h>

#include "../core/Socket.h"
#include "../core/Db.h"
#include "../core/Parser.h"


GtkBuilder *builder;
char *nickname[12];
Db_t *database;
int udpPort;

char chatPerson[12];


void *threadFn_join(void *varargp);

void *threadFn_list(void *varargp);

void *threadFn_leave(void *varargp);


void onDelete(GtkWidget *widget, GdkEvent *event, gpointer data) {
    g_print("delete event occured\n");
    pthread_t thread_web;
    pthread_create(&thread_web, NULL, threadFn_leave, NULL);
}


void onSubmit(GtkWidget *widget, GdkEvent *event, gpointer data) {
    gchar buffer[200];
    GtkWidget *txtPrompt = GTK_WIDGET (gtk_builder_get_object(builder, "txtPrompt"));
    gchar *input = gtk_entry_get_text(GTK_ENTRY(txtPrompt));
    strcpy(buffer, input);
    gtk_entry_set_text(GTK_ENTRY(txtPrompt), "");
    g_print(buffer);
    g_print("Submit clicked\n");
    // ------ real stuff here ------------
    Msg_t *msg = msg_fromUserInput(buffer);
    g_print("Cmd=%s Payload=%s\n", msg->command, msg->payload);

    if (strcmp(msg->command, "JOIN") == 0) {
        g_print("Join called\n");
        pthread_t thread_web;
        pthread_create(&thread_web, NULL, threadFn_join, NULL);

    } else if (strcmp(msg->command, "LIST") == 0) {
        g_print("List called\n");
        pthread_t thread_web;
        pthread_create(&thread_web, NULL, threadFn_list, NULL);

    } else if (strcmp(msg->command, "CHAT") == 0) {
        g_print("Chat called\n");
        strcpy(chatPerson, msg->payload);

    } else if (strcmp(msg->command, "send") == 0) {
        g_print("Chat message sent\n");
        DbEntry_t * entry = Db_findById(database, chatPerson);
        g_print("Attempting send to: %s@%s:%d\n", entry->nickname, entry->ip, entry->port);
        sendChatMessage(entry->ip, entry->port, msg->payload);

    } else if (strcmp(msg->command, "BYE") == 0) {
        g_print("Leave called\n");
        pthread_t thread_web;
        pthread_create(&thread_web, NULL, threadFn_leave, NULL);
    }

    msg_free(msg);
}


void sendChatMessage(char *hostname, int port, char *message) {
    int sock, bytes_read;
    unsigned int addr_len;
    struct sockaddr_in server_addr;
    struct hostent *host;

//Create a datagram socket (connection less)
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

//Fill-up details of Server Address (IP,Port...)
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port); //Server Port
    host = (struct hostent *) gethostbyname((char *) hostname); //server IP
    server_addr.sin_addr = *((struct in_addr *) host->h_addr); //server IP
    bzero(&(server_addr.sin_zero), 8);

    addr_len = sizeof(struct sockaddr);

    //Send a datagram to Server
    sendto(sock, message, strlen(message), 0,
            (struct sockaddr *)&server_addr, sizeof(struct sockaddr));

}


void gtkTextViewAppend(GtkWidget *textview, gchar *text) {
    GtkTextBuffer *tbuffer;
    GtkTextIter itr;

    tbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    gtk_text_buffer_get_end_iter(tbuffer, &itr);
    gtk_text_buffer_insert(tbuffer, &itr, text, -1);
}


void display(char *input) {
    GtkWidget *viewMain = gtk_builder_get_object(builder, "viewMain");
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


void *threadFn_join(void *varargp) {
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
    Msg_t *msgSend = msg_new("JOIN", nickname);
    char *raw = msg_toString(msgSend);
    send(sock, raw, 1024, 0);
}


void *threadFn_list(void *varargp) {
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
    Msg_t *msgSend = msg_new("LIST", nickname);
    char *raw = msg_toString(msgSend);
    send(sock, raw, 1024, 0);

    // expect reply
    recvBytes = recv(sock, recvData, 1024, 0);
    Db_deserialize(database, recvData);
    display("Database deserialized.\n");
    Db_print_custom(database, display);

}


void *threadFn_leave(void *varargp) {
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
    Msg_t *msgSend = msg_new("BYE", nickname);
    char *raw = msg_toString(msgSend);
    send(sock, raw, 1024, 0);
}


void *threadFn_udpServer(void *varargp) {
    int sock;
    int bytesRead;
    unsigned int addrLen;
    char recv_data[1024], reply_data[1024];
    struct sockaddr_in server_addr, client_addr;

    //Create a datagram socket (connection less)
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
        perror("Socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(udpPort);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    bzero(&(server_addr.sin_zero), 8);

    //bind the sockt to local Internet Address (IP and PORT).
    if (bind(sock, (struct sockaddr *) &server_addr,
            sizeof(struct sockaddr)) == -1) {
        perror("Bind");
        exit(1);
    }

    addrLen = sizeof(struct sockaddr);

    printf("\nUDPServer Waiting for DATA from client on port %d\n", udpPort);
    fflush(stdout);

    while (1) {
        //receive data from the client
        bytesRead = recvfrom(sock, recv_data, 1024, 0,
                (struct sockaddr *) &client_addr, &addrLen);
        recv_data[bytesRead] = '\0';

        //process data. Here, we just print it and Reply to Client
        printf("\n(%s , %d) said : ", inet_ntoa(client_addr.sin_addr),
                ntohs(client_addr.sin_port));
        printf("%s", recv_data);
        fflush(stdout);
    }
    return 0;

}

int main(int argc, const char *argv[]) {
    // retrieve nickname ----------------
    printf("Enter your nickname: ");
    scanf("%s", nickname);
    //------------name-----------------------

    if (argc != 2) {
        udpPort = 6500;
    } else {
        udpPort = atoi(argv[1]);
        printf("Port=%d\n", udpPort);
    }

    // setup database
    database = Db_create();

    // host-related stuff
    pthread_t thread_ui, thread_server, thread_udpServer;
    pthread_create(&thread_ui, NULL, threadFn_ui, NULL);

    sleep(2);

    pthread_create(&thread_server, NULL, threadFn_join, NULL);
    pthread_create(&thread_udpServer, NULL, threadFn_udpServer, NULL);

    while (1) {}
    Db_free(database);
    return 0;
}
