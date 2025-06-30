import subprocess
import sys
import os

def install_pip():
    try:
        import pip
        print("Pip is already installed")
    except ImportError:
        print("Installing pip...")
        try:
            # Get ensurepip module
            import ensurepip
            ensurepip._bootstrap()
            import pip
            print("Pip has been installed")
        except Exception as e:
            print(f"Error installing pip: {e}")
            return False
    return True

def install_package(package_name):
    print(f"Installing {package_name}...")
    try:
        subprocess.check_call([sys.executable, "-m", "pip", "install", package_name])
        print(f"Successfully installed {package_name}")
        return True
    except Exception as e:
        print(f"Error installing {package_name}: {e}")
        return False

# Required packages
packages = [
    "requests",
    "numpy"
]

# Install pip if needed
if install_pip():
    # Install required packages
    for package in packages:
        install_package(package)

print("\nDependency installation complete. You can now try enabling the SEP Engine addon.")
print("Python executable path:", sys.executable)
print("Python version:", sys.version)