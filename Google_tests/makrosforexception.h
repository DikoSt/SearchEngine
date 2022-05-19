#pragma once

// макрос для проверки генерации исключения и сообщения которое оно
// генерирует

#define ASSERT_EXCEPTION(TRY_BLOCK, EXCEPTION_TYPE, MESSAGE)        \
try                                                                   \
{                                                                     \
    TRY_BLOCK                                                         \
    FAIL() << "exception '" << MESSAGE << "' not thrown at all!";     \
}                                                                     \
catch( const EXCEPTION_TYPE& e )                                      \
{                                                                     \
    EXPECT_EQ( MESSAGE, e.what() )                                    \
        << " exception message is incorrect. Expected the following " \
           "message:\n"                                               \
        << MESSAGE << "\n"                                            \
        << "Presetn: \n"<< e.what() <<"\n";                                           \
}                                                                     \
catch( ... )                                                          \
{                                                                     \
    FAIL() << "exception '" << MESSAGE                                \
           << "' not thrown with expected type '" << #EXCEPTION_TYPE  \
           << "'!";                                                   \
}
