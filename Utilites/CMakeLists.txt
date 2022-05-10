cmake_minimum_required(VERSION 3.16)
project (utilites)

include_directories(../include)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
add_executable(createJSONs createJSONs.cpp ShowReults.cpp ShowReults.h) # В первом приближении надо же сделать первые JSON файлы
target_link_libraries(createJSONs PRIVATE nlohmann_json::nlohmann_json)

add_executable(createManyFiles createManyFiles.cpp) # В первом приближении надо же сделать первые JSON файлы