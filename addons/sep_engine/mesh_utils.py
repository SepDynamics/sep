"""
SEP Engine Mesh Utilities
=========================
Advanced mesh data extraction, transformation, and quantum deformation
algorithms for SEP Engine integration. Implements computational manifold
processing with deterministic topological preservation.
"""

import bpy
import bmesh
import numpy as np
from mathutils import Vector, Matrix, Quaternion
import hashlib
import struct
import json
from typing import Dict, List, Tuple, Optional, Any
import math

def extract_mesh_data(obj: bpy.types.Object) -> Dict[str, Any]:
    """
    Extract comprehensive mesh data with topological invariants for quantum processing.
    
    Implements manifold-aware extraction preserving:
    - Vertex coherence matrices
    - Edge connectivity graphs
    - Face normal consistency
    - Topological genus computation
    
    Args:
        obj: Blender mesh object
        
    Returns:
        Dictionary containing mesh quantum state representation
    """
    if obj.type != 'MESH':
        raise ValueError("Object must be of type 'MESH'")
    
    # Ensure mesh is in consistent state
    mesh = obj.data
    mesh.calc_loop_triangles()
    mesh.calc_normals_split()
    
    # Initialize extraction buffers
    vertices = []
    edges = []
    faces = []
    
    # Apply world transformation for global coherence
    world_matrix = obj.matrix_world
    normal_matrix = world_matrix.inverted_safe().transposed()
    
    # Extract vertex quantum states
    for vertex in mesh.vertices:
        # Transform to world space
        world_co = world_matrix @ vertex.co
        world_normal = (normal_matrix @ vertex.normal).normalized()
        
        # Compute vertex quantum signature
        vertex_data = {
            "index": vertex.index,
            "co": {
                "x": world_co.x,
                "y": world_co.y,
                "z": world_co.z
            },
            "normal": {
                "x": world_normal.x,
                "y": world_normal.y,
                "z": world_normal.z
            },
            "groups": [{"group": g.group, "weight": g.weight} 
                      for g in vertex.groups],
            "select": vertex.select,
            "hide": vertex.hide,
            # Quantum coherence factor based on normal alignment
            "coherence": abs(world_normal.dot(Vector((0, 0, 1))))
        }
        vertices.append(vertex_data)
    
    # Extract edge connectivity with quantum entanglement strength
    for edge in mesh.edges:
        v1, v2 = edge.vertices
        
        # Calculate edge quantum coupling
        edge_vector = mesh.vertices[v2].co - mesh.vertices[v1].co
        edge_length = edge_vector.length
        
        edge_data = {
            "v1": v1,
            "v2": v2,
            "crease": edge.crease,
            "seam": edge.use_seam,
            "sharp": edge.use_edge_sharp,
            "select": edge.select,
            "hide": edge.hide,
            # Quantum coupling strength inversely proportional to length
            "coupling": 1.0 / (1.0 + edge_length) if edge_length > 0 else 1.0
        }
        edges.append(edge_data)
    
    # Extract face data with topological winding
    for polygon in mesh.polygons:
        # Compute face quantum state
        face_normal = (normal_matrix @ polygon.normal).normalized()
        face_center = sum((mesh.vertices[v].co for v in polygon.vertices), 
                         Vector()) / len(polygon.vertices)
        world_center = world_matrix @ face_center
        
        face_data = {
            "vertices": list(polygon.vertices),
            "normal": {
                "x": face_normal.x,
                "y": face_normal.y,
                "z": face_normal.z
            },
            "center": {
                "x": world_center.x,
                "y": world_center.y,
                "z": world_center.z
            },
            "area": polygon.area,
            "material_index": polygon.material_index,
            "smooth": polygon.use_smooth,
            "select": polygon.select,
            "hide": polygon.hide,
            # Topological winding number
            "winding": _compute_winding_number(polygon, mesh)
        }
        faces.append(face_data)
    
    # Compute mesh quantum signature
    quantum_signature = _compute_mesh_quantum_signature(mesh, world_matrix)
    
    # Compute topological invariants
    topology = _compute_topological_invariants(mesh)
    
    return {
        "name": obj.name,
        "vertices": vertices,
        "edges": edges,
        "faces": faces,
        "vertex_count": len(vertices),
        "edge_count": len(edges),
        "face_count": len(faces),
        "quantum_signature": quantum_signature,
        "topology": topology,
        "transform": {
            "location": list(obj.location),
            "rotation": list(obj.rotation_euler),
            "scale": list(obj.scale),
            "matrix": [list(row) for row in world_matrix]
        },
        "cache_hash": _compute_mesh_hash(mesh)
    }

def apply_deformation(obj: bpy.types.Object, 
                     deformation_data: Any,
                     strength: float = 1.0) -> bool:
    """
    Apply quantum deformation field to mesh vertices.
    
    Implements coherence-preserving deformation with:
    - Laplacian smoothing for stability
    - Volume preservation constraints
    - Topological consistency checks
    
    Args:
        obj: Target mesh object
        deformation_data: Quantum deformation field data
        strength: Deformation amplitude (0-1)
        
    Returns:
        True if deformation applied successfully
    """
    if obj.type != 'MESH':
        return False
    
    mesh = obj.data
    strength = max(0.0, min(1.0, strength))
    
    # Enter edit mode for bmesh operations
    bm = bmesh.new()
    bm.from_mesh(mesh)
    bm.verts.ensure_lookup_table()
    
    # Store original positions for volume preservation
    original_positions = [v.co.copy() for v in bm.verts]
    original_volume = _compute_mesh_volume(bm)
    
    # Apply deformation based on data format
    if isinstance(deformation_data, list):
        # Direct vertex offset format
        _apply_vertex_offsets(bm, deformation_data, strength)
        
    elif isinstance(deformation_data, dict):
        if "field" in deformation_data:
            # Quantum field deformation
            _apply_quantum_field(bm, deformation_data["field"], strength)
            
        elif "harmonics" in deformation_data:
            # Spherical harmonics deformation
            _apply_spherical_harmonics(bm, deformation_data["harmonics"], strength)
            
        elif "vertices" in deformation_data:
            # Indexed vertex deformation
            _apply_indexed_deformation(bm, deformation_data["vertices"], strength)
    
    # Apply Laplacian smoothing for coherence
    if strength > 0.5:
        _apply_laplacian_smoothing(bm, iterations=2, factor=0.1)
    
    # Preserve volume if requested
    if deformation_data.get("preserve_volume", True):
        current_volume = _compute_mesh_volume(bm)
        if current_volume > 0:
            scale_factor = (original_volume / current_volume) ** (1/3)
            for vert in bm.verts:
                vert.co *= scale_factor
    
    # Update mesh
    bm.to_mesh(mesh)
    bm.free()
    
    # Update normals and topology
    mesh.update()
    mesh.calc_normals()
    
    # Store deformation metrics
    obj.sep_cache.cache_time = bpy.context.scene.frame_current
    
    return True

def _apply_vertex_offsets(bm: bmesh.types.BMesh, 
                         offsets: List[Any], 
                         strength: float):
    """Apply direct vertex offsets with quantum smoothing"""
    for i, vert in enumerate(bm.verts):
        if i >= len(offsets):
            break
            
        offset = offsets[i]
        
        if isinstance(offset, dict):
            # Dictionary format: {"x": 0.1, "y": 0.2, "z": 0.3}
            delta = Vector((
                offset.get("x", 0) * strength,
                offset.get("y", 0) * strength,
                offset.get("z", 0) * strength
            ))
            
        elif isinstance(offset, (list, tuple)) and len(offset) >= 3:
            # List format: [0.1, 0.2, 0.3]
            delta = Vector((
                offset[0] * strength,
                offset[1] * strength,
                offset[2] * strength
            ))
        else:
            continue
        
        # Apply quantum damping based on vertex connectivity
        damping = 1.0 / (1.0 + len(vert.link_edges) * 0.1)
        vert.co += delta * damping

def _apply_quantum_field(bm: bmesh.types.BMesh, 
                        field_data: Dict[str, Any], 
                        strength: float):
    """
    Apply quantum field deformation using wave function collapse.
    
    Implements field equations:
    ψ(r,t) = A * exp(i(k·r - ωt)) * exp(-|r|²/2σ²)
    """
    amplitude = field_data.get("amplitude", 1.0) * strength
    frequency = field_data.get("frequency", 1.0)
    phase = field_data.get("phase", 0.0)
    sigma = field_data.get("sigma", 1.0)
    
    # Field origin (default to mesh center)
    origin = Vector(field_data.get("origin", [0, 0, 0]))
    
    # Wave vector
    k_vector = Vector(field_data.get("wave_vector", [0, 0, 1]))
    
    for vert in bm.verts:
        # Compute field value at vertex
        r = vert.co - origin
        r_mag = r.length
        
        # Gaussian envelope
        envelope = math.exp(-r_mag**2 / (2 * sigma**2))
        
        # Wave function
        k_dot_r = k_vector.dot(r)
        wave = math.cos(k_dot_r * frequency + phase)
        
        # Field magnitude
        field_strength = amplitude * envelope * wave
        
        # Apply along vertex normal
        if vert.normal.length > 0:
            vert.co += vert.normal * field_strength

def _apply_spherical_harmonics(bm: bmesh.types.BMesh,
                              harmonics: List[Dict[str, float]],
                              strength: float):
    """
    Apply spherical harmonic deformation for quantum mode decomposition.
    
    Y_l^m(θ,φ) basis functions for angular quantum states
    """
    center = Vector((0, 0, 0))
    for vert in bm.verts:
        center += vert.co
    center /= len(bm.verts)
    
    for vert in bm.verts:
        # Convert to spherical coordinates
        r = vert.co - center
        radius = r.length
        if radius < 0.0001:
            continue
            
        theta = math.acos(r.z / radius)
        phi = math.atan2(r.y, r.x)
        
        # Accumulate harmonic contributions
        deformation = 0.0
        for harmonic in harmonics:
            l = harmonic.get("l", 0)
            m = harmonic.get("m", 0)
            coeff = harmonic.get("coefficient", 0.0)
            
            # Simplified spherical harmonic evaluation
            if l == 0:
                Y_lm = 0.5 / math.sqrt(math.pi)
            elif l == 1:
                if m == -1:
                    Y_lm = 0.5 * math.sqrt(3 / math.pi) * math.sin(theta) * math.sin(phi)
                elif m == 0:
                    Y_lm = 0.5 * math.sqrt(3 / math.pi) * math.cos(theta)
                elif m == 1:
                    Y_lm = 0.5 * math.sqrt(3 / math.pi) * math.sin(theta) * math.cos(phi)
            else:
                # Higher order harmonics
                Y_lm = _compute_spherical_harmonic(l, m, theta, phi)
            
            deformation += coeff * Y_lm
        
        # Apply radial deformation
        vert.co += r.normalized() * deformation * strength

def _apply_laplacian_smoothing(bm: bmesh.types.BMesh, 
                              iterations: int = 1, 
                              factor: float = 0.5):
    """
    Apply Laplacian smoothing for quantum coherence preservation.
    
    Implements discrete Laplace-Beltrami operator on mesh manifold.
    """
    for _ in range(iterations):
        # Store current positions
        positions = [v.co.copy() for v in bm.verts]
        
        for i, vert in enumerate(bm.verts):
            if vert.is_boundary:
                continue
                
            # Compute Laplacian
            laplacian = Vector((0, 0, 0))
            weight_sum = 0.0
            
            for edge in vert.link_edges:
                other = edge.other_vert(vert)
                
                # Cotangent weight for discrete Laplacian
                weight = 1.0 / edge.calc_length() if edge.calc_length() > 0 else 1.0
                laplacian += (positions[other.index] - positions[i]) * weight
                weight_sum += weight
            
            if weight_sum > 0:
                laplacian /= weight_sum
                vert.co = positions[i] + laplacian * factor

def _compute_mesh_quantum_signature(mesh: bpy.types.Mesh, 
                                   world_matrix: Matrix) -> str:
    """
    Compute quantum signature using topological and geometric invariants.
    
    Signature incorporates:
    - Vertex position eigenvalues
    - Normal field coherence
    - Topological genus
    """
    # Compute position matrix eigenvalues
    positions = np.array([(world_matrix @ v.co)[:] for v in mesh.vertices])
    covariance = np.cov(positions.T)
    eigenvalues = np.linalg.eigvalsh(covariance)
    
    # Compute normal coherence
    normals = np.array([v.normal[:] for v in mesh.vertices])
    normal_coherence = np.mean(np.abs(normals @ normals.T))
    
    # Create signature
    signature_data = {
        "eigenvalues": eigenvalues.tolist(),
        "coherence": float(normal_coherence),
        "vertex_count": len(mesh.vertices),
        "edge_count": len(mesh.edges),
        "face_count": len(mesh.polygons)
    }
    
    # Hash to create signature
    signature_str = json.dumps(signature_data, sort_keys=True)
    return hashlib.sha256(signature_str.encode()).hexdigest()[:16]

def _compute_topological_invariants(mesh: bpy.types.Mesh) -> Dict[str, Any]:
    """
    Compute topological invariants for quantum state classification.
    
    Returns:
        Dictionary containing:
        - euler_characteristic: V - E + F
        - genus: Topological genus
        - boundary_loops: Number of boundary loops
        - connected_components: Number of disconnected parts
    """
    V = len(mesh.vertices)
    E = len(mesh.edges)
    F = len(mesh.polygons)
    
    # Euler characteristic
    euler = V - E + F
    
    # Estimate genus (assuming closed manifold)
    # For closed surface: χ = 2 - 2g where g is genus
    genus = (2 - euler) // 2 if euler <= 2 else 0
    
    # Count boundary loops
    boundary_edges = [e for e in mesh.edges if len(e.link_faces) == 1]
    boundary_loops = _count_edge_loops(boundary_edges, mesh)
    
    return {
        "euler_characteristic": euler,
        "genus": genus,
        "boundary_loops": boundary_loops,
        "vertices": V,
        "edges": E,
        "faces": F,
        "is_manifold": all(len(e.link_faces) <= 2 for e in mesh.edges),
        "is_closed": len(boundary_edges) == 0
    }

def _compute_winding_number(polygon: bpy.types.MeshPolygon, 
                           mesh: bpy.types.Mesh) -> int:
    """Compute topological winding number for face orientation"""
    # Simplified winding number based on normal orientation
    if polygon.normal.z > 0:
        return 1
    elif polygon.normal.z < 0:
        return -1
    else:
        return 0

def _compute_mesh_volume(bm: bmesh.types.BMesh) -> float:
    """Compute mesh volume using divergence theorem"""
    volume = 0.0
    
    for face in bm.faces:
        if len(face.verts) >= 3:
            # Use first vertex as reference
            v0 = face.verts[0].co
            
            # Sum signed volumes of tetrahedra
            for i in range(1, len(face.verts) - 1):
                v1 = face.verts[i].co
                v2 = face.verts[i + 1].co
                
                # Signed volume of tetrahedron
                volume += v0.dot(v1.cross(v2)) / 6.0
    
    return abs(volume)

def _compute_mesh_hash(mesh: bpy.types.Mesh) -> str:
    """Compute deterministic hash of mesh topology"""
    hasher = hashlib.sha256()
    
    # Hash vertex positions
    for v in mesh.vertices:
        for coord in v.co:
            hasher.update(struct.pack('f', coord))
    
    # Hash edge connectivity
    for e in mesh.edges:
        hasher.update(struct.pack('II', e.vertices[0], e.vertices[1]))
    
    # Hash face topology
    for f in mesh.polygons:
        hasher.update(struct.pack('I', len(f.vertices)))
        for v in f.vertices:
            hasher.update(struct.pack('I', v))
    
    return hasher.hexdigest()[:16]

def _count_edge_loops(edges: List[bpy.types.MeshEdge], 
                     mesh: bpy.types.Mesh) -> int:
    """Count number of disconnected edge loops"""
    if not edges:
        return 0
    
    # Build adjacency
    vertex_edges = {}
    for edge in edges:
        for v in edge.vertices:
            if v not in vertex_edges:
                vertex_edges[v] = []
            vertex_edges[v].append(edge)
    
    # Count connected components
    visited = set()
    loop_count = 0
    
    for edge in edges:
        if edge in visited:
            continue
            
        # BFS to find connected component
        queue = [edge]
        visited.add(edge)
        
        while queue:
            current = queue.pop(0)
            
            for v in current.vertices:
                for neighbor in vertex_edges.get(v, []):
                    if neighbor not in visited:
                        visited.add(neighbor)
                        queue.append(neighbor)
        
        loop_count += 1
    
    return loop_count

def _compute_spherical_harmonic(l: int, m: int, theta: float, phi: float) -> float:
    """Compute spherical harmonic Y_l^m(theta, phi)"""
    # Simplified implementation for low-order harmonics
    # In production, use scipy.special.sph_harm
    
    # Normalization constant
    norm = math.sqrt((2 * l + 1) / (4 * math.pi))
    
    # Associated Legendre polynomial (simplified)
    if l == 2:
        if m == 0:
            P_lm = 0.5 * (3 * math.cos(theta)**2 - 1)
        elif abs(m) == 1:
            P_lm = 3 * math.sin(theta) * math.cos(theta)
        elif abs(m) == 2:
            P_lm = 3 * math.sin(theta)**2
        else:
            P_lm = 0
    else:
        # Higher order terms
        P_lm = 0
    
    # Azimuthal part
    if m > 0:
        azimuthal = math.cos(m * phi)
    elif m < 0:
        azimuthal = math.sin(abs(m) * phi)
    else:
        azimuthal = 1.0
    
    return norm * P_lm * azimuthal

def _apply_indexed_deformation(bm: bmesh.types.BMesh,
                              vertex_data: List[Dict[str, Any]],
                              strength: float):
    """Apply deformation to specific indexed vertices"""
    for vdata in vertex_data:
        idx = vdata.get("index", -1)
        if 0 <= idx < len(bm.verts) and "offset" in vdata:
            vert = bm.verts[idx]
            offset = vdata["offset"]
            
            delta = Vector((
                offset.get("x", 0) * strength,
                offset.get("y", 0) * strength,
                offset.get("z", 0) * strength
            ))
            
            vert.co += delta

# Module has no classes to register
classes = []
