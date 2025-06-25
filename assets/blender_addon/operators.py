"""
SEP Engine Operators
====================
All operators for SEP Engine interaction and pattern processing.
"""

import bpy
from bpy.types import Operator
from bpy.props import StringProperty, FloatProperty, IntProperty, BoolProperty, EnumProperty
import time
import json
import os
from typing import Set, Dict, Any, List
from . import connection
from . import mesh_utils
from . import pattern_processor

class SEP_OT_connect(Operator):
    """Connect to SEP Engine service"""
    bl_idname = "sep.connect"
    bl_label = "Connect to SEP Engine"
    bl_options = {'REGISTER', 'UNDO'}
    
    def execute(self, context):
        manager = connection.get_manager()
        
        if manager.is_connected():
            self.report({'INFO'}, "Already connected")
            return {'FINISHED'}
        
        # Update settings
        settings = context.scene.sep_settings
        if hasattr(manager, 'http_connection') and manager.http_connection:
            manager.http_connection.host = settings.host
            manager.http_connection.port = settings.port
        
        # Connect
        if manager.connect():
            self.report({'INFO'}, f"Connected: {manager.get_status()}")
            
            # Trigger initial sync
            bpy.ops.sep.sync_memory()
            
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Failed to connect to SEP Engine")
            return {'CANCELLED'}

class SEP_OT_disconnect(Operator):
    """Disconnect from SEP Engine"""
    bl_idname = "sep.disconnect"
    bl_label = "Disconnect"
    bl_options = {'REGISTER'}
    
    def execute(self, context):
        manager = connection.get_manager()
        manager.disconnect()
        self.report({'INFO'}, "Disconnected from SEP Engine")
        return {'FINISHED'}

class SEP_OT_process_object(Operator):
    """Process active object through SEP Engine"""
    bl_idname = "sep.process_object"
    bl_label = "Process Object"
    bl_description = "Process the active mesh object through SEP Engine quantum algorithms"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod
    def poll(cls, context):
        obj = context.active_object
        return (obj is not None and 
                obj.type == 'MESH' and
                connection.get_manager().is_connected())
    
    def execute(self, context):
        obj = context.active_object
        settings = obj.sep_settings
        scene_settings = context.scene.sep_settings
        
        # Extract mesh data
        mesh_data = mesh_utils.extract_mesh_data(obj)
        
        # Prepare pattern data with current metrics
        pattern_data = {
            "type": "mesh",
            "name": obj.name,
            "mesh_data": mesh_data,
            "pattern": {
                "coherence": settings.metrics.coherence,
                "stability": settings.metrics.stability,
                "entropy": settings.metrics.entropy,
                "complexity": settings.metrics.complexity
            },
            "options": {
                "return_deformation": settings.apply_deformation,
                "preserve_topology": True,
                "evolution_steps": 1,
                "mode": settings.deformation_mode.lower()
            }
        }
        
        # Process synchronously for immediate feedback
        manager = connection.get_manager()
        start_time = time.time()
        
        result = manager.active_connection.process_pattern(pattern_data)
        
        if result:
            # Apply results
            if "result" in result:
                # Update metrics
                if "metrics" in result["result"]:
                    settings.metrics.from_dict(result["result"]["metrics"])
                    
                # Update quantum state
                if "quantum_state" in result["result"]:
                    state = result["result"]["quantum_state"]
                    settings.quantum_state.qbsa_state = state.get("qbsa", 0)
                    settings.quantum_state.qfh_level = state.get("qfh_level", 0)
                    settings.quantum_state.rupture_detected = state.get("rupture", False)
                    
                # Apply deformation if enabled
                if settings.apply_deformation and "deformation" in result["result"]:
                    mesh_utils.apply_deformation(
                        obj, 
                        result["result"]["deformation"],
                        settings.deformation_strength
                    )
                    
                # Store pattern ID
                if "pattern_id" in result["result"]:
                    settings.pattern_id = result["result"]["pattern_id"]
                    
                    # Add to scene patterns
                    pattern = scene_settings.patterns.add()
                    pattern.pattern_id = result["result"]["pattern_id"]
                    pattern.source_object = obj.name
                    pattern.metrics.from_dict(result["result"].get("metrics", {}))
                    pattern.creation_time = time.time()
                    pattern.last_update = time.time()
                    
            elapsed = time.time() - start_time
            self.report({'INFO'}, f"Processing complete in {elapsed:.2f}s")
            
            # Update viewport
            context.view_layer.update()
            
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Processing failed")
            return {'CANCELLED'}

class SEP_OT_batch_process(Operator):
    """Process multiple selected objects"""
    bl_idname = "sep.batch_process"
    bl_label = "Batch Process"
    bl_description = "Process all selected mesh objects"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod
    def poll(cls, context):
        return (len([obj for obj in context.selected_objects if obj.type == 'MESH']) > 0 and
                connection.get_manager().is_connected())
    
    def execute(self, context):
        mesh_objects = [obj for obj in context.selected_objects if obj.type == 'MESH']
        
        if not mesh_objects:
            self.report({'WARNING'}, "No mesh objects selected")
            return {'CANCELLED'}
        
        processed = 0
        failed = 0
        
        for obj in mesh_objects:
            # Make it active
            context.view_layer.objects.active = obj
            
            # Process
            result = bpy.ops.sep.process_object()
            
            if result == {'FINISHED'}:
                processed += 1
            else:
                failed += 1
        
        self.report({'INFO'}, f"Batch complete: {processed} processed, {failed} failed")
        return {'FINISHED'}

class SEP_OT_evolve_pattern(Operator):
    """Evolve pattern through quantum states"""
    bl_idname = "sep.evolve_pattern"
    bl_label = "Evolve Pattern"
    bl_description = "Evolve the pattern through quantum state transitions"
    bl_options = {'REGISTER', 'UNDO'}
    
    pattern_id: StringProperty(
        name="Pattern ID",
        default=""
    )
    
    steps: IntProperty(
        name="Evolution Steps",
        description="Number of evolution steps",
        default=1,
        min=1,
        max=100
    )
    
    @classmethod
    def poll(cls, context):
        return connection.get_manager().is_connected()
    
    def execute(self, context):
        obj = context.active_object
        if not obj or not obj.sep_settings.pattern_id:
            self.report({'ERROR'}, "No pattern associated with object")
            return {'CANCELLED'}
        
        pattern_id = self.pattern_id or obj.sep_settings.pattern_id
        
        # Request evolution
        pattern_data = {
            "action": "evolve",
            "pattern_id": pattern_id,
            "steps": self.steps,
            "options": {
                "return_trajectory": True,
                "apply_mutations": True
            }
        }
        
        manager = connection.get_manager()
        result = manager.active_connection.process_pattern(pattern_data)
        
        if result and "result" in result:
            # Update object
            if "metrics" in result["result"]:
                obj.sep_settings.metrics.from_dict(result["result"]["metrics"])
                
            # Update evolution count
            obj.sep_settings.evolution_count += self.steps
            
            self.report({'INFO'}, f"Pattern evolved {self.steps} steps")
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Evolution failed")
            return {'CANCELLED'}

class SEP_OT_entangle_patterns(Operator):
    """Entangle patterns from two objects"""
    bl_idname = "sep.entangle_patterns"
    bl_label = "Entangle Patterns"
    bl_description = "Create quantum entanglement between two patterns"
    bl_options = {'REGISTER', 'UNDO'}
    
    @classmethod
    def poll(cls, context):
        return (len(context.selected_objects) == 2 and
                all(obj.type == 'MESH' and obj.sep_settings.pattern_id 
                    for obj in context.selected_objects) and
                connection.get_manager().is_connected())
    
    def execute(self, context):
        objects = list(context.selected_objects)
        
        pattern_data = {
            "action": "entangle",
            "pattern_ids": [obj.sep_settings.pattern_id for obj in objects],
            "options": {
                "coupling_strength": 0.5,
                "preserve_individual": True
            }
        }
        
        manager = connection.get_manager()
        result = manager.active_connection.process_pattern(pattern_data)
        
        if result and "result" in result:
            self.report({'INFO'}, "Patterns entangled successfully")
            
            # Update both objects
            for i, obj in enumerate(objects):
                if f"pattern_{i}" in result["result"]:
                    metrics = result["result"][f"pattern_{i}"].get("metrics", {})
                    obj.sep_settings.metrics.from_dict(metrics)
                    
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Entanglement failed")
            return {'CANCELLED'}

class SEP_OT_sync_memory(Operator):
    """Synchronize memory tiers with SEP Engine"""
    bl_idname = "sep.sync_memory"
    bl_label = "Sync Memory"
    bl_description = "Synchronize memory tiers with SEP Engine"
    
    def execute(self, context):
        settings = context.scene.sep_settings
        manager = connection.get_manager()
        
        if not manager.is_connected():
            self.report({'ERROR'}, "Not connected to SEP Engine")
            return {'CANCELLED'}
        
        # Request memory state
        sync_data = {
            "action": "sync_memory",
            "options": {
                "include_metrics": True,
                "include_patterns": True
            }
        }
        
        result = manager.active_connection.process_pattern(sync_data)
        
        if result and "memory_state" in result:
            state = result["memory_state"]
            
            # Update tier information
            for tier_data in state.get("tiers", []):
                tier_type = tier_data["type"]
                for tier in settings.memory_tiers:
                    if tier.tier_type == tier_type:
                        tier.usage = tier_data.get("usage", 0)
                        tier.capacity = tier_data.get("capacity", tier.capacity)
                        break
            
            # Update patterns
            for pattern_data in state.get("patterns", []):
                # Find or create pattern
                pattern = None
                for p in settings.patterns:
                    if p.pattern_id == pattern_data["id"]:
                        pattern = p
                        break
                
                if not pattern:
                    pattern = settings.patterns.add()
                    pattern.pattern_id = pattern_data["id"]
                
                # Update pattern data
                pattern.memory_tier = pattern_data.get("tier", "NONE")
                if "metrics" in pattern_data:
                    pattern.metrics.from_dict(pattern_data["metrics"])
                pattern.last_update = time.time()
            
            self.report({'INFO'}, f"Synchronized {len(state.get('patterns', []))} patterns")
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Memory sync failed")
            return {'CANCELLED'}

class SEP_OT_clear_patterns(Operator):
    """Clear all patterns from memory"""
    bl_idname = "sep.clear_patterns"
    bl_label = "Clear Patterns"
    bl_description = "Clear all patterns from local memory"
    bl_options = {'REGISTER', 'UNDO'}
    
    confirm: BoolProperty(
        name="Confirm",
        description="Confirm clearing all patterns",
        default=False
    )
    
    def execute(self, context):
        if not self.confirm:
            return self.invoke(context, None)
        
        settings = context.scene.sep_settings
        settings.patterns.clear()
        
        # Clear object associations
        for obj in bpy.data.objects:
            if hasattr(obj, 'sep_settings'):
                obj.sep_settings.pattern_id = ""
        
        self.report({'INFO'}, "All patterns cleared")
        return {'FINISHED'}
    
    def invoke(self, context, event):
        return context.window_manager.invoke_confirm(self, event)

class SEP_OT_export_patterns(Operator):
    """Export patterns to JSON file"""
    bl_idname = "sep.export_patterns"
    bl_label = "Export Patterns"
    bl_description = "Export all patterns to JSON file"
    
    filepath: StringProperty(
        name="File Path",
        description="Path to export file",
        default="patterns.json",
        subtype='FILE_PATH'
    )
    
    def execute(self, context):
        settings = context.scene.sep_settings
        
        # Collect pattern data
        patterns_data = []
        for pattern in settings.patterns:
            patterns_data.append({
                "id": pattern.pattern_id,
                "source": pattern.source_object,
                "metrics": pattern.metrics.to_dict(),
                "tier": pattern.memory_tier,
                "created": pattern.creation_time,
                "updated": pattern.last_update,
                "evolutions": pattern.evolution_count
            })
        
        # Export to JSON
        try:
            with open(self.filepath, 'w') as f:
                json.dump({
                    "version": "1.0",
                    "timestamp": time.time(),
                    "patterns": patterns_data
                }, f, indent=2)
            
            self.report({'INFO'}, f"Exported {len(patterns_data)} patterns")
            return {'FINISHED'}
        except Exception as e:
            self.report({'ERROR'}, f"Export failed: {str(e)}")
            return {'CANCELLED'}
    
    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

class SEP_OT_visualize_quantum_field(Operator):
    """Visualize quantum field in viewport"""
    bl_idname = "sep.visualize_quantum_field"
    bl_label = "Visualize Quantum Field"
    bl_description = "Create visual representation of quantum field"
    bl_options = {'REGISTER', 'UNDO'}
    
    resolution: IntProperty(
        name="Resolution",
        description="Field grid resolution",
        default=32,
        min=8,
        max=128
    )
    
    field_type: EnumProperty(
        name="Field Type",
        items=[
            ('COHERENCE', "Coherence", "Visualize coherence field"),
            ('STABILITY', "Stability", "Visualize stability field"),
            ('QUANTUM', "Quantum", "Visualize quantum probability field"),
            ('PHASE', "Phase", "Visualize phase field")
        ],
        default='COHERENCE'
    )
    
    def execute(self, context):
        # Create visualization mesh
        mesh = bpy.data.meshes.new(name=f"QuantumField_{self.field_type}")
        obj = bpy.data.objects.new(name=f"QuantumField_{self.field_type}", object_data=mesh)
        context.collection.objects.link(obj)
        
        # Generate field visualization
        pattern_processor.generate_field_visualization(
            obj, 
            self.resolution,
            self.field_type,
            context.scene.sep_settings
        )
        
        # Make it active and select
        context.view_layer.objects.active = obj
        obj.select_set(True)
        
        self.report({'INFO'}, f"Created {self.field_type} field visualization")
        return {'FINISHED'}

class SEP_OT_analyze_scene(Operator):
    """Analyze entire scene for patterns"""
    bl_idname = "sep.analyze_scene"
    bl_label = "Analyze Scene"
    bl_description = "Analyze all mesh objects in scene for quantum patterns"
    bl_options = {'REGISTER', 'UNDO'}
    
    def execute(self, context):
        mesh_objects = [obj for obj in context.scene.objects if obj.type == 'MESH']
        
        if not mesh_objects:
            self.report({'WARNING'}, "No mesh objects in scene")
            return {'CANCELLED'}
        
        # Collect all mesh data
        scene_data = {
            "action": "analyze_scene",
            "objects": []
        }
        
        for obj in mesh_objects:
            mesh_data = mesh_utils.extract_mesh_data(obj)
            scene_data["objects"].append({
                "name": obj.name,
                "mesh": mesh_data,
                "transform": {
                    "location": list(obj.location),
                    "rotation": list(obj.rotation_euler),
                    "scale": list(obj.scale)
                }
            })
        
        # Process scene
        manager = connection.get_manager()
        result = manager.active_connection.process_pattern(scene_data)
        
        if result and "analysis" in result:
            analysis = result["analysis"]
            
            # Apply results to objects
            for obj_result in analysis.get("objects", []):
                obj_name = obj_result["name"]
                obj = context.scene.objects.get(obj_name)
                
                if obj and "metrics" in obj_result:
                    obj.sep_settings.metrics.from_dict(obj_result["metrics"])
                    obj.sep_settings.enabled = True
            
            # Report overall metrics
            overall = analysis.get("overall", {})
            self.report({'INFO'}, 
                       f"Scene coherence: {overall.get('coherence', 0):.3f}, "
                       f"Complexity: {overall.get('complexity', 0):.3f}")
            
            return {'FINISHED'}
        else:
            self.report({'ERROR'}, "Scene analysis failed")
            return {'CANCELLED'}

# Classes to register
classes = [
    SEP_OT_connect,
    SEP_OT_disconnect,
    SEP_OT_check_connection,
    SEP_OT_process_object,
    SEP_OT_batch_process,
    SEP_OT_evolve_pattern,
    SEP_OT_entangle_patterns,
    SEP_OT_sync_memory,
    SEP_OT_clear_patterns,
    SEP_OT_export_patterns,
    SEP_OT_visualize_quantum_field,
    SEP_OT_analyze_scene
]
