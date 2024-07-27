const llama = require('./cpp/build/Release/llamacpp_node_bindings.node');

module.exports = {
  LlamaWrapper: llama.LlamaWrapper
};
