/**
 * @file lexer.cpp
 * @author Zhou Jing (254644528@qq.com)
 * @brief The lexer of calculator
 * @version 0.1
 * @date 2022-08-31
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "lexer.h"
#include "StringSwitch.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
using namespace std;
using namespace Calculator;

bool CalculatorLexer::init(string FileName) {
  // TODO: Replace plain C++ io with SourceManager.
  ifstream File(FileName);

  if (!File.good()) {
    CurBuf = FileName;
  } else {
    // Get pointer to associated buffer object
    filebuf *FileBuf = File.rdbuf();

    // Get file size using buffer's members
    size_t Size = FileBuf->pubseekoff(0, File.end, File.in);
    FileBuf->pubseekpos(0, File.in);

    char *Buf = new char[Size];

    FileBuf->sgetn(Buf, Size);

    // Read into the buffer used by lexer
    CurBuf = string(Buf, Size);

    delete[] Buf;

    File.close();
  }
  StartLine = 1;
  EndColumn = 1;
  CurPtr = &*CurBuf.cbegin();

  return false;
}

char CalculatorLexer::getNextChar() {
  char CurChar = *CurPtr++;
  // cout << CurChar << int(CurChar) << endl;
  switch (CurChar) {
  default:
    return (unsigned char)CurChar;

  case 0: {
    // A NUL character in the stream is either the end of the current buffer of
    // a spurious NUL in the file. Disambiguate that here.
    if (CurPtr - 1 == &*CurBuf.cend()) {
      --CurPtr; // Arrange for another call to return EOF again.
      return EOF;
    }

    cerr << "NUL character is invalid in source, treated as space.\n";
    return ' ';
  }

  case '\n':
  case '\r':
    // Handle the newline character by ignoring it and incrementing the line
    // count. However, be careful about 'dos style' files with \n\r in them.
    // Only treat a \n\r or \r\n as a single line.
    if ((*CurPtr == '\n' || *CurPtr == '\r') && *CurPtr != CurChar)
      ++CurPtr;

    return '\n';
  }
}

char CalculatorLexer::peekNextChar(int Index) { return *(CurPtr + Index); }

Calculator::Token CalculatorLexer::lookAhead(int Count) {
  assert(Count <= 1 && "Only LL(2) is supported!");
  Calculator::Token NewCode = Calculator::Token::Error;

  // Save lexer state
  // FIXME: We can't use memcpy(&L, this, sizeof(ADSLexer)) yet, valgrind fails.
  const char *OldPtr = CurPtr;
  const char *OldTokStart = TokStart;
  Calculator::Token OldCode = CurCode;
  string OldStrVal = CurStrVal;
  string OldNumberVal = CurNumberVal;
  int OldLine = StartLine;
  int OldColumn = EndColumn;

  while (Count-- && CurCode != Calculator::Eof)
    NewCode = lexToken();

  // Restore lexer state
  CurPtr = OldPtr;
  TokStart = OldTokStart;
  CurCode = OldCode;
  CurStrVal = OldStrVal;
  CurNumberVal = OldNumberVal;
  StartLine = OldLine;
  EndColumn = OldColumn;
  return NewCode;
}

Calculator::Token CalculatorLexer::lexToken(bool IsLineStart) {
  TokStart = CurPtr;

  // This is always consumes at least one character.
  char CurChar = getNextChar();
  EndColumn++;
  switch (CurChar) {
  default:

    if (isalpha(CurChar) || CurChar == '_' || CurChar == '"')
      return lexIdentifier();
    Reporter.report("Unexpected char: '" + CurChar, getLoc());
    return Calculator::Token::Error;
  case EOF:
    return Calculator::Token::Eof; // Reaches the end of a netlist file

  case '(':
    return Calculator::Token::LeftParen;
  case ')':
    return Calculator::Token::RightParen;
  // '+' and '-' can be either sign symbol or binary operator, let
  // parser handle the difference of them.
  case '-':
    return Calculator::Token::Minor;
  case '+':
    return Calculator::Token::Plus;
  case '*':
    return Calculator::Token::Multiply;
  case '/':
    return Calculator::Token::Divide;
  case ' ':
  case '\t':
    // Ignore whitespaces.
    return lexToken(IsLineStart);
  case '.':
    // Floating point number
    if (!isdigit(peekNextChar(0))) {
      cerr << "Expect digit\n";
      return Calculator::Token::Error;
    }
    // FALL THROUGH
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    return lexNumber();
  case '#':
    return Calculator::Eof;
  }
}

// ADS netlist allow double quoted string as identifier if the string is started
// by digit instead of alpha.
Calculator::Token CalculatorLexer::lexIdentifier() {
  // The first letter is [a-zA-Z_"]
  const char *IdStart = TokStart;
  bool ExpectDQuote = false;

  // Current lexed token kind
  Calculator::Token Kind = Calculator::Token::Id;

  // Double quote quoted identifier
  if (CurPtr[-1] == '"')
    ExpectDQuote = true;

  // Double quote expect an ending pair
  while (ExpectDQuote && *CurPtr != '"') {
    ++CurPtr;
    ++EndColumn;
  }

  if (ExpectDQuote) {
    ++CurPtr; // Eat ending '"'
    ++EndColumn;
  }

  while (isalpha(*CurPtr) || isdigit(*CurPtr) || *CurPtr == '_' ||
         *CurPtr == '[' || *CurPtr == ']') {
    ++CurPtr;
    ++EndColumn;
  }

  string Str(IdStart, CurPtr);
  CurStrVal = Str;
  Kind = StringSwitch<Calculator::Token>(Str)
             .Case("sin", Calculator::Funcall)
             .Case("tan", Calculator::Funcall)
             .Case("cos", Calculator::Funcall)
             // TODO: add other dot commands
             .Default(Calculator::Id);
  return Kind;
}

// Lex any kind of numbers(without leading sign '+' and '-') including:
// Integer: [0-9]+[uf]?
// Float: [0-9]*\.[0-9]+[uf]?
// Scientific: [0-9]*(\.[0-9]+)?[Ee][+-]?[0-9]+
Calculator::Token CalculatorLexer::lexNumber() {
  bool HasDot = false;
  const char *NumStart = CurPtr - 1;

  if (CurPtr[-1] == '.') {
    // Float/SciNum: .123, .5e-9

    // Eat [0-9]+ after the dot '.'
    while (isdigit(*CurPtr)) {
      ++CurPtr;
      ++EndColumn;
    }

    if (CurPtr[0] != 'e' && CurPtr[0] != 'E') {
      CurNumberVal = string(NumStart, CurPtr);
      return Calculator::Token::FloatVal;
    }
  }

  // Eat digit in 0-9
  while (isdigit(*CurPtr)) {
    ++CurPtr;
    ++EndColumn;
  }

  // Integer literal 1u, float literal 1f, but not sth like 1else
  if (CurPtr[0] != '.' && CurPtr[0] != 'e' && CurPtr[0] != 'E' ||
      isalpha(CurPtr[1])) {
    Calculator::Token K = Calculator::Token::IntVal;

    CurNumberVal = string(NumStart, CurPtr);
    return K;
  }

  // Eat optional '.' in scientific number
  if (CurPtr[0] == '.') {
    HasDot = true;
    ++CurPtr;
    ++EndColumn;
  }

  // Eat mantissa [0-9]+
  while (isdigit(*CurPtr)) {
    ++CurPtr;
    ++EndColumn;
  }

  if ((CurPtr[0] == 'e' || CurPtr[0] == 'E') && !isalpha(CurPtr[1])) {
    // Scientific number
    ++CurPtr; // Eat 'e' or 'E'
    ++EndColumn;

    if (CurPtr[0] == '+' || CurPtr[0] == '-') {
      ++CurPtr; // Eat sign '-' or '+'
      ++EndColumn;
    }

    int i = 0;
    // Eat exponent digital [0-9]+
    while (isdigit(*CurPtr)) {
      ++CurPtr;
      ++EndColumn;
      ++i;
    }

    if (i == 0) {
      cerr << "Missing digital in exponent part\n";
      return Calculator::Token::Error;
    }

    CurNumberVal = string(NumStart, CurPtr);
    return Calculator::Token::SciNumVal;
  } else if (HasDot) {
    CurNumberVal = string(NumStart, CurPtr);
    return Calculator::Token::FloatVal;
  } else {
    cerr << "Unknown kind of number\n";
    return Calculator::Token::Error;
  }
}

Calculator::TokenLoc CalculatorLexer::getLoc() {
  return {StartLine, EndColumn};
}

const char CalculatorLexer::getCharByToken(Calculator::Token Tok) {
  switch (Tok) {
  case Token::Plus:
    return '+';
  case Token::Minor:
    return '-';
  case Token::Multiply:
    return '*';
  case Token::Divide:
    return '/';
  case Token::Remain:
    return '%';
  case Token::Fac:
    return '!';
  default:
    return ' ';
  }
}
const Calculator::Token CalculatorLexer::getTokenByChar(char Tok) {
  switch (Tok) {
  case '+':
    return Token::Plus;
  case '-':
    return Token::Minor;
  case '*':
    return Token::Multiply;
  case '/':
    return Token::Divide;
  case '%':
    return Token::Remain;
  case '!':
    return Token::Fac;
  default:
    return Token::Error;
  }
}