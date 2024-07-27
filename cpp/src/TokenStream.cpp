#include "TokenStream.h"

Napi::FunctionReference TokenStream::constructor;

Napi::Object TokenStream::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "TokenStream", {
        InstanceMethod("read", &TokenStream::Read),
        InstanceMethod("push", &TokenStream::Push),
        InstanceMethod("end", &TokenStream::End),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("TokenStream", func);
    return exports;
}

Napi::Object TokenStream::NewInstance(Napi::Env env, Napi::Value arg) {
    Napi::EscapableHandleScope scope(env);
    Napi::Object obj = constructor.New({arg});
    return scope.Escape(napi_value(obj)).ToObject();
}

TokenStream::TokenStream(const Napi::CallbackInfo& info) : Napi::ObjectWrap<TokenStream>(info) {}

Napi::Value TokenStream::Read(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    std::unique_lock<std::mutex> lock(mutex);
    cv.wait(lock, [this] { return !tokenQueue.empty() || finished; });

    if (tokenQueue.empty() && finished) {
        return env.Null();
    }

    std::string token = tokenQueue.front();
    tokenQueue.pop();
    return Napi::String::New(env, token);
}

Napi::Value TokenStream::Push(const Napi::CallbackInfo& info) {
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(info.Env(), "String expected").ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }

    std::string token = info[0].As<Napi::String>().Utf8Value();
    Push(token);
    return info.Env().Undefined();
}

void TokenStream::Push(const std::string& token) {
    {
        std::lock_guard<std::mutex> lock(mutex);
        tokenQueue.push(token);
    }
    cv.notify_one();
}

Napi::Value TokenStream::End(const Napi::CallbackInfo& info) {
    End();
    return info.Env().Undefined();
}

void TokenStream::End() {
    {
        std::lock_guard<std::mutex> lock(mutex);
        finished = true;
    }
    cv.notify_all();
}

