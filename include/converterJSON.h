//
// Created by Denis on 19.04.2022.
//
#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include <string>
#include "config.h"
#include "searchserver.h"
#include "stringutilites.h"
#include "exception.h"

/**
* Класс для работы с JSON-файлами
*/
class ConverterJSON {
private:
    bool isConfigured = false;
// пути к файлам
    std::string mAnswerFileName = defaultAnswerFileName;
    std::string mConfigFileName = defaultConfigFileName;
    std::string mRequestsFileName = defaultRequestsFileName;

// переменные кофигурации поисковика и т.п.
    nlohmann::json mJsonConfig;

    void ReadJsonFile(const std::string &fileName, nlohmann::json &jsonResult);
    void WriteJsonFile(const std::string &fileName, nlohmann::json &jsonToWrite);

 /** чтение конфигурационных данных из файла config.json
  * при удачном чтении устанавливается isConfigured = true;
  */
    void ReadConfig();

    nlohmann::json CreatAnswerJson(const std::vector<std::vector<RelativeIndex>> &answers);

public:
    ConverterJSON();
    ~ConverterJSON() = default;
/*** Метод получения запросов из файла requests.json
* @return возвращает список запросов из файла requests.json
*/
    std::vector<std::string> GetRequests();

/** Метода возвращает вектор имён файлов для индексаци
 *
 * @return - вектор строк имен файлов
 */
    std::vector<std::string> GetFileNames();

/*** Метод считывает поле mMaxResponses для определения предельного
* количества ответов на один запрос
* @return максимальное количество результатов поиска max_responses
*/
    int GetResponsesLimit();

/*** Метод считывает поле name
* @return строка с наименованием программного продукта
*/
    std::string GetNameOfApp();

/*** Метод считывает поле version
* @return строка-версия файла config.json
*
*/
    std::string GetVersionApp();

/** Метод проверяет соответсвыие версии ПО и версии JSON файла config
 * @return true если версия файла config.json свпадает с версией программы
 */
    bool IsValidVersion();

/*** Положить в файл answers.json результаты поисковых запросов
*/
    void PutAnswers(const std::vector<std::vector<std::pair<size_t, double>>> &answers);

/*** Положить в файл answers.json результаты поисковых запросов
*/
    void PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers);

    void SetConfigFileName(const std::string &fileName);

    void SetRequestsFileName(const std::string &fileName);

    void SetAnswerFileName(const std::string &fileName);

    int GetMethodOfSearch();

    void SetMethodOfSearch(int methodOfSearch);

    void SetMaxResponses(int newMaxResponses);
};



