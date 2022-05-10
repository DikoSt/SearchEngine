#include "invertedindex.h"
#include "stringutilites.h"
#include "searchserver.h"
#include "gtest/gtest.h"

using namespace std;
TEST(TestCaseMapCompare, TwoSameElements){
    std::map<int,float> firstMap{{1,1},{2,2},{3,3}};
    std::map<int,float> secondMap{{1,1},{2,2},{3,3}};
    ASSERT_EQ(firstMap, secondMap);
}

TEST(TestCaseSearchServer, Test_EmptyQuery_BM25) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {" "};
    const std::vector<std::vector<size_t>> expectedV ={{}};

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(1);
    std::vector<std::vector<RelativeIndex>> res = srv.Search(request);

    std::vector<std::vector<size_t>> result;
    for (auto item:res){
        result.push_back(std::vector<size_t>());
        for (auto rIndex:item){
            result.back().push_back(rIndex.docId);
        }
    }

    ASSERT_EQ(result, expectedV);
}

TEST(TestCaseSearchServer, Test_MissingQuery_BM25) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water",
            "milk water water",
            "milk milk milk milk milk water "
    };
    const vector<string> request = {"back"};
    const std::vector<std::vector<size_t>> expectedV ={ {} };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(1);
    std::vector<std::vector<RelativeIndex>> res = srv.Search(request);

    std::vector<std::vector<size_t>> result;
    for (auto item:res){
        result.push_back(std::vector<size_t>());
        for (auto rIndex:item){
            result.back().push_back(rIndex.docId);
        }
    }

    ASSERT_EQ(result, expectedV);
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
    const std::vector<std::vector<size_t>> expectedV ={
            {9,2,0,1,5,4,8,6,7},
            {8},
            {8,9,2,0,1,5,4,6,7}
    };

    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.8},
                    {9, 0.4},
                    {1,0.3},
                    {8,0.2},
                    {7,0.1},
                    {6, 0.1},
                    {5,0.1},
                    {4, 0.1},
            },
            {
                    {8,1}
            },
            {
                    {2,1},
                    {0,0.8},
                    {9,0.4},
                    {8,0.3},
                    {1,0.3},
                    {7,0.1},
                    {6,0.1},
                    {5,0.1},
                    {4,0.1},
            }

    };

    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(1);
    std::vector<std::vector<RelativeIndex>> res = srv.Search(request);

    std::vector<std::vector<size_t>> result;
    for (auto item:res){
        result.push_back(std::vector<size_t>());
        for (auto rIndex:item){
            result.back().push_back(rIndex.docId);
        }
    }

    ASSERT_EQ(result, expectedV);
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
                    {7, 1}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(1);
    std::vector<vector<RelativeIndex>> result = srv.Search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestSimple_method2) {
    const vector<string> docs = {
            "milk milk milk milk milk water water water", //0
            "milk water water",                           //1
            "milk milk milk milk milk water water water water water", //2
            "Americano Cappuccino", //3
            "americano milk", //4
            "coppucino water", //5
            "water", //6
            "milk", //7
            "milk water sugar", //8
            "milk water milk water" //9
    };
    const vector<string> request = {"milk water", "sugar", "milk water sugar"};
    const std::vector<vector<RelativeIndex>> expected = {
            {
                    {2, 1},
                    {0, 0.8},
                    {9, 0.4},
                    {1,0.3},
                    {8,0.2},
                    {4,0.1},
                    {5, 0.1},
                    {6,0.1},
                    {7, 0.1},
            },
            {
                    {8,1}
            },
            {
                    {2,1},
                    {0,0.8},
                    {9,0.4},
                    {8,0.3},
                    {1,0.3},
                    {4,0.1},
                    {5,0.1},
                    {6,0.1},
                    {7,0.1},
            }

    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(3);
    std::vector<vector<RelativeIndex>> result = srv.Search(request);
    ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
const vector<string> docs = {
        "milk milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "Americano Cappuccino",
        "americano milk",
        "coppucino water",
        "water",
        "milk",
        "milk water sugar",
        "milk water milk water"
};
const vector<string> request = {"milk water", "sugar", "milk water sugar"};
const std::vector<vector<RelativeIndex>> expected = {
        {
                {2, 1},
                {0, 0.8},
                {9, 0.4},
                {1,0.3},
                {8,0.2}
        },
        {
                {8,1}
        },
        {
                {8,1}
        }
};
InvertedIndex idx;
idx.UpdateDocumentBase(docs);
SearchServer srv(idx);
    srv.SetMethodOfSearch(2);
std::vector<vector<RelativeIndex>> result = srv.Search(request);
ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestTop5) {
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
                    {14,1},
                    {29,1},
                    {36,1},
                    {51,1}
            }
    };
    InvertedIndex idx;
    idx.UpdateDocumentBase(docs);
    SearchServer srv(idx);
    srv.SetMethodOfSearch(3);
    std::vector<vector<RelativeIndex>> result = srv.Search(request);

    for (auto &vector1 : result){
        for (auto &v2: vector1){
            std::cout << "[" << v2.docId << ", " << v2.rank << "]" << std::endl;
        }
        std::cout << std::endl;
    }
    ASSERT_EQ(result, expected);
std::cout << "Test pass" << std::endl;
}