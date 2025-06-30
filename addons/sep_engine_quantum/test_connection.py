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
