export class TokenStream {
  read(): Promise<string | null>;
}

export class Llama {
  constructor();
  initialize(modelPath: string, contextSize?: number): boolean;
  runQuery(prompt: string, maxTokens?: number): string;
  runQueryStream(prompt: string, maxTokens?: number): TokenStream;
}
