/**
 * @file parser.h
 * @author Zhou Jing (254644528@qq.com)
 * @brief The parser header file of calculator
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
class CalculatorParser {

  CalculatorLexer Lexer;
  ErrorReporter Reporter;
  std::string ErrorMsg;

public:
  /// Entry point for parsing calculators
  /// the argument is the calculator content
  bool parse(string);
  bool parseCalculatorContent();
  void setErrorMsg(string Msg) { ErrorMsg = Msg; };
  string getErrorMsg(string Msg) { return ErrorMsg; };
};

#endif // PARSER_H
