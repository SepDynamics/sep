#!/bin/bash
# SEP Engine Blender Addon Installation Script

ADDON_NAME="sep_engine_quantum"
BLENDER_VERSION="5.0"
ADDON_DIR="$HOME/.config/blender/$BLENDER_VERSION/scripts/addons/$ADDON_NAME"

echo "=== SEP Engine Blender Addon Installer ==="
echo "Installing for Blender $BLENDER_VERSION"

# Create addon directory
echo "Creating addon directory..."
mkdir -p "$ADDON_DIR"

# Copy addon files
echo "Copying addon files..."
cp -r addons/sep_engine/* "$ADDON_DIR/"

# Create __init__.py from the main module
echo "Setting up addon structure..."
cat > "$ADDON_DIR/__init__.py" << 'EOF'
"""
SEP Engine Blender Integration
==============================
Real quantum pattern processing integration for Blender 5.0
NO RANDOM - Only deterministic quantum evolution
"""

bl_info = {
    "name": "SEP Engine Quantum Integration",
    "author": "SEP Project",
    "version": (2, 0, 0),
    "blender": (5, 0, 0),
    "location": "View3D > Sidebar > SEP",
    "description": "Quantum pattern processing with SEP Engine",
    "category": "Physics",
}

# Import all modules
from . import operators
from . import panels
from . import properties
from . import connection

def register():
    operators.register()
    panels.register()
    properties.register()

def unregister():
    operators.unregister()
    panels.unregister()
    properties.unregister()

if __name__ == "__main__":
    register()
EOF

# Create a test script
echo "Creating test script..."
cat > "$ADDON_DIR/test_connection.py" << 'EOF'
import bpy

# Test SEP Engine connection
def test_sep_connection():
    print("Testing SEP Engine connection...")
    
    # Set connection parameters
    bpy.context.scene.sep_host = "localhost"
    bpy.context.scene.sep_port = 8080
    
    # Try to connect
    result = bpy.ops.sep.connect_engine()
    
    if result == {'FINISHED'}:
        print("✓ Successfully connected to SEP Engine!")
        print("  You can now process meshes with quantum algorithms")
    else:
        print("✗ Failed to connect to SEP Engine")
        print("  Make sure the service is running: sudo systemctl status sep-engine")

# Run test
if __name__ == "__main__":
    test_sep_connection()
EOF

echo ""
echo "=== Installation Complete ==="
echo ""
echo "To activate the addon in Blender:"
echo "1. Open Blender $BLENDER_VERSION"
echo "2. Go to Edit > Preferences > Add-ons"
echo "3. Search for 'SEP Engine'"
echo "4. Enable the addon"
echo ""
echo "To test the connection:"
echo "1. Make sure SEP Engine is running:"
echo "   sudo systemctl status sep-engine"
echo "2. In Blender, go to the 3D Viewport"
echo "3. Press 'N' to open the sidebar"
echo "4. Click on the 'SEP' tab"
echo "5. Click 'Connect' to connect to the engine"
echo ""
echo "Addon installed to: $ADDON_DIR"