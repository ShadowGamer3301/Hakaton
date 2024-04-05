#Dodaj do kompilacji wszystkie pliki C++ z folderu src
file(GLOB src "src/*.cpp")
add_executable(Launcher ${src})
target_link_libraries(Launcher PRIVATE VmDetect "d3d11.lib")
target_include_directories(Launcher PRIVATE "../VmDetect/src")