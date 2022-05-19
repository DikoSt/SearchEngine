//
// Created by Denis on 19.04.2022.
//

#include "converterJSON.h"
#include <fstream>

ConverterJSON::ConverterJSON(std::string pathConfigJSON,
                             std::string pathRequestJSON,
                             std::string pathAnswersJSON) :
        mConfigFileName(pathConfigJSON),
        mRequestsFileName(pathRequestJSON),
        mAnswerFileName(pathAnswersJSON) {}

void ConverterJSON::ReadConfig() {
    nlohmann::json configJSONFile;
    std::fstream mFile;

    mFile.open(mConfigFileName, std::ios::in);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable read file " + mConfigFileName);
    } else {
        try {
            mFile >> configJSONFile;
        } catch (...) {
            throw ExceptionError("File " + mConfigFileName + " corrupted");
        }
        mFile.close();
    }
    try {
        mVersionApp = configJSONFile["config"]["version"];
        mNameOfApp = configJSONFile["config"]["name"];
        SetMaxResponses(configJSONFile["config"]["max_responses"]);
        SetMethodOfSearch(configJSONFile["config"]["method_search"]);

        mFileNames = std::vector<std::string>(configJSONFile["files"].begin(), configJSONFile["files"].end());
        isConfigured = true;
    } catch (...) {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
    }
}

void ConverterJSON::PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers) {
    nlohmann::json answerJSON;

    if (!isConfigured) {
        ReadConfig();
    }

    int numberRequest = 1;
    for (auto &requestAnswer:answers) {
        int response = 0;
        nlohmann::json relevance = nlohmann::json::array();
        for (auto item2 = requestAnswer.begin();
             item2 != requestAnswer.end() && response < mMaxResponses; ++item2, ++response) {
            relevance.push_back({{"docID", item2->docId},
                                 {"rank",  item2->rank}});
        }
        std::string strNumberRequest =
                "request" + std::string(3 - std::to_string(numberRequest).size(), '0') + std::to_string(numberRequest);

        if (!requestAnswer.empty()) {
            answerJSON["answers"][strNumberRequest]["result"] = true;
            answerJSON["answers"][strNumberRequest]["relevance"] = relevance;
        } else {
            answerJSON["answers"][strNumberRequest]["result"] = false;
        }
        numberRequest++;
//        relevance.clear();
    }
    WriteAnswersJSON(answerJSON);
}

void ConverterJSON::WriteAnswersJSON(const nlohmann::json &j) {
    std::fstream mFile;
    mFile.open(mAnswerFileName, std::ios::out);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to create file answers.json");
    } else {
        try {
            mFile << j.dump(4);
            mFile.close();
        } catch (...) {
            throw ExceptionError("Unable to write file answers.json");
        }
    }
}

std::vector<std::string> ConverterJSON::GetRequests() {
    nlohmann::json requestJSON;
    std::fstream mFile;

    int countOfRequests = 0;
    mFile.open(mRequestsFileName);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to read file " + mRequestsFileName);
    } else {
        try {
            mFile >> requestJSON;
        } catch (...) {
            if (mFile.is_open()) mFile.close();
            throw ExceptionError("Error requests in " + mRequestsFileName + " file");
        }
        mFile.close();
    }
    if (requestJSON["requests"].empty() || requestJSON.empty()) {
        std::cout << "WARNING!!: Request file empty" << std::endl;
        return std::vector<std::string>();
    }
    auto endRequestsList = requestJSON["requests"].end();
    if (requestJSON["requests"].size() > MAX_REQUESTS) {
        std::cout << "WARNING!!: To many requests" << std::endl;
        endRequestsList = requestJSON["requests"].begin() + MAX_REQUESTS;
    }

    return std::vector<std::string>(requestJSON["requests"].begin(), endRequestsList);
};

std::vector<std::string> ConverterJSON::GetFileNames() {
    if (!isConfigured) {
        ReadConfig();
    }

    if (mFileNames.empty()) {
        std::cerr << "WARNING: List of filenames is empty." << std::endl;
        return std::vector<std::string>();
    }
    return mFileNames;
}

int ConverterJSON::GetResponsesLimit() {
    if (!isConfigured) {
        ReadConfig();
    }
    return mMaxResponses;
}

std::string ConverterJSON::GetNameOfApp() {
    if (!isConfigured) {
        ReadConfig();
    }
    return mNameOfApp;
}

std::string ConverterJSON::GetVersionApp() {
    if (!isConfigured) {
        ReadConfig();
    }
    return mVersionApp;
}

bool ConverterJSON::IsValidVersion() {
    GetVersionApp();
    return mVersionApp == VERSION_APP;
}

void ConverterJSON::SetConfigFileName(const std::string &fileName) {
    mConfigFileName = fileName;
    isConfigured = false;
}

void ConverterJSON::SetRequestsFileName(const std::string &fileName) {
    mRequestsFileName = fileName;
    isConfigured = false;
}

void ConverterJSON::SetAnswerFileName(const std::string &fileName) {
    mAnswerFileName = fileName;
    isConfigured = false;
}

void ConverterJSON::SetMethodOfSearch(int methodOfSearch) {
    if (methodOfSearch < 1 || methodOfSearch > 3) {
        std::cerr << "WARNING! Method of search maust be from 1 to 3" << std::endl;
        std::cout << "Method search set by 1";
    }
    mMethodOfSearch = methodOfSearch;
}

int ConverterJSON::GetMethodOfSearch() {
    if (!isConfigured) {
        ReadConfig();
    }
    return mMethodOfSearch;
}

void ConverterJSON::SetMaxResponses(int newMaxResponses) {
    if (mMaxResponses <= 0) {
        std::cout << "WARNING!: max_responses cannot be less than or equal to zero" << std::endl;
        std::cout << "set max_responses = 5" << std::endl;
        mMaxResponses = 5;
    } else if (mMaxResponses > MAX_REQUESTS) {
        std::cout << "WARNING!: max_responses cannot be grether than " + MAX_REQUESTS << std::endl;
        std::cout << "set max_responses = " + MAX_REQUESTS << std::endl;
        mMaxResponses = MAX_REQUESTS;
    }
    mMaxResponses = newMaxResponses;
}