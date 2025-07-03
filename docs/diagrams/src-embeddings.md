# Embeddings Implementation Flow

`src/embeddings` contains the implementation of the `SimpleEmbeddingModel`. While compact, it illustrates the basic pattern used for derived embedding models.

## Processing Steps

```mermaid
sequenceDiagram
    participant Caller
    participant Model as SimpleEmbeddingModel
    Caller->>Model: compute(text)
    Model->>Model: accumulate weighted sums
    Model->>Model: normalize vector
    Model-->>Caller: std::vector<double>
```

1. **Initialization**: The constructor fills `weights_` with deterministic constants.
2. **Accumulation**: `compute` iterates over characters, multiplies by each weight and sums into a vector of length `kDim`.
3. **Normalization**: After accumulation the vector is normalized to unit length.

The CMake file exports this object as `sep_embeddings`, allowing other modules to link against the library.
