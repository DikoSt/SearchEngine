#include "stringutilites.h"

std::vector<std::string> SplitIntoWords(const std::string &text) {
    std::vector<std::string> words;
    std::string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
            }
            word.clear();
        } else {
            word.push_back(c);
        }
    }
    if (!word.empty()) words.push_back(word);
    return words;
}

std::string skipSpecSymbols(const std::string &inputString) {
    std::string resultString;
    for (const auto &character:inputString) {
        //  if ((c >= '!' && c<='//') || (c>=':' && c<='`') || (c>='[' && c<='')  ) result += c;
        if ((character >= 'A' && character <= 'Z') || (character >= 'a' && character <= 'z')) {
            resultString += tolower(character);
        }
    }
    return resultString;
}