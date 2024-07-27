import { Readable } from 'node:stream';

export class Llama {
  constructor();
  initialize(modelPath: string, contextSize?: number): boolean;
  runQuery(prompt: string, maxTokens?: number): string;
  runQueryStream(prompt: string, maxTokens: number): Readable;
}
