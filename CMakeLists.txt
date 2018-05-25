cmake_minimum_required(VERSION 3.10)
project(OS_Assignment C)

set(CMAKE_C_STANDARD 99)

add_executable(eggshell main.c InterComm.c ExterComm.c linenoise-master/linenoise.c)