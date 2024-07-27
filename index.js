const { LlamaCPP } = require('./cpp/build/Release/llamacpp_node_bindings.node');
const { Readable } = require('stream');

class Llama {
  constructor() {
    this.llamaCPP = new LlamaCPP();
  }

  initialize(modelPath, contextSize = 80000) {
    return this.llamaCPP.initialize(modelPath, contextSize);
  }

  runQuery(prompt, maxTokens = 1000) {
    return this.llamaCPP.runQuery(prompt, maxTokens);
  }

  runQueryStream(prompt, maxTokens = 1000) {
    const stream = new Readable({
      read() {}
    });

    this.llamaCPP.runQueryStream(prompt, (token) => {
      stream.push(token);
    }, maxTokens);

    // End the stream when the C++ side is done
    setImmediate(() => stream.push(null));

    return stream;
  }
}

module.exports = Llama;
