export class Llama {
  constructor();
  initialize(modelPath: string, contextSize?: number): boolean;
  runQuery(prompt: string, maxTokens?: number): string;
  runQueryStream(prompt: string, callback: (token: string) => void, maxTokens?: number): void;
}
