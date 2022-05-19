//
// Created by Denis on 19.04.2022.
//
/**Программа создания множества тестовых файлов из одного большого*/
/** количество слов в каждом файле 1000 */
#include <iostream>
#include <fstream>
#include <string>
#include "config.h"

// пропустить все символы кроме букв
#define SKIP_SPEC_SYMBOL
// перобразовать все буквва с прописные
#define CONVERT_TO_LOWER_CASE


#ifdef SKIP_SPEC_SYMBOL

std::string skipSpecSymbols(const std::string &s1) {
    std::string result;
    for (const auto &c:s1) {
        //  if ((c >= '!' && c<='//') || (c>=':' && c<='`') || (c>='[' && c<='')  ) result += c;
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) {
#ifdef CONVERT_TO_LOWER_CASE
            result += tolower(c);
#else
            result += c;
#endif // CONVERT_TO_LOWER_CASE
        }
    }
    return result;
}

#endif  // SKIP_SPEC_SYMBOL

int main() {
    std::string pathToFile = "../../resource/";
    std::ifstream inputFile("../../resource/The Number of the Beast_.txt");
    int numberOfFile = 1;
    int counterWords = 0;
    size_t maxWordsInFile = 100 + std::rand() % (MAX_WORDS_IN_FILE - 100);

    if (inputFile.is_open()) {
        std::string currWord, currOutputString;

        while (!inputFile.eof()) {
            inputFile >> currWord;

#ifdef SKIP_SPEC_SYMBOL
            currOutputString += skipSpecSymbols(currWord) + " ";
#else
            currOutputString += currWord + " ";
#endif
            counterWords++;
            if (counterWords == maxWordsInFile || inputFile.eof()) {
                std::ofstream outputFile(pathToFile + "file" +
                                         std::string(5 - std::to_string(numberOfFile).size(), '0') +
                                         std::to_string(numberOfFile) + ".txt");

                if (outputFile.is_open()) {
                    outputFile << currOutputString;
                    outputFile.close();
                    numberOfFile++;
                    currOutputString = "";
                    counterWords = 0;

                    maxWordsInFile = 100 + std::rand() % (MAX_WORDS_IN_FILE - 100);
                } else {
                    std::cerr << "ERROR!!: Can not write to file " + pathToFile + std::to_string(numberOfFile)
                              << std::endl;
                }
            }
        }

    } else {
        std::cerr << "ERROR!!! : Can not open file " << std::endl;
        return 0;
    }

    std::cout << "OK";

    return 0;

}

