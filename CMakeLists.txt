find_file(CLANG_PATH clang)
find_file(CLANG++_PATH clang++)
if (CLANG_PATH)
    set(C_COMPILER "${CLANG_PATH}")
else ()
    find_file(GCC_PATH gcc)
    set(C_COMPILER "${GCC_PATH}")
endif ()
if (CLANG++_PATH)
    set(CXX_COMPILER "${CLANG++_PATH}")
else ()
    find_file(GCC++_PATH g++)
    set(CXX_COMPILER "${GCC++_PATH}")
endif ()
set(CMAKE_C_COMPILER "${C_COMPILER}")
set(CMAKE_CXX_COMPILER "${CXX_COMPILER}")
set(CMAKE_C_FLAGS "-std=c99 -g")
set(CMAKE_CXX_FLAGS "-std=c++17 -g")

project(cpp)

#sqlite3 library
find_library(SQLITE3_LIB sqlite3)
if (NOT SQLITE3_LIB)
    add_library(sqlite3 SHARED third_party/sqlite3-single-c/sqlite3.c)
    find_library(DL_LIB dl REQUIRED)
    target_link_libraries(sqlite3 "${DL_LIB}")
endif ()

find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
if (SQLITE3_INCLUDE_DIR)
    include_directories("${SQLITE3_INCLUDE_DIR}")
else ()
    include_directories(third_party/sqlite3-single-c)
    set(SQLITE3_LIB sqlite3)
endif ()

#pthread library
find_library(PTHREAD_LIB pthread REQUIRED)
find_path(PTHREAD_INCLUDE_DIR pthread.h)
include_directories("${PTHREAD_INCLUDE_DIR}")

add_library(
        zhcLib
        SHARED
        ComplexIntegral.cpp
        ComplexValue.cpp
        Epicycle.cpp
        FourierSeries.cpp
        utf8.cpp
        CountCharacters.cpp
        Concurrent.cpp
        Sqlite3.cpp
        io.cpp
        String.cpp
        reverseCharacters.cpp
        utils.cpp
        File.cpp
        Base128Lib.cpp
)
target_link_libraries(zhcLib "${SQLITE3_LIB}" "${PTHREAD_LIB}")

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
