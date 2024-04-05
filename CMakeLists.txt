cmake_minimum_required(VERSION 3.19)

#Wymuœ u¿ycie standardu C++ 20 i C 17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

#Utwórz projetk
project(VmDetect VERSION 1.0)
project(DemoApp VERSION 1.0)
project(Launcher VERSION 1.0)

add_subdirectory(VmDetect)
add_subdirectory(DemoApp)
add_subdirectory(Launcher)

#Ustaw foldery wyjœciowe
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)