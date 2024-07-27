#pragma once

#include <napi.h>
#include <queue>
#include <mutex>
#include <condition_variable>

class TokenStream : public Napi::ObjectWrap<TokenStream> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, Napi::Value arg);
    TokenStream(const Napi::CallbackInfo& info);

    void Push(const std::string& token);
    void End();

private:
    static Napi::FunctionReference constructor;

    Napi::Value Read(const Napi::CallbackInfo& info);
    Napi::Value Push(const Napi::CallbackInfo& info);
    Napi::Value End(const Napi::CallbackInfo& info);

    std::queue<std::string> tokenQueue;
    std::mutex mutex;
    std::condition_variable cv;
    bool finished = false;
};
