add_library (Gauge Socket.c Socket.h Db.c Db.h Parser.h Parser.c)
target_include_directories (Gauge PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})