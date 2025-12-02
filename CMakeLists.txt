cmake_minimum_required(VERSION 3.10)
project(March25)

set(CMAKE_C_STANDARD 99)

set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
find_package(Curses REQUIRED)

add_executable(March25 src/main.c
        src/menu.c
        src/client.c
        src/product.c
        src/order.c
        src/utils.c)

target_include_directories(March25 PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CURSES_INCLUDE_DIRS})
target_compile_definitions(March25 PRIVATE _XOPEN_SOURCE_EXTENDED)
target_link_libraries(March25 ncursesw)