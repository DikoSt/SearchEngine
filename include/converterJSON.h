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
    std::fstream mFile;
    bool isConfigured = false;
// пути к файлам 
    std::string mAnswerFileName;
    std::string mConfigFileName;
    std::string mRequestsFileName;

// переменные кофигурации поисковика и т.п.
    std::string mNameOfApp;
    std::string mVersionApp;
    int mMaxResponses = 5;
    int mMethodOfSearch = 1;
    std::vector<std::string> mFileNames;

/** чтение конфигурационных данных из файла config.json
 * устанавливаются соответсвущие переменные,
 * при удачном чтении устанавливается isConfigured = true;
 */
    void ReadConfig();

/**
 * Метод записывает файл answer.json
 * @param answerJSON
 */
    void WriteAnswersJSON(const nlohmann::json&);

public:
    //ConverterJSON();
    ConverterJSON(  std::string pathConfigJSON = defaultConfigFileName,
                    std::string pathRequestJSON = defaultRequestsFileName,
                    std::string pathAnswersJSON = defaultAnswerFileName);
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



