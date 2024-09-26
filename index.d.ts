export class TokenStream {
  read(): Promise<string | null>;
}

export class Llama {
  constructor();
  initialize(modelPath: string, modelParams?: object, contextParams?: object): boolean;
  setSystemPrompt(systemPrompt: string): void;
  prompt(userMessage: string): TokenStream;
  resetConversation(): void;
}
