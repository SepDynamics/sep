To use the nomic-embed-code model (from nomic-ai/nomic-embed-code on Hugging Face), which is a 7B parameter code embedding model based on Qwen2.5 and compatible with the Sentence Transformers framework, follow these steps. It's designed specifically for code retrieval tasks, supporting languages like Python, Java, Ruby, PHP, JavaScript, and Go, and it outperforms models like Voyage Code 3 on benchmarks such as CodeSearchNet.<grok:render card_id="3c471a" card_type="citation_card" type="render_inline_citation">
<argument name="citation_id">10</argument>
</grok:render>

### Installation
Install the required libraries via pip (assuming you have Python and pip set up):

```
pip install transformers sentence-transformers torch
```

This sets up everything needed to load and run the model.

### Usage
You can use it in two main ways: directly with the Transformers library (for more control) or with Sentence Transformers (simpler API, recommended for most cases). The model generates 768-dimensional embeddings.

#### With Sentence Transformers (Easier for Quick Embedding)
Load the model and encode text or code. For queries (e.g., natural language descriptions), use the `prompt_name="query"` parameter to prefix the input appropriately. For code snippets, encode them directly.

Example code:

```python
from sentence_transformers import SentenceTransformer

# Load the model
model = SentenceTransformer("nomic-ai/nomic-embed-code")

# Example query and code snippet
queries = ['Calculate the n-th factorial']  # Natural language query
code_snippets = ['def fact(n):\n if n < 0:\n  raise ValueError\n return 1 if n == 0 else n * fact(n - 1)']  # Code to embed

# Encode
query_emb = model.encode(queries, prompt_name="query")  # Use 'query' prompt for searches
code_emb = model.encode(code_snippets)  # No prompt needed for code

# Compute similarity (cosine similarity)
similarity = model.similarity(query_emb[0], code_emb[0])
print(similarity)  # Outputs a tensor with the similarity score
```

This will output a similarity score between the query and code embeddings.<grok:render card_id="d395d3" card_type="citation_card" type="render_inline_citation">
<argument name="citation_id">10</argument>
</grok:render>

#### With Transformers (More Flexible)
This gives lower-level access. You'll handle tokenization, pooling, and normalization manually.

Example code:

```python
import torch
import torch.nn.functional as F
from transformers import AutoTokenizer, AutoModel

# Load tokenizer and model
tokenizer = AutoTokenizer.from_pretrained("nomic-ai/nomic-embed-code")
model = AutoModel.from_pretrained("nomic-ai/nomic-embed-code")

# Custom pooling function (uses last token pooling)
def last_token_pooling(hidden_states, attention_mask):
    sequence_lengths = attention_mask.sum(-1) - 1
    return hidden_states[torch.arange(hidden_states.shape[0]), sequence_lengths]

# Example inputs (prefix queries manually if needed, e.g., 'Represent this query for searching relevant code: ')
queries = ['Represent this query for searching relevant code: Calculate the n-th factorial']
codes = ['def fact(n):\n if n < 0:\n  raise ValueError\n return 1 if n == 0 else n * fact(n - 1)']
code_snippets = queries + codes  # Combine for batch processing

# Tokenize and run inference
encoded_input = tokenizer(code_snippets, padding=True, truncation=True, return_tensors='pt')
model.eval()
with torch.no_grad():
    model_output = model(**encoded_input)[0]

# Pool and normalize embeddings
embeddings = last_token_pooling(model_output, encoded_input['attention_mask'])
embeddings = F.normalize(embeddings, p=2, dim=1)
print(embeddings.shape)  # Should be torch.Size([2, 768]) for two inputs

# Compute similarity
similarity = F.cosine_similarity(embeddings[0], embeddings[1], dim=0)
print(similarity)  # Outputs the similarity score
```

### Additional Notes
- **Prefixing Inputs**: For queries in Sentence Transformers, use `prompt_name="query"` to optimize for search tasks. In Transformers, you can manually prefix queries (e.g., "Represent this query for searching relevant code: ") as shown in the example.
- **Performance**: On CodeSearchNet, it achieves high scores (e.g., 81.7 for Python, 93.8 for Go). Embeddings are normalized for cosine similarity.
- **Limitations**: It's optimized for code retrieval, so it may not perform as well on general text tasks. The model is large (7B parameters), so ensure you have sufficient GPU memory (e.g., at least 16GB VRAM for inference).
- For more details, including the training dataset (CoRNStack) and full benchmarks, check the model's GitHub repo at https://github.com/gangiswag/cornstack/.<grok:render card_id="29438d" card_type="citation_card" type="render_inline_citation">
<argument name="citation_id">10</argument>
</grok:render>