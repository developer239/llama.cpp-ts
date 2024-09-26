const {Llama} = require('../index.js');

async function main() {
    const llama = new Llama();
    const modelPath = __dirname + "/models/Meta-Llama-3.1-8B-Instruct-Q3_K_S.gguf";
    const modelParams = {nGpuLayers: 32};
    const contextParams = {nContext: 2048};

    if (!llama.initialize(modelPath, modelParams, contextParams)) {
        console.error("Failed to initialize the model");
        return;
    }

    llama.setSystemPrompt("You are a helpful assistant. Always provide clear, concise, and accurate answers. ");

    const questions = [
        "What is the capital of France?",
        "What's the population of that city?",
        "What country is the city in?"
    ];

    for (const question of questions) {
        const tokenStream = llama.prompt(question);

        console.log("Question: ");
        console.log(question);
        console.log("");

        console.log("Answer: ");

        while (true) {
            const token = await tokenStream.read();
            if (token === null) {
                break;
            }
            process.stdout.write(token);
        }

        console.log("");
    }
}

main().catch(console.error);
