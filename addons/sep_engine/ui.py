"""
SEP Engine UI Module
====================
User interface panels, drawing functions, and viewport visualization
for the SEP Engine integration.
"""

import bpy
from bpy.types import Panel, UIList, Menu
import bpy_extras
import gpu
from gpu_extras.batch import batch_for_shader
from mathutils import Vector, Matrix
import math
import time
from . import connection

class SEP_UL_patterns(UIList):
    """Pattern list UI element"""
    
    def draw_item(self, context, layout, data, item, icon, active_data, active_propname):
        pattern = item
        
        if self.layout_type in {'DEFAULT', 'COMPACT'}:
            row = layout.row(align=True)
            
            # Pattern ID (truncated)
            row.label(text=pattern.pattern_id[:8] + "...", icon='RADIOBUT_ON')
            
            # Source object
            if pattern.source_object:
                row.label(text=pattern.source_object, icon='MESH_DATA')
            
            # Memory tier indicator
            tier_icons = {
                'STM': 'SORTTIME',
                'MTM': 'SORTSIZE',
                'LTM': 'FILE_CACHE',
                'NONE': 'CHECKBOX_DEHLT'
            }
            row.label(text="", icon=tier_icons.get(pattern.memory_tier, 'QUESTION'))
            
            # Metrics preview
            metrics = pattern.metrics
            row.label(text=f"C:{metrics.coherence:.2f}")
            
        elif self.layout_type == 'GRID':
            layout.alignment = 'CENTER'
            layout.label(text=pattern.pattern_id[:6], icon='RADIOBUT_ON')

class SEP_PT_main(Panel):
    """Main SEP Engine panel"""
    bl_label = "SEP Engine"
    bl_idname = "SEP_PT_main"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "SEP"
    
    def draw_header(self, context):
        layout = self.layout
        manager = connection.get_manager()
        
        if manager.is_connected():
            layout.label(text="", icon='LINKED')
        else:
            layout.label(text="", icon='UNLINKED')
    
    def draw(self, context):
        layout = self.layout
        settings = context.scene.sep_settings
        manager = connection.get_manager()
        
        # Connection status box
        box = layout.box()
        col = box.column()
        
        # Connection header
        row = col.row(align=True)
        row.label(text="Connection", icon='NETWORK_DRIVE')
        
        if manager.is_connected():
            row.label(text=manager.get_status(), icon='CHECKMARK')
        else:
            row.label(text="Disconnected", icon='ERROR')
        
        # Connection controls
        row = col.row(align=True)
        row.enabled = not manager.is_connected()
        row.prop(settings, "host", text="")
        row.prop(settings, "port", text="")
        row.prop(settings, "connection_mode", text="")
        
        # Connection buttons
        row = col.row(align=True)
        if manager.is_connected():
            row.operator("sep.disconnect", icon='UNLINKED')
            
            # Show metrics
            metrics = manager.get_metrics()
            if metrics.patterns_processed > 0:
                col.separator(factor=0.5)
                sub = col.column(align=True)
                sub.scale_y = 0.8
                sub.label(text=f"Latency: {metrics.latency_ms:.1f}ms")
                sub.label(text=f"Patterns: {metrics.patterns_processed}")
                sub.label(text=f"Success: {metrics.success_rate*100:.0f}%")
        else:
            row.operator("sep.connect", icon='LINKED')
        
        # Processing section
        if manager.is_connected():
            layout.separator()
            
            # Active object processing
            obj = context.active_object
            if obj and obj.type == 'MESH':
                box = layout.box()
                col = box.column()
                
                # Object header
                row = col.row()
                row.label(text="Active Object", icon='OBJECT_DATA')
                row.prop(obj.sep_settings, "enabled", text="")
                
                if obj.sep_settings.enabled:
                    # Process button
                    col.operator("sep.process_object", icon='PLAY')
                    
                    # Current metrics
                    if obj.sep_settings.pattern_id:
                        col.separator(factor=0.5)
                        
                        metrics_box = col.box()
                        metrics_col = metrics_box.column(align=True)
                        metrics_col.scale_y = 0.9
                        
                        metrics = obj.sep_settings.metrics
                        self._draw_metric_bar(metrics_col, "Coherence", metrics.coherence, 'SORTTIME')
                        self._draw_metric_bar(metrics_col, "Stability", metrics.stability, 'RIGID_BODY')
                        self._draw_metric_bar(metrics_col, "Entropy", metrics.entropy, 'FORCE_TURBULENCE')
                        self._draw_metric_bar(metrics_col, "Complexity", metrics.complexity, 'PARTICLES')
                    
                    # Deformation settings
                    col.separator()
                    col.prop(obj.sep_settings, "apply_deformation")
                    
                    if obj.sep_settings.apply_deformation:
                        sub = col.column(align=True)
                        sub.prop(obj.sep_settings, "deformation_strength")
                        sub.prop(obj.sep_settings, "deformation_mode")
            
            # Batch operations
            if len([o for o in context.selected_objects if o.type == 'MESH']) > 1:
                layout.separator()
                box = layout.box()
                box.label(text="Batch Operations", icon='GROUP')
                box.operator("sep.batch_process", icon='PLAY')
                box.operator("sep.entangle_patterns", icon='PHYSICS')
    
    def _draw_metric_bar(self, layout, label, value, icon):
        """Draw a metric progress bar"""
        row = layout.row(align=True)
        row.label(text="", icon=icon)
        
        sub = row.row(align=True)
        sub.scale_x = 2.0
        sub.prop(
            bpy.context.scene, "frame_current",  # Dummy prop for progress bar
            text=f"{label}: {value:.3f}",
            slider=True
        )

class SEP_PT_patterns(Panel):
    """Pattern management panel"""
    bl_label = "Patterns"
    bl_idname = "SEP_PT_patterns"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "SEP"
    bl_parent_id = "SEP_PT_main"
    bl_options = {'DEFAULT_CLOSED'}
    
    @classmethod
    def poll(cls, context):
        return connection.get_manager().is_connected()
    
    def draw(self, context):
        layout = self.layout
        settings = context.scene.sep_settings
        
        # Pattern list
        row = layout.row()
        row.template_list(
            "SEP_UL_patterns", "",
            settings, "patterns",
            settings, "active_pattern_index",
            rows=4
        )
        
        # Pattern operations
        col = row.column(align=True)
        col.operator("sep.sync_memory", icon='FILE_REFRESH', text="")
        col.operator("sep.clear_patterns", icon='X', text="")
        col.separator()
        col.operator("sep.export_patterns", icon='EXPORT', text="")
        
        # Active pattern details
        if settings.patterns and settings.active_pattern_index < len(settings.patterns):
            pattern = settings.patterns[settings.active_pattern_index]
            
            box = layout.box()
            col = box.column()
            
            # Pattern info
            col.label(text=f"ID: {pattern.pattern_id[:16]}...")
            col.label(text=f"Source: {pattern.source_object}")
            col.label(text=f"Tier: {pattern.memory_tier}")
            col.label(text=f"Evolutions: {pattern.evolution_count}")
            
            # Evolution controls
            col.separator()
            row = col.row(align=True)
            op = row.operator("sep.evolve_pattern", icon='FORCE_CHARGE')
            op.pattern_id = pattern.pattern_id
            op.steps = 1
            
            op = row.operator("sep.evolve_pattern", text="x10")
            op.pattern_id = pattern.pattern_id
            op.steps = 10

class SEP_PT_memory(Panel):
    """Memory tier visualization panel"""
    bl_label = "Memory Tiers"
    bl_idname = "SEP_PT_memory"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "SEP"
    bl_parent_id = "SEP_PT_main"
    bl_options = {'DEFAULT_CLOSED'}
    
    @classmethod
    def poll(cls, context):
        return connection.get_manager().is_connected()
    
    def draw(self, context):
        layout = self.layout
        settings = context.scene.sep_settings
        
        # Memory tier stats
        for tier in settings.memory_tiers:
            box = layout.box()
            col = box.column()
            
            # Tier header
            row = col.row()
            tier_names = {'STM': "Short-Term", 'MTM': "Medium-Term", 'LTM': "Long-Term"}
            row.label(text=tier_names.get(tier.tier_type, tier.tier_type), 
                     icon='MEMORY' if tier.tier_type == 'STM' else 'FILE_CACHE')
            
            # Usage bar
            if tier.capacity > 0:
                usage_pct = tier.usage / tier.capacity
                row.progress(
                    factor=usage_pct,
                    type='BAR',
                    text=f"{tier.usage}/{tier.capacity}"
                )
            
            # Tier settings
            sub = col.column(align=True)
            sub.scale_y = 0.8
            sub.prop(tier, "coherence_threshold", slider=True)
        
        # Memory operations
        layout.separator()
        col = layout.column(align=True)
        col.prop(settings, "auto_promote")
        if settings.auto_promote:
            col.prop(settings, "promotion_threshold", slider=True)

class SEP_PT_visualization(Panel):
    """Visualization settings panel"""
    bl_label = "Visualization"
    bl_idname = "SEP_PT_visualization"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "SEP"
    bl_parent_id = "SEP_PT_main"
    bl_options = {'DEFAULT_CLOSED'}
    
    def draw(self, context):
        layout = self.layout
        settings = context.scene.sep_settings
        
        # Display options
        col = layout.column()
        col.prop(settings, "show_metrics")
        col.prop(settings, "show_quantum_state")
        
        if settings.show_metrics or settings.show_quantum_state:
            col.prop(settings, "metric_colors")
        
        # Field visualization
        layout.separator()
        layout.label(text="Quantum Field Visualization", icon='FORCE_MAGNETIC')
        
        col = layout.column(align=True)
        op = col.operator("sep.visualize_quantum_field", text="Coherence Field")
        op.field_type = 'COHERENCE'
        
        op = col.operator("sep.visualize_quantum_field", text="Stability Field")
        op.field_type = 'STABILITY'
        
        op = col.operator("sep.visualize_quantum_field", text="Quantum Field")
        op.field_type = 'QUANTUM'

class SEP_PT_advanced(Panel):
    """Advanced settings panel"""
    bl_label = "Advanced"
    bl_idname = "SEP_PT_advanced"
    bl_space_type = 'VIEW_3D'
    bl_region_type = 'UI'
    bl_category = "SEP"
    bl_parent_id = "SEP_PT_main"
    bl_options = {'DEFAULT_CLOSED'}
    
    def draw(self, context):
        layout = self.layout
        settings = context.scene.sep_settings
        
        # Processing settings
        box = layout.box()
        box.label(text="Processing", icon='SETTINGS')
        col = box.column()
        col.prop(settings, "auto_process")
        if settings.auto_process:
            col.prop(settings, "process_interval")
        col.prop(settings, "batch_size")
        
        # Debug settings
        box = layout.box()
        box.label(text="Debug", icon='CONSOLE')
        col = box.column()
        col.prop(settings, "debug_mode")
        col.prop(settings, "log_level")
        
        # Scene analysis
        layout.separator()
        layout.operator("sep.analyze_scene", icon='SCENE_DATA')

# Viewport overlay drawing
def draw_sep_overlay():
    """Draw SEP information in the viewport"""
    context = bpy.context
    
    if not context.scene.sep_settings.show_metrics:
        return
    
    obj = context.active_object
    if not obj or not obj.type == 'MESH' or not obj.sep_settings.enabled:
        return
    
    # Get viewport region
    region = context.region
    rv3d = context.region_data
    
    # Calculate object center in screen space
    obj_center = obj.matrix_world.translation
    screen_co = bpy_extras.view3d_utils.location_3d_to_region_2d(
        region, rv3d, obj_center
    )
    
    if not screen_co:
        return
    
    # Draw metrics
    metrics = obj.sep_settings.metrics
    quantum = obj.sep_settings.quantum_state
    
    # Prepare text
    lines = [
        f"Coherence: {metrics.coherence:.3f}",
        f"Stability: {metrics.stability:.3f}",
        f"Entropy: {metrics.entropy:.3f}",
        f"Complexity: {metrics.complexity:.3f}"
    ]
    
    if context.scene.sep_settings.show_quantum_state:
        lines.extend([
            "",
            f"QBSA: {quantum.qbsa_state}",
            f"QFH Level: {quantum.qfh_level}",
            f"Phase: {quantum.phase:.3f}"
        ])
        
        if quantum.rupture_detected:
            lines.append("RUPTURE DETECTED")
    
    # Draw background
    shader = gpu.shader.from_builtin('UNIFORM_COLOR')
    batch = batch_for_shader(
        shader, 'TRI_FAN',
        {"pos": [
            (screen_co.x - 5, screen_co.y + 20),
            (screen_co.x + 150, screen_co.y + 20),
            (screen_co.x + 150, screen_co.y + 20 + len(lines) * 15),
            (screen_co.x - 5, screen_co.y + 20 + len(lines) * 15)
        ]}
    )
    
    shader.bind()
    shader.uniform_float("color", (0, 0, 0, 0.7))
    batch.draw(shader)
    
    # Draw text
    import blf
    font_id = 0
    blf.size(font_id, 11)
    blf.color(font_id, 1, 1, 1, 1)
    
    y_offset = 25
    for line in lines:
        blf.position(font_id, screen_co.x, screen_co.y + y_offset, 0)
        blf.draw(font_id, line)
        y_offset += 15

# Drawing handler management
_draw_handler = None

def register_draw_handler():
    """Register viewport drawing handler"""
    global _draw_handler
    if _draw_handler is None:
        _draw_handler = bpy.types.SpaceView3D.draw_handler_add(
            draw_sep_overlay, (), 'WINDOW', 'POST_PIXEL'
        )

def unregister_draw_handler():
    """Unregister viewport drawing handler"""
    global _draw_handler
    if _draw_handler is not None:
        bpy.types.SpaceView3D.draw_handler_remove(_draw_handler, 'WINDOW')
        _draw_handler = None

# Classes to register
classes = [
    SEP_UL_patterns,
    SEP_PT_main,
    SEP_PT_patterns,
    SEP_PT_memory,
    SEP_PT_visualization,
    SEP_PT_advanced
]
