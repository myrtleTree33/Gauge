add_executable (testDb DbTest.c)

target_link_libraries (testDb LINK_PUBLIC Socket)
