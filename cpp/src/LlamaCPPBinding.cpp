#include "LlamaCPPBinding.h"
#include <napi.h>
#include <thread>

Napi::FunctionReference LlamaCPPBinding::constructor;

Napi::Object LlamaCPPBinding::Init(Napi::Env env, Napi::Object exports) {
    Napi::Function func = DefineClass(env, "LlamaCPP", {
        InstanceMethod("initialize", &LlamaCPPBinding::Initialize),
        InstanceMethod("setSystemPrompt", &LlamaCPPBinding::SetSystemPrompt),
        InstanceMethod("prompt", &LlamaCPPBinding::PromptStream),
        InstanceMethod("resetConversation", &LlamaCPPBinding::ResetConversation),
    });

    constructor = Napi::Persistent(func);
    constructor.SuppressDestruct();

    exports.Set("LlamaCPP", func);
    return exports;
}

LlamaCPPBinding::LlamaCPPBinding(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<LlamaCPPBinding>(info) {
    llama_ = std::make_unique<LlamaChat>();
}

Napi::Value LlamaCPPBinding::Initialize(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "Model path must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string modelPath = info[0].As<Napi::String>().Utf8Value();

    ModelParams modelParams;
    ContextParams contextParams;

    if (info.Length() > 1 && info[1].IsObject()) {
        Napi::Object modelParamsObj = info[1].As<Napi::Object>();
        if (modelParamsObj.Has("nGpuLayers")) {
            modelParams.nGpuLayers = modelParamsObj.Get("nGpuLayers").As<Napi::Number>().Int32Value();
        }
        // Add parsing for other ModelParams if needed
    }

    if (info.Length() > 2 && info[2].IsObject()) {
        Napi::Object contextParamsObj = info[2].As<Napi::Object>();
        if (contextParamsObj.Has("nContext")) {
            contextParams.nContext = contextParamsObj.Get("nContext").As<Napi::Number>().Uint32Value();
        }
        // Add parsing for other ContextParams if needed
    }

    if (!llama_->InitializeModel(modelPath, modelParams)) {
        Napi::Error::New(env, "Failed to initialize the model").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!llama_->InitializeContext(contextParams)) {
        Napi::Error::New(env, "Failed to initialize the context").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    return Napi::Boolean::New(env, true);
}

Napi::Value LlamaCPPBinding::SetSystemPrompt(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "System prompt must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string systemPrompt = info[0].As<Napi::String>().Utf8Value();
    llama_->SetSystemPrompt(systemPrompt);
    return env.Null();
}

Napi::Value LlamaCPPBinding::ResetConversation(const Napi::CallbackInfo& info) {
    llama_->ResetConversation();
    return info.Env().Undefined();
}

Napi::Value LlamaCPPBinding::PromptStream(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    if (info.Length() < 1 || !info[0].IsString()) {
        Napi::TypeError::New(env, "User message must be a string").ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string userMessage = info[0].As<Napi::String>().Utf8Value();

    Napi::Object streamObj = TokenStream::NewInstance(env, env.Null());
    TokenStream* stream = Napi::ObjectWrap<TokenStream>::Unwrap(streamObj);

    LlamaChat* llama_ptr = llama_.get();

    std::thread([llama_ptr, userMessage, stream]() {
        try {
            llama_ptr->Prompt(userMessage, [stream](const std::string& piece) {
                stream->Push(piece);
            });
            stream->End();
        } catch (const std::exception& e) {
            stream->End();
        }
    }).detach();

    return streamObj;
}
