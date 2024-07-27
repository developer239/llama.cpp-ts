#pragma once

#include <napi.h>
#include "llama-wrapper.h"
#include "TokenStream.h"

class LlamaCPPBinding : public Napi::ObjectWrap<LlamaCPPBinding> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    LlamaCPPBinding(const Napi::CallbackInfo& info);

private:
    static Napi::FunctionReference constructor;

    Napi::Value Initialize(const Napi::CallbackInfo& info);
    Napi::Value RunQuery(const Napi::CallbackInfo& info);
    Napi::Value RunQueryStream(const Napi::CallbackInfo& info);

    std::unique_ptr<LlamaWrapper> llama_;
};
