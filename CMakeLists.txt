project(cpp)
add_executable(a a.cpp b.cpp zhc.cpp)
add_executable(T1 ./p/T1.cpp ./zhc.cpp)
add_executable(T2 ./p/T2.cpp ./zhc.cpp FourierSeries.h ComplexIntegral.cpp ComplexValue.cpp Epicycle.cpp FourierSeries.cpp)
add_executable(T3 p/T3.cpp ./zhc.cpp FourierSeries.h ComplexIntegral.cpp ComplexValue.cpp Epicycle.cpp FourierSeries.cpp)
add_executable(fourierSeries FourierSeries.h ComplexIntegral.cpp ComplexValue.cpp Epicycle.cpp FourierSeries.cpp)

find_package(Threads REQUIRED)
target_link_libraries(fourierSeries Threads::Threads)