#Dodaj do kompilacji wszystkie pliki C++ z folderu src
file(GLOB src "src/*.cpp")
add_library(DemoApp ${src})
target_link_libraries(DemoApp PRIVATE VmDetect)