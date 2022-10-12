/**
 * @file parser.cpp
 * @author Zhou Jing (254644528@qq.com)
 * @brief The parser of calculator
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "parser.h"

using namespace std;
bool CalculatorParser::parse(string Content) {
  ErrorMsg = "";
  if (Lexer.init(Content))
    cout << "Calculator init failed" << endl;
  // Start recursively parsing
  Lexer.lex();
  while (!parseCalculatorContent()) {
    // Do nothing
  }
  if (ErrorMsg.size() >= 1) {
    // THis means error happens when parsing the content
    cerr << "Error happened" << endl;
  }
  // false means the parse procedure goes well
  return false;
}

bool CalculatorParser::parseCalculatorContent() {
  switch (Lexer.getCode()) {
  case Calculator::Token::Eof:
    return true;

  default:
    setErrorMsg("TEst");
    return true;
  }
}
