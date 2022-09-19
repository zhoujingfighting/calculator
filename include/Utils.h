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
#include <map>
namespace Calculator {
enum Token {

  LeftParen,  // (
  RightParen, // )
  Id,         // identifier
  // Basic binary operator
  Plus,     // +
  Multiply, // *
  Minor,    // -
  Divide,   // /
  Eof,      // end of netlist string
  Error,    // lexer error
  SciNumVal, // scientific number
  FloatVal, // float number
  IntVal
};

/// Arithmetic operator priority map
extern std::map<Token, int> OpcodePriMap;

/// Token location including starting line and end column
typedef struct TokenLoc {
  int startLine;
  int endColumn;
} TokenLoc;
} // namespace Calculator
