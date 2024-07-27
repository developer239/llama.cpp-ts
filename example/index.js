const Llama = require('../index.js');

async function main() {
    const llama = new Llama();

    llama.initialize(__dirname + "/models/Meta-Llama-3.1-8B-Instruct-Q3_K_S.gguf");

    console.log("\nRunning a simple query:");
    const response = llama.runQuery("Tell me a short story.", 100);
    console.log("Response:", response);
}

main().catch(console.error);
