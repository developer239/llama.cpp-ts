const Llama = require('../index.js');

async function main() {
  //
  // Initialize
  const llama = new Llama();
  const modelPath = __dirname + "/models/Meta-Llama-3.1-8B-Instruct-Q3_K_S.gguf";

  if (!llama.initialize(modelPath)) {
    console.error("Failed to initialize the model");
    return;
  }

  //
  // Query
  const query = "Hello.";

  //
  // Sync query
  const response = llama.runQuery(query, 100);
  console.log(response)

  //
  // Stream query
  const tokenStream = llama.runQueryStream(query, 200);
  let streamingResponse = "";

  while (true) {
    const token = await tokenStream.read();
    if (token === null) break;
    process.stdout.write(token);
    streamingResponse += token;
  }
}

main().catch(console.error);
