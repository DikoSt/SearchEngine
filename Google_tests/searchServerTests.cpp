#include "invertedindex.h"
#include "stringutilites.h"
#include "searchserver.h"
#include "gtest/gtest.h"

using namespace std;
//#define PRINT_RESULT
//#define PRINT_STRING_RESULT

string GenerateFileName_(const string &fileName, int numberFile) {
    return fileName +
           std::string(3 - std::to_string(numberFile).size(), '0') + std::to_string(numberFile) + ".txt";
}

void PrintResult(std::vector<vector<RelativeIndex>> result) {
    for (auto &vect:result) {
        std::cout << "{" << std::endl;
        for (auto &rlidx:vect) {
            std::cout << "{" << rlidx.docId << ", " << rlidx.rank << "}," << std::endl;
        }
        std::cout << "}," << std::endl;
    }
}

void
PrintResult(std::vector<vector<RelativeIndex>> result, const vector<string> &requests, const vector<string> &Docs) {
    int request = 0;
    for (auto &vect:result) {
        std::cout << "Request: \"" << requests[request] << "\":" << std::endl;
        for (auto &rlidx:vect) {
            std::cout << rlidx.docId << ": \t\"" << Docs[rlidx.docId] << "\"; " << rlidx.rank << std::endl;
        }
        request++;
    }
}

void TestSearchServerFunctionality(
        const vector<string> &Docs,
        const vector<string> &requests,
        const std::vector<vector<RelativeIndex>> &expected,
        int methodSearch,
        int maxRequests) {
    std::vector<vector<RelativeIndex>> result;
    std::vector<std::string> fileNames({});
    InvertedIndex idx;

    std::fstream file;
// создадим файлы для проверки
    int numberFile = 1;
    for (const auto &oneDoc:Docs) {
        string fileName = GenerateFileName_("testfile", numberFile);
        fileNames.push_back(fileName);
        file.open(fileName, std::ios::out);
        file << oneDoc;
        file.close();
        numberFile++;
    }
    idx.UpdateDocumentBase(fileNames, 2);

//удалим ненужные файлы
    for (const auto &fileName:fileNames) {
        std::remove(fileName.c_str());
    }

    SearchServer srv(idx);
    srv.SetMethodOfSearch(methodSearch);
    srv.SetMaxRequests(maxRequests);
    result = srv.Search(requests);


#ifdef PRINT_RESULT
    PrintResult(result);
#endif
#ifdef PRINT_STRING_RESULT
    PrintResult(result,requests,Docs);
#endif


    ASSERT_EQ(result, expected);
};




TEST(TestCaseMapCompare, TwoSameElements) {
    std::map<int, float> firstMap{{1, 1},
                                  {2, 2},
                                  {3, 3}};
    std::map<int, float> secondMap{{1, 1},
                                   {2, 2},
                                   {3, 3}};
    ASSERT_EQ(firstMap, secondMap);
}

/// TEST Method 1 (BM_25) ***************************************************************************************

TEST(TestCaseSearchServer, BM25_Test_EmptyQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const std::vector<vector<RelativeIndex>> expected = {};
    const vector<string> request = {};
    TestSearchServerFunctionality(docs, request, expected, 1, docs.size());
}

TEST(TestCaseSearchServer, BM25_Test_MissingQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<vector<RelativeIndex>> expected = {{}};
    TestSearchServerFunctionality(docs, request, expected, 1, docs.size());
}


TEST(TestCaseSearchServer, TestSimple_BM25) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water a a a a a a a a a a a a a a a a a a a a a a a a a a", //0
            "milk water water a a a a a a a a a a a a a a a a aa  aa a a a a a a a  a a a a a a",                           //1
            "milk milk milk milk milk water water water water water a a  aa a a a a a a a a a a a a a a a a a aa a a a a a ", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "coppucino water", //5
            "water  a a  aa a a a a a a a a a a a a a a a a a aa a a a a a ", //6
            "milk a a  aa a a a a a a a a a a a a a a a a a aa a a a a aa a  aa a a a a a a a a a a a a a a a a a aa a a a a a", //7
            "milk water sugar a a  aa a a a a a a a a a a a a a a a a a aa a a a a a", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<std::vector<size_t>> expectedV = {
            {9, 2, 0, 1, 5, 4, 8, 6, 7},
            {8},
            {8, 9, 2, 0, 1, 5, 4, 6, 7}
    };

    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {9, 1},
                    {2, 0.873},
                    {0, 0.802},
                    {1, 0.471},
                    {5, 0.424},
                    {4, 0.424},
                    {8, 0.4},
                    {6, 0.208},
                    {7, 0.138},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
                    {9, 0.591},
                    {2, 0.516},
                    {0, 0.474},
                    {1, 0.278},
                    {5, 0.25},
                    {4, 0.25},
                    {6, 0.123},
                    {7, 0.082},
            }

    };

    TestSearchServerFunctionality(docs, request, expected, 1, docs.size());
}

TEST(TestCaseSearchServer, TestTop5_BM25) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {73, 1},
                    {51, 1},
                    {29, 1},
                    {7, 1},
                    {80, 0.746}
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 1, 5);
}

/// TEST Method 2 ***************************************************************************************

TEST(TestCaseSearchServer, Method2_Test_EmptyQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const std::vector<vector<RelativeIndex>> expected = {};
    const vector<string> request = {};
    TestSearchServerFunctionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_Test_MissingQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<vector<RelativeIndex>> expected = {{}};
    TestSearchServerFunctionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 2, docs.size());
}

TEST(TestCaseSearchServer, Method2_TestResultCut) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 2, 3);
}


TEST(TestCaseSearchServer, Method2_TestRsult0) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
            },
            {
                    {8, 1}
            },
            {
                    {8, 1},
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 2, 0);
}


TEST(TestCaseSearchServer, Method2_TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland",
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"

    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {29, 1},
                    {51, 1},
                    {73, 1}
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 2, docs.size());
}
/// TEST Method 3 ***************************************************************************************

TEST(TestCaseSearchServer, Method3_Test_EmptyQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const std::vector<vector<RelativeIndex>> expected = {};
    const vector<string> request = {};
    TestSearchServerFunctionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_Test_MissingQuery) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<vector<RelativeIndex>> expected = {{}};
    TestSearchServerFunctionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_TestSimple) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2},
                    {4, 0.1},
                    {5, 0.1},
                    {6, 0.1},
                    {7, 0.1},
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.3},
                    {4, 0.1},
                    {5, 0.1},
                    {6, 0.1},
                    {7, 0.1},
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 3, docs.size());
}

TEST(TestCaseSearchServer, Method3_TestResultCut) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4}
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4}
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 3, 3);
}


TEST(TestCaseSearchServer, Method3_TestRsult0) {
    const vector<string> docs = {
            "milk milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "cappuccino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.2}
            },
            {
                    {8, 1}
            },
            {
                    {2, 1},
                    {0, 0.9},
                    {9, 0.4},
                    {1, 0.3},
                    {8, 0.3}
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 3, 0);
}


TEST(TestCaseSearchServer, Method3_TestTop5) {
    const vector<string> docs = {
            "london is the capital of great britain",
            "paris is the capital of france",
            "berlin is the capital of germany",
            "rome is the capital of italy",
            "madrid is the capital of spain",
            "lisboa is the capital of portugal",
            "bern is the capital of switzerland",
            "moscow is the capital of russia",
            "kiev is the capital of ukraine",
            "minsk is the capital of belarus",
            "astana is the capital of kazakhstan",
            "beijing is the capital of china",
            "tokyo is the capital of japan",
            "bangkok is the capital of thailand",
            "welcome to moscow the capital of russia the third rome",
            "amsterdam is the capital of netherlands",
            "helsinki is the capital of finland",
            "oslo is the capital of norway",
            "stockholm is the capital of sweden",
            "riga is the capital of latvia",
            "tallinn is the capital of estonia",
            "warsaw is the capital of poland"
    };
    const vector<string> request = {"moscow is the capital of russia"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {7, 1},
                    {14, 1},
                    {0, 0.667},
                    {1, 0.667},
                    {2, 0.667}
            }
    };
    TestSearchServerFunctionality(docs, request, expected, 3, 5);
}