project(cpp)
add_executable(a a.cpp b.cpp zhc.cpp)
add_executable(T1 ./p/T1.cpp ./zhc.cpp)
add_executable(T2 ./p/T2.cpp ./zhc.cpp)
add_executable(T3 ./p/T3.c ./zhc.cpp)
add_executable(fourierSeries. ./fourierSeries.cpp ComplexDefinite.cpp ComplexValue.cpp)

find_package(Threads REQUIRED)
target_link_libraries(T1 Threads::Threads)