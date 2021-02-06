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
    list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
    list(APPEND zhcLibLinkingLibs)

    #zhcLib
    list(
            APPEND zhcLibSrc
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
            app/base128/Base128Lib.cpp
            utf8.cpp
            number_parser.cpp
    )

    #sqlite3
    find_package(SQLite3)
    if (SQLite3_FOUND)
        include_directories("${SQLite3_INCLUDE_DIRS}")
        set(SQLITE3_LIB "${SQLite3_LIBRARIES}")
    else ()
        message(STATUS "Not found sqlite3, use local sqlite3 library.")
        include_directories(third_party/sqlite3-single-c)
        set(SQLITE3_LIB sqlite3)

        find_library(DL_LIB dl REQUIRED)
        set(sqlite3Src third_party/sqlite3-single-c/sqlite3.c)
        add_library(sqlite3 SHARED "${sqlite3Src}")
        target_link_libraries(sqlite3 "${DL_LIB}")
        add_library(sqlite3Static STATIC ${sqlite3Src})
        target_link_libraries(sqlite3Static "${DL_LIB}")
        set_target_properties(sqlite3Static PROPERTIES OUTPUT_NAME sqlite3)
    endif ()
    list(APPEND zhcLibLinkingLibs "${SQLITE3_LIB}")


    #pthread
    find_package(Threads REQUIRED)
    list(APPEND zhcLibLinkingLibs Threads::Threads)

    #pcre
    find_package(PCRE)
    if (PCRE_FOUND)
        include_directories("${PCRE_INCLUDE_DIR}")
        set(PCRE_LIB "${PCRE_LIBRARY}")
        list(APPEND zhcLibLinkingLibs "${PCRE_LIB}")
        list(APPEND zhcLibSrc regex.cpp)
        list(APPEND MACRO_DEFINITIONS CMAKE_PCRE_FOUND)
        set(MACRO_DEFINITIONS "${MACRO_DEFINITIONS}" PARENT_SCOPE)
    else ()
        message(STATUS "Not found libPCRE, disable regex.")
    endif ()


    add_library(zhcLib SHARED ${zhcLibSrc})
    target_link_libraries(zhcLib ${zhcLibLinkingLibs})

    add_library(zhcLibStatic STATIC ${zhcLibSrc})
    target_link_libraries(zhcLibStatic ${zhcLibLinkingLibs})
    set_target_properties(zhcLibStatic PROPERTIES OUTPUT_NAME zhcLib)
    if (STATIC_COMPILE)
        if (STATIC_COMPILE EQUAL 1)
            set_target_properties(zhcLib PROPERTIES LINK_FLAGS "-static-libstdc++ -static-libgcc")
        endif ()
    endif ()
endfunction()
