add_executable (testServer testServer.c)
add_executable (server server.c EventListener.h EventListener.c)
add_executable (EventListenerTest EventListener.h EventListener.c EventListenerTest.c)

target_link_libraries (server LINK_PUBLIC Socket)
