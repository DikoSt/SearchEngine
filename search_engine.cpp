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

    searchServer.SetMethodOfSearch(converterJson.GetMethodOfSearch());
    searchServer.SetMaxRequests(converterJson.GetResponsesLimit());
    auto  queries =converterJson.GetRequests();
    auto rIndex = searchServer.Search(queries);
    converterJson.PutAnswers(rIndex);
    return 0;
}
