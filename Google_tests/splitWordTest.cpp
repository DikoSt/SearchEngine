
#include "gtest/gtest.h"
#include "stringutilites.h"

using namespace std;

void testPrivateMethod(const std::string &strInput, const std::vector<std::string> &expected) {
    ASSERT_EQ(SplitIntoWords(strInput), expected);
};

TEST(TestCaseSplitIntoWord, SplitIntoWord_oneSpace){
    const std::string sInput = "a b c d e f";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_empty){
    const std::string sInput = "";
    const std::vector<std::string> sExpected = {};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_spaceStart){
    const std::string sInput = " a b c d e f";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_spaceEnd){
    const std::string sInput = "a b c d e f ";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_TwoTreeSpaces){
    const std::string sInput = "a  b   c      d      e     f";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_TwoStartSpaces){
    const std::string sInput = "  a b c d e f";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_TreeStartSpaces){
    const std::string sInput = "   a b c d e f";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_TwoEndtSpaces){
    const std::string sInput = "a b c d e f  ";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}

TEST(TestCaseSplitIntoWord, SplitIntoWord_TreeEndSpaces){
    const std::string sInput = "a b c d e f   ";
    const std::vector<std::string> sExpected = {"a", "b", "c", "d", "e", "f"};
    testPrivateMethod(sInput, sExpected
);
}
