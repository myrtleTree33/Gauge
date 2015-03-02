add_executable (testClient testClient.c)
add_executable (client client.c)

target_link_libraries (client LINK_PUBLIC Socket)
