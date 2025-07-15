from sentence_transformers import SentenceTransformer
import torch

def test_model():
    print("Loading nomic-embed-code model...")
    try:
        # Force CPU usage
        device = "cpu"
        torch.set_default_device(device)
        model = SentenceTransformer("nomic-ai/nomic-embed-code", device=device)
        
        # Test queries and code from the documentation
        queries = ['Calculate the n-th factorial']
        code_snippets = ['def fact(n):\n if n < 0:\n  raise ValueError\n return 1 if n == 0 else n * fact(n - 1)']
        
        print("Generating embeddings...")
        query_emb = model.encode(queries, prompt_name="query")
        code_emb = model.encode(code_snippets)
        
        similarity = model.similarity(query_emb[0], code_emb[0])
        print(f"Similarity score: {similarity}")
        
        print("Model test successful!")
        return True
    except Exception as e:
        print(f"Error testing model: {str(e)}")
        return False

if __name__ == "__main__":
    test_model()