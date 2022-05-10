#pragma once
#include <iostream>

class ExceptionError: public std::exception{
protected:
    std::string mMsg;
public:
    ExceptionError() {
        mMsg = "Error!!!";
    }
    ExceptionError(const std::string &msg) {
        mMsg = "Error: " + msg;
    }
    const char* what() const noexcept override
    {
        return mMsg.c_str();
    }
};

//
//class ExceptionWarning: public std::exception{
//protected:
//    std::string mMsg;
//public:
//    ExceptionWarning(){
//        mMsg = "Warning!!! ";
//    }
//    ExceptionWarning(const std::string &msg){
//       mMsg = "Warning: " + msg;
//    }
//
//    const char* what() const noexcept override
//    {
//
//        return mMsg.c_str();
//    }
//};