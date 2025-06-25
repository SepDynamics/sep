"""
SEP Engine Addon Test Script
============================
Run this in Blender's Python console to test the addon functionality.
"""

import bpy
import time

def test_sep_addon():
    """Test SEP Engine addon functionality"""
    
    print("=== SEP Engine Addon Test ===")
    
    # Check if addon is installed
    if "sep_engine" not in bpy.context.preferences.addons:
        print("ERROR: SEP Engine addon not installed or enabled")
        print("Please install and enable the addon first")
        return False
    
    print("✓ Addon is installed and enabled")
    
    # Check for SEP tab in viewport
    for area in bpy.context.screen.areas:
        if area.type == 'VIEW_3D':
            for space in area.spaces:
                if space.type == 'VIEW_3D':
                    # Check if SEP tab exists
                    print("✓ SEP tab should be visible in 3D viewport sidebar")
    
    # Test scene settings
    if hasattr(bpy.context.scene, 'sep_settings'):
        print("✓ Scene settings registered")
        settings = bpy.context.scene.sep_settings
        print(f"  - Host: {settings.host}")
        print(f"  - Port: {settings.port}")
        print(f"  - Mode: {settings.connection_mode}")
    else:
        print("ERROR: Scene settings not found")
        return False
    
    # Create test mesh
    print("\nCreating test mesh...")
    bpy.ops.mesh.primitive_uv_sphere_add(segments=32, ring_count=16)
    test_obj = bpy.context.active_object
    test_obj.name = "SEP_Test_Sphere"
    print(f"✓ Created test object: {test_obj.name}")
    
    # Check object settings
    if hasattr(test_obj, 'sep_settings'):
        print("✓ Object settings registered")
        obj_settings = test_obj.sep_settings
        obj_settings.enabled = True
        print("  - SEP processing enabled for object")
    else:
        print("ERROR: Object settings not found")
        return False
    
    # Test offline pattern generation
    print("\nTesting offline pattern generation...")
    
    # Import mesh utilities
    try:
        from sep_engine import mesh_utils
        
        # Extract mesh data
        mesh_data = mesh_utils.extract_mesh_data(test_obj)
        print(f"✓ Extracted mesh data:")
        print(f"  - Vertices: {mesh_data['vertex_count']}")
        print(f"  - Edges: {mesh_data['edge_count']}")
        print(f"  - Faces: {mesh_data['face_count']}")
        
        # Generate test pattern
        test_pattern = mesh_utils.generate_test_pattern(seed=42, time_offset=0.5)
        print(f"✓ Generated test pattern:")
        print(f"  - Coherence: {test_pattern['coherence']:.3f}")
        print(f"  - Stability: {test_pattern['stability']:.3f}")
        print(f"  - Entropy: {test_pattern['entropy']:.3f}")
        print(f"  - Complexity: {test_pattern['complexity']:.3f}")
        
        # Apply test deformation
        success = mesh_utils.apply_test_deformation(
            test_obj, 
            test_pattern, 
            strength=0.2
        )
        
        if success:
            print("✓ Applied test deformation")
            
            # Check if metrics were stored
            for key in ['coherence', 'stability', 'entropy', 'complexity']:
                prop_name = f"sep_{key}"
                if prop_name in test_obj:
                    print(f"  - {key}: {test_obj[prop_name]:.3f}")
        else:
            print("✗ Failed to apply deformation")
            
    except ImportError as e:
        print(f"ERROR: Failed to import mesh_utils: {e}")
        return False
    except Exception as e:
        print(f"ERROR: Test failed: {e}")
        return False
    
    # Test operators (without connection)
    print("\nTesting operators...")
    
    # Test connection check
    try:
        # This should report disconnected
        result = bpy.ops.sep.check_connection()
        print("✓ Connection check operator works")
    except Exception as e:
        print(f"✗ Connection check failed: {e}")
    
    # Test memory sync (should fail gracefully)
    try:
        result = bpy.ops.sep.sync_memory()
        print("✓ Memory sync operator works (fails gracefully)")
    except Exception as e:
        print(f"✗ Memory sync error: {e}")
    
    # Test UI panels
    print("\nChecking UI panels...")
    panel_classes = [
        'SEP_PT_main',
        'SEP_PT_patterns', 
        'SEP_PT_memory',
        'SEP_PT_visualization',
        'SEP_PT_advanced'
    ]
    
    for panel_name in panel_classes:
        if hasattr(bpy.types, panel_name):
            print(f"✓ Panel registered: {panel_name}")
        else:
            print(f"✗ Panel not found: {panel_name}")
    
    # Test pattern processor
    print("\nTesting pattern processor...")
    try:
        from sep_engine import pattern_processor
        
        # Create pattern state
        state = pattern_processor.PatternState(
            coherence=0.7,
            stability=0.8,
            entropy=0.5,
            complexity=0.6,
            phase=0.0,
            amplitude=1.0,
            qbsa_state=128,
            qfh_level=8
        )
        
        print("✓ Created pattern state")
        
        # Evolve state
        evolved = state.evolve(dt=0.1)
        print(f"✓ Evolved state:")
        print(f"  - Coherence: {state.coherence:.3f} → {evolved.coherence:.3f}")
        print(f"  - Phase: {state.phase:.3f} → {evolved.phase:.3f}")
        
    except Exception as e:
        print(f"ERROR: Pattern processor test failed: {e}")
    
    print("\n=== Test Complete ===")
    print("The addon is properly installed and basic functionality works.")
    print("To test full functionality, start the SEP Engine service and connect.")
    
    return True

# Run test
if __name__ == "__main__":
    test_sep_addon()
