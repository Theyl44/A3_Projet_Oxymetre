cmake_minimum_required(VERSION 3.7.2)
project(projet C)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(CMAKE_C_STANDARD 11)

add_executable(projet main.c affichage.c affichage.h define.h fichiers.c fichiers.h fir.c fir.h iir.c iir.h integration.c integration.h lecture.c lecture.h mesure.c mesure.h)