# Embedding Headers Overview

## Header Breakdown

### `simple_embedding_model.h`
Defines `sep::embeddings::SimpleEmbeddingModel`, a lightweight class that transforms a string into a fixed‑size vector.

```mermaid
classDiagram
    class SimpleEmbeddingModel {
        +compute(text: string) std::vector<double>
        -weights_[5] : double
    }
```
