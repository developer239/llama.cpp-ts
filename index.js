const { LlamaCPP } = require('./build/Release/llamacpp_node_bindings.node');
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
    return new Promise((resolve, reject) => {
      const stream = new Readable({
        read() {}
      });

      this.llamaCPP.runQueryStream(prompt, (token) => {
        stream.push(token);
      }, maxTokens);

      stream.on('end', () => {
        resolve();
      });

      stream.on('error', (err) => {
        reject(err);
      });

      resolve(stream);
    });
  }
}

module.exports = Llama;
