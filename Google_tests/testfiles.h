#pragma once
#include "nlohmann/json.hpp"
// файл для проверки метода ConverterJSON.GetFilePathes()
nlohmann::json requestFile = {
    {"requests", [
        "file001.txt",
        "file002.txt",
        "file003.txt",
        "file004.txt",
        "file005.txt",
        "file006.txt",
        "file007.txt",
        "file008.txt",
    ]}
}