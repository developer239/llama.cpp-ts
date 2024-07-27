const Llama = require('../index.js');

async function main() {
    const llama = new Llama();

    console.log("Initializing model...");
    const initialized = llama.initialize(__dirname + "/models/Meta-Llama-3.1-8B-Instruct-Q3_K_S.gguf");
    if (!initialized) {
        console.error("Failed to initialize the model.");
        return;
    }
    console.log("Model initialized successfully.");

    console.log("\nRunning a simple query:");
    const response = llama.runQuery("What is the capital of France?", 100);
    console.log("Response:", response);

    console.log("\nRunning a streaming query:");
    llama.runQueryStream("Tell me a short story about a robot.", (token) => {
        console.log(token);
    }, 1000);
}

main().catch(console.error);
