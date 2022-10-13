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
#include <cmath>
#include <stack>
bool CalculatorParser::parse(string Content) {
  // before parse, we must clear the cached error message
  Reporter.setErrorMsg("");
  if (Lexer.init(Content))
    cout << "Calculator init failed" << endl;
  // Start recursively parsing
  Lexer.lex();
  if (parseExpression())
    return true;
  // false means the parse procedure goes well
  return false;
}

/// @brief Parse according to the lexter tokens, this is the whole entry point
/// for the parse program, and get positfix expression
/// @return a boolean flag to indicate whether the calculator process succeeded
bool CalculatorParser::parseExpression() {
  while (Lexer.getCode() != Calculator::Eof) {
    // Eat token utils the content string ends
    if (isOperator(Lexer.getCode())) {
      // Here to push operator into operator stack
      if (OperatorStack.empty() || OperatorStack.top() == '(') {
        OperatorStack.push(Lexer.getCharByToken(Lexer.getCode()));
      } else if (getPriLev(Lexer.getCode()) >=
                 getPriLev(Lexer.getTokenByChar(OperatorStack.top()))) {
        OperatorStack.push(Lexer.getCharByToken(Lexer.getCode()));
      } else {
        PostfixStack.push_back(string(1,OperatorStack.top()));
        OperatorStack.pop();
        continue;
      }
    } else if (Lexer.getCode() == Calculator::LeftParen) {
      OperatorStack.push('(');
    } else if (Lexer.getCode() == Calculator::RightParen) {
      while (OperatorStack.top() != '(') {
        PostfixStack.push_back(string(1, OperatorStack.top()));
        OperatorStack.pop();
      }
      OperatorStack.pop(); // Pop unused (
    }
    else {
      PostfixStack.push_back(Lexer.getCurNumVal());
    }
    // Eat every token
    Lexer.lex();
  }
  while(!OperatorStack.empty()){
     PostfixStack.push_back(string(1, OperatorStack.top()));
     OperatorStack.pop();
  }
  return false;
}

/// @brief
/// @param PostfixExp the postfix string vector to be calculated
/// @return
double CalculatorParser::calculate(vector<string> PostfixExp) {
  stack<double> Tmp;
  double Number1 = 0;
  double Number2 = 0;
  double TmpResult = 0;
  for (int i = 0; i < PostfixExp.size(); i++) {
    if (PostfixExp[i][0] == '+' && PostfixExp[i].size() == 1) {
      Number1 = Tmp.top();
      Tmp.pop();
      Number2 = Tmp.top();
      Tmp.pop();
      Tmp.push(Number1 + Number2);
    }
    else if (PostfixExp[i][0] == '-' && PostfixExp[i].size() == 1) {
      Number1 = Tmp.top();
      Tmp.pop();
      Number2 = Tmp.top();
      Tmp.pop();
      Tmp.push(Number2 - Number1);
    }
    else if (PostfixExp[i][0] == '/' && PostfixExp[i].size() == 1) {
      Number1 = Tmp.top();
      Tmp.pop();
      Number2 = Tmp.top();
      Tmp.pop();
      Tmp.push(floor(Number2 / Number1));
    }
    else if (PostfixExp[i][0] == '*' && PostfixExp[i].size() == 1) {
      Number1 = Tmp.top();
      Tmp.pop();
      Number2 = Tmp.top();
      Tmp.pop();
      Tmp.push(Number1 * Number2);
    } else {
      Tmp.push(std::stod(PostfixExp[i]));
    }
  }
  return Tmp.top();
}