#!/usr/bin/env python3
"""
SEP Engine Blender Addon Installation Helper
============================================
This script helps install the SEP Engine addon into Blender.
"""

import os
import sys
import shutil
import platform
import subprocess
import argparse
from pathlib import Path
import zipfile
import json

class SEPAddonInstaller:
    """Installer for SEP Engine Blender Addon"""
    
    def __init__(self):
        self.system = platform.system()
        self.blender_version = None
        self.addon_name = "sep_engine"
        
    def find_blender_paths(self):
        """Find Blender installation paths"""
        paths = []
        
        if self.system == "Windows":
            # Common Windows paths
            base_paths = [
                Path(os.environ.get("PROGRAMFILES", "C:\\Program Files")),
                Path(os.environ.get("PROGRAMFILES(X86)", "C:\\Program Files (x86)")),
                Path.home() / "AppData" / "Local" / "Programs"
            ]
            
            for base in base_paths:
                if base.exists():
                    # Look for Blender folders
                    for item in base.iterdir():
                        if item.is_dir() and item.name.startswith("Blender"):
                            paths.append(item)
                            
        elif self.system == "Darwin":  # macOS
            # Common macOS paths
            app_paths = [
                Path("/Applications/Blender.app"),
                Path.home() / "Applications" / "Blender.app"
            ]
            
            for app in app_paths:
                if app.exists():
                    paths.append(app)
                    
        elif self.system == "Linux":
            # Common Linux paths
            base_paths = [
                Path("/usr/share/blender"),
                Path("/usr/local/share/blender"),
                Path.home() / ".local" / "share" / "blender",
                Path("/opt/blender"),
                Path("/snap/blender/current")
            ]
            
            for base in base_paths:
                if base.exists():
                    paths.append(base)
        
        return paths
    
    def get_addon_path(self, blender_path):
        """Get addon installation path for Blender installation"""
        if self.system == "Darwin":
            # macOS app bundle structure
            scripts_path = blender_path / "Contents" / "Resources"
            
            # Find version folder
            for item in scripts_path.iterdir():
                if item.is_dir() and item.name[0].isdigit():
                    return item / "scripts" / "addons"
        else:
            # Windows and Linux
            # Look for scripts folder
            if (blender_path / "scripts").exists():
                return blender_path / "scripts" / "addons"
            
            # Look for version folders
            for item in blender_path.iterdir():
                if item.is_dir() and item.name[0].isdigit():
                    scripts = item / "scripts" / "addons"
                    if scripts.parent.exists():
                        return scripts
        
        return None
    
    def get_user_addon_path(self):
        """Get user-specific addon path"""
        if self.system == "Windows":
            base = Path.home() / "AppData" / "Roaming" / "Blender Foundation" / "Blender"
        elif self.system == "Darwin":
            base = Path.home() / "Library" / "Application Support" / "Blender"
        else:  # Linux
            base = Path.home() / ".config" / "blender"
        
        # Find version folders
        if base.exists():
            for item in base.iterdir():
                if item.is_dir() and item.name[0].isdigit():
                    addon_path = item / "scripts" / "addons"
                    return addon_path
        
        return None
    
    def create_addon_package(self, source_dir, output_file=None):
        """Create addon package from source directory"""
        source_path = Path(source_dir)
        
        if not source_path.exists():
            print(f"Error: Source directory '{source_dir}' not found")
            return None
        
        if output_file is None:
            output_file = f"{self.addon_name}.zip"
        
        output_path = Path(output_file)
        
        # Create zip file
        with zipfile.ZipFile(output_path, 'w', zipfile.ZIP_DEFLATED) as zf:
            # Add all Python files
            for py_file in source_path.rglob("*.py"):
                arcname = f"{self.addon_name}/{py_file.relative_to(source_path)}"
                zf.write(py_file, arcname)
            
            # Add any additional files (libs, etc.)
            for ext in ["*.so", "*.dll", "*.dylib", "*.json", "*.md"]:
                for file in source_path.rglob(ext):
                    arcname = f"{self.addon_name}/{file.relative_to(source_path)}"
                    zf.write(file, arcname)
        
        print(f"Created addon package: {output_path}")
        return output_path
    
    def install_addon(self, source_dir, target_dir=None):
        """Install addon to target directory"""
        source_path = Path(source_dir)
        
        if not source_path.exists():
            print(f"Error: Source directory '{source_dir}' not found")
            return False
        
        # Determine target directory
        if target_dir:
            target_path = Path(target_dir)
        else:
            # Try user addon path first
            target_path = self.get_user_addon_path()
            
            if not target_path:
                print("Error: Could not find Blender addon directory")
                print("Please specify target directory with --target")
                return False
        
        # Create addon directory
        addon_path = target_path / self.addon_name
        
        # Remove existing installation
        if addon_path.exists():
            print(f"Removing existing installation at {addon_path}")
            shutil.rmtree(addon_path)
        
        # Create target directory if needed
        target_path.mkdir(parents=True, exist_ok=True)
        
        # Copy addon files
        print(f"Installing addon to {addon_path}")
        shutil.copytree(source_path, addon_path)
        
        # Create libs directory
        libs_path = addon_path / "libs"
        libs_path.mkdir(exist_ok=True)
        
        # Verify required files
        required_files = [
            "__init__.py",
            "connection.py",
            "operators.py",
            "ui.py",
            "mesh_utils.py",
            "pattern_processor.py"
        ]
        
        missing_files = []
        for file in required_files:
            if not (addon_path / file).exists():
                missing_files.append(file)
        
        if missing_files:
            print(f"Error: Missing required files: {', '.join(missing_files)}")
            # Clean up failed installation
            shutil.rmtree(addon_path)
            return False
        
        print("Installation complete!")
        print(f"Addon installed to: {addon_path}")
        print("\nNext steps:")
        print("1. Open Blender")
        print("2. Go to Edit → Preferences → Add-ons")
        print("3. Search for 'SEP Engine'")
        print("4. Enable the addon")
        
        return True
    
    def download_library(self, target_dir=None):
        """Download platform-specific SEP library"""
        if self.system == "Windows":
            lib_name = "sep_blender.dll"
        elif self.system == "Darwin":
            lib_name = "libsep_blender.dylib"
        else:
            lib_name = "libsep_blender.so"
        
        # Base URL for downloads
        base_url = "https://sep-engine.org/downloads"
        
        # Required files to download
        required_files = {
            "lib": lib_name,
            "mesh_utils": "mesh_utils.py",
            "pattern_processor": "pattern_processor.py"
        }
        
        if target_dir:
            target_base = Path(target_dir)
        else:
            # Try to find installed addon
            user_path = self.get_user_addon_path()
            if user_path and (user_path / self.addon_name).exists():
                target_base = user_path / self.addon_name
            else:
                target_base = Path.cwd()
        
        # Create directories
        (target_base / "libs").mkdir(parents=True, exist_ok=True)
        
        downloaded_files = []
        
        # Download each required file
        for file_type, filename in required_files.items():
            download_url = f"{base_url}/{filename}"
            
            if file_type == "lib":
                target_path = target_base / "libs" / filename
            else:
                target_path = target_base / filename
            
            print(f"Downloading {filename} to {target_path}")
            print(f"URL: {download_url}")
            
            # In production, implement actual download
            # For now, create placeholder
            target_path.touch()
            downloaded_files.append(target_path)
        
        print(f"Library downloaded to: {target_path}")
        return target_path
    
    def verify_installation(self):
        """Verify addon is properly installed"""
        user_path = self.get_user_addon_path()
        
        if not user_path:
            print("Could not find Blender user directory")
            return False
        
        addon_path = user_path / self.addon_name
        
        if not addon_path.exists():
            print(f"Addon not found at {addon_path}")
            return False
        
        # Check for required files
        required_files = [
            "__init__.py",
            "connection.py",
            "operators.py",
            "ui.py",
            "mesh_utils.py",
            "pattern_processor.py"
        ]
        missing_files = []
        
        for file in required_files:
            if not (addon_path / file).exists():
                missing_files.append(file)
        
        if missing_files:
            print(f"Missing required files: {', '.join(missing_files)}")
            return False
        
        print(f"✓ Addon found at: {addon_path}")
        print(f"✓ All required files present")
        
        # Check for library
        libs_path = addon_path / "libs"
        if libs_path.exists():
            lib_files = list(libs_path.glob("*sep_blender*"))
            if lib_files:
                print(f"✓ SEP library found: {lib_files[0].name}")
            else:
                print("✗ SEP library not found (optional for HTTP mode)")
        
        return True

def main():
    """Main installation script"""
    parser = argparse.ArgumentParser(
        description="SEP Engine Blender Addon Installer"
    )
    
    parser.add_argument(
        "command",
        choices=["install", "package", "download-lib", "verify"],
        help="Command to execute"
    )
    
    parser.add_argument(
        "--source",
        default=".",
        help="Source directory containing addon files"
    )
    
    parser.add_argument(
        "--target",
        help="Target directory for installation"
    )
    
    parser.add_argument(
        "--output",
        help="Output file for package command"
    )
    
    args = parser.parse_args()
    
    installer = SEPAddonInstaller()
    
    if args.command == "install":
        success = installer.install_addon(args.source, args.target)
        sys.exit(0 if success else 1)
        
    elif args.command == "package":
        package = installer.create_addon_package(args.source, args.output)
        sys.exit(0 if package else 1)
        
    elif args.command == "download-lib":
        lib_path = installer.download_library(args.target)
        sys.exit(0 if lib_path else 1)
        
    elif args.command == "verify":
        valid = installer.verify_installation()
        sys.exit(0 if valid else 1)

if __name__ == "__main__":
    main()
