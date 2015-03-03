cmake_minimum_required (VERSION 2.8.7)
find_package(PkgConfig REQUIRED)

pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

project(GAUGE)

add_subdirectory(src/server)
add_subdirectory(src/client)
add_subdirectory(src/core)
add_subdirectory(src/test)
