//===--- StringSwitch.h - Switch-on-literal-string Construct ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
///  This file implements the StringSwitch template, which mimics a switch()
///  statement whose cases are string literals.
///
//===----------------------------------------------------------------------===//

#ifndef STRINGSWITCH_H
#define STRINGSWITCH_H

#include <cassert>
#include <optional>
#include <string>

using namespace std;

/// A switch()-like statement whose cases are string literals.
///
/// The StringSwitch class is a simple form of a switch() statement that
/// determines whether the given string matches one of the given string
/// literals. The template type parameter \p T is the type of the value that
/// will be returned from the string-switch expression. For example,
/// the following code switches on the name of a color in \c argv[i]:
///
/// \code
/// Color color = StringSwitch<Color>(argv[i])
///   .Case("red", Red)
///   .Case("orange", Orange)
///   .Case("yellow", Yellow)
///   .Case("green", Green)
///   .Case("blue", Blue)
///   .Case("indigo", Indigo)
///   .Cases("violet", "purple", Violet)
///   .Default(UnknownColor);
/// \endcode
template <typename T, typename R = T> class StringSwitch {
  /// The string we are matching.
  const string Str;

  /// The pointer to the result of this switch statement, once known,
  /// null before that.
  std::optional<T> Result;

public:
  explicit StringSwitch(string S) : Str(S), Result() {}

  // StringSwitch is not copyable.
  StringSwitch(const StringSwitch &) = delete;

  // StringSwitch is not assignable due to 'Str' being 'const'.
  void operator=(const StringSwitch &) = delete;
  void operator=(StringSwitch &&other) = delete;

  StringSwitch(StringSwitch &&other)
      : Str(other.Str), Result(std::move(other.Result)) {}

  ~StringSwitch() = default;

  // Case-sensitive case matchers
  StringSwitch &Case(string S, T Value) {
    if (!Result && Str == S) {
      Result = std::move(Value);
    }
    return *this;
  }

  // StringSwitch &EndsWith(string S, T Value) {
  //   if (!Result && Str.ends_with(S)) {
  //     Result = std::move(Value);
  //   }
  //   return *this;
  // }

  // StringSwitch &StartsWith(string S, T Value) {
  //   if (!Result && Str.starts_with(S)) {
  //     Result = std::move(Value);
  //   }
  //   return *this;
  // }

  StringSwitch &Cases(string S0, string S1, T Value) {
    return Case(S0, Value).Case(S1, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, T Value) {
    return Case(S0, Value).Cases(S1, S2, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      string S5, T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, S5, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      string S5, string S6, T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      string S5, string S6, string S7, T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      string S5, string S6, string S7, string S8, T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, S8, Value);
  }

  StringSwitch &Cases(string S0, string S1, string S2, string S3, string S4,
                      string S5, string S6, string S7, string S8, string S9,
                      T Value) {
    return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, S8, S9, Value);
  }

  // Case-insensitive case matchers.
  StringSwitch &CaseLower(string S, T Value) {
    assert(0 && "TODO: Implement tolower on string!");
    /*
    if (!Result && Str.equals_insensitive(S))
      Result = std::move(Value);
    */
    return *this;
  }

  StringSwitch &EndsWithLower(string S, T Value) {
    assert(0 && "TODO: unimplemented");
    /*
    if (!Result && Str.endswith_insensitive(S))
      Result = Value;
    */
    return *this;
  }

  StringSwitch &StartsWithLower(string S, T Value) {
    assert(0 && "TODO: unimplemented");
    /*
    if (!Result && Str.startswith_insensitive(S))
      Result = std::move(Value);
    */
    return *this;
  }

  StringSwitch &CasesLower(string S0, string S1, T Value) {
    return CaseLower(S0, Value).CaseLower(S1, Value);
  }

  StringSwitch &CasesLower(string S0, string S1, string S2, T Value) {
    return CaseLower(S0, Value).CasesLower(S1, S2, Value);
  }

  StringSwitch &CasesLower(string S0, string S1, string S2, string S3,
                           T Value) {
    return CaseLower(S0, Value).CasesLower(S1, S2, S3, Value);
  }

  StringSwitch &CasesLower(string S0, string S1, string S2, string S3,
                           string S4, T Value) {
    return CaseLower(S0, Value).CasesLower(S1, S2, S3, S4, Value);
  }

  R Default(T Value) {
    if (Result)
      return std::move(*Result);
    return Value;
  }

  operator R() {
    assert(Result && "Fell off the end of a string-switch");
    return std::move(*Result);
  }
};

#endif // STRINGSWITCH_H
