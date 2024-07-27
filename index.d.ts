export class LlamaWrapper {
  constructor();
  initialize(modelPath: string, contextSize?: number): boolean;
  runQuery(prompt: string, maxTokens?: number): string;
  runQueryStream(prompt: string, maxTokens: number, callback: (response: string) => void): void;
}
