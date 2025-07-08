import argparse
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

def run_script(script_path, install_cuda=False, minimal=False):
    env = os.environ.copy()
    env["INSTALL_CUDA"] = "1" if install_cuda else "0"
    env["INSTALL_MINIMAL"] = "1" if minimal else "0"
    try:
        subprocess.check_call(["bash", script_path], env=env)
        return True
    except Exception as e:
        print(f"Error running {script_path}: {e}")
        return False

# Required Python packages
packages = ["requests", "numpy"]

# Install pip if needed
def main():
    parser = argparse.ArgumentParser(description="Install SEP Engine dependencies")
    group = parser.add_mutually_exclusive_group()
    group.add_argument("--with-cuda", action="store_true", help="Install CUDA toolkit")
    group.add_argument("--no-cuda", action="store_true", help="Skip CUDA installation (default)")
    parser.add_argument("--minimal", action="store_true", help="Install only minimal dependencies")
    args = parser.parse_args()

    if install_pip():
        for package in packages:
            install_package(package)

    script = os.path.join(os.path.dirname(__file__), "scripts", "install_dependencies.sh")
    install_cuda = args.with_cuda and not args.no_cuda
    if not run_script(script, install_cuda=install_cuda, minimal=args.minimal):
        sys.exit(1)

    print("\nDependency installation complete. You can now try enabling the SEP Engine addon.")
    print("Python executable path:", sys.executable)
    print("Python version:", sys.version)

if __name__ == "__main__":
    main()
