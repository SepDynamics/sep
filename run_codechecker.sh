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
mkdir -p .codechecker/reports .codechecker/html .codechecker/output
chmod -R 777 .codechecker

echo "Running CodeChecker analysis..."

# Run the analysis in the Docker container to ensure consistent environment
docker run --rm \
    -v $(pwd):/sep \
    -v $(pwd)/.codechecker:/home/codecheck/.codechecker \
    -e USER_ID=$USER_ID \
    -e GROUP_ID=$GROUP_ID \
    sep-engine-builder bash -c '
        # Run as root and fix permissions later
        cd /sep
        # Set clang-tidy path explicitly
        export PATH="/usr/bin:$PATH"
        which clang-tidy
        
        CodeChecker analyze compile_commands.json \
            --output /home/codecheck/.codechecker/reports \
            --analyzers clang-tidy \
            --skip /sep/.codechecker_skip \
            --verbose debug

        CodeChecker parse /home/codecheck/.codechecker/reports \
            --export html \
            --output /home/codecheck/.codechecker/html
        
        # Fix permissions
        chown -R $USER_ID:$GROUP_ID /home/codecheck/.codechecker
    '

echo "CodeChecker analysis complete. View the report at .codechecker/html/index.html"

# Auto-update sepdynamics.com website
echo "Updating sepdynamics.com website..."

# Generate current project status
generate_status_update() {
    cat > .codechecker/project_status.json << EOF
{
    "last_updated": "$(date -u +"%Y-%m-%dT%H:%M:%SZ")",
    "repository": "https://github.com/SepDynamics/sep",
    "branch": "$(git branch --show-current 2>/dev/null || echo 'main')",
    "latest_commit": "$(git log -1 --format='%H %s' 2>/dev/null || echo 'No git info')",
    "build_status": "$([ -f build/examples/pattern_metric_example ] && echo 'success' || echo 'pending')",
    "codechecker_run": "$(date -u +"%Y-%m-%dT%H:%M:%SZ")",
    "analysis_files": [
        {
            "report": ".codechecker/html/index.html",
            "type": "CodeChecker HTML Report"
        }
    ]
}
EOF
}

# Deploy to GitHub Pages (assumes sepdynamics.com points to GitHub Pages)
deploy_website() {
    # Generate status update
    generate_status_update
    
    # Copy index.html and related files to .codechecker for deployment
    cp index.html .codechecker/
    cp style.css .codechecker/ 2>/dev/null || true
    cp main.js .codechecker/ 2>/dev/null || true
    cp logo.png .codechecker/ 2>/dev/null || true
    cp concepts.json .codechecker/ 2>/dev/null || true
    
    # Create a comprehensive status page
    cat > .codechecker/status.html << 'EOF'
<!DOCTYPE html>
<html>
<head>
    <title>SEP Dynamics - Live Project Status</title>
    <meta http-equiv="refresh" content="300">
    <style>
        body { font-family: Arial, sans-serif; margin: 20px; background: #1a1a1a; color: #fff; }
        .status-box { background: #2d2d2d; padding: 20px; margin: 10px 0; border-radius: 8px; }
        .success { border-left: 4px solid #4CAF50; }
        .pending { border-left: 4px solid #FF9800; }
        .error { border-left: 4px solid #f44336; }
        a { color: #64B5F6; }
        .timestamp { color: #888; font-size: 0.9em; }
    </style>
</head>
<body>
    <h1>SEP Dynamics - Live Project Status</h1>
    <div class="status-box success">
        <h3>🔍 Latest CodeChecker Analysis</h3>
        <p><strong>Run Time:</strong> <span class="timestamp" id="analysis-time"></span></p>
        <p><a href="html/index.html">View Full Analysis Report →</a></p>
    </div>
    <div class="status-box">
        <h3>📊 Project Overview</h3>
        <p><strong>Repository:</strong> <a href="https://github.com/SepDynamics/sep">github.com/SepDynamics/sep</a></p>
        <p><strong>Current Branch:</strong> <span id="branch"></span></p>
        <p><strong>Latest Commit:</strong> <span id="commit"></span></p>
        <p><strong>Build Status:</strong> <span id="build-status"></span></p>
    </div>
    <div class="status-box">
        <h3>🚀 Quick Links</h3>
        <p><a href="index.html">Project Concepts Dashboard →</a></p>
        <p><a href="html/index.html">CodeChecker Analysis →</a></p>
    </div>
    
    <script>
        fetch('project_status.json')
            .then(response => response.json())
            .then(data => {
                document.getElementById('analysis-time').textContent = new Date(data.codechecker_run).toLocaleString();
                document.getElementById('branch').textContent = data.branch;
                document.getElementById('commit').textContent = data.latest_commit;
                document.getElementById('build-status').textContent = data.build_status;
            })
            .catch(() => console.log('Status data not available'));
    </script>
</body>
</html>
EOF
    
    # If this is a git repo, try to push to gh-pages branch
    if [ -d ".git" ] && command -v git &> /dev/null; then
        echo "Committing analysis results to Git..."
        git add .codechecker/ 2>/dev/null || true
        git commit -m "Update CodeChecker analysis - $(date)" 2>/dev/null || true
        
        # Try to push to gh-pages if it exists
        if git show-ref --verify --quiet refs/heads/gh-pages; then
            echo "Pushing to gh-pages branch..."
            git subtree push --prefix=.codechecker origin gh-pages 2>/dev/null || echo "Note: gh-pages push failed - may need manual setup"
        fi
    fi
}

# Run the deployment
deploy_website

echo "Website update complete. Check sepdynamics.com for live status."