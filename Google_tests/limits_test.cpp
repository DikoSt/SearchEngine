//
// Created by Denis on 18.04.2022.
//

#include "gtest/gtest.h"
#include "config.h"

/** Проверка соответствия всех лимитных утверждений */
/** Максимальные и другие ограничеия получены из ТЗ,
 * так же предполагпается в будущем, что могут зависеть
 * от версии программы и других условий развия продукта
**/

/** каждый документ содержит не более 1000 слов */
TEST(limitsCase, maxWordsInFile){
    EXPECT_EQ(MAX_WORDS_IN_FILE,1000);
}
/** с максимальной длиной каждого в 100 символов. */
TEST(limitsCase, maxCharInWord){
    EXPECT_EQ(MAX_CHAR_IN_WORD, 100);
}
/** Поле спска запросов содержит не более 1000 запросов. */
TEST(limitsCase, maxRequests){
    EXPECT_EQ(MAX_REQUESTS,1000);
}
/** Каждый запрос включает от одного до десяти слов */
TEST(limitsCase, maxWordRequest){
    EXPECT_EQ(MAX_WORDS_REQ,10);
}