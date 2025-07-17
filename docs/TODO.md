# SEP Engine Service Deployment - Fedora 42

## Overview
Deploy SEP Engine as a production service on Fedora 42, removing demo components and configuring for data ingestion and quantum pattern processing.

## Prerequisites
- [x] Valkey (Redis fork) installed and running
- [x] Ollama installed and running  
- [ ] SEP Engine built with Release configuration
- [ ] System user 'sep' created

## Phase 1: Remove Demo Components

### 1.1 Comment Out Workbench UI
- [ ] Edit `src/api_main.cpp` - remove CyclesRenderer initialization
- [ ] Comment out workbench demo registrations
- [ ] Remove demo route handlers from API server

### 1.2 Disable Demo Dependencies
- [ ] Remove ImGui demo code references
- [ ] Disable demo orchestrator in build
- [ ] Clean up demo-specific includes

## Phase 2: Configure for Production

### 2.1 Update MCP Tools for Valkey
- [ ] Update Redis connection strings to use Valkey
- [ ] Test memory tier persistence with Valkey
- [ ] Verify pattern storage functionality

### 2.2 API Server Configuration
- [ ] Set production port (default 3000)
- [ ] Configure authentication middleware
- [ ] Enable CORS for production endpoints
- [ ] Set up rate limiting

### 2.3 Memory Tier Configuration
- [ ] Configure STM pool size (1,000 patterns)
- [ ] Configure MTM pool size (10,000 patterns)
- [ ] Configure LTM pool size (100,000 patterns)
- [ ] Set up Valkey persistence (AOF/RDB)

## Phase 3: Systemd Service Setup

### 3.1 Create Service Files
- [ ] Create `/etc/systemd/system/sep-engine.service`
- [ ] Set up environment variables
- [ ] Configure service dependencies
- [ ] Enable security hardening

### 3.2 Directory Structure
- [ ] Create `/opt/sep-engine/` installation directory
- [ ] Set up `/var/lib/sep-engine/` for data
- [ ] Configure `/var/log/sep-engine/` for logs
- [ ] Set proper permissions

## Phase 4: Integration & Testing

### 4.1 Valkey Integration
- [ ] Test pattern storage/retrieval
- [ ] Verify TTL enforcement
- [ ] Check memory tier transitions
- [ ] Monitor defragmentation

### 4.2 API Endpoint Testing
- [ ] Test `/api/v1/pattern/evolve`
- [ ] Verify coherence validation
- [ ] Check quantum processing pipeline
- [ ] Monitor memory usage

### 4.3 Performance Tuning
- [ ] Enable TBB parallelization
- [ ] Configure thread pools
- [ ] Set memory limits
- [ ] Optimize Valkey for pattern workload

## Phase 5: Monitoring & Operations

### 5.1 Logging Setup
- [ ] Configure spdlog for production
- [ ] Set up log rotation
- [ ] Enable structured logging
- [ ] Configure log levels

### 5.2 Metrics & Monitoring
- [ ] Export Prometheus metrics
- [ ] Set up health check endpoint
- [ ] Monitor pattern processing rates
- [ ] Track memory tier statistics

### 5.3 Operational Procedures
- [ ] Document startup/shutdown procedures
- [ ] Create backup/restore scripts
- [ ] Set up alerting rules
- [ ] Write troubleshooting guide

## Implementation Order

1. : Remove demo components, configure for production
2. : Set up systemd service, test Valkey integration
3. : Performance tuning, monitoring setup
4. : Documentation, operational procedures

## Key Commands

```bash
# Build for production
cmake -B build -DCMAKE_BUILD_TYPE=Release -DBUILD_DEMO=OFF
cmake --build build -j$(nproc)

# Install service
sudo cp build/bin/sep_api_server /opt/sep-engine/bin/
sudo systemctl daemon-reload
sudo systemctl enable sep-engine.service
sudo systemctl start sep-engine.service

# Check status
sudo systemctl status sep-engine.service
journalctl -u sep-engine.service -f
```

## Notes
- Valkey is drop-in compatible with Redis, minimal code changes needed
- Focus on API endpoints for pattern ingestion
- Monitor memory usage closely during initial deployment
- Consider rate limiting for pattern submission endpoints