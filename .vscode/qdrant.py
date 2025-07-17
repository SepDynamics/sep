from qdrant_client import QdrantClient

qdrant_client = QdrantClient(
    url="https://14078592-0e60-4aa2-8e5b-eba64bdb8eb4.us-east4-0.gcp.cloud.qdrant.io:6333", 
    api_key="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJhY2Nlc3MiOiJtIn0.wsnOD_5_VKRsitBSIps7cBg5UOQvIGiW0Lkcc-eKtr0",
)

print(qdrant_client.get_collections())