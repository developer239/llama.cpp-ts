#include <napi.h>
#include "LlamaCPPBinding.h"
#include "TokenStream.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  TokenStream::Init(env, exports);
  return LlamaCPPBinding::Init(env, exports);
}

NODE_API_MODULE(llamacpp_node_bindings, InitAll);
