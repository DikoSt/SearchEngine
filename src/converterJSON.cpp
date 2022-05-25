//
// Created by Denis on 19.04.2022.
//

#include "converterJSON.h"
#include <fstream>
ConverterJSON::ConverterJSON(){};

void ConverterJSON::ReadJsonFile(const std::string &fileName, nlohmann::json &jsonResult) {
    std::fstream mFile;

    mFile.open(mConfigFileName, std::ios::in);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to read file " + fileName);
    } else {
        try {
            mFile >> jsonResult;
        } catch (...) {
            throw ExceptionError("File " + fileName + " corrupted");
        }
        mFile.close();
    }
}

void ConverterJSON::WriteJsonFile(const std::string &fileName, nlohmann::json &jsonToWrite) {
    std::fstream mFile;
    const int SPACES_SEPARATOR = 4;
    mFile.open(mAnswerFileName, std::ios::out);
    if (!mFile.is_open()) {
        throw ExceptionError("Unable to create file answers.json");
    } else {
        try {
            mFile << jsonToWrite.dump(SPACES_SEPARATOR);
            mFile.close();
        } catch (...) {
            throw ExceptionError("Unable to write file answers.json");
        }
    }
}

nlohmann::json ConverterJSON::CreatAnswerJson(const std::vector<std::vector<RelativeIndex>> &answers){
    nlohmann::json answerJson;

    int maxResponses = GetResponsesLimit();
    int numberRequest = 1;
    for (auto &requestAnswer:answers) {
        int response = 0;
        nlohmann::json relevance = nlohmann::json::array();
        for (auto item2 = requestAnswer.begin();
             item2 != requestAnswer.end() && response < maxResponses; ++item2, ++response) {
            relevance.push_back({{"docID", item2->docId},
                                 {"rank",  item2->rank}});
        }
        std::string strNumberRequest =
                "request" + std::string(3 - std::to_string(numberRequest).size(), '0') + std::to_string(numberRequest);

        if (!requestAnswer.empty()) {
            answerJson["answers"][strNumberRequest]["result"] = true;
            answerJson["answers"][strNumberRequest]["relevance"] = relevance;
        } else {
            answerJson["answers"][strNumberRequest]["result"] = false;
        }
        numberRequest++;
    }
    return answerJson;
}
void ConverterJSON::ReadConfig() {
    ReadJsonFile(mConfigFileName, mJsonConfig);
    isConfigured = true;
    if (!IsValidVersion()){
        isConfigured = false;
        mJsonConfig.clear();
    }
}


void ConverterJSON::PutAnswers(const std::vector<std::vector<RelativeIndex>> &answers) {
    if (!isConfigured) {
        ReadConfig();
    }
    auto answersJson = CreatAnswerJson(answers);
    WriteJsonFile(mAnswerFileName, answersJson);
}


std::vector<std::string> ConverterJSON::GetRequests() {
    nlohmann::json requestJSON;
    ReadJsonFile(mRequestsFileName, requestJSON);

try {
    if (requestJSON["requests"].empty() || requestJSON.empty()) {
        std::cout << "WARNING!!: Request file empty" << std::endl;
        return std::vector<std::string>();
    }
    //TODO Ограничения накладывать сверху, в программе не проверять
    auto endRequestsList = requestJSON["requests"].end();
    if (requestJSON["requests"].size() > MAX_REQUESTS) {
        std::cout << "WARNING!!: To many requests" << std::endl;
        endRequestsList = requestJSON["requests"].begin() + MAX_REQUESTS;
    }
    return std::vector<std::string>(requestJSON["requests"].begin(), endRequestsList);
} catch (...) {
    //TODO error в вектор ошибок записывать
    throw ExceptionError("Error config in " + mConfigFileName + " file.");
}
};

std::vector<std::string> ConverterJSON::GetFileNames() {
    if (!isConfigured) {
        ReadConfig();
    }
    if (mJsonConfig.empty()) {
        std::cout << "WARNING!!: List of file names is empty" << std::endl;
        return std::vector<std::string>();
    }
       try {
         return std::vector<std::string>(mJsonConfig["files"].begin(), mJsonConfig["files"].end());
    } catch (...) {
        //TODO error в вектор ошибок записывать
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
    }
}

int ConverterJSON::GetResponsesLimit() {
    if (!isConfigured) {
        ReadConfig();
    }
    if (!mJsonConfig.empty()) {
        try {
            return mJsonConfig["config"]["max_responses"];
        } catch (...) {
            throw ExceptionError("Error config in " + mConfigFileName + " file.");
        }
    }else {
        return 0;
    }
}

std::string ConverterJSON::GetNameOfApp() {
    if (!isConfigured) {
        ReadConfig();
    }
    if (!mJsonConfig.empty()) {
        try {
        return mJsonConfig["config"]["name"];
    } catch (...) {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
		}
	}else{
	return std::string();
	}
}

std::string ConverterJSON::GetVersionApp() {
    if (!isConfigured) {
        ReadConfig();
    }
    if (!mJsonConfig.empty()) {
        try {
        return mJsonConfig["config"]["version"];
    } catch (...) {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
		}
	}else{
        return std::string();
	}
}

bool ConverterJSON::IsValidVersion() {
    if (!isConfigured) {
        ReadConfig();
    }
    return GetVersionApp() == VERSION_APP;
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

int ConverterJSON::GetMethodOfSearch() {
    if (!isConfigured) {
        ReadConfig();
    }
    int methodOfSearch;
    if (!mJsonConfig.empty()) {
        try {
        methodOfSearch = mJsonConfig["config"]["method_search"];
        if (methodOfSearch >= 1 && methodOfSearch <=3) {
            return methodOfSearch;
        }
        else {
             std::cout << "WARNING!!! Method of search must be from 1 to 3. Set 1." << std::endl;
             return 1;
        }
    } catch (...) {
        throw ExceptionError("Error config in " + mConfigFileName + " file.");
		}
	}else{
		return 0;	
	}
}