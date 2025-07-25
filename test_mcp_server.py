#!/usr/bin/env python3
import json
import sys

# Test the MCP server by sending a test request
test_requests = [
    # Initialize request
    {
        "jsonrpc": "2.0",
        "id": 1,
        "method": "initialize",
        "params": {}
    },
    # List tools request
    {
        "jsonrpc": "2.0",
        "id": 2,
        "method": "tools/list",
        "params": {}
    }
]

print("Testing MCP server...")
print("Sending requests:")
for req in test_requests:
    print(json.dumps(req))

print("\nExpected responses:")
print("1. Initialize response with server info")
print("2. Tools list with analyze_build tool")
