add_executable (testClient testClient.c)
add_executable (client client.c)
add_executable (udpClient udpclient.c)

target_link_libraries (client LINK_PUBLIC Gauge)
target_link_libraries(client LINK_PUBLIC ${GTK3_LIBRARIES})