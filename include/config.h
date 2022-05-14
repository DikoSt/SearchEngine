#pragma once
/** 
 *  *********************** ВНИМАНИЕ!!!!  ******************************
 *    данный файл генерируется автоматически, для внесения изменений в 
 * конфигурации программ необходимо вносить изименения в файле config.in
 * *********************************************************************
 */

/** Версия файла config.json  с которой работает программа */
const std::string VERSION_APP = "0.1";

/** каждый документ содержит не более 1000 слов */
const int MAX_WORDS_IN_FILE = 1000;

/** с максимальной длиной каждого в 100 символов. */
const int MAX_CHAR_IN_WORD = 100;

/** Поле списка запросов содержит не более 1000 запросов. */
const int MAX_REQUESTS = 1000;

/** Каждый запрос включает от одного до десяти слов */
const int MAX_WORDS_REQ = 10;

const std::string defaultConfigFileName = "config.json";
const std::string defaultRequestsFileName = "requests.json";
const std::string defaultAnswerFileName = "answer.json";
