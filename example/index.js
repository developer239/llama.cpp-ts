const Llama = require('../index.js');

async function main() {
    const llama = new Llama();

    llama.initialize(__dirname + "/models/Meta-Llama-3.1-8B-Instruct-Q3_K_S.gguf");

    console.log("\nRunning a simple query:");
    const response = llama.runQuery("Tell me a short story.", 100);
    console.log("Response:", response);

    console.log("\nRunning a streaming query:");
    let streamingResponse = "";
    llama.runQueryStream(
        "List 5 interesting facts about space.",
        (token) => {
            process.stdout.write(token);
            streamingResponse += token;
        },
        200
    );

    // Wait for the streaming to finish
    await new Promise(resolve => setTimeout(resolve, 5000));

    console.log("\n\nFull streaming response:");
    console.log(streamingResponse);
}

main().catch(console.error);
