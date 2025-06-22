from qdrant_client import QdrantClient

qdrant_client = QdrantClient(
    url="https://454dfdd6-4ee5-4a65-91d8-9ba9da6e2d19.us-east4-0.gcp.cloud.qdrant.io:6333", 
    api_key="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3MiOiJtIn0.eIxHQb448A5HQum8f5o-v7FwmRVrbJefKiLRHkq5E28",
)

print(qdrant_client.get_collections())