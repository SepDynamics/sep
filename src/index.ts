import { McpServer } from "@modelcontextprotocol/sdk/dist/esm/server/mcp.js";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/dist/esm/server/stdio.js";

const server = new McpServer({
  name: "code-analyzer",
  version: "0.1.0",
  protocolVersion: "2025-06-18"
});

const transport = new StdioServerTransport();

async function main() {
  await server.connect(transport);
}

main().catch(console.error);