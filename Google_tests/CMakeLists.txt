# 'Google_test' is the subproject name
project(Google_tests)

include_directories(../include
                   ../src
)

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(Google_Tests_All_Test
        limits_test.cpp
        invertedIndexTests.cpp
        ../src/invertedindex.cpp

        searchServerTests.cpp
        ../src/searchserver.cpp

        converterJSONtests.cpp
        ../src/converterJSON.cpp
        ../src/stringutilites.cpp
         )

target_link_libraries(Google_Tests_All_Test gtest gtest_main)
#target_link_libraries(Google_Tests_All_Test nlohmann_json::nlohmann_json)

add_executable(GTests_Limits_Test limits_test.cpp)
target_link_libraries(GTests_Limits_Test gtest gtest_main)

add_executable(GTests_InvertedIndex_Test        invertedIndexTests.cpp
        ../src/invertedindex.cpp
        ../src/stringutilites.cpp
)
target_link_libraries(GTests_InvertedIndex_Test gtest gtest_main)


add_executable(GTests_SearchServer_Test         searchServerTests.cpp
        ../src/searchserver.cpp
        ../src/stringutilites.cpp
        ../src/invertedindex.cpp
        
)
target_link_libraries(GTests_SearchServer_Test gtest gtest_main)


add_executable(GTests_ConverterJSON_Test        converterJSONtests.cpp
        ../src/converterJSON.cpp
        ../src/stringutilites.cpp
       
)
target_link_libraries(GTests_ConverterJSON_Test gtest gtest_main)