# SEP API Client

`sep_client.js` is a small Node.js script that demonstrates how to interact with the SEP Engine HTTP API. It sends text to the `/pattern/analyze` endpoint and prints the embeddings returned by the engine.

## Prerequisites

- Node.js 18 or later
- The SEP Engine running locally with the API available on `http://localhost:8080`

Install dependencies once using `npm install` or `pnpm install` in the repository root. The only runtime dependency is `node-fetch`.

## Running

Execute the script from the project root:

```bash
node sep_client.js
```

It will POST a sample text and log a JSON structure containing the generated embeddings:

```json
{
  "embeddings": [ ... ]
}
```

Use this as a template for integrating the API with other tools or services.
