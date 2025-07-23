# SEP Workbench Connection Requirements

## Overview

The SEP Workbench is a visualization and demonstration platform that **REQUIRES** a connection to the SEP Engine. The workbench cannot function without the engine - they are a unified system.

## Architecture

```
SEP Workbench
     │
     ├── ServiceConnector (REQUIRED connection to SEP Engine)
     │   ├── TCP Connection (port 3000)
     │   ├── Health Monitoring (HTTP /health endpoint)
     │   └── Service Engine (MUST be valid)
     │
     └── Demo Orchestrator (requires active engine)
```

## Requirements

1. **SEP Engine**: MUST be running before launching workbench
2. **CUDA**: REQUIRED - The SEP engine uses GPU acceleration
3. **Connection**: TCP connection on port 3000 is mandatory

## Starting the System

### 1. Start the SEP Engine Service

```bash
# Option 1: Direct execution
./build/sep service

# Option 2: Using systemd
sudo systemctl start sep_api_server
```

### 2. Verify Engine is Running

```bash
# Check service status
curl http://localhost:3000/health

# Or check systemd status
sudo systemctl status sep_api_server
```

### 3. Launch Workbench

```bash
./build/sep_workbench
```

## Connection Status

The workbench will display connection status:
- **"Service: Connected"** - Normal operation
- **Any other status** - System failure, check engine

## Troubleshooting

### Connection Failed

1. Ensure SEP engine is running:
   ```bash
   ps aux | grep sep
   ```

2. Check port availability:
   ```bash
   netstat -tlnp | grep 3000
   ```

3. Verify CUDA is available:
   ```bash
   nvidia-smi
   ```

### Engine Not Responding

1. Check engine logs:
   ```bash
   journalctl -u sep_api_server -f
   ```

2. Restart the service:
   ```bash
   sudo systemctl restart sep_api_server
   ```

## System Requirements

- **GPU**: NVIDIA GPU with CUDA 12.9+ support
- **Memory**: Minimum 8GB GPU memory
- **Network**: Port 3000 must be available
- **OS**: Linux with systemd support

## Important Notes

- The workbench is NOT a standalone application
- There is NO offline mode
- CUDA is NOT optional
- The engine connection is MANDATORY
