cmake_minimum_required(VERSION 3.10)

include(./functions.cmake)

setCompiler()
setFlags()

project(bczhc_cpp)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
list(APPEND MACRO_DEFINITIONS TEST00)

confLib()

link_libraries(zhcLib)

add_executable(
        diary_characters_statistics
        app/diaryCharactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(print_unicode_characters app/printUnicodeCharacters.cpp)
add_executable(
        demoCpp
        demo/demo.cpp
        third_party/crypto-algorithms/sha256.c
)
add_executable(fourier_seriesTest demo/fourierSeriesTest.cpp)
add_executable(sqlite3_to_JSON demo/sqlite3ToJSON.cpp)
add_executable(snake app/snake.cpp)
add_executable(sqlite_test demo/sqliteTest.cpp)
add_executable(
        characters_statistics
        app/charactersStatistics/charactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(thread demo/thread.cpp)
add_executable(producer_and_consumer demo/producerAndConsumerTest.cpp)
add_executable(
        reverse
        app/characterReverser/reverse.cpp
        app/characterReverser/reverseCharacters.cpp
)
add_executable(echo_and_write app/echoAndWrite.cpp)
add_executable(shuffle_string app/shuffleString.cpp)
add_executable(bases128 app/base128/base128.cpp)
add_executable(stdin_size app/stdinSize.cpp)
if (UNIX)
    add_executable(
            stc_flash
            app/stc_flash/stc_flash.cpp
            app/stc_flash/stc_flash_lib.cpp
            app/stc_flash/serial_linux.cpp
    )
endif ()

add_executable(demoC demo/demo.c demo/api.cpp third_party/crypto-algorithms/base64.c)
add_executable(
        test
        test/test.cpp
        test/string.cpp
)

add_compile_definitions("${MACRO_DEFINITIONS}")
