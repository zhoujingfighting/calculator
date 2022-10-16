/**
 * @file util.h
 * @author Zhou Jing (254644528@qq.com)
 * @brief The lexer header file of calculator
 * @version 0.1
 * @date 2022-09-10
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <map>
namespace Calculator {
enum Token {

  LeftParen,  // (
  RightParen, // )
  Id,         // identifier
  // Basic binary operator
  Plus,      // +
  Multiply,  // *
  Minor,     // -
  Divide,    // /
  Remain,    // %
  Pow,       // ^
  Fac,       // !
  Eof,       // end of netlist string
  Error,     // lexer error
  SciNumVal, // scientific number
  FloatVal,  // float number
  IntVal,
  // TODO:function using
  FunCall
};

/// Arithmetic operator priority map
extern std::map<Token, int> OpcodePriMap;

/// Token location including starting line and end column
typedef struct TokenLoc {
  int startLine;
  int endColumn;
} TokenLoc;
} // namespace Calculator

/// @brief The operator priority level
enum PRIO_LEVEL {
  PRIO_LV0,
  PRIO_LV1,
  PRIO_LV2,
  PRIO_LV3,
  PRIO_LV4,
  PRIO_LV5,
};

static PRIO_LEVEL getPriLev(Calculator::Token Tok) {
  switch (Tok) {
  case Calculator::Plus:
  case Calculator::Minor:
    return PRIO_LEVEL::PRIO_LV1;
  case Calculator::Multiply:
  case Calculator::Divide:
  case Calculator::Remain:
    return PRIO_LEVEL::PRIO_LV2;
  case Calculator::Pow:
    return PRIO_LEVEL::PRIO_LV3;    
  case Calculator::Fac:
    return PRIO_LEVEL::PRIO_LV4;    
  default:
    return PRIO_LEVEL::PRIO_LV5;
  }
}
#endif // UTILS_H