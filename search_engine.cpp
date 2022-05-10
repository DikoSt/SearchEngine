#include <iostream>
#include <nlohmann/json.hpp>
#include "converterJSON.h"
#include "invertedindex.h"
#include "searchserver.h"
#include "exception.h"

int main() {
ConverterJSON converterJson;
InvertedIndex invertedIndex;

try {
    if (!converterJson.IsValidVersion()) {
        std::cerr << "Version application not equeal"<< std::endl;
        std::cout << "Exit with error." << std::endl;
        return 0;
    }

    std::cout << "Start " << converterJson.GetNameOfApp() << std::endl;
    std::cout << "Version: " << converterJson.GetVersionApp() << std::endl;
    std::cout << "Max request limit: " << converterJson.GetResponsesLimit() << std::endl;
    std::cout << "Method search: " << converterJson.GetMethodOfSearch() << std::endl;

    invertedIndex.UpdateDocumentBase(converterJson.GetTextDocuments(), 8);
} catch (ExceptionError &e) {
    std::cerr << e.what() << std::endl;
    exit(0);
}

SearchServer searchServer(invertedIndex);

    std::vector<std::string> queries =converterJson.GetRequests();
    std::vector<std::vector<RelativeIndex>> rIndex = searchServer.Search(queries, converterJson.GetMethodOfSearch());
    converterJson.PutAnswers(rIndex);

    return 0;
}
