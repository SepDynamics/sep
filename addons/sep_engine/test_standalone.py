#!/usr/bin/env python3

"""
Standalone test for SEP Engine Blender integration
This script doesn't require the SEP Engine to be running
"""

import os
import json
import sys
import math
import time
from dataclasses import dataclass
from typing import List, Dict, Any, Tuple

@dataclass
class Vec3:
    x: float
    y: float
    z: float
    
    def __iter__(self):
        yield self.x
        yield self.y
        yield self.z
        
    def to_dict(self):
        return {"x": self.x, "y": self.y, "z": self.z}


@dataclass
class Vertex:
    position: Vec3
    normal: Vec3
    
    def to_dict(self):
        return {
            "position": self.position.to_dict(),
            "normal": self.normal.to_dict()
        }


@dataclass
class Mesh:
    vertices: List[Vertex]
    name: str
    
    def to_dict(self):
        return {
            "name": self.name,
            "vertices": [v.to_dict() for v in self.vertices],
            "vertex_count": len(self.vertices)
        }


class PatternGenerator:
    """Generate quantum-like patterns for testing"""
    
    def __init__(self, seed=42):
        self.seed = seed
        self.time = 0.0
        
    def next(self):
        """Generate next pattern state"""
        self.time += 0.1
        
        pattern = {
            "coherence": 0.5 + 0.5 * math.sin(self.time),
            "stability": 0.5 + 0.4 * math.cos(self.time * 0.7),
            "entropy": 0.3 + 0.2 * math.sin(self.time * 1.3),
            "complexity": 0.6 + 0.3 * math.cos(self.time * 0.5)
        }
        
        return pattern
        
    def apply_to_mesh(self, mesh: Mesh, pattern: Dict[str, float]) -> Mesh:
        """Apply pattern to mesh vertices"""
        new_vertices = []
        coherence = pattern["coherence"]
        stability = pattern["stability"]
        
        for v in mesh.vertices:
            # Create ripple effect based on pattern
            distance = math.sqrt(v.position.x**2 + v.position.y**2 + v.position.z**2)
            factor = math.sin(distance * 3.0 + self.time * 2.0) * 0.1 * coherence
            
            # More stable patterns have less displacement
            factor *= (1.0 - stability) * 2.0
            
            # Create new vertex with displacement
            new_pos = Vec3(
                v.position.x + factor * v.normal.x,
                v.position.y + factor * v.normal.y,
                v.position.z + factor * v.normal.z
            )
            
            new_vertices.append(Vertex(new_pos, v.normal))
            
        return Mesh(new_vertices, mesh.name)


def create_test_mesh(size=10) -> Mesh:
    """Create a simple grid mesh for testing"""
    vertices = []
    
    for x in range(-size, size+1):
        for y in range(-size, size+1):
            # Normalize coordinates
            nx = x / size
            ny = y / size
            
            # Create position
            z = math.sin(nx * math.pi) * math.cos(ny * math.pi) * 0.5
            pos = Vec3(nx, ny, z)
            
            # Create normal (approximate)
            dx = math.cos(nx * math.pi) * math.cos(ny * math.pi) * 0.5 * math.pi / size
            dy = -math.sin(nx * math.pi) * math.sin(ny * math.pi) * 0.5 * math.pi / size
            normal = Vec3(-dx, -dy, 1.0)
            
            # Normalize normal
            length = math.sqrt(normal.x**2 + normal.y**2 + normal.z**2)
            normal = Vec3(normal.x/length, normal.y/length, normal.z/length)
            
            vertices.append(Vertex(pos, normal))
    
    return Mesh(vertices, "test_grid")


def save_mesh_obj(mesh: Mesh, filename: str):
    """Save mesh as OBJ file for visualization"""
    with open(filename, 'w') as f:
        f.write(f"# SEP Engine test mesh: {mesh.name}\n")
        f.write(f"# Vertices: {len(mesh.vertices)}\n\n")
        
        # Write vertices
        for i, v in enumerate(mesh.vertices):
            f.write(f"v {v.position.x} {v.position.y} {v.position.z}\n")
        
        # Write normals
        for v in mesh.vertices:
            f.write(f"vn {v.normal.x} {v.normal.y} {v.normal.z}\n")
        
        # For a grid mesh, we'd need to write faces, but this is just for
        # visualization of vertices, so we'll skip that


def simulate_quantum_processing(mesh: Mesh, iterations=10):
    """Simulate quantum processing on a mesh"""
    output_dir = "quantum_simulation"
    os.makedirs(output_dir, exist_ok=True)
    
    generator = PatternGenerator()
    
    print(f"Starting quantum simulation with {iterations} iterations")
    print(f"Input mesh: {mesh.name} with {len(mesh.vertices)} vertices")
    
    current_mesh = mesh
    
    for i in range(iterations):
        # Generate pattern
        pattern = generator.next()
        
        print(f"Iteration {i+1}/{iterations}")
        print(f"  Pattern: coherence={pattern['coherence']:.3f}, stability={pattern['stability']:.3f}")
        
        # Apply pattern to mesh
        current_mesh = generator.apply_to_mesh(current_mesh, pattern)
        
        # Save result
        output_file = os.path.join(output_dir, f"iter_{i+1:03d}.obj")
        save_mesh_obj(current_mesh, output_file)
        print(f"  Saved mesh to {output_file}")
        
        # Save pattern data
        pattern_file = os.path.join(output_dir, f"pattern_{i+1:03d}.json")
        with open(pattern_file, 'w') as f:
            json.dump(pattern, f, indent=2)
        
        time.sleep(0.1)  # Small delay for visualization
    
    print("Simulation complete!")
    return current_mesh


if __name__ == "__main__":
    print("SEP Engine Blender Integration Test")
    print("----------------------------------")
    
    # Create test mesh
    mesh = create_test_mesh(size=5)
    print(f"Created test mesh with {len(mesh.vertices)} vertices")
    
    # Save original mesh
    os.makedirs("quantum_simulation", exist_ok=True)
    save_mesh_obj(mesh, "quantum_simulation/original.obj")
    print("Saved original mesh")
    
    # Run simulation
    result_mesh = simulate_quantum_processing(mesh, iterations=20)
    
    print("\nTest completed successfully!")
    print(f"Result mesh has {len(result_mesh.vertices)} vertices")
    print("Mesh files saved in 'quantum_simulation' directory")
    print("You can import these .obj files into Blender to visualize the results")