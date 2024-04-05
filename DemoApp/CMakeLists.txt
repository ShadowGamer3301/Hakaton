#Dodaj do kompilacji wszystkie pliki C++ z folderu src
file(GLOB src "src/*.cpp")
add_executable(DemoApp ${src})
target_link_libraries(DemoApp PRIVATE VmDetect)
target_include_directories(DemoApp PRIVATE "../VmDetect/src")