⚠️ Experimental Feature: This feature is under active development and may change significantly in future releases.

Codebase Indexing enables semantic code search across your entire project using AI embeddings. Instead of searching for exact text matches, it understands the meaning of your queries, helping Kilo Code find relevant code even when you don't know specific function names or file locations.

Codebase Indexing Settings
What It Does
When enabled, the indexing system:

Parses your code using Tree-sitter to identify semantic blocks (functions, classes, methods)
Creates embeddings of each code block using AI models
Stores vectors in a Qdrant database for fast similarity search
Provides the codebase_search tool to Kilo Code for intelligent code discovery
This enables natural language queries like "user authentication logic" or "database connection handling" to find relevant code across your entire project.

Key Benefits
Semantic Search: Find code by meaning, not just keywords
Enhanced AI Understanding: Kilo Code can better comprehend and work with your codebase
Cross-Project Discovery: Search across all files, not just what's open
Pattern Recognition: Locate similar implementations and code patterns
Setup Requirements
Embedding Provider
Choose one of these options for generating embeddings:

OpenAI (Recommended)

Requires OpenAI API key
Supports all OpenAI embedding models
Default: text-embedding-3-small
Processes up to 100,000 tokens per batch
Ollama (Local)

Requires local Ollama installation
No API costs or internet dependency
Supports any Ollama-compatible embedding model
Requires Ollama base URL configuration
Vector Database
Qdrant is required for storing and searching embeddings:

Local: http://localhost:6333 (recommended for testing)
Cloud: Qdrant Cloud or self-hosted instance
Authentication: Optional API key for secured deployments
Setting Up Qdrant
Quick Local Setup
Using Docker:

docker run -p 6333:6333 qdrant/qdrant

Using Docker Compose:

version: '3.8'
services:
  qdrant:
    image: qdrant/qdrant
    ports:
      - '6333:6333'
    volumes:
      - qdrant_storage:/qdrant/storage
volumes:
  qdrant_storage:

Production Deployment
For team or production use:

Qdrant Cloud - Managed service
Self-hosted on AWS, GCP, or Azure
Local server with network access for team sharing
Configuration
Open Kilo Code settings ( icon)
Navigate to Experimental section
Enable "Enable Codebase Indexing"
Configure your embedding provider:
OpenAI: Enter API key and select model
Ollama: Enter base URL and select model
Set Qdrant URL and optional API key
Click Save to start initial indexing
Understanding Index Status
The interface shows real-time status with color indicators:

Standby (Gray): Not running, awaiting configuration
Indexing (Yellow): Currently processing files
Indexed (Green): Up-to-date and ready for searches
Error (Red): Failed state requiring attention
How Files Are Processed
Smart Code Parsing
Tree-sitter Integration: Uses AST parsing to identify semantic code blocks
Language Support: All languages supported by Tree-sitter
Fallback: Line-based chunking for unsupported file types
Block Sizing:
Minimum: 100 characters
Maximum: 1,000 characters
Splits large functions intelligently
Automatic File Filtering
The indexer automatically excludes:

Binary files and images
Large files (>1MB)
Git repositories (.git folders)
Dependencies (node_modules, vendor, etc.)
Files matching .gitignore and .kilocode patterns
Incremental Updates
File Watching: Monitors workspace for changes
Smart Updates: Only reprocesses modified files
Hash-based Caching: Avoids reprocessing unchanged content
Branch Switching: Automatically handles Git branch changes


Nomic Embed Code: A State-of-the-Art Code Retriever
Blog | Technical Report | AWS SageMaker | Atlas Embedding and Unstructured Data Analytics Platform

nomic-embed-code is a state-of-the-art code embedding model that excels at code retrieval tasks:

High Performance: Outperforms Voyage Code 3 and OpenAI Embed 3 Large on CodeSearchNet
Multilingual Code Support: Trained for multiple programming languages (Python, Java, Ruby, PHP, JavaScript, Go)
Advanced Architecture: 7B parameter code embedding model
Fully Open-Source: Model weights, training data, and evaluation code released
Model	Python	Java	Ruby	PHP	JavaScript	Go
Nomic Embed Code	81.7	80.5	81.8	72.3	77.1	93.8
Voyage Code 3	80.8	80.5	84.6	71.7	79.2	93.2
OpenAI Embed 3 Large	70.8	72.9	75.3	59.6	68.1	87.6
Nomic CodeRankEmbed-137M	78.4	76.9	79.3	68.8	71.4	92.7
CodeSage Large v2 (1B)	74.2	72.3	76.7	65.2	72.5	84.6
CodeSage Large (1B)	70.8	70.2	71.9	61.3	69.5	83.7
Qodo Embed 1 7B	59.9	61.6	68.4	48.5	57.0	81.4
Model Architecture
Total Parameters: 7B
Training Approach: Trained on the CoRNStack dataset with dual-consistency filtering and progressive hard negative mining
Supported Languages: Python, Java, Ruby, PHP, JavaScript, and Go
Usage Guide
Installation
You can install the necessary dependencies with:

pip install transformers sentence-transformers torch

Transformers
import torch
import torch.nn.functional as F
from transformers import AutoTokenizer, AutoModel

tokenizer = AutoTokenizer.from_pretrained("nomic-ai/nomic-embed-code")
model = AutoModel.from_pretrained("nomic-ai/nomic-embed-code")

def last_token_pooling(hidden_states, attention_mask):
    sequence_lengths = attention_mask.sum(-1) - 1
    return hidden_states[torch.arange(hidden_states.shape[0]), sequence_lengths]

queries = ['Represent this query for searching relevant code: Calculate the n-th factorial']
codes = ['def fact(n):\n if n < 0:\n  raise ValueError\n return 1 if n == 0 else n * fact(n - 1)']
code_snippets = queries + codes

encoded_input = tokenizer(code_snippets, padding=True, truncation=True, return_tensors='pt')
model.eval()
with torch.no_grad():
    model_output = model(**encoded_input)[0]

embeddings = last_token_pooling(model_output, encoded_input['attention_mask'])
embeddings = F.normalize(embeddings, p=2, dim=1)
print(embeddings.shape)

similarity = F.cosine_similarity(embeddings[0], embeddings[1], dim=0)
print(similarity)

SentenceTransformers
from sentence_transformers import SentenceTransformer

queries = ['Calculate the n-th factorial']
code_snippets = ['def fact(n):\n if n < 0:\n  raise ValueError\n return 1 if n == 0 else n * fact(n - 1)']

model = SentenceTransformer("nomic-ai/nomic-embed-code")
query_emb = model.encode(queries, prompt_name="query")
code_emb = model.encode(code_snippets)

similarity = model.similarity(query_emb[0], code_emb[0])
print(similarity)

CoRNStack Dataset Curation
Starting with the deduplicated Stackv2, we create text-code pairs from function docstrings and respective code. We filtered out low-quality pairs where the docstring wasn't English, too short, or that contained URLs, HTML tags, or invalid characters. We additionally kept docstrings with text lengths of 256 tokens or longer to help the model learn long-range dependencies.

image/png

After the initial filtering, we used dual-consistency filtering to remove potentially noisy examples. We embed each docstring and code pair and compute the similarity between each docstring and every code example. We remove pairs from the dataset if the corresponding code example is not found in the top-2 most similar examples for a given docstring.

During training, we employ a novel curriculum-based hard negative mining strategy to ensure the model learns from challenging examples. We use a softmax-based sampling strategy to progressively sample hard negatives with increasing difficulty over time.

