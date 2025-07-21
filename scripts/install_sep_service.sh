#!/bin/bash
# SEP Engine Service Installation Script

set -e

echo "Installing SEP Engine as system service..."

# Check if running as root
if [ "$EUID" -ne 0 ]; then 
    echo "Please run as root (use sudo)"
    exit 1
fi

# Create sep user if doesn't exist
if ! id -u sep >/dev/null 2>&1; then
    echo "Creating sep user..."
    useradd -r -s /bin/false -d /var/lib/sep -m sep
fi

# Create directories
echo "Creating directories..."
mkdir -p /opt/sep
mkdir -p /etc/sep
mkdir -p /var/lib/sep/memory
mkdir -p /var/log/sep

# Set ownership
chown -R sep:sep /var/lib/sep
chown -R sep:sep /var/log/sep

#Stop service
sudo systemctl stop sep-engine

# Copy executable
echo "Installing SEP executable..."
if [ -f "build/sep" ]; then
    cp build/sep /usr/local/bin/
    chmod +x /usr/local/bin/sep
else
    echo "Error: build/sep not found. Please build first with ./build_sep_simple.sh"
    exit 1
fi

# Create default config
echo "Creating default configuration..."
cat > /etc/sep/engine.conf << EOF
{
    "service": {
        "port": 3000,
        "host": "127.0.0.1",
        "workers": 4
    },
    "engine": {
        "gpu_enabled": true,
        "memory_tiers": {
            "L1_size": "1GB",
            "L2_size": "4GB",
            "L3_size": "16GB"
        },
        "pattern_cache": "/var/lib/sep/memory/patterns.db",
        "response_model": "/var/lib/sep/memory/responses.nn"
    },
    "telemetry": {
        "prometheus_port": 9090,
        "websocket_port": 8080,
        "update_interval_ms": 100
    }
}
EOF

# Install systemd service
echo "Installing systemd service..."
cp config/sep-engine.service /etc/systemd/system/
systemctl daemon-reload

echo "SEP Engine service installed successfully!"
sudo systemctl enable sep-engine
sudo systemctl status sep-engine
