add_executable (testDb DbTest.c)
add_executable (testParser ParserTest.c)

target_link_libraries (testDb LINK_PUBLIC Gauge)
target_link_libraries (testParser LINK_PUBLIC Gauge)
