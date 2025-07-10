import express from 'express';
import autonomousBrain from './autonomous-brain.js';
import { setupLLM } from './llm/inference.js';

const app = express();
app.use(express.json());

const llm = await setupLLM();

app.post('/complete', async (req, res) => {
  try {
    const brainResult = await autonomousBrain.process(req.body);
    const completion = await llm.complete(brainResult.response.message);
    res.json({
      completion,
      coherence: brainResult.coherence,
      memory_tier: brainResult.memory_tier
    });
  } catch (e) {
    console.error(e);
    res.status(500).json({ error: 'Completion Error' });
  }
});

app.listen(3000, () => console.log('MCP Server on port 3000'));