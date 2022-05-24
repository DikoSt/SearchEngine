cmake_minimum_required(VERSION 3.16)
project(SearchEngine VERSION 0.1)
configure_file(config.in ${CMAKE_CURRENT_SOURCE_DIR}/include/config.h)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.10.5/json.tar.xz)
FetchContent_MakeAvailable(json)
include_directories(${json_SOURCE_DIR}/include)

# gtest block start
FetchContent_Declare(
    googletest
    URL
    https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
# gtest block end

#include_directories(lib/thread-pool-2.0.0)
FetchContent_Declare(
  thread-pool
  GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
  GIT_TAG        737c6036103ac034682ee11c31634ebe6769781f # tag: v2.0.0 
)
FetchContent_MakeAvailable(thread-pool)

include_directories(${thread-pool_SOURCE_DIR})

add_subdirectory(Google_tests)
#add_subdirectory(Utilites)

include_directories(include src)

add_executable(SearchEngine search_engine.cpp
                            src/converterJSON.cpp
                            src/invertedindex.cpp
                            src/searchserver.cpp
                            src/stringutilites.cpp
                            src/exception.cpp   
)


target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json)