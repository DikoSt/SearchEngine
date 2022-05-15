# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
message(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(../lib/GTest_lib /Google_tests)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(../lib/nlohmann_json/include)
include_directories(../include)
include_directories(../src)
include_directories(../lib/thread-pool-2.0.0)


# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests

add_executable(Google_Tests_All_Test
        limits_test.cpp
        invertedIndexTests.cpp
        ../src/invertedindex.cpp
        ../include/invertedindex.h

        searchServerTests.cpp
        ../src/searchserver.cpp
        ../include/searchserver.h


        converterJSONtests.cpp
        ../src/converterJSON.cpp
        ../include/converterJSON.h
        ../src/stringutilites.cpp
        ../include/stringutilites.h

         makrosforexception.h)
target_link_libraries(Google_Tests_All_Test gtest gtest_main)

add_executable(GTests_Limits_Test limits_test.cpp)
target_link_libraries(GTests_Limits_Test gtest gtest_main)

add_executable(GTests_InvertedIndex_Test        invertedIndexTests.cpp
        ../src/invertedindex.cpp
        ../include/invertedindex.h
        ../src/stringutilites.cpp
        ../include/stringutilites.h

)
target_link_libraries(GTests_InvertedIndex_Test gtest gtest_main)


add_executable(GTests_SearchServer_Test         searchServerTests.cpp
        ../src/searchserver.cpp
        ../include/searchserver.h
        ../src/stringutilites.cpp
        ../include/stringutilites.h
        ../src/invertedindex.cpp
        ../include/invertedindex.h
)
target_link_libraries(GTests_SearchServer_Test gtest gtest_main)


add_executable(GTests_ConverterJSON_Test        converterJSONtests.cpp
        ../src/converterJSON.cpp
        ../include/converterJSON.h
        ../src/stringutilites.cpp
        ../include/stringutilites.h
)
target_link_libraries(GTests_ConverterJSON_Test gtest gtest_main)