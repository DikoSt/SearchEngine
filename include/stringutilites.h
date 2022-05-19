#pragma once

#include <vector>
#include <iostream>
#include <string>

/**
 * Функция разделяет входную строку на слова
 * @param text - строка-текст;
 * @return вектор слов;
 */
std::vector<std::string> SplitIntoWords(const std::string &text);

/**
* Функция преобразования строки в набор слов разделённых пробелом, преобразует заглавные буквы в строчные,
* игнорирует все символы кроме букв латинского алфавита
* @input исходная строка
* @return строку слов разделённых пробелами, без знаков припинания, регист low
*/
std::string skipSpecSymbols(const std::string &inputString);