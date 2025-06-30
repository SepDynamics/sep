"""
SEP Engine Properties
=====================
Defines all custom properties and data structures for the addon.
"""

import bpy
from bpy.props import (
    StringProperty, IntProperty, FloatProperty, BoolProperty,
    EnumProperty, PointerProperty, CollectionProperty, FloatVectorProperty
)
from bpy.types import PropertyGroup, Scene, Object, Mesh
import time
from typing import Dict, Any

class SEPPatternMetrics(PropertyGroup):
    """Pattern metrics from SEP Engine"""
    
    coherence: FloatProperty(
        name="Coherence",
        description="Pattern coherence value (0-1)",
        default=0.5,
        min=0.0,
        max=1.0,
        precision=3,
        subtype='FACTOR'
    )
    
    stability: FloatProperty(
        name="Stability",
        description="Pattern stability value (0-1)",
        default=0.5,
        min=0.0,
        max=1.0,
        precision=3,
        subtype='FACTOR'
    )
    
    entropy: FloatProperty(
        name="Entropy",
        description="Pattern entropy value (0-1)",
        default=0.5,
        min=0.0,
        max=1.0,
        precision=3,
        subtype='FACTOR'
    )
    
    complexity: FloatProperty(
        name="Complexity",
        description="Pattern complexity value (0-1)",
        default=0.5,
        min=0.0,
        max=1.0,
        precision=3,
        subtype='FACTOR'
    )
    
    timestamp: FloatProperty(
        name="Timestamp",
        description="When this metric was calculated",
        default=0.0
    )
    
    def to_dict(self) -> Dict[str, float]:
        """Convert to dictionary"""
        return {
            "coherence": self.coherence,
            "stability": self.stability,
            "entropy": self.entropy,
            "complexity": self.complexity,
            "timestamp": self.timestamp
        }
    
    def from_dict(self, data: Dict[str, Any]):
        """Update from dictionary"""
        self.coherence = data.get("coherence", 0.5)
        self.stability = data.get("stability", 0.5)
        self.entropy = data.get("entropy", 0.5)
        self.complexity = data.get("complexity", 0.5)
        self.timestamp = time.time()

class SEPQuantumState(PropertyGroup):
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

class SEPMemoryTier(PropertyGroup):
    """Memory tier information"""
    
    tier_type: EnumProperty(
        name="Tier Type",
        description="Memory tier type",
        items=[
            ('STM', "Short-Term", "Short-term memory (active patterns)"),
            ('MTM', "Medium-Term", "Medium-term memory (recent patterns)"),
            ('LTM', "Long-Term", "Long-term memory (stable patterns)")
        ],
        default='STM'
    )
    
    capacity: IntProperty(
        name="Capacity",
        description="Maximum patterns in this tier",
        default=1000,
        min=1
    )
    
    usage: IntProperty(
        name="Usage",
        description="Current patterns in this tier",
        default=0,
        min=0
    )
    
    coherence_threshold: FloatProperty(
        name="Coherence Threshold",
        description="Minimum coherence for this tier",
        default=0.5,
        min=0.0,
        max=1.0,
        subtype='FACTOR'
    )

class SEPPattern(PropertyGroup):
    """Individual pattern data"""
    
    pattern_id: StringProperty(
        name="Pattern ID",
        description="Unique pattern identifier",
        default=""
    )
    
    source_object: StringProperty(
        name="Source Object",
        description="Name of source object",
        default=""
    )
    
    metrics: PointerProperty(
        type=SEPPatternMetrics,
        name="Metrics"
    )
    
    quantum_state: PointerProperty(
        type=SEPQuantumState,
        name="Quantum State"
    )
    
    memory_tier: EnumProperty(
        name="Memory Tier",
        description="Current memory tier",
        items=[
            ('STM', "Short-Term", "Short-term memory"),
            ('MTM', "Medium-Term", "Medium-term memory"),
            ('LTM', "Long-Term", "Long-term memory"),
            ('NONE', "None", "Not in memory")
        ],
        default='NONE'
    )
    
    creation_time: FloatProperty(
        name="Creation Time",
        description="When pattern was created",
        default=0.0
    )
    
    last_update: FloatProperty(
        name="Last Update",
        description="When pattern was last updated",
        default=0.0
    )
    
    evolution_count: IntProperty(
        name="Evolution Count",
        description="Number of evolution steps",
        default=0,
        min=0
    )

class SEPObjectSettings(PropertyGroup):
    """Per-object SEP settings"""
    
    enabled: BoolProperty(
        name="Enable SEP Processing",
        description="Enable SEP pattern processing for this object",
        default=False
    )
    
    pattern_id: StringProperty(
        name="Pattern ID",
        description="Associated pattern ID",
        default=""
    )
    
    auto_evolve: BoolProperty(
        name="Auto Evolve",
        description="Automatically evolve pattern on frame change",
        default=False
    )
    
    evolution_rate: FloatProperty(
        name="Evolution Rate",
        description="Rate of pattern evolution",
        default=1.0,
        min=0.0,
        max=10.0
    )
    
    metrics: PointerProperty(
        type=SEPPatternMetrics,
        name="Current Metrics"
    )
    
    quantum_state: PointerProperty(
        type=SEPQuantumState,
        name="Quantum State"
    )
    
    # Deformation settings
    apply_deformation: BoolProperty(
        name="Apply Deformation",
        description="Apply pattern-based deformation to mesh",
        default=True
    )
    
    deformation_strength: FloatProperty(
        name="Deformation Strength",
        description="Strength of pattern deformation",
        default=0.1,
        min=0.0,
        max=1.0,
        subtype='FACTOR'
    )
    
    deformation_mode: EnumProperty(
        name="Deformation Mode",
        description="How to apply deformation",
        items=[
            ('NORMAL', "Normal", "Along vertex normals"),
            ('RADIAL', "Radial", "Radial from object center"),
            ('WAVE', "Wave", "Wave-like deformation"),
            ('QUANTUM', "Quantum", "Quantum field deformation")
        ],
        default='NORMAL'
    )

class SEPSceneSettings(PropertyGroup):
    """Global SEP settings for the scene"""
    
    # Connection settings
    host: StringProperty(
        name="Host",
        description="SEP Engine host address",
        default="localhost"
    )
    
    port: IntProperty(
        name="Port",
        description="SEP Engine port",
        default=8080,
        min=1,
        max=65535
    )
    
    connection_mode: EnumProperty(
        name="Connection Mode",
        description="How to connect to SEP Engine",
        items=[
            ('http', "HTTP API", "Connect via HTTP REST API"),
            ('direct', "Direct Bridge", "Connect via C API bridge"),
            ('hybrid', "Hybrid", "Use both connections")
        ],
        default='http'
    )
    
    library_path: StringProperty(
        name="Library Path",
        description="Path to SEP bridge library",
        default="",
        subtype='FILE_PATH'
    )
    
    # Processing settings
    auto_process: BoolProperty(
        name="Auto Process",
        description="Automatically process patterns on changes",
        default=False
    )
    
    batch_size: IntProperty(
        name="Batch Size",
        description="Number of patterns to process at once",
        default=32,
        min=1,
        max=1024
    )
    
    process_interval: FloatProperty(
        name="Process Interval",
        description="Seconds between automatic processing",
        default=0.1,
        min=0.01,
        max=10.0
    )
    
    # Memory settings
    memory_tiers: CollectionProperty(
        type=SEPMemoryTier,
        name="Memory Tiers"
    )
    
    auto_promote: BoolProperty(
        name="Auto Promote",
        description="Automatically promote stable patterns",
        default=True
    )
    
    promotion_threshold: FloatProperty(
        name="Promotion Threshold",
        description="Coherence threshold for promotion",
        default=0.8,
        min=0.0,
        max=1.0,
        subtype='FACTOR'
    )
    
    # Pattern collection
    patterns: CollectionProperty(
        type=SEPPattern,
        name="Patterns"
    )
    
    active_pattern_index: IntProperty(
        name="Active Pattern",
        default=0,
        min=0
    )
    
    # Visualization settings
    show_metrics: BoolProperty(
        name="Show Metrics",
        description="Display pattern metrics in viewport",
        default=True
    )
    
    show_quantum_state: BoolProperty(
        name="Show Quantum State",
        description="Visualize quantum state in viewport",
        default=False
    )
    
    metric_colors: FloatVectorProperty(
        name="Metric Colors",
        description="Colors for metric visualization",
        subtype='COLOR',
        default=(1.0, 1.0, 1.0, 1.0),
        size=4,
        min=0.0,
        max=1.0
    )
    
    # Debug settings
    debug_mode: BoolProperty(
        name="Debug Mode",
        description="Enable debug output",
        default=False
    )
    
    log_level: EnumProperty(
        name="Log Level",
        description="Logging verbosity",
        items=[
            ('ERROR', "Error", "Only errors"),
            ('WARNING', "Warning", "Warnings and errors"),
            ('INFO', "Info", "General information"),
            ('DEBUG', "Debug", "Detailed debug output")
        ],
        default='INFO'
    )

class SEPMeshCache(PropertyGroup):
    """Cache for mesh data"""
    
    vertex_count: IntProperty(
        name="Vertex Count",
        default=0
    )
    
    face_count: IntProperty(
        name="Face Count",
        default=0
    )
    
    edge_count: IntProperty(
        name="Edge Count",
        default=0
    )
    
    cached_hash: StringProperty(
        name="Cached Hash",
        default=""
    )
    
    cache_time: FloatProperty(
        name="Cache Time",
        default=0.0
    )

def register_properties():
    """Register all properties"""
    # Scene properties
    Scene.sep_settings = PointerProperty(
        type=SEPSceneSettings,
        name="SEP Settings"
    )
    
    # Object properties
    Object.sep_settings = PointerProperty(
        type=SEPObjectSettings,
        name="SEP Settings"
    )
    
    Object.sep_cache = PointerProperty(
        type=SEPMeshCache,
        name="SEP Cache"
    )
    
    # Initialize memory tiers on registration
    def init_memory_tiers(scene):
        if len(scene.sep_settings.memory_tiers) == 0:
            # STM
            tier = scene.sep_settings.memory_tiers.add()
            tier.tier_type = 'STM'
            tier.capacity = 100
            tier.coherence_threshold = 0.3
            
            # MTM
            tier = scene.sep_settings.memory_tiers.add()
            tier.tier_type = 'MTM'
            tier.capacity = 500
            tier.coherence_threshold = 0.6
            
            # LTM
            tier = scene.sep_settings.memory_tiers.add()
            tier.tier_type = 'LTM'
            tier.capacity = 1000
            tier.coherence_threshold = 0.8
    
    # Initialize on first scene load
    if bpy.context.scene:
        init_memory_tiers(bpy.context.scene)

def unregister_properties():
    """Unregister all properties"""
    del Scene.sep_settings
    del Object.sep_settings
    del Object.sep_cache

# Classes to register
classes = [
    SEPPatternMetrics,
    SEPQuantumState,
    SEPMemoryTier,
    SEPPattern,
    SEPObjectSettings,
    SEPSceneSettings,
    SEPMeshCache
]
