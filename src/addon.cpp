/**
 * @file addon.cpp
 * @author Zhou Jing (254644528@qq.com)
 * @brief wrapper of calculator addon
 * @version 0.1
 * @date 2022-08-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../node_modules/node-addon-api/napi.h"
#include "parser.h"
Napi::Number calculate(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  CalculatorParser Parser;
  std::string Content = info[0].ToString().Utf8Value();
  Parser.parse(Content);
  return Napi::Number::New(env, Parser.calculate(Parser.getPostFix()));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "calculate"),
              Napi::Function::New(env, calculate));
  return exports;
}

NODE_API_MODULE(calculator, Init)