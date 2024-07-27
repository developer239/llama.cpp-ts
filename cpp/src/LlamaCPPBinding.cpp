#include "LlamaCPPBinding.h"
#include <napi.h>
#include <thread>

Napi::FunctionReference LlamaCPPBinding::constructor;

Napi::Object LlamaCPPBinding::Init(Napi::Env env, Napi::Object exports) {
    Napi::HandleScope scope(env);

    Napi::Function func = DefineClass(env, "LlamaCPP", {
        InstanceMethod("initialize", &LlamaCPPBinding::Initialize),
        InstanceMethod("runQuery", &LlamaCPPBinding::RunQuery),
        InstanceMethod("runQueryStream", &LlamaCPPBinding::RunQueryStream)
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LlamaCPP", func);
    return exports;
}

LlamaCPPBinding::LlamaCPPBinding(const Napi::CallbackInfo& info) : Napi::ObjectWrap<LlamaCPPBinding>(info) {
    Napi::Env env = info.Env();
    Napi::HandleScope scope(env);

    llama_ = std::make_unique<LlamaWrapper>();
}

Napi::Value LlamaCPPBinding::Initialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string model_path = info[0].As<Napi::String>().Utf8Value();
    size_t context_size = 80000;
    if (info.Length() > 1 && info[1].IsNumber()) {
        context_size = info[1].As<Napi::Number>().Uint32Value();
    }

    bool success = llama_->Initialize(model_path, context_size);
    return Napi::Boolean::New(env, success);
}

Napi::Value LlamaCPPBinding::RunQuery(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string prompt = info[0].As<Napi::String>().Utf8Value();
    size_t max_tokens = 1000;
    if (info.Length() > 1 && info[1].IsNumber()) {
        max_tokens = info[1].As<Napi::Number>().Uint32Value();
    }

    std::string response = llama_->RunQuery(prompt, max_tokens);
    return Napi::String::New(env, response);
}

Napi::Value LlamaCPPBinding::RunQueryStream(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string prompt = info[0].As<Napi::String>().Utf8Value();
    size_t max_tokens = 1000;
    if (info.Length() > 1 && info[1].IsNumber()) {
        max_tokens = info[1].As<Napi::Number>().Uint32Value();
    }

    Napi::Object streamObj = TokenStream::NewInstance(env, env.Null());
    TokenStream* stream = Napi::ObjectWrap<TokenStream>::Unwrap(streamObj);

    std::thread([this, prompt, max_tokens, stream]() {
        llama_->RunQueryStream(prompt, max_tokens, [stream](const std::string& token) {
            stream->Push(token);
        });
        stream->End();
    }).detach();

    return streamObj;
}
