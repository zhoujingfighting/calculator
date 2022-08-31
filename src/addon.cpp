/**
 * @file addon.cpp
 * @author zhou jing (254644528@qq.com)
 * @brief wrapper of calculator addon
 * @version 0.1
 * @date 2022-08-31
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "../node_modules/node-addon-api/napi.h"

Napi::String Method(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  return Napi::String::New(env, "world");
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "hello"),
              Napi::Function::New(env, Method));
  return exports;
}

NODE_API_MODULE(hello, Init)