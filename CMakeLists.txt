cmake_minimum_required(VERSION 3.10)

include(./functions.cmake)

setCompiler()
setFlags()

project(bczhc_cpp)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
list(APPEND MACRO_DEFINITIONS TEST00)

confLib()

link_libraries(zhcLib)

add_executable(
        diary-characters-statistics
        app/diaryCharactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(print-unicode-characters app/printUnicodeCharacters.cpp)
add_executable(
        demo-cpp
        demo/demo.cpp
        third_party/crypto-algorithms/sha256.c
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(fourier-seriesTest demo/fourierSeriesTest.cpp)
add_executable(sqlite3-to-JSON demo/sqlite3ToJSON.cpp)
add_executable(snake app/snake.cpp)
add_executable(sqlite-test demo/sqliteTest.cpp)
add_executable(
        characters-statistics
        app/charactersStatistics/charactersStatistics.cpp
        app/charactersStatistics/CountCharacters.cpp
)
add_executable(thread demo/thread.cpp)
add_executable(producer-and-consumer demo/producerAndConsumerTest.cpp)
add_executable(
        reverse
        app/characterReverser/reverse.cpp
        app/characterReverser/reverseCharacters.cpp
)
add_executable(echo-and-write app/echoAndWrite.cpp)
add_executable(shuffle-string app/shuffleString.cpp)
add_executable(bases128 app/base128/base128.cpp)
add_executable(stdin-size app/stdinSize.cpp)
if (UNIX)
    add_executable(
            stc-flash
            app/stc_flash/stc_flash.cpp
            app/stc_flash/stc_flash_lib.cpp
            app/stc_flash/serial_linux.cpp
    )
endif ()

add_executable(demo-c demo/demo.c demo/api.cpp third_party/crypto-algorithms/base64.c)
add_executable(
        test
        test/test.cpp
        test/string.cpp
)

add_executable(zero-width-codec app/zero_width_codec.cpp)
add_executable(file-size app/file_size.cpp)
add_executable(utf-converter app/utf_converter.cpp)

add_compile_definitions("${MACRO_DEFINITIONS}")
