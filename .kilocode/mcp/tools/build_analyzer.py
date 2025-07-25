#!/usr/bin/env python3
import sys
import json
import subprocess
import time
from typing import Dict, Any

def send_response(response: Dict[str, Any]):
    """Send JSON-RPC response"""
    print(json.dumps(response))
    sys.stdout.flush()

def handle_initialize(request_id: Any):
    """Handle initialize request"""
    response = {
        "jsonrpc": "2.0",
        "id": request_id,
        "result": {
            "protocolVersion": "2024-11-05",
            "capabilities": {
                "tools": {}
            },
            "serverInfo": {
                "name": "build_analyzer",
                "version": "1.0.0"
            }
        }
    }
    send_response(response)

def handle_tools_list(request_id: Any):
    """Handle tools/list request"""
    response = {
        "jsonrpc": "2.0",
        "id": request_id,
        "result": {
            "tools": [{
                "name": "analyze_build",
                "description": "Run build.sh and analyze output for errors",
                "inputSchema": {
                    "type": "object",
                    "properties": {
                        "verbose": {
                            "type": "boolean",
                            "description": "Include full build output",
                            "default": False
                        }
                    },
                    "required": []
                }
            }]
        }
    }
    send_response(response)

def analyze_build(params: Dict[str, Any]) -> Dict[str, Any]:
    """Run build.sh and analyze output for errors"""
    try:
        # Run build script
        result = subprocess.run(
            ['./build.sh'],
            capture_output=True,
            text=True,
            cwd='/sep'
        )
        
        # Combine stdout and stderr
        output = result.stdout + result.stderr
        
        # Extract error lines
        errors = []
        for line in output.splitlines():
            if 'error' in line.lower():
                errors.append(line.strip())
        
        # Remove duplicates while preserving order
        unique_errors = list(dict.fromkeys(errors))
        
        # Prepare response
        response = {
            "status": "error" if unique_errors else "success",
            "error_count": len(unique_errors),
            "errors": unique_errors
        }
        
        # Add full output if verbose mode
        if params.get("verbose", False):
            response["full_output"] = output
            
        return response
        
    except Exception as e:
        return {
            "status": "error",
            "error_count": 1,
            "errors": [f"Failed to run build: {str(e)}"]
        }

def handle_tools_call(request_id: Any, params: Dict[str, Any]):
    """Handle tools/call request"""
    tool_name = params.get("name")
    
    if tool_name == "analyze_build":
        result = analyze_build(params.get("arguments", {}))
        response = {
            "jsonrpc": "2.0",
            "id": request_id,
            "result": result
        }
    else:
        response = {
            "jsonrpc": "2.0",
            "id": request_id,
            "error": {
                "code": -32601,
                "message": f"Unknown tool: {tool_name}"
            }
        }
    
    send_response(response)

def main():
    """Main MCP server loop"""
    # Add a delay to ensure server is ready
    time.sleep(1)
    
    while True:
        try:
            # Read line from stdin
            line = sys.stdin.readline()
            if not line:
                break
            
            # Parse JSON-RPC request
            request = json.loads(line.strip())
            request_id = request.get("id")
            method = request.get("method")
            params = request.get("params", {})
            
            # Handle different methods
            if method == "initialize":
                handle_initialize(request_id)
            elif method == "tools/list":
                handle_tools_list(request_id)
            elif method == "tools/call":
                handle_tools_call(request_id, params)
            else:
                # Unknown method
                response = {
                    "jsonrpc": "2.0",
                    "id": request_id,
                    "error": {
                        "code": -32601,
                        "message": f"Method not found: {method}"
                    }
                }
                send_response(response)
                
        except json.JSONDecodeError as e:
            # Parse error
            error_response = {
                "jsonrpc": "2.0",
                "id": None,
                "error": {
                    "code": -32700,
                    "message": f"Parse error: {str(e)}"
                }
            }
            send_response(error_response)
        except Exception as e:
            # Server error
            error_response = {
                "jsonrpc": "2.0",
                "id": None,
                "error": {
                    "code": -32000,
                    "message": f"Server error: {str(e)}"
                }
            }
            send_response(error_response)

if __name__ == "__main__":
    main()
