bl_info = {
    "name": "SEP Engine Integration",
    "author": "Alexander J Nagy",
    "version": (0, 2),
    "blender": (2, 80, 0),
    "location": "View3D > Sidebar > SEP",
    "description": "Integrates with SEP Engine for quantum pattern processing",
    "category": "3D View",
}

import bpy
import bmesh
import json
import os
import time
import socket
from bpy.props import FloatProperty, BoolProperty, EnumProperty, StringProperty, IntProperty, PointerProperty

# Import quantum components
from . import quantum_state
from . import quantum_processor

# Optional imports - will be handled gracefully if missing
REQUESTS_AVAILABLE = False
NUMPY_AVAILABLE = False

try:
    import requests
    REQUESTS_AVAILABLE = True
except ImportError:
    print("SEP Engine: 'requests' module not available. HTTP API will be disabled.")

try:
    import numpy as np
    NUMPY_AVAILABLE = True
except ImportError:
    print("SEP Engine: 'numpy' module not available. Some features will be limited.")

# Import direct bridge if available
try:
    from . import direct_bridge
    DIRECT_BRIDGE_AVAILABLE = True
except ImportError:
    DIRECT_BRIDGE_AVAILABLE = False

# SEP Engine connection settings
class SEPEngineSettings:
    """Singleton class to manage SEP Engine connection settings"""
    _instance = None
    
    @classmethod
    def get_instance(cls):
        if cls._instance is None:
            cls._instance = SEPEngineSettings()
        return cls._instance
    
    def __init__(self):
        self.host = "localhost"
        self.port = 8080
        self.api_base = "/api/v1"
        self.connection_timeout = 2.0
        self.last_status = "Unknown"
        self.last_check_time = 0
        self.connected = False
        self.connection_mode = "http"  # "http" or "direct"
        self.direct_bridge = None
        self.lib_path = os.path.join(os.path.dirname(__file__), "libs", "libsep_blender.so")
    
    def get_base_url(self):
        return f"http://{self.host}:{self.port}{self.api_base}"
    
    def try_direct_bridge(self):
        """Try to initialize the direct bridge connection"""
        if not DIRECT_BRIDGE_AVAILABLE:
            return False
            
        try:
            # Initialize direct bridge
            self.direct_bridge = direct_bridge.SEPEngineBridge()
            if not self.direct_bridge.load_library(self.lib_path):
                self.last_status = "Direct bridge: Library not found"
                self.direct_bridge = None
                return False
                
            # Try to initialize
            if not self.direct_bridge.initialize():
                self.last_status = "Direct bridge: Initialization failed"
                self.direct_bridge = None
                return False
                
            # Get version info
            version = self.direct_bridge.get_version()
            self.last_status = f"Direct bridge: {version}"
            self.connection_mode = "direct"
            self.connected = True
            return True
            
        except Exception as e:
            self.last_status = f"Direct bridge error: {str(e)}"
            self.direct_bridge = None
            return False
    
    def check_connection(self, force=False):
        """Check connection to SEP Engine service"""
        # Only check every 5 seconds unless forced
        current_time = time.time()
        if not force and current_time - self.last_check_time < 5.0:
            return self.connected
            
        self.last_check_time = current_time
        
        # Try direct bridge first if available
        if DIRECT_BRIDGE_AVAILABLE and (force or self.connection_mode == "direct"):
            if self.try_direct_bridge():
                return True
        
        # Fall back to HTTP API if requests module is available
        if REQUESTS_AVAILABLE:
            try:
                response = requests.get(
                    f"{self.get_base_url()}/health",
                    timeout=self.connection_timeout
                )
                self.connected = response.ok
                if response.ok:
                    self.last_status = "HTTP API: Connected"
                    self.connection_mode = "http"
                    # Parse health data if available
                    try:
                        data = response.json()
                        if "status" in data:
                            self.last_status = f"HTTP API: {data['status']}"
                    except:
                        pass
                else:
                    self.last_status = f"HTTP Error: {response.status_code}"
                    self.connected = False
            except Exception as e:
                self.connected = False
                if isinstance(e, requests.exceptions.ConnectionError):
                    self.last_status = "HTTP API: Connection failed"
                elif isinstance(e, requests.exceptions.Timeout):
                    self.last_status = "HTTP API: Connection timeout"
                else:
                    self.last_status = f"Error: {str(e)}"
        else:
            # HTTP API not available - requests module missing
            self.connected = False
            self.last_status = "HTTP API unavailable - 'requests' module missing"
            
        return self.connected
    
    def cleanup(self):
        """Clean up resources"""
        if self.direct_bridge:
            self.direct_bridge.cleanup()
            self.direct_bridge = None

# Initialize settings
sep_settings = SEPEngineSettings.get_instance()

class SEP_OT_ResetQuantumState(bpy.types.Operator):
    """Reset quantum state to default values"""
    bl_idname = "sep.reset_quantum"
    bl_label = "Reset State"
    bl_options = {'REGISTER', 'UNDO'}
    
    def execute(self, context):
        state = context.scene.sep_quantum_state
        state.qbsa_state = 0
        state.qfh_level = 0
        state.rupture_detected = False
        state.phase = 0.0
        state.amplitude = 1.0
        self.report({'INFO'}, "Reset quantum state")
        return {'FINISHED'}

class SEP_OT_DetectRupture(bpy.types.Operator):
    """Detect quantum coherence ruptures in mesh"""
    bl_idname = "sep.detect_rupture"
    bl_label = "Detect Rupture"
    bl_options = {'REGISTER', 'UNDO'}
    
    def execute(self, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object first")
            return {'CANCELLED'}
            
        try:
            # Get quantum state
            state = context.scene.sep_quantum_state
            
            # Analyze mesh for ruptures
            result = quantum_processor.process_mesh_quantum(
                obj,
                mode=quantum_processor.ProcessingMode.COHERENCE,
                strength=1.0
            )
            
            # Update quantum state
            if result.get("coherence_applied"):
                if state.qbsa_state > 200:  # High QBSA state indicates instability
                    state.rupture_detected = True
                    state.qfh_level = min(state.qfh_level + 2, 16)
                    self.report({'WARNING'}, "Quantum rupture detected!")
                else:
                    self.report({'INFO'}, "No ruptures detected")
            
        except Exception as e:
            self.report({'ERROR'}, f"Error: {str(e)}")
            return {'CANCELLED'}
            
        return {'FINISHED'}

class SEP_OT_ProcessMesh(bpy.types.Operator):
    """Process the active mesh through SEP Engine"""
    bl_idname = "sep.process_mesh"
    bl_label = "Process Mesh"
    bl_options = {'REGISTER', 'UNDO'}
    
    coherence: FloatProperty(
        name="Coherence",
        description="Pattern coherence value",
        default=0.8,
        min=0.0,
        max=1.0
    )
    
    stability: FloatProperty(
        name="Stability",
        description="Pattern stability value",
        default=0.7,
        min=0.0,
        max=1.0
    )
    
    entropy: FloatProperty(
        name="Entropy",
        description="Pattern entropy value",
        default=0.5,
        min=0.0,
        max=1.0
    )
    
    complexity: FloatProperty(
        name="Complexity",
        description="Pattern complexity value",
        default=0.6,
        min=0.0,
        max=1.0
    )
    
    def extract_mesh_data(self, obj):
        """Extract mesh data from Blender object"""
        me = obj.data
        vertices = []
        edges = []
        faces = []
        
        # Get vertices
        for v in me.vertices:
            vertices.append({
                "x": v.co.x,
                "y": v.co.y,
                "z": v.co.z,
                "normal": {
                    "x": v.normal.x,
                    "y": v.normal.y,
                    "z": v.normal.z
                }
            })
        
        # Get edges
        for e in me.edges:
            edges.append({
                "v1": e.vertices[0],
                "v2": e.vertices[1]
            })
        
        # Get faces
        for p in me.polygons:
            face_verts = []
            for v_idx in p.vertices:
                face_verts.append(v_idx)
            faces.append({
                "vertices": face_verts,
                "normal": {
                    "x": p.normal.x,
                    "y": p.normal.y,
                    "z": p.normal.z
                }
            })
        
        # Construct data structure
        return {
            "name": obj.name,
            "vertices": vertices,
            "edges": edges,
            "faces": faces,
            "vertex_count": len(vertices),
            "edge_count": len(edges),
            "face_count": len(faces)
        }
    
    def apply_patterns(self, obj, response_data):
        """Apply patterns from SEP Engine back to mesh"""
        me = obj.data
        if "result" not in response_data:
            self.report({'ERROR'}, "Invalid response from SEP Engine")
            return False
            
        result = response_data["result"]
        
        # Store pattern metrics as custom properties
        metrics = ["coherence", "stability", "entropy", "complexity"]
        for metric in metrics:
            if metric in result:
                obj[f"sep_{metric}"] = result[metric]
                
        # Apply deformation based on pattern data
        if "patterns" in result:
            # Get the first pattern or the one with highest coherence
            pattern = None
            if isinstance(result["patterns"], list) and len(result["patterns"]) > 0:
                # Sort by coherence and get the highest one
                patterns = sorted(result["patterns"],
                                  key=lambda p: p.get("coherence", 0),
                                  reverse=True)
                pattern = patterns[0]
            elif isinstance(result["patterns"], dict):
                pattern = result["patterns"]
                
            if pattern and "deformation" in pattern:
                self.apply_deformation(obj, pattern["deformation"])
                return True
                
        # Legacy support for direct deformation
        if "deformation" in result:
            self.apply_deformation(obj, result["deformation"])
            return True
            
        return False
        
    def apply_deformation(self, obj, deformation):
        """Apply deformation to mesh vertices"""
        me = obj.data
        
        # Different formats supported
        if isinstance(deformation, list):
            # List of offsets per vertex
            for i, v in enumerate(me.vertices):
                if i < len(deformation):
                    if isinstance(deformation[i], dict):
                        # Format: [{"x": 0.1, "y": 0.2, "z": 0.3}, ...]
                        v.co.x += deformation[i].get("x", 0)
                        v.co.y += deformation[i].get("y", 0)
                        v.co.z += deformation[i].get("z", 0)
                    elif isinstance(deformation[i], list) and len(deformation[i]) >= 3:
                        # Format: [[0.1, 0.2, 0.3], ...]
                        v.co.x += deformation[i][0]
                        v.co.y += deformation[i][1]
                        v.co.z += deformation[i][2]
        elif isinstance(deformation, dict) and "vertices" in deformation:
            # Format: {"vertices": [{"index": 0, "offset": {"x": 0.1, "y": 0.2, "z": 0.3}}, ...]}
            for vertex_data in deformation["vertices"]:
                idx = vertex_data.get("index", -1)
                if 0 <= idx < len(me.vertices) and "offset" in vertex_data:
                    offset = vertex_data["offset"]
                    me.vertices[idx].co.x += offset.get("x", 0)
                    me.vertices[idx].co.y += offset.get("y", 0)
                    me.vertices[idx].co.z += offset.get("z", 0)
                    
        me.update()
    
    def execute(self, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object first")
            return {'CANCELLED'}
        
        try:
            # Get quantum state
            state = context.scene.sep_quantum_state
            
            # Select processing mode based on quantum state
            mode = quantum_processor.ProcessingMode.EVOLUTION
            if state.rupture_detected:
                mode = quantum_processor.ProcessingMode.COLLAPSE
            elif state.qfh_level > 8:
                mode = quantum_processor.ProcessingMode.ENTANGLEMENT
            elif state.qbsa_state > 128:
                mode = quantum_processor.ProcessingMode.COHERENCE
            
            # Process mesh through quantum algorithms
            result = quantum_processor.process_mesh_quantum(
                obj,
                mode=mode,
                strength=state.amplitude
            )
            
            # Update quantum state based on result
            if result.get("coherence_applied"):
                state.qbsa_state = min(state.qbsa_state + 32, 255)
                self.report({'INFO'}, "Applied quantum coherence")
            elif result.get("evolution_applied"):
                state.phase += 0.1
                self.report({'INFO'}, "Applied quantum evolution")
            elif result.get("entanglement_applied"):
                state.qfh_level = min(state.qfh_level + 1, 16)
                self.report({'INFO'}, "Applied quantum entanglement")
            elif result.get("collapse_applied"):
                state.rupture_detected = False
                state.qbsa_state = max(state.qbsa_state - 64, 0)
                self.report({'INFO'}, "Applied quantum collapse")
            else:
                self.report({'WARNING'}, "No quantum changes applied")
                
        except Exception as e:
            self.report({'ERROR'}, f"Error: {str(e)}")
            return {'CANCELLED'}
            
        return {'FINISHED'}

class SEP_OT_CheckConnection(bpy.types.Operator):
    """Check connection to SEP Engine service"""
    bl_idname = "sep.check_connection"
    bl_label = "Check Connection"
    
    def execute(self, context):
        if sep_settings.check_connection(force=True):
            self.report({'INFO'}, f"Connected to SEP Engine: {sep_settings.last_status}")
        else:
            self.report({'ERROR'}, f"Failed to connect: {sep_settings.last_status}")
        return {'FINISHED'}

class SEP_OT_ToggleConnectionMode(bpy.types.Operator):
    """Toggle between HTTP API and Direct Bridge connection mode"""
    bl_idname = "sep.toggle_connection_mode"
    bl_label = "Toggle Connection Mode"
    
    def execute(self, context):
        settings = sep_settings
        
        # Toggle mode
        if settings.connection_mode == "http":
            # Try to switch to direct mode
            if settings.try_direct_bridge():
                self.report({'INFO'}, "Switched to direct bridge mode")
                context.scene.sep_connection_mode = 'DIRECT'
            else:
                self.report({'ERROR'}, f"Failed to initialize direct bridge: {settings.last_status}")
        else:
            # Switch to HTTP mode
            settings.connection_mode = "http"
            if settings.direct_bridge:
                settings.direct_bridge.cleanup()
                settings.direct_bridge = None
            context.scene.sep_connection_mode = 'HTTP'
            settings.check_connection(force=True)
            self.report({'INFO'}, "Switched to HTTP API mode")
            
        return {'FINISHED'}

class SEP_OT_ExportMesh(bpy.types.Operator):
    """Export processed mesh to OBJ file"""
    bl_idname = "sep.export_mesh"
    bl_label = "Export Processed Mesh"
    
    def execute(self, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object first")
            return {'CANCELLED'}
            
        # Create output directory
        output_dir = os.path.join(os.path.dirname(__file__), "output")
        os.makedirs(output_dir, exist_ok=True)
        
        # Create filename with timestamp
        timestamp = time.strftime("%Y%m%d_%H%M%S")
        filename = f"{obj.name}_{timestamp}.obj"
        filepath = os.path.join(output_dir, filename)
        
        # Export mesh
        try:
            bpy.ops.export_scene.obj(
                filepath=filepath,
                use_selection=True,
                use_mesh_modifiers=True,
                use_normals=True
            )
            self.report({'INFO'}, f"Mesh exported to {filepath}")
        except Exception as e:
            self.report({'ERROR'}, f"Export failed: {str(e)}")
            return {'CANCELLED'}
            
        return {'FINISHED'}

class SEP_PT_MeshPanel(bpy.types.Panel):
    """SEP Engine control panel"""
    bl_label = "SEP Engine"
    bl_idname = "SEP_PT_MeshPanel"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = 'SEP'
    
    def draw(self, context):
        layout = self.layout
        
        # Quantum state
        box = layout.box()
        box.label(text="Quantum State", icon='PHYSICS')
        
        state = context.scene.sep_quantum_state
        col = box.column(align=True)
        col.prop(state, "qbsa_state")
        col.prop(state, "qfh_level")
        col.prop(state, "phase")
        col.prop(state, "amplitude")
        col.prop(state, "rupture_detected")
        
        row = box.row()
        row.operator("sep.reset_quantum", icon='FILE_REFRESH')
        
        # Engine connection settings
        box = layout.box()
        box.label(text="SEP Engine Connection", icon='NETWORK_DRIVE')
        
        row = box.row()
        row.prop(context.scene, "sep_host", text="Host")
        row.prop(context.scene, "sep_port", text="Port")
        
        # Connection mode selector
        row = box.row()
        row.label(text="Connection Mode:")
        row.prop(context.scene, "sep_connection_mode", text="")
        row.operator("sep.toggle_connection_mode", icon='FILE_REFRESH', text="")
        
        # Direct bridge settings if available
        if DIRECT_BRIDGE_AVAILABLE:
            if context.scene.sep_connection_mode == 'DIRECT':
                lib_row = box.row()
                lib_row.prop(context.scene, "sep_library_path", text="Library")
        
        # Engine status
        status_row = box.row()
        
        # Check connection status
        connected = sep_settings.check_connection()
        if connected:
            status_row.label(text=sep_settings.last_status, icon='CHECKMARK')
        else:
            status_row.label(text=sep_settings.last_status, icon='ERROR')
        
        status_row.operator("sep.check_connection", icon='FILE_REFRESH', text="")
        
        # Process mesh
        box = layout.box()
        box.label(text="Process Mesh", icon='MESH_DATA')
        
        row = box.row()
        row.operator("sep.detect_rupture", icon='ERROR')
        row.operator("sep.process_mesh", icon='PLAY')
        
        # Export/visualization options
        row = box.row()
        row.operator("sep.export_mesh", icon='EXPORT')
        
        # Pattern parameters
        box = layout.box()
        box.label(text="Pattern Parameters", icon='PHYSICS')
        
        col = box.column(align=True)
        col.prop(context.scene, "sep_coherence")
        col.prop(context.scene, "sep_stability")
        col.prop(context.scene, "sep_entropy")
        col.prop(context.scene, "sep_complexity")

def update_connection_settings(self, context):
    """Callback for updating connection settings"""
    settings = SEPEngineSettings.get_instance()
    settings.host = context.scene.sep_host
    settings.port = context.scene.sep_port
    settings.check_connection(force=True)
    return None

def update_connection_mode(self, context):
    """Callback for updating connection mode"""
    settings = SEPEngineSettings.get_instance()
    mode = context.scene.sep_connection_mode
    
    if mode == 'DIRECT' and settings.connection_mode != "direct":
        # Try to switch to direct mode
        if not settings.try_direct_bridge():
            # If failed, revert to HTTP mode
            context.scene.sep_connection_mode = 'HTTP'
    elif mode == 'HTTP' and settings.connection_mode != "http":
        # Switch to HTTP mode
        settings.connection_mode = "http"
        if settings.direct_bridge:
            settings.direct_bridge.cleanup()
            settings.direct_bridge = None
        settings.check_connection(force=True)
    
    return None

def update_library_path(self, context):
    """Callback for updating library path"""
    settings = SEPEngineSettings.get_instance()
    settings.lib_path = context.scene.sep_library_path
    
    # Only try to reload if we're in direct mode
    if settings.connection_mode == "direct" and settings.direct_bridge:
        # Cleanup existing bridge
        settings.direct_bridge.cleanup()
        settings.direct_bridge = None
        
        # Try to initialize with new path
        settings.try_direct_bridge()
    
    return None

def register():
    # Install dependencies if needed
    if not REQUESTS_AVAILABLE:
        try:
            from . import install_script
            if install_script.install_requirements():
                global REQUESTS_AVAILABLE
                import requests
                REQUESTS_AVAILABLE = True
        except Exception as e:
            print(f"Failed to install dependencies: {e}")
    
    # Register quantum components first
    from . import quantum_state
    from . import quantum_processor
    quantum_state.register()
    
    # Register operators
    bpy.utils.register_class(SEP_OT_ResetQuantumState)
    bpy.utils.register_class(SEP_OT_DetectRupture)
    bpy.utils.register_class(SEP_OT_ProcessMesh)
    bpy.utils.register_class(SEP_OT_CheckConnection)
    bpy.utils.register_class(SEP_OT_ToggleConnectionMode)
    bpy.utils.register_class(SEP_OT_ExportMesh)
    
    # Register panel
    bpy.utils.register_class(SEP_PT_MeshPanel)
    
    # Register connection properties
    bpy.types.Scene.sep_host = StringProperty(
        name="Host",
        description="SEP Engine host address",
        default="localhost",
        update=update_connection_settings
    )
    
    bpy.types.Scene.sep_port = IntProperty(
        name="Port",
        description="SEP Engine port",
        default=8080,
        min=1,
        max=65535,
        update=update_connection_settings
    )
    
    # Connection mode property
    bpy.types.Scene.sep_connection_mode = EnumProperty(
        name="Connection Mode",
        description="Method to connect to SEP Engine",
        items=[
            ('HTTP', "HTTP API", "Connect via HTTP API"),
            ('DIRECT', "Direct Bridge", "Connect via direct C API bridge"),
        ],
        default='HTTP',
        update=update_connection_mode
    )
    
    # Library path for direct bridge
    default_lib_path = os.path.join(os.path.dirname(__file__), "libs", "libsep_blender.so")
    bpy.types.Scene.sep_library_path = StringProperty(
        name="Library Path",
        description="Path to libsep_blender.so",
        default=default_lib_path,
        subtype='FILE_PATH',
        update=update_library_path
    )
    
    # Register pattern properties
    bpy.types.Scene.sep_coherence = FloatProperty(
        name="Coherence",
        description="Pattern coherence value",
        default=0.8,
        min=0.0,
        max=1.0
    )
    
    bpy.types.Scene.sep_stability = FloatProperty(
        name="Stability",
        description="Pattern stability value",
        default=0.7,
        min=0.0,
        max=1.0
    )
    
    bpy.types.Scene.sep_entropy = FloatProperty(
        name="Entropy",
        description="Pattern entropy value",
        default=0.5,
        min=0.0,
        max=1.0
    )
    
    bpy.types.Scene.sep_complexity = FloatProperty(
        name="Complexity",
        description="Pattern complexity value",
        default=0.6,
        min=0.0,
        max=1.0
    )
    
    # Initialize settings with values from scene properties
    if bpy.context and hasattr(bpy.context, 'scene'):
        settings = SEPEngineSettings.get_instance()
        settings.host = bpy.context.scene.sep_host
        settings.port = bpy.context.scene.sep_port
        settings.lib_path = bpy.context.scene.sep_library_path

def unregister():
    # Clean up resources
    settings = SEPEngineSettings.get_instance()
    if settings.direct_bridge:
        settings.direct_bridge.cleanup()
        settings.direct_bridge = None
    
    # Unregister quantum components first
    from . import quantum_state
    quantum_state.unregister()
    
    # Unregister operators
    bpy.utils.unregister_class(SEP_OT_ResetQuantumState)
    bpy.utils.unregister_class(SEP_OT_DetectRupture)
    bpy.utils.unregister_class(SEP_OT_ProcessMesh)
    bpy.utils.unregister_class(SEP_OT_CheckConnection)
    bpy.utils.unregister_class(SEP_OT_ToggleConnectionMode)
    bpy.utils.unregister_class(SEP_OT_ExportMesh)
    
    # Unregister panel
    bpy.utils.unregister_class(SEP_PT_MeshPanel)
    
    # Unregister properties
    del bpy.types.Scene.sep_host
    del bpy.types.Scene.sep_port
    del bpy.types.Scene.sep_connection_mode
    del bpy.types.Scene.sep_library_path
    del bpy.types.Scene.sep_coherence
    del bpy.types.Scene.sep_stability
    del bpy.types.Scene.sep_entropy
    del bpy.types.Scene.sep_complexity

if __name__ == "__main__":
    register()