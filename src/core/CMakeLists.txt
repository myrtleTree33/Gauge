add_library (Socket Socket.c Socket.h Db.c Db.h Parser.h Parser.c)
target_include_directories (Socket PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})