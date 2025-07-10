import { exec } from 'child_process';
import coherenceEngine from '../coherence-engine.js';

export async function setupLLM(model = 'llama2:70b') {
  return {
    complete: async (prompt) => {
      const coherenceResult = await coherenceEngine.analyzePatterns(prompt);
      const refinedPrompt = coherenceResult.message.replace(/"/g, '\\"');

      return new Promise((resolve, reject) => {
        exec(`ollama run ${model} "${refinedPrompt}"`, (err, stdout) => {
          if (err) reject(err);
          else resolve(stdout.trim());
        });
      });
    }
  };
}