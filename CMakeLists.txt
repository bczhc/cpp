cmake_minimum_required(VERSION 3.10)

include(./functions.cmake)

setCompiler()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -g")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g")

project(cpp)

confLib()

link_libraries(zhcLib)

add_executable(T1 test/T1.cpp)
add_executable(T2 test/T2.cpp)
add_executable(T3 test/T3.cpp)
add_executable(T4 test/T4.cpp)
add_executable(diaryCharactersStatistics test/DiaryCharactersStatistics.cpp)
add_executable(printUnicodeCharacters test/printUnicodeCharacters.cpp)
add_executable(test test/test.cpp)
add_executable(fourierSeriesTest test/fourierSeriesTest.cpp)
add_executable(sqlite3ToJSON test/sqlite3ToJSON.cpp)
add_executable(snake snake.cpp)
add_executable(snake2 snake2.cpp)
add_executable(sqliteTest test/sqliteTest.cpp)
add_executable(charactersStatistics CharactersStatistics.cpp)
add_executable(thread test/thread.cpp)
add_executable(producerAndConsumer test/producerAndConsumer.cpp)
add_executable(reverse test/reverse.cpp)
add_executable(echoAndWrite test/echoAndWrite.cpp)
add_executable(shuffleString test/shuffleString.cpp)
add_executable(bases128 Base128.cpp)
add_executable(stdinSize test/stdinSize.cpp)
