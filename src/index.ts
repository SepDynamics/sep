import { Server } from "@modelcontextprotocol/sdk/server";
import { StdioServerTransport } from "@modelcontextprotocol/sdk/server/stdio";

const server = new Server({
  name: "code-analyzer",
  version: "0.1.0",
  transport: new StdioServerTransport(),
  tools: []
});

server.start();