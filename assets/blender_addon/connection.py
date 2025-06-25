"""
SEP Engine Connection Manager
=============================
Manages connections to the SEP Engine service with support for both
HTTP API and direct C bridge connections.
"""

import bpy
import ctypes
import threading
import queue
import time
import json
import socket
from typing import Optional, Dict, Any, Callable, Union
from dataclasses import dataclass
from enum import Enum
from concurrent.futures import ThreadPoolExecutor
import numpy as np

# Connection modes
class ConnectionMode(Enum):
    HTTP = "http"
    DIRECT = "direct"
    HYBRID = "hybrid"  # Use direct for heavy ops, HTTP for control

# Connection states
class ConnectionState(Enum):
    DISCONNECTED = "disconnected"
    CONNECTING = "connecting"
    CONNECTED = "connected"
    ERROR = "error"

@dataclass
class ConnectionMetrics:
    """Metrics for connection performance"""
    latency_ms: float = 0.0
    throughput_mbps: float = 0.0
    patterns_processed: int = 0
    last_update: float = 0.0
    error_count: int = 0
    success_rate: float = 1.0

class SEPConnection:
    """Base class for SEP Engine connections"""
    
    def __init__(self):
        self.state = ConnectionState.DISCONNECTED
        self.metrics = ConnectionMetrics()
        self.callbacks = {}
        
    def connect(self) -> bool:
        """Establish connection"""
        raise NotImplementedError
        
    def disconnect(self):
        """Close connection"""
        raise NotImplementedError
        
    def is_connected(self) -> bool:
        """Check if connected"""
        return self.state == ConnectionState.CONNECTED
        
    def process_pattern(self, pattern_data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        """Process a pattern through the engine"""
        raise NotImplementedError
        
    def get_metrics(self) -> ConnectionMetrics:
        """Get connection metrics"""
        return self.metrics

class HTTPConnection(SEPConnection):
    """HTTP API connection to SEP Engine"""
    
    def __init__(self, host: str = "localhost", port: int = 8080):
        super().__init__()
        self.host = host
        self.port = port
        self.base_url = f"http://{host}:{port}/api/v1"
        self.session = None
        self.timeout = 5.0
        
    def connect(self) -> bool:
        """Establish HTTP connection"""
        try:
            self.state = ConnectionState.CONNECTING
            self.session = requests.Session()
            
            # Test connection with health endpoint
            response = self.session.get(
                f"{self.base_url}/health",
                timeout=self.timeout
            )
            
            if response.ok:
                self.state = ConnectionState.CONNECTED
                health_data = response.json()
                print(f"Connected to SEP Engine: {health_data.get('status', 'OK')}")
                return True
            else:
                self.state = ConnectionState.ERROR
                return False
                
        except Exception as e:
            self.state = ConnectionState.ERROR
            print(f"HTTP connection failed: {e}")
            return False
            
    def disconnect(self):
        """Close HTTP connection"""
        if self.session:
            self.session.close()
            self.session = None
        self.state = ConnectionState.DISCONNECTED
        
    def process_pattern(self, pattern_data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        """Process pattern via HTTP API"""
        if not self.is_connected():
            return None
            
        start_time = time.time()
        
        try:
            # Try different endpoints based on pattern type
            endpoints = [
                "/pattern/evolve",
                "/pattern/analyze",
                "/quantum/process",
                "/memory/query"
            ]
            
            for endpoint in endpoints:
                try:
                    response = self.session.post(
                        f"{self.base_url}{endpoint}",
                        json=pattern_data,
                        timeout=self.timeout
                    )
                    
                    if response.ok:
                        # Update metrics
                        elapsed = (time.time() - start_time) * 1000
                        self.metrics.latency_ms = elapsed
                        self.metrics.patterns_processed += 1
                        self.metrics.last_update = time.time()
                        
                        return response.json()
                        
                except requests.exceptions.RequestException:
                    continue
                    
            self.metrics.error_count += 1
            return None
            
        except Exception as e:
            print(f"Pattern processing error: {e}")
            self.metrics.error_count += 1
            return None

class DirectBridge(SEPConnection):
    """Direct C API bridge connection"""
    
    def __init__(self, lib_path: str = None):
        super().__init__()
        self.lib_path = lib_path or self._find_library()
        self.lib = None
        self.bridge_handle = None
        self._setup_done = False
        
    def _find_library(self) -> str:
        """Find the SEP bridge library"""
        import os
        import pathlib
        
        # Common locations to search
        search_paths = [
            pathlib.Path(__file__).parent / "libs",
            pathlib.Path.home() / ".blender" / "scripts" / "addons" / "sep_engine" / "libs",
            pathlib.Path("/usr/local/lib"),
            pathlib.Path("/usr/lib"),
        ]
        
        lib_names = ["libsep_blender.so", "sep_blender.dll", "libsep_blender.dylib"]
        
        for path in search_paths:
            for lib_name in lib_names:
                lib_path = path / lib_name
                if lib_path.exists():
                    return str(lib_path)
                    
        return None
        
    def _setup_functions(self):
        """Setup C function signatures"""
        if not self.lib or self._setup_done:
            return
            
        # sep_blender_init
        self.lib.sep_blender_init.argtypes = [
            ctypes.c_void_p,  # gpu_context
            ctypes.c_void_p,  # config
            ctypes.POINTER(ctypes.c_void_p)  # bridge_handle
        ]
        self.lib.sep_blender_init.restype = ctypes.c_int
        
        # sep_process_pattern
        self.lib.sep_process_pattern.argtypes = [
            ctypes.c_void_p,  # bridge_handle
            ctypes.c_char_p,  # json_data
            ctypes.c_size_t,  # data_len
            ctypes.POINTER(ctypes.c_char_p),  # result
            ctypes.POINTER(ctypes.c_size_t)  # result_len
        ]
        self.lib.sep_process_pattern.restype = ctypes.c_int
        
        # sep_blender_cleanup
        self.lib.sep_blender_cleanup.argtypes = [ctypes.c_void_p]
        self.lib.sep_blender_cleanup.restype = ctypes.c_int
        
        self._setup_done = True
        
    def connect(self) -> bool:
        """Initialize direct bridge"""
        try:
            self.state = ConnectionState.CONNECTING
            
            if not self.lib_path:
                print("SEP bridge library not found")
                self.state = ConnectionState.ERROR
                return False
                
            # Load library
            self.lib = ctypes.CDLL(self.lib_path)
            self._setup_functions()
            
            # Initialize bridge
            bridge_pp = ctypes.POINTER(ctypes.c_void_p)()
            result = self.lib.sep_blender_init(
                None,  # No GPU context for now
                None,  # Default config
                ctypes.byref(bridge_pp)
            )
            
            if result == 0:  # SEP_SUCCESS
                self.bridge_handle = bridge_pp.contents
                self.state = ConnectionState.CONNECTED
                print("Direct bridge initialized")
                return True
            else:
                self.state = ConnectionState.ERROR
                return False
                
        except Exception as e:
            print(f"Direct bridge error: {e}")
            self.state = ConnectionState.ERROR
            return False
            
    def disconnect(self):
        """Cleanup direct bridge"""
        if self.lib and self.bridge_handle:
            self.lib.sep_blender_cleanup(self.bridge_handle)
            self.bridge_handle = None
        self.state = ConnectionState.DISCONNECTED
        
    def process_pattern(self, pattern_data: Dict[str, Any]) -> Optional[Dict[str, Any]]:
        """Process pattern via direct bridge"""
        if not self.is_connected():
            return None
            
        start_time = time.time()
        
        try:
            # Convert to JSON
            json_data = json.dumps(pattern_data).encode('utf-8')
            
            # Prepare output buffers
            result_ptr = ctypes.c_char_p()
            result_len = ctypes.c_size_t()
            
            # Call bridge
            ret = self.lib.sep_process_pattern(
                self.bridge_handle,
                json_data,
                len(json_data),
                ctypes.byref(result_ptr),
                ctypes.byref(result_len)
            )
            
            if ret == 0 and result_ptr.value:  # SEP_SUCCESS
                # Parse result
                result_json = result_ptr.value.decode('utf-8')
                result = json.loads(result_json)
                
                # Free result buffer
                self.lib.sep_free_result(result_ptr)
                
                # Update metrics
                elapsed = (time.time() - start_time) * 1000
                self.metrics.latency_ms = elapsed
                self.metrics.patterns_processed += 1
                self.metrics.last_update = time.time()
                
                return result
            else:
                self.metrics.error_count += 1
                return None
                
        except Exception as e:
            print(f"Direct bridge processing error: {e}")
            self.metrics.error_count += 1
            return None

class ConnectionManager:
    """Manages SEP Engine connections"""
    
    _instance = None
    
    @classmethod
    def get_instance(cls):
        """Get singleton instance"""
        if cls._instance is None:
            cls._instance = ConnectionManager()
        return cls._instance
        
    def __init__(self):
        self.mode = ConnectionMode.HTTP
        self.http_connection = None
        self.direct_connection = None
        self.active_connection = None
        self.executor = ThreadPoolExecutor(max_workers=2)
        self.pattern_queue = queue.Queue()
        self.result_callbacks = {}
        self._running = False
        self._worker_thread = None
        
    def initialize(self, mode: ConnectionMode = ConnectionMode.HTTP):
        """Initialize connection manager"""
        self.mode = mode
        
        if mode in [ConnectionMode.HTTP, ConnectionMode.HYBRID]:
            self.http_connection = HTTPConnection(
                host=bpy.context.scene.sep_settings.host,
                port=bpy.context.scene.sep_settings.port
            )
            
        if mode in [ConnectionMode.DIRECT, ConnectionMode.HYBRID]:
            self.direct_connection = DirectBridge(
                lib_path=bpy.context.scene.sep_settings.library_path
            )
            
        # Start worker thread
        self._running = True
        self._worker_thread = threading.Thread(target=self._process_queue)
        self._worker_thread.daemon = True
        self._worker_thread.start()
        
    def connect(self) -> bool:
        """Establish connection based on mode"""
        success = False
        
        if self.mode == ConnectionMode.HTTP and self.http_connection:
            success = self.http_connection.connect()
            if success:
                self.active_connection = self.http_connection
                
        elif self.mode == ConnectionMode.DIRECT and self.direct_connection:
            success = self.direct_connection.connect()
            if success:
                self.active_connection = self.direct_connection
                
        elif self.mode == ConnectionMode.HYBRID:
            # Try direct first, fall back to HTTP
            if self.direct_connection and self.direct_connection.connect():
                self.active_connection = self.direct_connection
                success = True
            elif self.http_connection and self.http_connection.connect():
                self.active_connection = self.http_connection
                success = True
                
        return success
        
    def disconnect(self):
        """Disconnect all connections"""
        if self.http_connection:
            self.http_connection.disconnect()
        if self.direct_connection:
            self.direct_connection.disconnect()
        self.active_connection = None
        
    def cleanup(self):
        """Cleanup resources"""
        self._running = False
        if self._worker_thread:
            self._worker_thread.join(timeout=1.0)
        self.disconnect()
        self.executor.shutdown()
        
    def is_connected(self) -> bool:
        """Check if connected"""
        return self.active_connection and self.active_connection.is_connected()
        
    def get_status(self) -> str:
        """Get connection status"""
        if not self.active_connection:
            return "Disconnected"
        return f"{self.mode.value}: {self.active_connection.state.value}"
        
    def get_metrics(self) -> ConnectionMetrics:
        """Get connection metrics"""
        if self.active_connection:
            return self.active_connection.get_metrics()
        return ConnectionMetrics()
        
    def queue_pattern(self, pattern_data: Dict[str, Any], 
                     callback: Optional[Callable] = None) -> str:
        """Queue pattern for processing"""
        import uuid
        pattern_id = str(uuid.uuid4())
        
        if callback:
            self.result_callbacks[pattern_id] = callback
            
        self.pattern_queue.put((pattern_id, pattern_data))
        return pattern_id
        
    def _process_queue(self):
        """Worker thread for processing patterns"""
        while self._running:
            try:
                # Get pattern from queue with timeout
                pattern_id, pattern_data = self.pattern_queue.get(timeout=0.1)
                
                if self.active_connection:
                    # Process pattern
                    result = self.active_connection.process_pattern(pattern_data)
                    
                    # Call callback if registered
                    if pattern_id in self.result_callbacks:
                        callback = self.result_callbacks.pop(pattern_id)
                        try:
                            callback(result)
                        except Exception as e:
                            print(f"Callback error: {e}")
                            
            except queue.Empty:
                continue
            except Exception as e:
                print(f"Queue processing error: {e}")

# Module functions
def initialize():
    """Initialize connection manager"""
    manager = ConnectionManager.get_instance()
    settings = bpy.context.scene.sep_settings
    mode = ConnectionMode(settings.connection_mode)
    manager.initialize(mode)

def cleanup():
    """Cleanup connection manager"""
    manager = ConnectionManager.get_instance()
    manager.cleanup()

def get_manager() -> ConnectionManager:
    """Get connection manager instance"""
    return ConnectionManager.get_instance()

# No classes to register - this is a pure logic module
classes = []
