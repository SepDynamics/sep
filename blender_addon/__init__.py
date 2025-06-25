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
import requests
import bmesh
import json
import numpy as np
import os
import time
import socket
from bpy.props import FloatProperty, BoolProperty, EnumProperty, StringProperty, IntProperty

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
    
    def get_base_url(self):
        return f"http://{self.host}:{self.port}{self.api_base}"
    
    def check_connection(self, force=False):
        """Check connection to SEP Engine service"""
        # Only check every 5 seconds unless forced
        current_time = time.time()
        if not force and current_time - self.last_check_time < 5.0:
            return self.connected
            
        self.last_check_time = current_time
        
        try:
            response = requests.get(
                f"{self.get_base_url()}/health",
                timeout=self.connection_timeout
            )
            self.connected = response.ok
            if response.ok:
                self.last_status = "Connected"
                # Parse health data if available
                try:
                    data = response.json()
                    if "status" in data:
                        self.last_status = f"Connected: {data['status']}"
                except:
                    pass
            else:
                self.last_status = f"Error: {response.status_code}"
        except requests.exceptions.ConnectionError:
            self.connected = False
            self.last_status = "Connection failed"
        except requests.exceptions.Timeout:
            self.connected = False
            self.last_status = "Connection timeout"
        except Exception as e:
            self.connected = False
            self.last_status = f"Error: {str(e)}"
            
        return self.connected

# Initialize settings
sep_settings = SEPEngineSettings.get_instance()

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
        
        # Check connection to SEP Engine
        if not sep_settings.check_connection():
            self.report({'ERROR'}, f"Cannot connect to SEP Engine: {sep_settings.last_status}")
            return {'CANCELLED'}
        
        try:
            # Extract mesh data
            mesh_data = self.extract_mesh_data(obj)
            
            # Add pattern parameters
            pattern_data = {
                "pattern": {
                    "coherence": self.coherence,
                    "stability": self.stability,
                    "entropy": self.entropy,
                    "complexity": self.complexity
                },
                "mesh_data": mesh_data,
                "options": {
                    "return_deformation": True,
                    "preserve_topology": True
                }
            }
            
            # Send to SEP engine
            self.report({'INFO'}, "Sending data to SEP Engine...")
            
            # Determine which endpoint to use
            endpoints = [
                "/pattern/evolve",      # Primary endpoint for pattern evolution
                "/pattern/analyze",     # Alternative endpoint for analysis
                "/memory/query"         # Memory-based endpoint
            ]
            
            response = None
            endpoint_used = None
            
            for endpoint in endpoints:
                try:
                    api_url = f"{sep_settings.get_base_url()}{endpoint}"
                    response = requests.post(
                        api_url,
                        json=pattern_data,
                        headers={"Content-Type": "application/json"},
                        timeout=sep_settings.connection_timeout
                    )
                    if response.ok:
                        endpoint_used = endpoint
                        break
                except requests.exceptions.RequestException:
                    continue
            
            if response is None or not response.ok:
                if response is not None:
                    self.report({'ERROR'}, f"SEP Engine error: {response.status_code}")
                else:
                    self.report({'ERROR'}, "Failed to communicate with SEP Engine service")
                return {'CANCELLED'}
                
            # Apply quantum patterns back to mesh
            response_data = response.json()
            success = self.apply_patterns(obj, response_data)
            
            if success:
                self.report({'INFO'}, f"Applied SEP patterns from {endpoint_used} to mesh")
            else:
                self.report({'WARNING'}, "Received response but no pattern applied")
                
        except requests.exceptions.ConnectionError:
            self.report({'ERROR'}, "Connection to SEP Engine lost")
            return {'CANCELLED'}
        except requests.exceptions.Timeout:
            self.report({'ERROR'}, "SEP Engine request timed out")
            return {'CANCELLED'}
        except Exception as e:
            self.report({'ERROR'}, f"Error: {str(e)}")
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
        
        # Engine status
        box = layout.box()
        box.label(text="Engine Status")
        
        row = box.row()
        # Try to ping the engine
        try:
            response = requests.get("http://localhost:8080/api/v1/health", timeout=0.5)
            if response.ok:
                row.label(text="Connected", icon='CHECKMARK')
            else:
                row.label(text="Error", icon='ERROR')
        except:
            row.label(text="Not Connected", icon='X')
        
        # Process mesh
        box = layout.box()
        box.label(text="Process Mesh")
        
        op = box.operator("sep.process_mesh")
        
        # Pattern parameters
        box = layout.box()
        box.label(text="Pattern Parameters")
        box.prop(context.scene, "sep_coherence")
        box.prop(context.scene, "sep_stability")

def register():
    bpy.utils.register_class(SEP_OT_ProcessMesh)
    bpy.utils.register_class(SEP_PT_MeshPanel)
    
    # Register properties
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

def unregister():
    bpy.utils.unregister_class(SEP_OT_ProcessMesh)
    bpy.utils.unregister_class(SEP_PT_MeshPanel)
    
    # Unregister properties
    del bpy.types.Scene.sep_coherence
    del bpy.types.Scene.sep_stability

if __name__ == "__main__":
    register()