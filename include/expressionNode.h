/**
 * @file ec
 * @author Zhou Jing (254644528@qq.com)
 * @brief The expression nodes use in caculating expression
 * @version 0.1
 * @date 2022-10-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef EXPRESSION_NODE_H
#define EXPRESSION_NODE_H

#include <string>
enum NodeType {
  Operator,
  Number,
  FunCall,
};
/// Currently we only define belows nodes
/// Operator, Number, FunCall
class ExpressionNode {
  CalculatorLexer Lexer;
  NodeType Type;
  std::string Value;
  Calculator::TokenLoc Token;

public:
  ExpressionNode(NodeType Type, std::string Value)
      : Type(Type), Value(Value), Token(Lexer.getLoc()){};
  NodeType getNodeType() { return Type; };
  std::string getValue() { return Value; };
  Calculator::TokenLoc getToken() { return Token; };
};

#endif