# SEP Engine Blender Integration

A comprehensive Blender addon for integrating with the SEP Engine's quantum-inspired pattern processing framework. This addon provides deep integration between Blender's 3D environment and the SEP Engine's advanced computational manifold processing capabilities.

## Features

### Core Functionality
- **Quantum Pattern Processing**: Process mesh objects through SEP Engine's QBSA and QFH algorithms
- **Real-time Evolution**: Automatic pattern evolution synchronized with frame changes
- **Memory Tier Management**: Three-tiered memory system (STM/MTM/LTM) with automatic promotion
- **Quantum Field Visualization**: Generate and visualize quantum fields in 3D space
- **Pattern Entanglement**: Create quantum entanglement between multiple mesh patterns

### Connection Modes
- **HTTP API**: RESTful API connection for standard operations
- **Direct Bridge**: High-performance C API bridge for intensive processing
- **Hybrid Mode**: Automatic selection of optimal connection method

### Advanced Features
- **Batch Processing**: Process multiple objects simultaneously
- **Scene Analysis**: Analyze entire scenes for quantum coherence patterns
- **Deformation Modes**: Multiple deformation algorithms (quantum field, spherical harmonics, direct)
- **Pattern Export/Import**: Save and load pattern configurations
- **Real-time Metrics**: Live visualization of pattern metrics in viewport

## Installation

1. **Download the Addon**
   - Download the `sep_engine` folder containing all addon files
   - Ensure all Python files are present

2. **Install in Blender**
   - Open Blender 3.0+ (tested with Blender 5.0)
   - Go to Edit → Preferences → Add-ons
   - Click "Install" and select the addon folder
   - Enable "3D View: SEP Engine Integration"

3. **Install SEP Bridge Library** (Optional for Direct mode)
   - Go to addon preferences
   - Click "Install SEP Library" to download platform-specific library
   - Or manually place library in `addon_folder/libs/`

## Configuration

### Initial Setup

1. **Start SEP Engine Service**
   ```bash
   # Start the SEP Engine service
   sep_engine --port 8080
   ```

2. **Configure Connection**
   - In Blender, go to 3D Viewport → Sidebar → SEP tab
   - Set host and port (default: localhost:8080)
   - Choose connection mode (HTTP recommended for start)
   - Click "Connect"

### Addon Preferences

Access via Edit → Preferences → Add-ons → SEP Engine Integration:

- **Connection Defaults**: Set default host, port, and connection mode
- **Performance**: Configure batch sizes and worker threads
- **UI Settings**: Customize viewport overlay and display options
- **Debug Options**: Enable logging and profiling for troubleshooting

## Usage

### Basic Workflow

1. **Select a Mesh Object**
   - Select any mesh object in your scene
   - Enable SEP processing in the object's SEP settings

2. **Process Pattern**
   - Click "Process Object" to analyze and process the mesh
   - View real-time metrics in the panel
   - Enable deformation to see quantum field effects

3. **Evolve Patterns**
   - Enable "Auto Evolve" for continuous evolution
   - Or manually evolve with the "Evolve Pattern" button
   - Adjust evolution rate for different speeds

### Advanced Operations

#### Batch Processing
```
1. Select multiple mesh objects
2. Click "Batch Process" 
3. All selected objects will be processed sequentially
```

#### Pattern Entanglement
```
1. Select exactly two mesh objects with patterns
2. Click "Entangle Patterns"
3. Objects will share quantum states
```

#### Quantum Field Visualization
```
1. Click "Visualize Quantum Field"
2. Choose field type (Coherence, Stability, Quantum, Phase)
3. Adjust resolution for detail level
4. A new mesh will be created showing the field
```

### Pattern Metrics

Each processed object maintains these quantum metrics:

- **Coherence** (0-1): Pattern alignment and consistency
- **Stability** (0-1): Resistance to quantum fluctuations
- **Entropy** (0-1): Disorder and information content
- **Complexity** (0-1): Topological and structural complexity

### Memory Tiers

Patterns are automatically managed across three memory tiers:

- **STM (Short-Term Memory)**: Active patterns, low coherence threshold
- **MTM (Medium-Term Memory)**: Recent stable patterns
- **LTM (Long-Term Memory)**: Highly stable, persistent patterns

## Deformation Modes

### Normal
Deforms vertices along their normals based on pattern metrics.

### Radial
Creates radial deformations from object center.

### Wave
Applies wave-like deformations across the mesh.

### Quantum
Uses quantum field equations for realistic field deformations.

## API Reference

### Operators

- `sep.connect`: Connect to SEP Engine
- `sep.disconnect`: Disconnect from SEP Engine
- `sep.process_object`: Process active mesh object
- `sep.batch_process`: Process selected objects
- `sep.evolve_pattern`: Evolve pattern through time
- `sep.entangle_patterns`: Entangle two patterns
- `sep.sync_memory`: Synchronize memory tiers
- `sep.analyze_scene`: Analyze entire scene
- `sep.visualize_quantum_field`: Create field visualization

### Properties

Access via `bpy.context.scene.sep_settings`:
- `host`: SEP Engine host
- `port`: SEP Engine port
- `connection_mode`: Connection type
- `patterns`: Collection of all patterns
- `memory_tiers`: Memory tier information

Per-object settings via `obj.sep_settings`:
- `enabled`: Enable SEP processing
- `pattern_id`: Associated pattern ID
- `metrics`: Current pattern metrics
- `quantum_state`: Quantum state information

## Troubleshooting

### Connection Issues
- Ensure SEP Engine service is running
- Check firewall settings for port access
- Try HTTP mode if Direct mode fails
- Check console for detailed error messages

### Performance
- Reduce batch size for large meshes
- Disable auto-evolution for complex scenes
- Use cache timeout to balance updates
- Consider Direct mode for better performance

### Library Issues
- Ensure correct library for your platform
- Check library path in preferences
- Try reinstalling via preferences panel
- See logs for detailed loading errors

## Examples

### Example 1: Basic Pattern Processing
```python
import bpy

# Get active object
obj = bpy.context.active_object

# Enable SEP processing
obj.sep_settings.enabled = True

# Process object
bpy.ops.sep.process_object()

# Print metrics
metrics = obj.sep_settings.metrics
print(f"Coherence: {metrics.coherence}")
print(f"Stability: {metrics.stability}")
```

### Example 2: Automated Evolution
```python
# Enable auto-evolution
obj.sep_settings.auto_evolve = True
obj.sep_settings.evolution_rate = 2.0

# Play animation to see evolution
bpy.ops.screen.animation_play()
```

### Example 3: Field Visualization
```python
# Create coherence field
bpy.ops.sep.visualize_quantum_field(
    field_type='COHERENCE',
    resolution=32
)
```

## Development

### Project Structure
```
sep_engine/
├── __init__.py          # Main addon entry
├── connection.py        # Connection management
├── properties.py        # Property definitions
├── operators.py         # All operators
├── ui.py               # UI panels
├── handlers.py         # Event handlers
├── mesh_utils.py       # Mesh processing
├── pattern_processor.py # Pattern algorithms
├── preferences.py      # Addon preferences
└── libs/               # Platform libraries
```

### Extending the Addon

To add new features:
1. Add operators in `operators.py`
2. Add UI elements in `ui.py`
3. Register in appropriate module's `classes` list
4. Update `__init__.py` if adding new modules

## License

This addon is part of the SEP Engine project. See the main project repository for license information.

## Support

- GitHub Issues: [https://github.com/someaka/sep-engine/issues](https://github.com/someaka/sep-engine/issues)
- Documentation: [https://github.com/someaka/sep-engine/wiki](https://github.com/someaka/sep-engine/wiki)
- Discord: [SEP Engine Community](https://discord.gg/sep-engine)

## Credits

Developed by Alexander J Nagy as part of the SEP Engine project for quantum-inspired computational manifold processing.
