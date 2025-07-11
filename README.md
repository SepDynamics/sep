# SEP Engine Build System

This repository contains the SEP Engine, a high-performance C++ framework for quantum-inspired pattern analysis and evolution, along with supporting JavaScript/TypeScript tools.

## Project Structure

- **C++ Components**: The core SEP Engine is written in C++ and built using CMake
- **JavaScript/TypeScript Components**: Supporting tools written in TypeScript/JavaScript

## Build System

The build system has been simplified to use npm for JavaScript/TypeScript components and make for C++ components.

### Prerequisites

- Node.js 18+ and npm
- CMake 3.20+
- C++ compiler with C++17 support (GCC 10+ or Clang 12+)

### Installation

```bash
# Install JavaScript dependencies
npm install

# Build C++ components
make build-cpp

# Build JavaScript components
make build-js

# Or build everything at once
make
```

### Development

```bash
# Start the development environment
make dev

# Watch for TypeScript changes
make watch-tsc

# Watch for bundle changes
make watch-bundle
```

## Codebase Search

The project includes a codebase search tool that integrates with the Model Context Protocol (MCP). This allows for semantic search across the codebase.

### Using Codebase Search

```bash
# Start the codebase search MCP server
make codebase-search

# Or use npm directly
npm run codebase-search
```

### MCP Integration

The codebase search tool is available as an MCP server that can be used with compatible LLM interfaces. It provides the following capabilities:

- Semantic search across the codebase
- File content retrieval with line ranges
- Integration with the SEP Engine API

## Available Commands

| Command | Description |
|---------|-------------|
| `make` | Build all components |
| `make build-cpp` | Build C++ components only |
| `make build-js` | Build JavaScript components only |
| `make clean` | Clean build artifacts |
| `make install` | Install dependencies |
| `make dev` | Start development environment |
| `make watch-tsc` | Watch for TypeScript changes |
| `make watch-bundle` | Watch for bundle changes |
| `make codebase-search` | Start codebase search server |

## Project Components

- **SEP Engine**: Core C++ engine for pattern analysis
- **MCP Tool**: TypeScript tool for Model Context Protocol integration
- **Codebase Search**: Semantic search tool for the codebase