"""
SEP Engine Direct Bridge Example
This demonstrates how to use the C API bridge directly with ctypes
NOTE: This requires libsep_blender.so to be available
"""

import ctypes
import bpy
import os
from ctypes import c_void_p, c_float, c_uint64, c_uint32, c_int, c_bool, c_char_p, POINTER, Structure, c_size_t

# Define C structures to match SEP Engine API
class SEPConfig(Structure):
    _fields_ = [
        ("version", c_uint32),
        ("log_level", c_int),
        ("enable_cuda", c_bool)
    ]

class SEPPatternMetrics(Structure):
    _fields_ = [
        ("coherence", c_float),
        ("stability", c_float),
        ("entropy", c_float),
        ("complexity", c_float)
    ]

class SEPAudioMetrics(Structure):
    _fields_ = [
        ("amplitude", c_float),
        ("frequency", c_float),
        ("coherence", c_float)
    ]

# SEP API Result enum values
SEP_SUCCESS = 0
SEP_FAILURE = 1
SEP_INVALID_ARGUMENT = 2
SEP_INVALID_STATE = 3
SEP_INIT_FAILED = 4
SEP_INVALID_OBJECT = 5
SEP_ALLOCATION_FAILED = 6
SEP_UNKNOWN_ERROR = 999

# Memory tier enum values (placeholder)
SEP_MEMORY_TIER_STM = 0
SEP_MEMORY_TIER_MTM = 1
SEP_MEMORY_TIER_LTM = 2

class SEPEngineBridge:
    """Class to interact with SEP Engine through the C API"""
    
    def __init__(self):
        self.lib = None
        self.bridge = None
        self.mesh_handles = {}
    
    def load_library(self, lib_path="./libsep_blender.so"):
        """Load the SEP Engine shared library"""
        try:
            self.lib = ctypes.CDLL(lib_path)
            print(f"Loaded SEP Engine library: {lib_path}")
            
            # Define function signatures
            self._define_function_signatures()
            return True
        except Exception as e:
            print(f"Error loading SEP Engine library: {e}")
            return False
    
    def _define_function_signatures(self):
        """Define the function signatures for the C API"""
        if not self.lib:
            return
            
        # sep_blender_init
        self.lib.sep_blender_init.argtypes = [c_void_p, POINTER(SEPConfig), POINTER(POINTER(c_void_p))]
        self.lib.sep_blender_init.restype = c_int
        
        # sep_register_mesh
        self.lib.sep_register_mesh.argtypes = [c_void_p, c_void_p, c_void_p, POINTER(c_uint64)]
        self.lib.sep_register_mesh.restype = c_int
        
        # sep_update_mesh
        self.lib.sep_update_mesh.argtypes = [c_void_p, c_uint64, POINTER(SEPPatternMetrics), POINTER(c_bool)]
        self.lib.sep_update_mesh.restype = c_int
        
        # sep_process_audio
        self.lib.sep_process_audio.argtypes = [c_void_p, POINTER(c_float), c_size_t, POINTER(SEPAudioMetrics)]
        self.lib.sep_process_audio.restype = c_int
        
        # sep_sync_memory
        self.lib.sep_sync_memory.argtypes = [c_void_p, c_int, c_bool]
        self.lib.sep_sync_memory.restype = c_int
        
        # sep_blender_cleanup
        self.lib.sep_blender_cleanup.argtypes = [c_void_p]
        self.lib.sep_blender_cleanup.restype = c_int
        
        # sep_get_metrics
        self.lib.sep_get_metrics.argtypes = [c_void_p, POINTER(SEPPatternMetrics)]
        self.lib.sep_get_metrics.restype = c_int
        
        # sep_reset_state
        self.lib.sep_reset_state.argtypes = [c_void_p]
        self.lib.sep_reset_state.restype = c_int
        
        # Version information
        self.lib.sep_get_version.argtypes = []
        self.lib.sep_get_version.restype = c_char_p
        
        self.lib.sep_get_build_info.argtypes = []
        self.lib.sep_get_build_info.restype = c_char_p
    
    def initialize(self, gpu_context=None, enable_cuda=False):
        """Initialize the SEP Engine bridge"""
        if not self.lib:
            print("Library not loaded")
            return False
            
        # Create config struct
        config = SEPConfig()
        config.version = 1
        config.log_level = 2  # INFO
        config.enable_cuda = enable_cuda
        
        # Create bridge pointer
        bridge_pp = POINTER(c_void_p)()
        
        # Initialize bridge
        result = self.lib.sep_blender_init(
            c_void_p(gpu_context) if gpu_context else None,
            ctypes.byref(config),
            ctypes.byref(bridge_pp)
        )
        
        if result == SEP_SUCCESS:
            self.bridge = bridge_pp.contents
            print("SEP Engine bridge initialized successfully")
            return True
        else:
            print(f"Failed to initialize SEP Engine bridge: {result}")
            return False
    
    def register_mesh(self, blender_object, mesh_data):
        """Register a Blender mesh with SEP Engine"""
        if not self.lib or not self.bridge:
            print("Bridge not initialized")
            return None
            
        # Get pointers to Blender objects
        obj_ptr = c_void_p(blender_object.as_pointer())
        mesh_ptr = c_void_p(mesh_data.as_pointer())
        
        # Create handle output
        mesh_handle = c_uint64(0)
        
        # Register mesh
        result = self.lib.sep_register_mesh(
            self.bridge,
            obj_ptr,
            mesh_ptr,
            ctypes.byref(mesh_handle)
        )
        
        if result == SEP_SUCCESS:
            handle_value = mesh_handle.value
            self.mesh_handles[blender_object.name] = handle_value
            print(f"Registered mesh {blender_object.name} with handle {handle_value}")
            return handle_value
        else:
            print(f"Failed to register mesh: {result}")
            return None
    
    def update_mesh(self, mesh_handle, pattern_metrics):
        """Update a mesh based on pattern metrics"""
        if not self.lib or not self.bridge:
            print("Bridge not initialized")
            return False
            
        # Create metrics struct
        metrics = SEPPatternMetrics()
        metrics.coherence = pattern_metrics.get("coherence", 0.5)
        metrics.stability = pattern_metrics.get("stability", 0.5)
        metrics.entropy = pattern_metrics.get("entropy", 0.5)
        metrics.complexity = pattern_metrics.get("complexity", 0.5)
        
        # Create updated flag
        updated = c_bool(False)
        
        # Update mesh
        result = self.lib.sep_update_mesh(
            self.bridge,
            c_uint64(mesh_handle),
            ctypes.byref(metrics),
            ctypes.byref(updated)
        )
        
        if result == SEP_SUCCESS:
            was_updated = updated.value
            print(f"Mesh update {'applied' if was_updated else 'not needed'}")
            return was_updated
        else:
            print(f"Failed to update mesh: {result}")
            return False
    
    def sync_memory(self, tier=SEP_MEMORY_TIER_STM, force=True):
        """Synchronize SEP memory tier with Blender"""
        if not self.lib or not self.bridge:
            print("Bridge not initialized")
            return False
            
        # Sync memory
        result = self.lib.sep_sync_memory(
            self.bridge,
            c_int(tier),
            c_bool(force)
        )
        
        if result == SEP_SUCCESS:
            print(f"Memory tier {tier} synchronized")
            return True
        else:
            print(f"Failed to synchronize memory: {result}")
            return False
    
    def cleanup(self):
        """Clean up the SEP Engine bridge"""
        if not self.lib or not self.bridge:
            return True
            
        # Cleanup bridge
        result = self.lib.sep_blender_cleanup(self.bridge)
        
        if result == SEP_SUCCESS:
            self.bridge = None
            self.mesh_handles = {}
            print("SEP Engine bridge cleaned up")
            return True
        else:
            print(f"Failed to clean up SEP Engine bridge: {result}")
            return False
    
    def get_version(self):
        """Get SEP Engine version"""
        if not self.lib:
            return "Unknown"
            
        version = self.lib.sep_get_version()
        return version.decode('utf-8') if version else "Unknown"
    
    def get_build_info(self):
        """Get SEP Engine build information"""
        if not self.lib:
            return "Unknown"
            
        build_info = self.lib.sep_get_build_info()
        return build_info.decode('utf-8') if build_info else "Unknown"

# Demo usage
def test_sep_bridge():
    """Test SEP Engine bridge with active object"""
    try:
        # Get active object
        obj = bpy.context.active_object
        if not obj or obj.type != 'MESH':
            print("No active mesh object selected")
            return False
            
        # Initialize bridge
        bridge = SEPEngineBridge()
        if not bridge.load_library():
            print("Could not load SEP Engine library")
            return False
            
        # Initialize bridge
        if not bridge.initialize():
            print("Could not initialize SEP Engine bridge")
            return False
            
        # Print version info
        print(f"SEP Engine Version: {bridge.get_version()}")
        print(f"Build Info: {bridge.get_build_info()}")
        
        # Register mesh
        mesh_handle = bridge.register_mesh(obj, obj.data)
        if not mesh_handle:
            print("Could not register mesh")
            bridge.cleanup()
            return False
            
        # Update mesh with pattern metrics
        pattern_metrics = {
            "coherence": 0.8,
            "stability": 0.7,
            "entropy": 0.5,
            "complexity": 0.6
        }
        
        bridge.update_mesh(mesh_handle, pattern_metrics)
        
        # Sync memory
        bridge.sync_memory()
        
        # Cleanup
        bridge.cleanup()
        
        return True
    except Exception as e:
        print(f"Error testing SEP bridge: {e}")
        return False

if __name__ == "__main__":
    # This will run when the script is executed in Blender's text editor
    test_sep_bridge()