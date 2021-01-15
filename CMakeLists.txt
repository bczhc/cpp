cmake_minimum_required(VERSION 3.10)

include(./functions.cmake)

setCompiler()
setFlags()

project(cpp)

confLib()

link_libraries(zhcLib)

add_executable(
diaryCharactersStatistics
app/diaryCharactersStatistics.cpp
app/charactersStatistics/CountCharacters.cpp
)
add_executable(printUnicodeCharacters app/printUnicodeCharacters.cpp)
add_executable(testDev test/test.cpp)
add_executable(fourierSeriesTest test/fourierSeriesTest.cpp)
add_executable(sqlite3ToJSON test/sqlite3ToJSON.cpp)
add_executable(snake app/snake.cpp)
add_executable(sqliteTest test/sqliteTest.cpp)
add_executable(
charactersStatistics
app/charactersStatistics/charactersStatistics.cpp
app/charactersStatistics/CountCharacters.cpp
)
add_executable(thread test/thread.cpp)
add_executable(producerAndConsumer test/producerAndConsumerTest.cpp)
add_executable(
reverse
app/characterReverser/reverse.cpp
app/characterReverser/reverseCharacters.cpp
)
add_executable(echoAndWrite app/echoAndWrite.cpp)
add_executable(shuffleString app/shuffleString.cpp)
add_executable(bases128 app/base128.cpp)
add_executable(stdinSize app/stdinSize.cpp)
