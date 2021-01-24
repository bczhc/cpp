function(setCompiler)
    if (C_COMPILER)
        set(CMAKE_C_COMPILER "${C_COMPILER}" CACHE INTERNAL "CMAKE_C_COMPILER")
    endif ()
    if (CXX_COMPILER)
        set(CMAKE_CXX_COMPILER "${CXX_COMPILER}" CACHE INTERNAL "CMAKE_CXX_COMPILER")
    endif ()
endfunction()

function(setFlags)
    if (C_FLAGS)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${C_FLAGS} -O0" CACHE INTERNAL "CMAKE_C_FLAGS")
    endif ()
    if (CXX_FLAGS)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CXX_FLAGS} -O0" CACHE INTERNAL "CMAKE_CXX_FLAGS")
    endif ()
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

    #pcre
    find_library(PCRE_LIB pcre REQUIRED)
    find_path(PCRE_INCLUDE_DIR pcre.h)
    include_directories("${PCRE_INCLUDE_DIR}")


    #zhcLib
    set(
            zhcLibSrc
            math/big_decimal.cpp
            math/complex_integral.cpp
            math/complex_value.cpp
            math/epicycle.cpp
            math/fourier_series.cpp
            math/complex_integral.cpp
            concurrent.cpp
            sqlite3.cpp
            io.cpp
            string.cpp
            utils.cpp
            file.cpp
            base128/Base128Lib.cpp
            regex.cpp
            utf8.cpp
            number_parser.cpp
    )
    add_library(zhcLib SHARED ${zhcLibSrc})
    target_link_libraries(zhcLib "${SQLITE3_LIB}" "${PTHREAD_LIB}" "${PCRE_LIB}")
    add_library(zhcLibStatic STATIC ${zhcLibSrc})
    target_link_libraries(zhcLibStatic "${SQLITE3_LIB}" "${PTHREAD_LIB}")
    set_target_properties(zhcLibStatic PROPERTIES OUTPUT_NAME zhcLib)
    if (STATIC_COMPILE)
        if (STATIC_COMPILE EQUAL 1)
            set_target_properties(zhcLib PROPERTIES LINK_FLAGS "-static-libstdc++ -static-libgcc")
        endif ()
    endif ()
endfunction()
