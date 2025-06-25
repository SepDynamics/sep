"""
SEP Engine Pattern Processor
============================
Core pattern processing algorithms implementing quantum-inspired
transformations, field generation, and coherence analysis.
"""

import bpy
import bmesh
import numpy as np
from mathutils import Vector, Matrix, Quaternion, noise
import math
import time
from typing import Dict, List, Tuple, Optional, Any, Callable, Union
from dataclasses import dataclass
from enum import Enum
import json

# Pattern processing modes
class ProcessingMode(Enum):
    COHERENCE = "coherence"
    EVOLUTION = "evolution"
    ENTANGLEMENT = "entanglement"
    COLLAPSE = "collapse"
    MUTATION = "mutation"

@dataclass
class PatternState:
    """Quantum pattern state representation"""
    coherence: float
    stability: float
    entropy: float
    complexity: float
    phase: float
    amplitude: float
    qbsa_state: int
    qfh_level: int
    wavefunction: Optional[np.ndarray] = None
    
    def to_dict(self) -> Dict[str, Any]:
        """Convert to dictionary format"""
        return {
            "coherence": self.coherence,
            "stability": self.stability,
            "entropy": self.entropy,
            "complexity": self.complexity,
            "phase": self.phase,
            "amplitude": self.amplitude,
            "qbsa_state": self.qbsa_state,
            "qfh_level": self.qfh_level
        }
    
    def evolve(self, dt: float = 0.1) -> 'PatternState':
        """Evolve pattern state through time"""
        # Hamiltonian evolution
        H = self.complexity * self.entropy
        
        # Phase evolution
        new_phase = (self.phase + H * dt) % (2 * math.pi)
        
        # Coherence decay with stability factor
        coherence_decay = math.exp(-dt * (1.0 - self.stability) * 0.1)
        new_coherence = self.coherence * coherence_decay
        
        # Entropy increase
        entropy_growth = 1.0 - math.exp(-dt * 0.05)
        new_entropy = min(1.0, self.entropy + entropy_growth * (1.0 - self.entropy))
        
        # Stability adjustment based on coherence
        new_stability = self.stability * 0.9 + new_coherence * 0.1
        
        # Complexity evolution
        new_complexity = self.complexity * 0.95 + (self.entropy * self.coherence) * 0.05
        
        # Amplitude modulation
        new_amplitude = self.amplitude * (0.9 + 0.1 * math.cos(new_phase))
        
        # QBSA state transition
        new_qbsa = self.qbsa_state
        if new_coherence < 0.3 and np.random.random() < 0.1:
            new_qbsa = (new_qbsa + 1) % 256
        
        # QFH level adjustment
        new_qfh = self.qfh_level
        if new_entropy > 0.8:
            new_qfh = max(0, new_qfh - 1)
        elif new_coherence > 0.8:
            new_qfh = min(16, new_qfh + 1)
        
        return PatternState(
            coherence=new_coherence,
            stability=new_stability,
            entropy=new_entropy,
            complexity=new_complexity,
            phase=new_phase,
            amplitude=new_amplitude,
            qbsa_state=new_qbsa,
            qfh_level=new_qfh
        )

class QuantumField:
    """Quantum field representation for pattern interactions"""
    
    def __init__(self, resolution: int = 32):
        self.resolution = resolution
        self.field = np.zeros((resolution, resolution, resolution), dtype=np.complex128)
        self.time = 0.0
        
    def add_source(self, position: Vector, state: PatternState):
        """Add a pattern source to the field"""
        # Convert position to grid coordinates
        grid_pos = self._world_to_grid(position)
        
        # Create wavefunction
        for i in range(self.resolution):
            for j in range(self.resolution):
                for k in range(self.resolution):
                    r = Vector((i, j, k)) - grid_pos
                    distance = r.length
                    
                    # Gaussian wave packet
                    sigma = 2.0 + state.coherence * 3.0
                    envelope = math.exp(-distance**2 / (2 * sigma**2))
                    
                    # Phase contribution
                    phase = state.phase + distance * state.complexity
                    
                    # Add to field
                    self.field[i, j, k] += state.amplitude * envelope * np.exp(1j * phase)
    
    def evolve(self, dt: float = 0.1):
        """Evolve field using Schrödinger equation"""
        # Simplified evolution - in reality would use FFT for efficiency
        laplacian = self._compute_laplacian()
        
        # Time evolution operator
        self.field += 1j * dt * laplacian
        self.time += dt
        
        # Normalize to prevent divergence
        max_amp = np.max(np.abs(self.field))
        if max_amp > 10.0:
            self.field /= max_amp / 10.0
    
    def measure_at(self, position: Vector) -> Dict[str, float]:
        """Measure field properties at position"""
        grid_pos = self._world_to_grid(position)
        
        # Interpolate field value
        value = self._interpolate_field(grid_pos)
        
        # Extract quantum properties
        amplitude = np.abs(value)
        phase = np.angle(value)
        
        # Compute local derivatives for additional properties
        gradient = self._compute_gradient(grid_pos)
        
        return {
            "amplitude": float(amplitude),
            "phase": float(phase),
            "probability": float(amplitude**2),
            "gradient_magnitude": float(np.linalg.norm(gradient)),
            "coherence": float(np.abs(value) / (1.0 + np.linalg.norm(gradient)))
        }
    
    def _world_to_grid(self, position: Vector) -> Vector:
        """Convert world position to grid coordinates"""
        # Assume field covers -10 to 10 units in each dimension
        normalized = (Vector(position) + Vector((10, 10, 10))) / 20.0
        return Vector((
            normalized.x * (self.resolution - 1),
            normalized.y * (self.resolution - 1),
            normalized.z * (self.resolution - 1)
        ))
    
    def _compute_laplacian(self) -> np.ndarray:
        """Compute discrete Laplacian of field"""
        laplacian = np.zeros_like(self.field)
        
        # Simple finite difference approximation
        for axis in range(3):
            laplacian += np.roll(self.field, 1, axis=axis) - 2*self.field + np.roll(self.field, -1, axis=axis)
        
        return laplacian
    
    def _interpolate_field(self, grid_pos: Vector) -> complex:
        """Trilinear interpolation of field value"""
        # Clamp to grid bounds
        x = max(0, min(self.resolution - 2, grid_pos.x))
        y = max(0, min(self.resolution - 2, grid_pos.y))
        z = max(0, min(self.resolution - 2, grid_pos.z))
        
        # Integer and fractional parts
        ix, iy, iz = int(x), int(y), int(z)
        fx, fy, fz = x - ix, y - iy, z - iz
        
        # Trilinear interpolation
        c000 = self.field[ix, iy, iz]
        c100 = self.field[ix+1, iy, iz]
        c010 = self.field[ix, iy+1, iz]
        c110 = self.field[ix+1, iy+1, iz]
        c001 = self.field[ix, iy, iz+1]
        c101 = self.field[ix+1, iy, iz+1]
        c011 = self.field[ix, iy+1, iz+1]
        c111 = self.field[ix+1, iy+1, iz+1]
        
        # Interpolate along x
        c00 = c000 * (1-fx) + c100 * fx
        c10 = c010 * (1-fx) + c110 * fx
        c01 = c001 * (1-fx) + c101 * fx
        c11 = c011 * (1-fx) + c111 * fx
        
        # Interpolate along y
        c0 = c00 * (1-fy) + c10 * fy
        c1 = c01 * (1-fy) + c11 * fy
        
        # Interpolate along z
        return c0 * (1-fz) + c1 * fz
    
    def _compute_gradient(self, grid_pos: Vector) -> np.ndarray:
        """Compute field gradient at position"""
        h = 0.5  # Step size
        
        # Finite difference gradient
        dx = self._interpolate_field(grid_pos + Vector((h, 0, 0))) - \
             self._interpolate_field(grid_pos - Vector((h, 0, 0)))
        dy = self._interpolate_field(grid_pos + Vector((0, h, 0))) - \
             self._interpolate_field(grid_pos - Vector((0, h, 0)))
        dz = self._interpolate_field(grid_pos + Vector((0, 0, h))) - \
             self._interpolate_field(grid_pos - Vector((0, 0, h)))
        
        return np.array([np.abs(dx), np.abs(dy), np.abs(dz)]) / (2 * h)

def process_mesh_pattern(mesh_data: Dict[str, Any], 
                        mode: ProcessingMode = ProcessingMode.COHERENCE,
                        params: Optional[Dict[str, Any]] = None) -> Dict[str, Any]:
    """
    Process mesh data through quantum pattern algorithms.
    
    Args:
        mesh_data: Extracted mesh data
        mode: Processing mode
        params: Additional parameters
        
    Returns:
        Processing result with metrics and deformation
    """
    params = params or {}
    
    # Extract vertex data
    vertices = mesh_data["vertices"]
    edges = mesh_data["edges"]
    faces = mesh_data["faces"]
    
    # Create initial pattern state
    initial_state = _analyze_mesh_pattern(vertices, edges, faces)
    
    # Process based on mode
    if mode == ProcessingMode.COHERENCE:
        result = _process_coherence(initial_state, vertices, params)
    elif mode == ProcessingMode.EVOLUTION:
        result = _process_evolution(initial_state, vertices, params)
    elif mode == ProcessingMode.ENTANGLEMENT:
        result = _process_entanglement(initial_state, vertices, params)
    elif mode == ProcessingMode.COLLAPSE:
        result = _process_collapse(initial_state, vertices, params)
    elif mode == ProcessingMode.MUTATION:
        result = _process_mutation(initial_state, vertices, params)
    else:
        result = {"state": initial_state.to_dict()}
    
    # Generate deformation field
    if params.get("return_deformation", True):
        result["deformation"] = _generate_deformation_field(
            initial_state,
            result.get("state", initial_state),
            vertices,
            params
        )
    
    return result

def _analyze_mesh_pattern(vertices: List[Dict], 
                         edges: List[Dict], 
                         faces: List[Dict]) -> PatternState:
    """Analyze mesh topology to extract pattern state"""
    
    # Compute coherence from vertex normal alignment
    normals = np.array([[v["normal"]["x"], v["normal"]["y"], v["normal"]["z"]] 
                       for v in vertices])
    coherence_matrix = np.abs(normals @ normals.T)
    coherence = np.mean(coherence_matrix)
    
    # Compute stability from edge length variance
    edge_lengths = []
    for edge in edges:
        v1 = vertices[edge["v1"]]
        v2 = vertices[edge["v2"]]
        p1 = Vector((v1["co"]["x"], v1["co"]["y"], v1["co"]["z"]))
        p2 = Vector((v2["co"]["x"], v2["co"]["y"], v2["co"]["z"]))
        edge_lengths.append((p2 - p1).length)
    
    if edge_lengths:
        length_variance = np.var(edge_lengths)
        stability = 1.0 / (1.0 + length_variance)
    else:
        stability = 0.5
    
    # Compute entropy from face area distribution
    face_areas = [f["area"] for f in faces if "area" in f]
    if face_areas:
        # Normalize areas
        total_area = sum(face_areas)
        if total_area > 0:
            probabilities = np.array(face_areas) / total_area
            # Shannon entropy
            entropy = -np.sum(probabilities * np.log(probabilities + 1e-10))
            entropy = entropy / np.log(len(face_areas))  # Normalize to [0,1]
        else:
            entropy = 0.5
    else:
        entropy = 0.5
    
    # Compute complexity from topology
    V = len(vertices)
    E = len(edges)
    F = len(faces)
    euler_characteristic = V - E + F
    complexity = 1.0 - 1.0 / (1.0 + abs(2 - euler_characteristic))
    
    # Initialize quantum properties
    phase = 0.0
    amplitude = 1.0
    qbsa_state = int(coherence * 255)
    qfh_level = int(stability * 16)
    
    return PatternState(
        coherence=coherence,
        stability=stability,
        entropy=entropy,
        complexity=complexity,
        phase=phase,
        amplitude=amplitude,
        qbsa_state=qbsa_state,
        qfh_level=qfh_level
    )

def _process_coherence(state: PatternState, 
                      vertices: List[Dict],
                      params: Dict[str, Any]) -> Dict[str, Any]:
    """Process pattern for coherence optimization"""
    
    # Target coherence
    target_coherence = params.get("target_coherence", 0.9)
    iterations = params.get("iterations", 10)
    
    current_state = state
    trajectory = [state.to_dict()]
    
    for i in range(iterations):
        # Coherence gradient ascent
        coherence_error = target_coherence - current_state.coherence
        
        # Adjust stability to improve coherence
        stability_adjustment = coherence_error * 0.1
        new_stability = max(0, min(1, current_state.stability + stability_adjustment))
        
        # Reduce entropy for higher coherence
        entropy_adjustment = -coherence_error * 0.05
        new_entropy = max(0, min(1, current_state.entropy + entropy_adjustment))
        
        # Update state
        current_state = PatternState(
            coherence=current_state.coherence * 0.9 + target_coherence * 0.1,
            stability=new_stability,
            entropy=new_entropy,
            complexity=current_state.complexity,
            phase=current_state.phase + 0.1,
            amplitude=current_state.amplitude,
            qbsa_state=int(current_state.coherence * 255),
            qfh_level=int(new_stability * 16)
        )
        
        trajectory.append(current_state.to_dict())
        
        # Early stopping if target reached
        if abs(coherence_error) < 0.01:
            break
    
    return {
        "state": current_state.to_dict(),
        "trajectory": trajectory,
        "iterations": i + 1,
        "converged": abs(target_coherence - current_state.coherence) < 0.01
    }

def _process_evolution(state: PatternState,
                      vertices: List[Dict],
                      params: Dict[str, Any]) -> Dict[str, Any]:
    """Process pattern evolution through time"""
    
    steps = params.get("steps", 10)
    dt = params.get("dt", 0.1)
    
    current_state = state
    trajectory = [state.to_dict()]
    
    # Create quantum field
    field = QuantumField(resolution=16)
    
    # Add initial sources
    for i, vertex in enumerate(vertices[:10]):  # Limit for performance
        pos = Vector((vertex["co"]["x"], vertex["co"]["y"], vertex["co"]["z"]))
        field.add_source(pos, current_state)
    
    for step in range(steps):
        # Evolve state
        current_state = current_state.evolve(dt)
        
        # Evolve field
        field.evolve(dt)
        
        # Measure field influence
        total_influence = 0.0
        for vertex in vertices[:10]:
            pos = Vector((vertex["co"]["x"], vertex["co"]["y"], vertex["co"]["z"]))
            measurement = field.measure_at(pos)
            total_influence += measurement["probability"]
        
        # Modulate state based on field
        field_factor = total_influence / (len(vertices[:10]) + 1.0)
        current_state.coherence *= (0.9 + 0.1 * field_factor)
        
        trajectory.append(current_state.to_dict())
    
    return {
        "state": current_state.to_dict(),
        "trajectory": trajectory,
        "field_energy": float(np.sum(np.abs(field.field)**2)),
        "evolution_time": steps * dt
    }

def _process_entanglement(state: PatternState,
                         vertices: List[Dict],
                         params: Dict[str, Any]) -> Dict[str, Any]:
    """Process quantum entanglement between patterns"""
    
    # For single pattern, create self-entanglement
    coupling_strength = params.get("coupling_strength", 0.5)
    
    # Create entangled state
    entangled_state = PatternState(
        coherence=(state.coherence + state.stability) / 2,
        stability=state.stability * coupling_strength + state.coherence * (1 - coupling_strength),
        entropy=state.entropy * (1 - coupling_strength),
        complexity=state.complexity * (1 + coupling_strength),
        phase=state.phase,
        amplitude=state.amplitude * math.sqrt(coupling_strength),
        qbsa_state=(state.qbsa_state + int(coupling_strength * 128)) % 256,
        qfh_level=min(16, state.qfh_level + int(coupling_strength * 4))
    )
    
    # Compute entanglement entropy
    p1 = coupling_strength
    p2 = 1 - coupling_strength
    entanglement_entropy = -(p1 * math.log(p1 + 1e-10) + p2 * math.log(p2 + 1e-10))
    
    return {
        "state": entangled_state.to_dict(),
        "entanglement_entropy": entanglement_entropy,
        "coupling_strength": coupling_strength,
        "bell_state": coupling_strength > 0.7  # Simplified Bell state criterion
    }

def _process_collapse(state: PatternState,
                     vertices: List[Dict],
                     params: Dict[str, Any]) -> Dict[str, Any]:
    """Process wavefunction collapse"""
    
    collapse_threshold = params.get("collapse_threshold", 0.3)
    
    # Determine collapse based on coherence
    if state.coherence < collapse_threshold:
        # Collapse to low-energy state
        collapsed_state = PatternState(
            coherence=0.1,
            stability=0.9,
            entropy=0.1,
            complexity=0.1,
            phase=0.0,
            amplitude=0.1,
            qbsa_state=0,
            qfh_level=0
        )
        collapse_type = "ground_state"
    else:
        # Partial collapse
        collapsed_state = PatternState(
            coherence=state.coherence * 0.5,
            stability=min(1.0, state.stability * 1.5),
            entropy=state.entropy * 0.7,
            complexity=state.complexity * 0.8,
            phase=0.0,  # Phase reset
            amplitude=state.amplitude * 0.7,
            qbsa_state=state.qbsa_state // 2,
            qfh_level=max(0, state.qfh_level - 2)
        )
        collapse_type = "partial"
    
    return {
        "state": collapsed_state.to_dict(),
        "collapse_type": collapse_type,
        "energy_released": state.amplitude - collapsed_state.amplitude,
        "information_loss": state.entropy - collapsed_state.entropy
    }

def _process_mutation(state: PatternState,
                     vertices: List[Dict],
                     params: Dict[str, Any]) -> Dict[str, Any]:
    """Process pattern mutation"""
    
    mutation_rate = params.get("mutation_rate", 0.1)
    mutation_type = params.get("mutation_type", "random")
    
    # Apply mutation based on type
    if mutation_type == "random":
        # Random perturbations
        mutated_state = PatternState(
            coherence=max(0, min(1, state.coherence + np.random.normal(0, mutation_rate))),
            stability=max(0, min(1, state.stability + np.random.normal(0, mutation_rate))),
            entropy=max(0, min(1, state.entropy + np.random.normal(0, mutation_rate))),
            complexity=max(0, min(1, state.complexity + np.random.normal(0, mutation_rate))),
            phase=(state.phase + np.random.normal(0, mutation_rate * math.pi)) % (2 * math.pi),
            amplitude=max(0, state.amplitude + np.random.normal(0, mutation_rate)),
            qbsa_state=(state.qbsa_state + np.random.randint(-10, 11)) % 256,
            qfh_level=max(0, min(16, state.qfh_level + np.random.randint(-2, 3)))
        )
    elif mutation_type == "adaptive":
        # Mutation based on current state
        if state.coherence < 0.5:
            # Boost coherence
            coherence_boost = mutation_rate * 2
            stability_boost = mutation_rate
        else:
            # Increase complexity
            coherence_boost = -mutation_rate * 0.5
            stability_boost = -mutation_rate * 0.5
        
        mutated_state = PatternState(
            coherence=max(0, min(1, state.coherence + coherence_boost)),
            stability=max(0, min(1, state.stability + stability_boost)),
            entropy=state.entropy * (1 + mutation_rate),
            complexity=state.complexity * (1 + mutation_rate * 0.5),
            phase=state.phase,
            amplitude=state.amplitude,
            qbsa_state=state.qbsa_state,
            qfh_level=state.qfh_level
        )
    else:
        mutated_state = state
    
    # Calculate mutation distance
    mutation_distance = math.sqrt(
        (mutated_state.coherence - state.coherence)**2 +
        (mutated_state.stability - state.stability)**2 +
        (mutated_state.entropy - state.entropy)**2 +
        (mutated_state.complexity - state.complexity)**2
    )
    
    return {
        "state": mutated_state.to_dict(),
        "mutation_distance": mutation_distance,
        "mutation_type": mutation_type,
        "fitness_change": mutated_state.coherence - state.coherence
    }

def _generate_deformation_field(initial_state: PatternState,
                               final_state: Union[PatternState, Dict],
                               vertices: List[Dict],
                               params: Dict[str, Any]) -> Dict[str, Any]:
    """Generate deformation field based on state transition"""
    
    if isinstance(final_state, dict):
        # Convert dict to PatternState
        final_state = PatternState(**{k: final_state.get(k, getattr(initial_state, k)) 
                                     for k in initial_state.__dataclass_fields__})
    
    mode = params.get("deformation_mode", "quantum")
    strength = params.get("deformation_strength", 1.0)
    
    if mode == "quantum":
        # Quantum field-based deformation
        field = {
            "amplitude": abs(final_state.amplitude - initial_state.amplitude) * strength,
            "frequency": 1.0 + final_state.complexity,
            "phase": final_state.phase,
            "sigma": 2.0 / (1.0 + final_state.entropy),
            "origin": [0.0, 0.0, 0.0],
            "wave_vector": [
                math.sin(final_state.phase),
                math.cos(final_state.phase),
                final_state.coherence
            ]
        }
        
        return {"field": field, "preserve_volume": True}
        
    elif mode == "harmonic":
        # Spherical harmonic deformation
        harmonics = []
        
        # Generate harmonics based on state
        max_l = min(4, int(final_state.qfh_level / 4) + 1)
        
        for l in range(max_l + 1):
            for m in range(-l, l + 1):
                # Coefficient based on state properties
                coeff = (final_state.coherence - initial_state.coherence) * 0.1
                if l > 0:
                    coeff *= math.exp(-l * final_state.entropy)
                
                if abs(coeff) > 0.001:
                    harmonics.append({
                        "l": l,
                        "m": m,
                        "coefficient": coeff * strength
                    })
        
        return {"harmonics": harmonics, "preserve_volume": True}
        
    else:
        # Direct vertex deformation
        deformations = []
        
        for i, vertex in enumerate(vertices):
            # Compute per-vertex deformation
            normal = Vector((vertex["normal"]["x"], 
                           vertex["normal"]["y"], 
                           vertex["normal"]["z"]))
            
            # Deformation magnitude based on state change
            magnitude = (final_state.coherence - initial_state.coherence) * 0.1
            
            # Add noise based on entropy
            noise_factor = final_state.entropy
            magnitude += (noise.random() - 0.5) * noise_factor * 0.05
            
            # Apply along normal
            offset = normal * magnitude * strength
            
            deformations.append({
                "x": offset.x,
                "y": offset.y,
                "z": offset.z
            })
        
        return deformations

def generate_field_visualization(obj: bpy.types.Object,
                               resolution: int,
                               field_type: str,
                               settings: Any):
    """Generate a mesh visualization of quantum field"""
    
    # Create field
    field = QuantumField(resolution=resolution)
    
    # Add sources from scene patterns
    for pattern in settings.patterns[:5]:  # Limit for performance
        if pattern.source_object:
            source_obj = bpy.data.objects.get(pattern.source_object)
            if source_obj:
                state = PatternState(
                    coherence=pattern.metrics.coherence,
                    stability=pattern.metrics.stability,
                    entropy=pattern.metrics.entropy,
                    complexity=pattern.metrics.complexity,
                    phase=0.0,
                    amplitude=1.0,
                    qbsa_state=0,
                    qfh_level=0
                )
                field.add_source(source_obj.location, state)
    
    # Generate mesh
    bm = bmesh.new()
    
    # Create grid of vertices
    verts = []
    for x in range(resolution):
        for y in range(resolution):
            for z in range(resolution):
                # Convert to world space
                world_pos = Vector((
                    (x / (resolution - 1) - 0.5) * 20,
                    (y / (resolution - 1) - 0.5) * 20,
                    (z / (resolution - 1) - 0.5) * 20
                ))
                
                # Measure field at position
                measurement = field.measure_at(world_pos)
                
                # Determine if vertex should be created based on field type
                threshold = 0.1
                value = 0.0
                
                if field_type == 'COHERENCE':
                    value = measurement["coherence"]
                elif field_type == 'STABILITY':
                    value = measurement["amplitude"] * (1.0 - measurement["gradient_magnitude"])
                elif field_type == 'QUANTUM':
                    value = measurement["probability"]
                elif field_type == 'PHASE':
                    value = abs(math.sin(measurement["phase"]))
                
                if value > threshold:
                    vert = bm.verts.new(world_pos)
                    verts.append((vert, value))
    
    # Create iso-surface using marching cubes approximation
    # For simplicity, create small spheres at high-value points
    for vert, value in verts:
        if value > 0.5:
            # Create small icosphere
            bmesh.ops.create_icosphere(
                bm,
                subdivisions=1,
                radius=value * 0.5,
                matrix=Matrix.Translation(vert.co)
            )
    
    # Update mesh
    bm.to_mesh(obj.data)
    bm.free()
    
    # Add material for visualization
    _create_field_material(obj, field_type)

def _create_field_material(obj: bpy.types.Object, field_type: str):
    """Create material for field visualization"""
    
    # Create new material
    mat = bpy.data.materials.new(name=f"SEP_Field_{field_type}")
    mat.use_nodes = True
    
    # Get node tree
    nodes = mat.node_tree.nodes
    links = mat.node_tree.links
    
    # Clear default nodes
    nodes.clear()
    
    # Create nodes
    output = nodes.new('ShaderNodeOutputMaterial')
    emission = nodes.new('ShaderNodeEmission')
    fresnel = nodes.new('ShaderNodeFresnel')
    color_ramp = nodes.new('ShaderNodeColorRamp')
    
    # Set positions
    output.location = (300, 0)
    emission.location = (0, 0)
    fresnel.location = (-300, 0)
    color_ramp.location = (-150, -100)
    
    # Configure color ramp based on field type
    if field_type == 'COHERENCE':
        # Blue to white gradient
        color_ramp.color_ramp.elements[0].color = (0.1, 0.3, 1.0, 1.0)
        color_ramp.color_ramp.elements[1].color = (0.8, 0.9, 1.0, 1.0)
    elif field_type == 'STABILITY':
        # Green to yellow gradient
        color_ramp.color_ramp.elements[0].color = (0.1, 1.0, 0.3, 1.0)
        color_ramp.color_ramp.elements[1].color = (1.0, 0.9, 0.1, 1.0)
    elif field_type == 'QUANTUM':
        # Purple to cyan gradient
        color_ramp.color_ramp.elements[0].color = (0.6, 0.1, 1.0, 1.0)
        color_ramp.color_ramp.elements[1].color = (0.1, 0.9, 1.0, 1.0)
    else:  # PHASE
        # Red to orange gradient
        color_ramp.color_ramp.elements[0].color = (1.0, 0.1, 0.1, 1.0)
        color_ramp.color_ramp.elements[1].color = (1.0, 0.6, 0.1, 1.0)
    
    # Create links
    links.new(fresnel.outputs['Fac'], color_ramp.inputs['Fac'])
    links.new(color_ramp.outputs['Color'], emission.inputs['Color'])
    links.new(emission.outputs['Emission'], output.inputs['Surface'])
    
    # Set emission strength
    emission.inputs['Strength'].default_value = 2.0
    
    # Assign material to object
    obj.data.materials.clear()
    obj.data.materials.append(mat)

# Module has no classes to register
classes = []
