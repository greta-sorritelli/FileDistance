cmake_minimum_required(VERSION 3.15)
project(filedistance C)

set(CMAKE_C_STANDARD 99)

add_executable(filedistance main.c distance.c distance.h io.c io.h apply.c apply.h search.c search.h)