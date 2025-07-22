#!/bin/bash

# This script runs CodeChecker for static analysis using the existing build system
# It should be run from the root of the project after running build.sh

# Ensure we have compile_commands.json from the build
if [ ! -f "compile_commands.json" ]; then
    echo "compile_commands.json not found. Please run ./build.sh first"
    exit 1
fi

# Ensure directories exist with correct permissions
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Create directories if they don't exist and set permissions
for dir in .codechecker/{reports,html,output}; do
    mkdir -p "$dir"
    chmod -R 777 "$dir"
done

echo "Running CodeChecker analysis..."

# Run the analysis in the Docker container to ensure consistent environment
docker run --rm \
    -v $(pwd):/sep \
    -v $(pwd)/.codechecker:/home/codecheck/.codechecker \
    -e USER_ID=$USER_ID \
    -e GROUP_ID=$GROUP_ID \
    sep-engine-builder bash -c '
        # Switch to codecheck user
        sudo -u codecheck bash -c "
            cd /sep
            CodeChecker analyze compile_commands.json \
                --output /home/codecheck/.codechecker/reports \
                --enable sensitive \
                --enable extreme

            CodeChecker parse /home/codecheck/.codechecker/reports \
                --export html \
                --output /home/codecheck/.codechecker/html
        "
        
        # Fix permissions as root
        sudo chown -R $USER_ID:$GROUP_ID /home/codecheck/.codechecker
    '

echo "CodeChecker analysis complete. View the report at .codechecker/html/index.html"