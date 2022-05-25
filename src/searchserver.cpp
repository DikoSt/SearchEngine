//
// Created by Denis on 23.04.2022.
//

#include "searchserver.h"

SearchServer::SearchServer(InvertedIndex &idx, int maxRequests, int methodSearch) : mIndex(idx) {
    SetMethodOfSearch(methodSearch);
    SetMaxRequests(maxRequests);
};

std::vector<RelativeIndex> SearchServer::SearchByQuery_method2(const std::string &queryInput) {

    //std::multimap<int, std::string> queriesWords = SortWordByFreq(SplitIntoWords(queryInput)); /* частота, слово */
    std::vector<std::pair<std::string, int>> countWordsOfQuery;
    for (const auto &queryWord:SplitIntoWords(queryInput)) {
        countWordsOfQuery.push_back(std::pair(queryWord, mIndex.GetWordCountAllDocs(queryWord)));
    }
/*
    auto minEl  = std::min_element(countWordsOfQuery.begin(), countWordsOfQuery.end(),
                                   [](const auto &a, const auto &b) {
                         return a.second < b.second;
                     });
*/
    std::sort(countWordsOfQuery.begin(), countWordsOfQuery.end(),
              [](const auto &a, const auto &b) {
                  return a.second < b.second;
              });

    //std::vector<Entry> findDocs = mIndex.GetWordCount(minEl->first); // вектор документов с минимальным вхождением
    std::vector<Entry> findDocs = mIndex.GetWordCount(
            countWordsOfQuery[0].first); // вектор документов с минимальным вхождением
    if (findDocs.empty()) {
        return std::vector<RelativeIndex>();
        }

    for (auto it = countWordsOfQuery.begin() + 1; it != countWordsOfQuery.end(); ++it) {
//    for (const auto &[word, count]:countWordsOfQuery) {
        std::vector<Entry> nextDocs = mIndex.GetWordCount(it->first); //word
        //если документа из findDocs нет среди nextDocs, то этот документ удалям из findDocs

        auto docIdWordIt = findDocs.begin() + 1;
        size_t i = 0;
        auto endOfListFindDocs = findDocs.end();         
        while (docIdWordIt != endOfListFindDocs) {
            auto it = std::find_if(nextDocs.begin(), nextDocs.end(), [docIdWordIt](const Entry &a) {
                return a.docId == docIdWordIt->docId;
            });
            if (it == nextDocs.end()) {
                findDocs.erase(docIdWordIt);
                endOfListFindDocs = findDocs.end();
            } else {
                docIdWordIt->count += it->count;
                docIdWordIt++;
            }
        }
        if (findDocs.empty()) return std::vector<RelativeIndex>();
    }

    std::vector<RelativeIndex> result;
    std::multimap<size_t, size_t, std::greater<size_t>> sortResult;
    for (const auto &[docID, rank]:findDocs) {
        sortResult.insert({rank, docID});
        if (sortResult.size() > mMaxRequests) {
            sortResult.erase(std::prev(sortResult.end()));
        }
    }
// Проведём нормировку
    size_t maxRank = sortResult.begin()->first;
    for (const auto &[rank, docId]:sortResult) {
        result.push_back(RelativeIndex(docId, std::round(static_cast<double>(rank) / maxRank * 1000) / 1000));
    }
    return result;
};

std::vector<RelativeIndex> SearchServer::SearchByQuery_method3(const std::string &query) {
    std::map<size_t, size_t> docIdFreqWord; //docId, countwordsquery
    std::vector<RelativeIndex> result;
    for (const auto &queryWord: SplitIntoWords(query)) {
        for (auto &entry:mIndex.GetWordCount(queryWord)) {
            docIdFreqWord[entry.docId] += entry.count;
        }
    }
    if (docIdFreqWord.empty()) return std::vector<RelativeIndex>(); // возвращаем пустой ответ т.к. результат 0

    std::multimap<size_t, size_t, std::greater<size_t>> sortResult;
    for (const auto &[docID, rank]:docIdFreqWord) {
        sortResult.insert({rank, docID});
        if (sortResult.size() > mMaxRequests) {
            sortResult.erase(std::prev(sortResult.end()));
        }
    }
// Проведём нормировку
    size_t maxRank = sortResult.begin()->first;
    for (const auto &[rank, docId]:sortResult) {
        result.push_back(RelativeIndex(docId, std::round(static_cast<double>(rank) / maxRank * 1000) / 1000));
    }
    return result;
}

std::vector<RelativeIndex> SearchServer::SearchByQuery_BM25(const std::string &query) {
    std::vector<RelativeIndex> result;
    std::multimap<double, size_t> scoresDocs;
    double scoreDoc = 0;
    double k = 2.0, b = 0.75;

    for (size_t docId = 0; docId < mIndex.GetDocCount(); ++docId) {
        size_t D = mIndex.GetDocLength(docId);
        for (const auto &queryWord:SplitIntoWords(query)) {
            size_t TF = mIndex.GetWordCountInDoc(queryWord, docId);
            scoreDoc += mIndex.GetIDF(queryWord) * TF * (k + 1.0) / (
                    TF + k * (1.0 - b + b * (D / mIndex.GetAVGDL())));
//            scoreDoc += 1.0 * TF * (k + 1.0) / (
//                    TF + k * (1.0 - b + b * (D / mIndex.GetAVGDL())));
        }
        scoresDocs.insert(std::pair(scoreDoc, docId));
        scoreDoc = 0;
    }
    if (scoresDocs.empty()) return std::vector<RelativeIndex>();
    // std::cout << std::endl << "query: \""<<query << "\"" << std::endl;
    double maxRank = scoresDocs.rbegin()->first;
    int resultCount = 0;
    for (auto item = scoresDocs.rbegin();
         item != scoresDocs.rend() && resultCount < mMaxRequests; ++item, ++resultCount) {
        if (item->first > 0) {
            double normRank = std::round(item->first / maxRank * 1000) / 1000;
            result.push_back(RelativeIndex(item->second, normRank));
            //   std::cout << item->second << "_"<<item->first << ": " << mIndex.GetDoc(item->second) << std::endl;
        }
    }
    return result;
}

std::vector<std::vector<RelativeIndex>> SearchServer::Search(
        const std::vector<std::string> &queriesInput) {
    std::vector<std::vector<RelativeIndex>> result;

    typedef std::vector<RelativeIndex>(SearchServer::*func_ptr)(const std::string &);
    func_ptr current_ptr;

    switch (mMethodOfSearch) {
        case 1: {
            current_ptr = &SearchServer::SearchByQuery_BM25;
            break;
        }
        case 2: {
            current_ptr = &SearchServer::SearchByQuery_method2;
            break;
        }
        case 3: {
            current_ptr = &SearchServer::SearchByQuery_method3;
            break;
        }
    }
    for (const auto &query:queriesInput) {
        result.push_back((this->*current_ptr)(query));
    }

    return result;
};

void SearchServer::PrintResult(const std::vector<std::vector<RelativeIndex>> &result) {
    for (auto &elVect:result) {
        std::cout << "{" << std::endl;
        for (auto &relIndx:elVect) {
            std::cout << "{ " << relIndx.docId << ", " << relIndx.rank << " }," << std::endl;
        }
        std::cout << "}" << std::endl;
    }
}

void SearchServer::SetMethodOfSearch(int mMethodOfSearch) {
    if (mMethodOfSearch < 1 || mMethodOfSearch > 3) {
        std::cerr << "WARNING! Search method must from 1 to 3 " << std::endl;
        std::cout << "Search method set 1" << std::endl;
        SearchServer::mMethodOfSearch = 1;
    } else {
        SearchServer::mMethodOfSearch = mMethodOfSearch;
    }
}

void SearchServer::SetMaxRequests(int maxRequests) {
    if (maxRequests <= 0) {
        std::cerr << "WARNING! Max request bust be greater 0" << std::endl;
        std::cout << "Max request set 5" << std::endl;
        SearchServer::mMaxRequests = 5;
    } else {
        SearchServer::mMaxRequests = maxRequests;
    }
}

