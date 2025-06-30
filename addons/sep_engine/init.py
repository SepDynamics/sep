"""
SEP Engine Blender Integration
==============================
A comprehensive addon for integrating Blender with the SEP Engine's
quantum-inspired pattern processing framework.

Author: Alexander J Nagy
Version: 1.0.0
"""

bl_info = {
    "name": "SEP Engine Integration",
    "author": "Alexander J Nagy",
    "version": (1, 0, 0),
    "blender": (3, 0, 0),
    "location": "View3D > Sidebar > SEP Engine",
    "description": "Deep integration with SEP Engine for quantum pattern processing",
    "category": "3D View",
}

import bpy
from bpy.props import (
    StringProperty, IntProperty, FloatProperty, BoolProperty,
    EnumProperty, PointerProperty, CollectionProperty
)
from bpy.types import PropertyGroup

# Import all modules
from . import (
    connection,
    operators,
    ui,
    properties,
    handlers,
    preferences
)

# Module reload support for development
if "bpy" in locals():
    import importlib
    importlib.reload(connection)
    importlib.reload(operators)
    importlib.reload(ui)
    importlib.reload(properties)
    importlib.reload(handlers)
    importlib.reload(preferences)

classes = []
modules = [
    properties,
    preferences,
    connection,
    operators,
    ui,
    handlers
]

def register():
    """Register the addon"""
    # Gather all classes from modules
    global classes
    classes = []
    
    for module in modules:
        if hasattr(module, "classes"):
            classes.extend(module.classes)
    
    # Register all classes
    for cls in classes:
        bpy.utils.register_class(cls)
    
    # Register properties
    properties.register_properties()
    
    # Register handlers
    handlers.register_handlers()
    
    # Initialize connection manager
    connection.initialize()
    
    print("SEP Engine addon registered successfully")

def unregister():
    """Unregister the addon"""
    # Unregister handlers
    handlers.unregister_handlers()
    
    # Unregister properties
    properties.unregister_properties()
    
    # Cleanup connection
    connection.cleanup()
    
    # Unregister all classes
    for cls in reversed(classes):
        bpy.utils.unregister_class(cls)
    
    print("SEP Engine addon unregistered")

if __name__ == "__main__":
    register()
