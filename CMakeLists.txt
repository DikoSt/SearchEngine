cmake_minimum_required(VERSION 3.16)
project(SearchEngine VERSION 0.1)
configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)

set(CMAKE_CXX_STANDARD 17)

#if(${MY_TEST})
#    message("Add tests")
    add_subdirectory(Google_tests)
#endif()

#if(${MY_UTILITES})
#    message("Add Utilites")
    add_subdirectory(Utilites)
#endif()



set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(lib/nlohmann_json)
#include(FetchContent)
#FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
#FetchContent_MakeAvailable(json)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

include_directories(lib/thread-pool-2.0.0)
include_directories(include)
include_directories(src)

add_executable(SearchEngine search_engine.cpp
        include/config.h
        src/converterJSON.cpp
        include/converterJSON.h
        src/invertedindex.cpp
        include/invertedindex.h
        src/searchserver.cpp
        include/searchserver.h
        include/structures.h
        src/stringutilites.cpp
        include/stringutilites.h
        src/exception.cpp
        include/exception.h)

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)
#add_subdirectory(configurator) # В дальнейшем предполагаю будет конфигуратор