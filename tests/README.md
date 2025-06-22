# SEP Testbed

This directory contains standalone tests for verifying core modules of the SEP project.

The structure mirrors the main modules:

- **blender/** – tests Blender integration, shader handling and mesh utilities.
- **cuda/** – tests CUDA kernels and GPU utilities.
- **api/** – tests the API server stack and the Ollama client.
- **audio/** – tests audio capture and processing.

Each test can be built independently and is intended for experimentation without affecting the main test suite.
