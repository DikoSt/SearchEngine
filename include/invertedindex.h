//
// Created by Denis on 20.04.2022.
//
#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <thread>
#include <mutex>
#include <cmath>
#include "stringutilites.h"

struct Entry{
    size_t docId, count;

    // Данный оператор необходим для проведения тестовых сценариев
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
    Entry() = default;
    Entry(size_t docID, size_t Count):docId(docID), count(Count){};
};

class InvertedIndex {
protected:
    std::mutex *mLockDictionary = nullptr;
    std::vector<std::string> mDocs;
    std::map<std::string, std::vector<Entry>> mFreqDictionary; //TF
    std::map<std::string, double> mIDF;
    double mAVGDL ; // avarage document length
    std::map<size_t, int> mDocLength; // docId, Length of word
    size_t mAllDocLengthInWord = 0;

    void CalculateIDF();
    void CalcalateAVGDL();


/**
 * Метод частоты встречания слова в документе
 * @param docId - идентификатор документа
 * @param textDocument - строка-текст документа
 */
   void ToIndexDoc(const size_t &docId, const std::string &textDocument);

public:
    InvertedIndex() = default;

//    InvertedIndex(const InvertedIndex &other);

/**
 * Метод для обновить или заполнить базу документов, по которой будем совершать поиск
 * @param inputDocs - вектор строк-документов
 */

    void UpdateDocumentBase(const std::vector<std::string> &inputDocs);
    void UpdateDocumentBase(const std::vector<std::string> &inputDocs, const double &i);

/**
 * Метод выполняет обновление или заполнение базы документов, по которой будем совершать поиск.
 * Выполняется в несколькоих потоках
 * @param input_docs - вектор строк-документов
 * @param maxThreads - число, максимальное количество создаваемых потоков
 */
    void UpdateDocumentBase(const std::vector<std::string> &inputDocs, const int &maxThreads);

/**
 * Метод определяет количество вхождений слова word в загруженной базе документов
 * @param word слово, частоту вхождений которого необходимо определить
 * @return возвращает подготовленный список с частотой слов
 */
    std::vector<Entry> GetWordCount(const std::string& word);
/**
 * Метод определяет количество вхождений слова word в документ docId
 * @param word - слово количество вхождение которого определяем
 * @param docId  - номер документа в котоом определяем количество вхождений слова word
 * @return количество вхождение слова word  в документ docId
 */
    int GetWordCountInDoc(const std::string& word, size_t docId);

/**
 * Метод определяет количество вхождений слова word в загруженной базе документов
 * @param word слово, количество вхождений которого необходимо определить
 * @return число количество вхождений солова во всех документах
 */
    size_t GetWordCountAllDocs(const std::string& word);

/**
 * Метод возвращает среднюю длинну документа в списке поиска в словах
 * @return средняя длинна документа
 */
    double GetAVGDL();

/**
 * Метод возвращает inverse document frequency IDF, заданного слова
 * @param word - слово для которого вычислялась IDF
 * @param docId - документ для которого вычисляется IDF
 * @return IDF
 */
    double GetIDF(const std::string &word);

/**
 * Метод возвращает длину лдокумена в словах
 * @param docID - номер документа длинну которого узнаём
 * @return длинна документа в словах
 */
    size_t GetDocLength(const size_t &docID);

/**
 * Метод возвращает количество документов в базе
 * @return количество документов в базе
 */
    size_t GetDocCount();

/**
 * Метод возвращает содержанеи документа
 * @param docId - номер документа
 * @return содержание документа
 */
    std::string GetDoc(size_t docId);

};
