#Dodaj do kompilacji wszystkie pliki C++ z folderu src
file(GLOB src "src/*.cpp")
add_library(VmDetect ${src})
target_link_libraries(VmDetect PUBLIC "dxgi.lib" "d3d12.lib")