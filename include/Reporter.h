/**
 * @file ErrorReporter.h
 * @author Zhou Jing (254644528@qq.com)
 * @brief The error reporter for this calculator
 * @version 0.1
 * @date 2022-09-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef ERRORREPORTER_H
#define ERRORREPORTER_H

#include "Utils.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

class ErrorReporter {

  static string ErrorMsg;

public:
  ErrorReporter(){};

  string report(std::string msg, Calculator::TokenLoc loc) {
    std::string Content = msg + "Line: " + to_string(loc.startLine) + ", " +
                          "EndColumn: " + to_string(loc.endColumn);
    // This is to print message in the console
    std::cerr << Content << std::endl;
    setErrorMsg(Content);
    return Content;
  }

  /// Value can be string or a single char
  /// Lexer error can be printed in console
  /// The final error will be thrown by parser
  template <typename T>
  string report(std::string msg, Calculator::TokenLoc loc, T value,
                bool isLexerError = false) {
    std::string Content =
        msg + "Line: " + to_string(loc.startLine) + (string) "," +
        "EndColumn: " + to_string(loc.endColumn) + " ,Value: " + value;
    // This is to print message in the console
    std::cerr << Content << std::endl;
    setErrorMsg(Content);
    return Content;
  }

  string getErrorMsg() { return ErrorMsg; }

  static void setErrorMsg(string Msg) { ErrorMsg = Msg; }
};
#endif // ERRORREPORTER_H
