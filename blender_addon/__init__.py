bl_info = {
    "name": "SEP Engine Integration",
    "author": "Alexander J Nagy",
    "version": (0, 1),
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
from bpy.props import FloatProperty, BoolProperty, EnumProperty, StringProperty

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
    
    def extract_mesh_data(self, obj):
        """Extract mesh data from Blender object"""
        me = obj.data
        vertices = []
        
        # Get vertices
        for v in me.vertices:
            vertices.append({
                "x": v.co.x,
                "y": v.co.y,
                "z": v.co.z
            })
        
        # Construct data structure
        return {
            "name": obj.name,
            "vertices": vertices,
            "vertex_count": len(vertices)
        }
    
    def apply_patterns(self, obj, response_data):
        """Apply patterns from SEP Engine back to mesh"""
        me = obj.data
        if "result" not in response_data:
            self.report({'ERROR'}, "Invalid response from SEP Engine")
            return
            
        result = response_data["result"]
        if "coherence" in result:
            # Create a custom property to store the coherence value
            obj["sep_coherence"] = result["coherence"]
            
        # Apply deformation based on pattern (example)
        if "deformation" in result:
            deform = result["deformation"]
            for i, v in enumerate(me.vertices):
                if i < len(deform):
                    v.co.x += deform[i]["x"]
                    v.co.y += deform[i]["y"]
                    v.co.z += deform[i]["z"]
            
            me.update()
    
    def execute(self, context):
        obj = context.active_object
        if obj is None or obj.type != 'MESH':
            self.report({'ERROR'}, "Select a mesh object first")
            return {'CANCELLED'}
        
        try:
            # Extract mesh data
            mesh_data = self.extract_mesh_data(obj)
            
            # Add pattern parameters
            pattern_data = {
                "pattern": {
                    "coherence": self.coherence,
                    "stability": self.stability
                },
                "mesh_data": mesh_data
            }
            
            # Send to SEP engine
            self.report({'INFO'}, "Sending data to SEP Engine...")
            response = requests.post(
                "http://localhost:8080/api/v1/pattern/analyze",
                json=pattern_data,
                headers={"Content-Type": "application/json"}
            )
            
            if response.ok:
                # Apply quantum patterns back to mesh
                response_data = response.json()
                self.apply_patterns(obj, response_data)
                self.report({'INFO'}, "Applied SEP patterns to mesh")
            else:
                self.report({'ERROR'}, f"SEP Engine error: {response.status_code}")
                
        except requests.exceptions.ConnectionError:
            self.report({'ERROR'}, "Could not connect to SEP Engine. Is it running?")
        except Exception as e:
            self.report({'ERROR'}, f"Error: {str(e)}")
            
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