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
        PostfixStack.push_back(new ExpressionNode(
            NodeType::Operator, string(1, OperatorStack.top())));
        OperatorStack.pop();
        continue;
      }
    } else if (Lexer.getCode() == Calculator::LeftParen) {
      OperatorStack.push('(');
    } else if (Lexer.getCode() == Calculator::RightParen) {
      while (OperatorStack.top() != '(') {
        PostfixStack.push_back((new ExpressionNode(
            NodeType::Operator, string(1, OperatorStack.top()))));
        OperatorStack.pop();
      }
      OperatorStack.pop(); // Pop unused (
    } else if (Lexer.getCode() == Calculator::FunCall) {
      PostfixStack.push_back(new ExpressionNode(
          NodeType::Number, to_string(calculate(getFuncallPostFix()))));
      continue;
    } else {
      PostfixStack.push_back(
          new ExpressionNode(NodeType::Number, Lexer.getCurNumVal()));
    }
    // Eat every token
    Lexer.lex();
  }
  while (!OperatorStack.empty()) {
    PostfixStack.push_back(
        new ExpressionNode(NodeType::Operator, string(1, OperatorStack.top())));
    OperatorStack.pop();
  }
  return false;
}

/// @brief
/// @param PostfixExp the postfix string vector to be calculated
/// @return
double CalculatorParser::calculate(vector<ExpressionNode *> PostfixExp) {
  stack<double> Tmp;
  double Number1 = 0;
  double Number2 = 0;
  double TmpResult = 0;
  for (int i = 0; i < PostfixExp.size(); i++) {
    if (PostfixExp[i]->getNodeType() == NodeType::Operator) {
      Number1 = Tmp.top();
      Tmp.pop();
      Number2 = Tmp.top();
      Tmp.pop();
      switch (PostfixExp[i]->getValue()[0]) {
      case '+':
        Tmp.push(Number1 + Number2);
        break;
      case '-':
        Tmp.push(Number2 - Number1);
        break;
      case '*':
        Tmp.push(Number2 * Number1);
        break;
      case '/':
        Tmp.push(floor(Number2 / Number1));
        break;
      case '%':
        Tmp.push(int(Number2) % int(Number1));
        break;
      default:
        break;
      }
    } else if (PostfixExp[i]->getNodeType() == NodeType::FunCall) {
      // Add basic function support here
      if (PostfixExp[i + 1]) {
        if (PostfixExp[i]->getValue() == "sin") {
          Tmp.push(sin(std::stod(PostfixExp[i + 1]->getValue())));
        } else if (PostfixExp[i]->getValue() == "cos") {
          Tmp.push(cos(std::stod(PostfixExp[i + 1]->getValue())));
        } else if (PostfixExp[i]->getValue() == "tan") {
          Tmp.push(tan(std::stod(PostfixExp[i + 1]->getValue())));
        } else {
            Reporter.report((string)"Unsupported function: " + "'" + PostfixExp[i]->getValue() + "' ,", Lexer.getLoc());
        }
      }
      i++;
    } else {
      Tmp.push(std::stod(PostfixExp[i]->getValue()));
    }
  }
  return Tmp.top();
}

/// Return a temperary postfix stack to support funcall
/// Bacause funcall argument also receives values to calculate values
std::vector<ExpressionNode *> CalculatorParser::getFuncallPostFix() {
  // Eat until meet ) and the the tmp oprator stack is empty
  vector<ExpressionNode *> Tmp;
  Tmp.push_back(new ExpressionNode(NodeType::FunCall, Lexer.getCurStrVal()));
  Lexer.lex(); // eat funcall token
  stack<char> TmpOperatorStack;
  TmpOperatorStack.push('(');
  Lexer.lex();
  while (!TmpOperatorStack.empty() && Lexer.getCode() != Calculator::Eof) {
    if (isOperator(Lexer.getCode())) {
      // Here to push operator into operator stack
      if (TmpOperatorStack.empty() || TmpOperatorStack.top() == '(') {
        TmpOperatorStack.push(Lexer.getCharByToken(Lexer.getCode()));
      } else if (getPriLev(Lexer.getCode()) >=
                 getPriLev(Lexer.getTokenByChar(TmpOperatorStack.top()))) {
        TmpOperatorStack.push(Lexer.getCharByToken(Lexer.getCode()));
      } else {
        Tmp.push_back(new ExpressionNode(NodeType::Operator,
                                         string(1, TmpOperatorStack.top())));
        TmpOperatorStack.pop();
      }
    } else if (Lexer.getCode() == Calculator::LeftParen) {
      TmpOperatorStack.push('(');
    } else if (Lexer.getCode() == Calculator::RightParen) {
      while (TmpOperatorStack.top() != '(') {
        Tmp.push_back(new ExpressionNode(NodeType::Operator,
                                         string(1, TmpOperatorStack.top())));
        TmpOperatorStack.pop();
      }
      TmpOperatorStack.pop(); // Pop unused (
    } else if (Lexer.getCode() == Calculator::FunCall) {
      Tmp.push_back(new ExpressionNode(
          NodeType::Number, to_string(calculate(getFuncallPostFix()))));

    } else {
      Tmp.push_back(new ExpressionNode(NodeType::Number, Lexer.getCurNumVal()));
    }
    Lexer.lex();
  }
  return Tmp;
}