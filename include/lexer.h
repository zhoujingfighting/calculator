/**
 * @file lexer.h
 * @author zhou jing (254644528@qq.com)
 * @brief The lexer header file of calculator
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef CALCULATORLEXER_H
#define CALCULATORLEXER_H

#include "Reporter.h"
#include "Utils.h"
#include <assert.h>
#include <string>

using namespace std;

/// @brief basic class for spice lexers, define some functions to be implemented
class CalculatorLexer {
  /// The cursor index of the input buffer
  const char *CurPtr = nullptr;
  string CurBuf;

  /// Information about the current token
  const char *TokStart = nullptr;
  Calculator::Token CurCode = Calculator::Token::Eof;
  string CurStrVal;
  string CurNumberVal;
  int StartLine = 1;
  int EndColumn = 1;
  ErrorReporter Reporter;

public:
  ~CalculatorLexer() = default;
  /// Init the lexer source buffer with input file
  bool init(string FileName);

  Calculator::Token lex();

  Calculator::Token getCode();
  /// Get current token location
  Calculator::TokenLoc getLoc();

  /// ADS parameters list grammar is LL(2), e.g:
  /// `parameters Temp= Noise=1`
  /// We need to lookahead to second `=` to decide whether `Noise` is a
  /// right value variable or left value parameter.
  Calculator::Token lookAhead(int Count);

  const string &getCurStrVal();

  const char getCharByToken(Calculator::Token Tok);

  const string getStrByToken(Calculator::Token Tok);

  /// Get char literal by token code, used to reconstruct content in AST.
  /// Token can appear in general expression are handled in this function
  const char getCurCodeChar();

  /// Get string literal by token code, used to reconstruct content in AST.
  /// Token can appear in any expression are handled in this function
  const string getCurCodeStr();

  /// Get Current char pointer
  const char *getCurPtr();

  /// Get Current char pointer
  const char *getCurTokStart();

  /// Read the next token and return its code.
  Calculator::Token lexToken(bool IsLineStart = false);

  char getNextChar();
  int peekNextChar(int Index);
  void skipComment();
  Calculator::Token lexIdentifier();
  Calculator::Token lexNumber();
};

#endif // CALCULATORLEXER_H