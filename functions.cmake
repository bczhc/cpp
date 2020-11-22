function(setCompiler)
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
    set(CMAKE_C_COMPILER "${C_COMPILER}" CACHE INTERNAL "CMAKE_C_COMPILER")
    set(CMAKE_CXX_COMPILER "${CXX_COMPILER}" CACHE INTERNAL "CMAKE_CXX_COMPILER")
endfunction()

function(confLib)
    #sqlite3
    find_library(SQLITE3_LIB sqlite3)
    if (NOT SQLITE3_LIB)
        find_library(DL_LIB dl REQUIRED)
        set(sqlite3Src third_party/sqlite3-single-c/sqlite3.c)
        add_library(sqlite3 SHARED ${sqlite3Src})
        target_link_libraries(sqlite3 "${DL_LIB}")
        add_library(sqlite3Static STATIC ${sqlite3Src})
        target_link_libraries(sqlite3Static "${DL_LIB}")
        set_target_properties(sqlite3Static PROPERTIES OUTPUT_NAME sqlite3)
    endif ()

    find_path(SQLITE3_INCLUDE_DIR sqlite3.h)
    if (SQLITE3_INCLUDE_DIR)
        include_directories("${SQLITE3_INCLUDE_DIR}")
    else ()
        include_directories(third_party/sqlite3-single-c)
        set(SQLITE3_LIB sqlite3)
    endif ()


    #pthread
    find_library(PTHREAD_LIB pthread REQUIRED)
    find_path(PTHREAD_INCLUDE_DIR pthread.h)
    include_directories("${PTHREAD_INCLUDE_DIR}")


    #zhcLib
    set(
            zhcLibSrc
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
    add_library(zhcLib SHARED ${zhcLibSrc})
    target_link_libraries(zhcLib "${SQLITE3_LIB}" "${PTHREAD_LIB}")
    add_library(zhcLibStatic STATIC ${zhcLibSrc})
    target_link_libraries(zhcLibStatic "${SQLITE3_LIB}" "${PTHREAD_LIB}")
    set_target_properties(zhcLibStatic PROPERTIES OUTPUT_NAME zhcLib)
endfunction()