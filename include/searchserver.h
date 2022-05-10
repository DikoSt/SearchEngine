//
// Created by Denis on 23.04.2022.
//
#pragma once
#include <algorithm>
#include "invertedindex.h"
#include "stringutilites.h"

struct RelativeIndex{
    size_t docId;
    double rank;

    bool operator ==(const RelativeIndex& other) const {
        return (docId == other.docId && rank == other.rank);
    }
    RelativeIndex() = default;
    RelativeIndex(size_t docId, double rank){
        this->rank = rank; this->docId = docId;
    }
    RelativeIndex(std::pair<size_t,double> __p){
        this->rank = __p.second; this->docId = __p.first;
    }
};

class SearchServer {
private:
    InvertedIndex mIndex;
    std::multimap<int, std::string> SortWordByFreq(const std::vector<std::string> &queryString);
    size_t CountEntry(std::vector<Entry>, size_t docId);
    std::map<int, int> InvertMap (std::map<int,int> inputMap);

    void InvertMap (const std::map<size_t, size_t> &inputMap, std::multimap<size_t, size_t>&outputMultimap);

    int mMethodOfSearch = 1;
    int maxRequests = 5;


private:

    void PrintResult(const std::vector<std::vector<RelativeIndex>> &result);
public:

    /**Метод поиска документов соответствующих запросу
     * метод подразумевает наличие всех слов в документе имеющиеся в запросе
     * релевантность вычисляется подсчётом количества слов в документе,
     * сумма количества всех слов запроса, встречающихся в документе затем
     * нормируется.
     * В случае отсутствия одного из слов запроса в документе возвращается
     * пустой результат
     *
     * @param queryInput - строка запрос
     * @return вектор содержит документID и его релевантность данному запросу
     */
    std::vector<RelativeIndex> SearchByQuery_method2(const std::string &queryInput);

    /**Метод поиска документов соответствующих запросу
     * Метод подразумевает наличие хотя бы одного  слова в документе из запроса.
     * Релевантность вычисляется подсчётом количества слов в документе,
     * сумма количества всех слов запроса, встречающихся в документе затем
     * нормируется.
     * Пустой результат возвращается если нет ни одного документа содержащего
     * хотя бы одно слово из запроса
     *
     * @param queryInput - строка запрос
     * @return вектор содержит документID и его релевантность данному запросу
     */
    std::vector<RelativeIndex> SearchByQuery_method3(const std::string &query);

    /**Метод поиска документов соответствующих запросу
     * Метод поиска основан на функции вычисленяи релевантности Best Match 25.
     *
     * @param queryInput - строка запрос
     * @return вектор содержит документID и его релевантность данному запросу
     */
    std::vector<RelativeIndex> SearchByQuery_BM25(const std::string &query);

/**
* @param idx в конструктор класса передаётся ссылка на класс
* InvertedIndex, чтобы SearchServer мог узнать частоту слов
* встречаемых в запросе
*
*/
    SearchServer(InvertedIndex &idx, int maxRequests = 5, int methodSearch = 1);

/**
* Метод обработки поисковых запросов
* @param queries_input поисковые запросы взятые из файла requests.json
* @return возвращает отсортированный список релевантных ответов для
* заданных запросов
*/
    std::vector<std::vector<RelativeIndex>> Search(const std::vector<std::string>& queriesInput);
    void SetMethodOfSearch(int mMethodOfSearch);
    void SetMaxRequests(int maxRequests);
};
