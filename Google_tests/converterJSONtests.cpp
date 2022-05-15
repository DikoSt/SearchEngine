//
// Created by Denis on 20.04.2022.
//
#include <iostream>
#include "gtest/gtest.h"
#include "converterJSON.h"
#include "exception.h"
#include "makrosforexception.h"

ConverterJSON converterJson;
std::string fileNameTest = "test.json";

//проверка получения параметров конфигурации
nlohmann::json configFile = {
        {"config", {
                   {"max_responses", 15},
                   {"method_search", 1},
                   {"name", "TestAppName"},
                   {"version", "0.1"}
                   }
        },
        {"files", {
                  "resource\\file00001.txt",
                  "resource\\file00002.txt",
                  "resource\\file00003.txt"
                  }
        }
};

void SaveTestJsonFile(const std::string &fileName, const nlohmann::json &jFileContent){
    std::fstream file(fileName, std::ios::out);
    if (file.is_open()){
        file << jFileContent.dump(4);
    //    std::cout << "Testfile created and save" << std::endl;
        file.close();
    } else {
        std::cerr <<" Test error: file not created" << std::endl;
    }
}

TEST(converterJSON, GetNameOfApp){
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    EXPECT_EQ("TestAppName", converterJson.GetNameOfApp());
    std::remove (fileNameTest.c_str());

}

TEST(converterJSON, GetVersionApp){
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    EXPECT_EQ("0.1", converterJson.GetVersionApp());
  std::remove (fileNameTest.c_str());
}

TEST(converterJSON, GetResponsesLimit){
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    EXPECT_EQ(15, converterJson.GetResponsesLimit());
  std::remove (fileNameTest.c_str());
}

TEST(converterJSON, IsValidVersion_valid){
    configFile["config"]["version"] = VERSION_APP;
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    EXPECT_EQ(true, converterJson.IsValidVersion());
  std::remove (fileNameTest.c_str());
}

TEST(converterJSON, IsValidVersion_invalid){
    configFile["config"]["version"] = "100.200";
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    bool res = converterJson.IsValidVersion();
     EXPECT_EQ(false, converterJson.IsValidVersion());

  std::remove (fileNameTest.c_str());
}

TEST(converterJSON, GetRequests_OK){
    nlohmann::json requestFile;
       std::vector<std::string> expectedFiles;
    const int REQUEST_COUNT =100; // количество сгенерированных запросов

    for (int c=0; c < REQUEST_COUNT; ++c){
        expectedFiles.push_back(" a "); // запросов одинаковые и ' a ';
    }
    requestFile["requests"] = nlohmann::json(expectedFiles);

    SaveTestJsonFile(fileNameTest, requestFile);
    converterJson.SetRequestsFileName(fileNameTest);

    EXPECT_EQ(expectedFiles, converterJson.GetRequests());
    std::remove (fileNameTest.c_str());
}

TEST(converterJSON, GetRequests_Max_Count){
    nlohmann::json requestFile;
    std::vector<std::string> expectedFiles;
    const int REQUEST_COUNT =1010; // количество сгенерированных запросов

    for (int c=0; c < REQUEST_COUNT; ++c){
        expectedFiles.push_back(" a "); // запросов одинаковые и ' a ';
    }
    requestFile["requests"] = nlohmann::json(expectedFiles);

    SaveTestJsonFile(fileNameTest, requestFile);

    converterJson.SetRequestsFileName(fileNameTest);
    expectedFiles.resize(1000);
    EXPECT_EQ(expectedFiles, converterJson.GetRequests());
    EXPECT_EQ(converterJson.GetRequests().size(), 1000);
    std::remove (fileNameTest.c_str());
}

TEST(converterJSON, GetRequests_emptyFile_or_error){
    const nlohmann::json requestFile = {}; 
    const std::vector<std::string> expectedFilePathes = {};

    std::fstream file(fileNameTest, std::ios::out);
    if (file.is_open()){
//         file << requestFile;
         file.close();
    } else {
        std::cerr <<" Test error: file not created" << std::endl;
    }
    converterJson.SetRequestsFileName(fileNameTest);
    ASSERT_EXCEPTION({converterJson.GetRequests();}, ExceptionError, "Error: Error requests in "+fileNameTest+" file")
    std::remove (fileNameTest.c_str());
}

TEST(converterJSON, GetRequests_missingFile){
const std::string fileNameT = "missingFileName.json";
    converterJson.SetRequestsFileName(fileNameT);
    ASSERT_EXCEPTION({converterJson.GetRequests();},
                                ExceptionError,
                                "Error: Unable to read file "+fileNameT);
      std::remove (fileNameTest.c_str());
}

/** Тест проверки правильности запсии ответа и соответсвия формату загаданному
 * Формируется тестовый вектор ответов
 * из него формируется json файл - записывается
 * затем файл вычитывается и проверяеся так ли всё как хотелось
 *
 */
TEST(converterJSON, PutAnswers) {
    std::vector<std::vector<RelativeIndex>> answers = {
            {
                    RelativeIndex(0, 1),
                    RelativeIndex(1, 0.9),
                    RelativeIndex(2, 0.8),
                    RelativeIndex(3, 0.7)
            },
            {
                    RelativeIndex(0, 1),
                    RelativeIndex(1, 0.8),
                    RelativeIndex(1, 0.9),
                    RelativeIndex(1, 0.9)
            },
            {
                    RelativeIndex(0, 1),
                    RelativeIndex(1, 0.8),
                    RelativeIndex(1, 0.9),
                    RelativeIndex(1, 0.9)
            },
            {}

    };
    answers[0] ;
    converterJson.SetConfigFileName("config.json");
    converterJson.SetAnswerFileName("testAnswerFile.json");
    converterJson.PutAnswers(answers);

    nlohmann::json readJsonAnswer;
    std::fstream file("testAnswerFile.json", std::ios::in);
    if (file.is_open()) {
        file >> readJsonAnswer;
    } else {
        std::cerr << "Unable to read file" << std::endl;
    }

    ASSERT_EQ(readJsonAnswer.size(), 1);
    ASSERT_EQ("answers", readJsonAnswer.items().begin().key());
    ASSERT_EQ("request001", readJsonAnswer["answers"].begin().key());
    ASSERT_EQ("relevance", readJsonAnswer["answers"]["request001"].begin().key());
    ASSERT_EQ("docID", readJsonAnswer["answers"]["request001"]["relevance"][0].begin().key());
  //  ASSERT_EQ("rank", readJsonAnswer["answers"]["request001"]["relevance"][0].back().key());
//TODO подумать и доделать. Надо ли так глубоко проверять, может просто сделать чтение по всем полям
// там где отвалится там и ошибка....

    ASSERT_EQ(readJsonAnswer["answers"].size(), answers.size());


    //инспекция флага result
    std::vector<bool> expectedResult, factReadResult;
    expectedResult = {1, 1, 1, 0};
    for (const auto &item:readJsonAnswer["answers"]) {
        factReadResult.push_back(item["result"]);
    }
    ASSERT_EQ(expectedResult, factReadResult);

    //проверим что записываются reques001.... и т.д.
    int numberRequest = 1;
    auto it = answers.begin();
    for (auto item = readJsonAnswer["answers"].begin();
              item!= readJsonAnswer["answers"].end();
              ++item, ++numberRequest){
        ASSERT_EQ(
                "request" + std::string(3 - std::to_string(numberRequest).size(), '0')
                + std::to_string(numberRequest),
                item.key());
    }

}

TEST(converterJSON, GetFileNames){
    SaveTestJsonFile(fileNameTest, configFile);
    converterJson.SetConfigFileName(fileNameTest);
    std::vector<std::string> expected = {"resource\\file00001.txt",
                                         "resource\\file00002.txt",
                                         "resource\\file00003.txt"};
    EXPECT_EQ(expected, converterJson.GetFileNames());
    std::remove (fileNameTest.c_str());
}


