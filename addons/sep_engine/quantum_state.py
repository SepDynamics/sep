"""Quantum state management for SEP Engine"""

import bpy
from bpy.props import FloatProperty, BoolProperty, IntProperty

class SEPQuantumState(bpy.types.PropertyGroup):
    """Quantum state information"""
    
    qbsa_state: IntProperty(
        name="QBSA State",
        description="Quantum Binary State Analysis result",
        default=0
    )
    
    qfh_level: IntProperty(
        name="QFH Level", 
        description="Quantum Fourier Hierarchy level",
        default=0,
        min=0,
        max=16
    )
    
    rupture_detected: BoolProperty(
        name="Rupture Detected",
        description="Whether a coherence rupture was detected",
        default=False
    )
    
    phase: FloatProperty(
        name="Phase",
        description="Quantum phase angle",
        default=0.0,
        min=-3.14159,
        max=3.14159,
        subtype='ANGLE'
    )
    
    amplitude: FloatProperty(
        name="Amplitude",
        description="Quantum amplitude",
        default=1.0,
        min=0.0,
        precision=3
    )

def register():
    bpy.utils.register_class(SEPQuantumState)
    bpy.types.Scene.sep_quantum_state = bpy.props.PointerProperty(type=SEPQuantumState)

def unregister():
    del bpy.types.Scene.sep_quantum_state
    bpy.utils.unregister_class(SEPQuantumState)