#include <napi.h>
#include "LlamaCPPBinding.h"

Napi::Object InitAll(Napi::Env env, Napi::Object exports) {
  return LlamaCPPBinding::Init(env, exports);
}

NODE_API_MODULE(llamacpp_node_bindings, InitAll);
