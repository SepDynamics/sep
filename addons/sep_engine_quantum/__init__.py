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
