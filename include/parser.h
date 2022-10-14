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
#include "expressionNode.h"
#include <stack>
#include <vector>
class CalculatorParser {
  stack<char> OperatorStack;
  stack<double> NumberStack;
  std::vector<ExpressionNode*> PostfixStack;
  CalculatorLexer Lexer;
  ErrorReporter Reporter;
  // Final caculated result
  double Result;

public:
  /// Entry point for parsing calculators
  /// the argument is the calculator content
  bool parse(string);
  bool parseExpression();
  std::vector<ExpressionNode*> getPostFix() { return PostfixStack; };
  std::vector<ExpressionNode*> getFuncallPostFix();
  double calculate(vector<ExpressionNode*>);
  bool isOperator(Calculator::Token Tok) {
    switch (Tok) {
    case Calculator::Plus:
    case Calculator::Minor:
    case Calculator::Multiply:
    case Calculator::Divide:
    case Calculator::Remain:
    case Calculator::Pow:
    case Calculator::Fac:
      return true;
    default:
      return false;
    }
  }

  bool isNumber(Calculator::Token Tok) {
    switch (Tok) {
    case Calculator::FloatVal:
    case Calculator::IntVal:
    case Calculator::SciNumVal:
      return true;
    default:
      return false;
    }
  }
};

#endif // PARSER_H
