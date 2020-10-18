set(CMAKE_EXE_LINKER_FLAGS -static)

project(cpp)

add_executable(test test/test.cpp utf8.cpp Concurrent.cpp)
add_executable(T1 test/T1.cpp utf8.cpp)
