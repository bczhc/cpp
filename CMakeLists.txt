cmake_minimum_required(VERSION 3.10)

include(./functions.cmake)

setCompiler()
setFlags()

project(cpp)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

confLib()

link_libraries(zhcLib)

add_executable(
        diary_characters_statistics
        app/diaryCharactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(print_unicode_characters app/printUnicodeCharacters.cpp)
add_executable(test_demo test/test.cpp)
add_executable(fourier_seriesTest test/fourierSeriesTest.cpp)
add_executable(sqlite3_to_JSON test/sqlite3ToJSON.cpp)
add_executable(snake app/snake.cpp)
add_executable(sqlite_test test/sqliteTest.cpp)
add_executable(
        characters_statistics
        app/charactersStatistics/charactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(thread test/thread.cpp)
add_executable(producer_and_consumer test/producerAndConsumerTest.cpp)
add_executable(
        reverse
        app/characterReverser/reverse.cpp
        app/characterReverser/reverseCharacters.cpp
)
add_executable(echo_and_write app/echoAndWrite.cpp)
add_executable(shuffle_string app/shuffleString.cpp)
add_executable(bases128 app/base128.cpp)
add_executable(stdin_size app/stdinSize.cpp)
add_executable(
        stc_flash
        app/stc_flash/stc_flash.cpp
        app/stc_flash/stc_flash_lib.cpp
        app/stc_flash/serial_linux.cpp
)