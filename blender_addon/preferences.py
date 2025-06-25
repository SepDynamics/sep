"""
SEP Engine Addon Preferences
============================
User preferences and addon configuration settings.
"""

import bpy
from bpy.types import AddonPreferences, Operator
from bpy.props import (
    StringProperty, IntProperty, FloatProperty, BoolProperty,
    EnumProperty, CollectionProperty
)
import os
import platform
import subprocess
import webbrowser
from pathlib import Path

class SEPPresetItem(bpy.types.PropertyGroup):
    """Preset configuration item"""
    name: StringProperty(
        name="Name",
        description="Preset name",
        default="Custom"
    )
    
    host: StringProperty(
        name="Host",
        description="SEP Engine host",
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
        name="Mode",
        items=[
            ('http', "HTTP", "HTTP API connection"),
            ('direct', "Direct", "Direct C bridge"),
            ('hybrid', "Hybrid", "Use both")
        ],
        default='http'
    )

class SEP_OT_install_library(Operator):
    """Download and install SEP bridge library"""
    bl_idname = "sep.install_library"
    bl_label = "Install SEP Library"
    bl_description = "Download and install the SEP bridge library for your platform"
    
    def execute(self, context):
        # Get addon name dynamically
        addon_name = __name__.split('.')[0]
        preferences = context.preferences.addons[addon_name].preferences
        
        # Determine platform
        system = platform.system()
        machine = platform.machine()
        
        if system == "Linux":
            lib_name = "libsep_blender.so"
        elif system == "Darwin":
            lib_name = "libsep_blender.dylib"
        elif system == "Windows":
            lib_name = "sep_blender.dll"
        else:
            self.report({'ERROR'}, f"Unsupported platform: {system}")
            return {'CANCELLED'}
        
        # Download URL (would be actual URL in production)
        download_url = f"https://sep-engine.org/downloads/{lib_name}"
        
        # Target directory
        addon_dir = Path(__file__).parent
        libs_dir = addon_dir / "libs"
        libs_dir.mkdir(exist_ok=True)
        
        target_path = libs_dir / lib_name
        
        try:
            # In production, would download from URL
            # For now, just create placeholder
            self.report({'INFO'}, f"Library would be downloaded to: {target_path}")
            
            # Update preferences
            preferences.library_path = str(target_path)
            preferences.library_installed = True
            
            return {'FINISHED'}
            
        except Exception as e:
            self.report({'ERROR'}, f"Installation failed: {str(e)}")
            return {'CANCELLED'}
    
    def invoke(self, context, event):
        return context.window_manager.invoke_confirm(self, event)

class SEP_OT_add_preset(Operator):
    """Add connection preset"""
    bl_idname = "sep.add_preset"
    bl_label = "Add Preset"
    
    name: StringProperty(
        name="Name",
        description="Preset name",
        default="New Preset"
    )
    
    def execute(self, context):
        addon_name = __name__.split('.')[0]
        preferences = context.preferences.addons[addon_name].preferences
        settings = context.scene.sep_settings
        
        # Create new preset
        preset = preferences.connection_presets.add()
        preset.name = self.name
        preset.host = settings.host
        preset.port = settings.port
        preset.connection_mode = settings.connection_mode
        
        self.report({'INFO'}, f"Added preset: {self.name}")
        return {'FINISHED'}
    
    def invoke(self, context, event):
        return context.window_manager.invoke_props_dialog(self)

class SEP_OT_remove_preset(Operator):
    """Remove connection preset"""
    bl_idname = "sep.remove_preset"
    bl_label = "Remove Preset"
    
    def execute(self, context):
        addon_name = __name__.split('.')[0]
        preferences = context.preferences.addons[addon_name].preferences
        
        if preferences.active_preset_index < len(preferences.connection_presets):
            preset = preferences.connection_presets[preferences.active_preset_index]
            name = preset.name
            preferences.connection_presets.remove(preferences.active_preset_index)
            
            # Adjust index
            if preferences.active_preset_index > 0:
                preferences.active_preset_index -= 1
                
            self.report({'INFO'}, f"Removed preset: {name}")
        
        return {'FINISHED'}

class SEP_OT_load_preset(Operator):
    """Load connection preset"""
    bl_idname = "sep.load_preset"
    bl_label = "Load Preset"
    
    def execute(self, context):
        addon_name = __name__.split('.')[0]
        preferences = context.preferences.addons[addon_name].preferences
        settings = context.scene.sep_settings
        
        if preferences.active_preset_index < len(preferences.connection_presets):
            preset = preferences.connection_presets[preferences.active_preset_index]
            
            # Apply preset settings
            settings.host = preset.host
            settings.port = preset.port
            settings.connection_mode = preset.connection_mode
            
            self.report({'INFO'}, f"Loaded preset: {preset.name}")
        
        return {'FINISHED'}

class SEP_OT_open_documentation(Operator):
    """Open SEP Engine documentation"""
    bl_idname = "sep.open_documentation"
    bl_label = "Open Documentation"
    bl_description = "Open SEP Engine documentation in web browser"
    
    url: StringProperty(
        default="https://github.com/someaka/sep-engine/wiki"
    )
    
    def execute(self, context):
        webbrowser.open(self.url)
        return {'FINISHED'}

class SEPAddonPreferences(AddonPreferences):
    bl_idname = "sep_engine"  # This should match the addon folder name
    
    # Library settings
    library_path: StringProperty(
        name="Library Path",
        description="Path to SEP bridge library",
        subtype='FILE_PATH',
        default=""
    )
    
    library_installed: BoolProperty(
        name="Library Installed",
        description="Whether the SEP bridge library is installed",
        default=False
    )
    
    auto_install_library: BoolProperty(
        name="Auto Install Library",
        description="Automatically download and install library on first use",
        default=True
    )
    
    # Connection defaults
    default_host: StringProperty(
        name="Default Host",
        description="Default SEP Engine host",
        default="localhost"
    )
    
    default_port: IntProperty(
        name="Default Port",
        description="Default SEP Engine port",
        default=8080,
        min=1,
        max=65535
    )
    
    default_connection_mode: EnumProperty(
        name="Default Connection Mode",
        items=[
            ('http', "HTTP API", "Use HTTP REST API"),
            ('direct', "Direct Bridge", "Use C API bridge"),
            ('hybrid', "Hybrid", "Use both connections")
        ],
        default='http'
    )
    
    # Performance settings
    max_vertices_per_batch: IntProperty(
        name="Max Vertices Per Batch",
        description="Maximum vertices to process in single batch",
        default=10000,
        min=100,
        max=1000000
    )
    
    cache_timeout: FloatProperty(
        name="Cache Timeout",
        description="Seconds before cache expires",
        default=60.0,
        min=1.0,
        max=3600.0
    )
    
    worker_threads: IntProperty(
        name="Worker Threads",
        description="Number of background worker threads",
        default=2,
        min=1,
        max=8
    )
    
    # UI settings
    show_advanced_settings: BoolProperty(
        name="Show Advanced Settings",
        description="Show advanced configuration options",
        default=False
    )
    
    enable_viewport_overlay: BoolProperty(
        name="Enable Viewport Overlay",
        description="Show SEP information in viewport",
        default=True
    )
    
    overlay_font_size: IntProperty(
        name="Overlay Font Size",
        description="Font size for viewport overlay",
        default=11,
        min=8,
        max=24
    )
    
    # Debug settings
    enable_debug_logging: BoolProperty(
        name="Enable Debug Logging",
        description="Enable detailed debug logging",
        default=False
    )
    
    log_file_path: StringProperty(
        name="Log File Path",
        description="Path to debug log file",
        subtype='FILE_PATH',
        default=""
    )
    
    enable_profiling: BoolProperty(
        name="Enable Profiling",
        description="Enable performance profiling",
        default=False
    )
    
    # Connection presets
    connection_presets: CollectionProperty(
        type=SEPPresetItem,
        name="Connection Presets"
    )
    
    active_preset_index: IntProperty(
        name="Active Preset",
        default=0
    )
    
    # Experimental features
    enable_experimental: BoolProperty(
        name="Enable Experimental Features",
        description="Enable experimental and unstable features",
        default=False
    )
    
    enable_gpu_preview: BoolProperty(
        name="GPU Preview",
        description="Enable GPU-accelerated preview (experimental)",
        default=False
    )
    
    enable_ml_integration: BoolProperty(
        name="ML Integration",
        description="Enable machine learning integration (experimental)",
        default=False
    )
    
    def draw(self, context):
        layout = self.layout
        
        # Main settings
        box = layout.box()
        box.label(text="SEP Engine Configuration", icon='SETTINGS')
        
        # Library status
        col = box.column()
        if self.library_installed:
            row = col.row()
            row.label(text="Library Status:", icon='CHECKMARK')
            row.label(text="Installed")
        else:
            row = col.row()
            row.label(text="Library Status:", icon='ERROR')
            row.label(text="Not Installed")
            col.operator("sep.install_library", icon='IMPORT')
        
        if self.library_path:
            col.prop(self, "library_path")
        
        # Connection defaults
        box = layout.box()
        box.label(text="Connection Defaults", icon='NETWORK_DRIVE')
        col = box.column()
        col.prop(self, "default_host")
        col.prop(self, "default_port")
        col.prop(self, "default_connection_mode")
        
        # Connection presets
        box = layout.box()
        box.label(text="Connection Presets", icon='PRESET')
        
        row = box.row()
        col = row.column()
        col.template_list(
            "UI_UL_list", "sep_presets",
            self, "connection_presets",
            self, "active_preset_index",
            rows=3
        )
        
        col = row.column(align=True)
        col.operator("sep.add_preset", icon='ADD', text="")
        col.operator("sep.remove_preset", icon='REMOVE', text="")
        col.separator()
        col.operator("sep.load_preset", icon='FILE_TICK', text="")
        
        # Performance settings
        box = layout.box()
        box.label(text="Performance", icon='SORTTIME')
        col = box.column()
        col.prop(self, "max_vertices_per_batch")
        col.prop(self, "cache_timeout")
        col.prop(self, "worker_threads")
        
        # UI settings
        box = layout.box()
        box.label(text="User Interface", icon='WORKSPACE')
        col = box.column()
        col.prop(self, "enable_viewport_overlay")
        if self.enable_viewport_overlay:
            col.prop(self, "overlay_font_size")
        col.prop(self, "show_advanced_settings")
        
        # Advanced settings
        if self.show_advanced_settings:
            # Debug settings
            box = layout.box()
            box.label(text="Debug", icon='CONSOLE')
            col = box.column()
            col.prop(self, "enable_debug_logging")
            if self.enable_debug_logging:
                col.prop(self, "log_file_path")
            col.prop(self, "enable_profiling")
            
            # Experimental features
            box = layout.box()
            box.label(text="Experimental Features", icon='EXPERIMENTAL')
            col = box.column()
            col.prop(self, "enable_experimental")
            if self.enable_experimental:
                col.separator()
                col.prop(self, "enable_gpu_preview")
                col.prop(self, "enable_ml_integration")
                col.label(text="Warning: Experimental features may be unstable", icon='ERROR')
        
        # Documentation
        layout.separator()
        row = layout.row()
        row.operator("sep.open_documentation", icon='HELP')
        row.operator("sep.open_documentation", text="GitHub", icon='URL').url = "https://github.com/someaka/sep-engine"

def get_preferences() -> SEPAddonPreferences:
    """Get addon preferences"""
    addon_name = __name__.split('.')[0] if '.' in __name__ else 'sep_engine'
    return bpy.context.preferences.addons[addon_name].preferences

def apply_default_settings(scene):
    """Apply default settings from preferences"""
    prefs = get_preferences()
    settings = scene.sep_settings
    
    # Apply defaults if not already set
    if not settings.host:
        settings.host = prefs.default_host
    if settings.port == 0:
        settings.port = prefs.default_port
    if not settings.connection_mode:
        settings.connection_mode = prefs.default_connection_mode
    
    # Apply library path
    if prefs.library_path and not settings.library_path:
        settings.library_path = prefs.library_path

# Classes to register
classes = [
    SEPPresetItem,
    SEP_OT_install_library,
    SEP_OT_add_preset,
    SEP_OT_remove_preset,
    SEP_OT_load_preset,
    SEP_OT_open_documentation,
    SEPAddonPreferences
]
