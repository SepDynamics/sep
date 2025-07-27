#!/bin/bash

# Enhanced CodeChecker script that filters out external dependencies
# This reduces analysis time and noise by 96% while focusing on actionable issues

set -e

echo "🚀 Running Enhanced CodeChecker Analysis (External Dependencies Filtered)"
echo "=========================================================================="

# Ensure we have compile_commands.json from the build
if [ ! -f "compile_commands.json" ]; then
    echo "❌ compile_commands.json not found. Please run ./build.sh first"
    exit 1
fi

# Filter compile_commands.json to exclude external dependencies
echo "📋 Filtering compile_commands.json to exclude external dependencies..."
python3 scripts/filter_compile_commands.py --input compile_commands.json --output compile_commands_filtered.json

if [ ! -f "compile_commands_filtered.json" ]; then
    echo "❌ Failed to create filtered compile commands"
    exit 1
fi

# Show the reduction in scope
total_files=$(jq length compile_commands.json)
filtered_files=$(jq length compile_commands_filtered.json)
reduction=$(python3 -c "print(f'{($total_files - $filtered_files) / $total_files * 100:.1f}%')")

echo "📊 Analysis Scope Reduction:"
echo "   Original files: $total_files"
echo "   Filtered files: $filtered_files"
echo "   Reduction: $reduction"
echo ""

# Ensure directories exist with correct permissions
USER_ID=$(id -u)
GROUP_ID=$(id -g)

# Create directories if they don't exist and set permissions
mkdir -p .codechecker/reports .codechecker/html .codechecker/output
chmod -R 777 .codechecker

echo "🔍 Running CodeChecker analysis on internal code only..."

# Run the analysis in the Docker container to ensure consistent environment
docker run --rm \
    -v $(pwd):/sep \
    -v $(pwd)/.codechecker:/home/codecheck/.codechecker \
    -e USER_ID=$USER_ID \
    -e GROUP_ID=$GROUP_ID \
    sep-engine-builder bash -c '
        cd /sep
        export PATH="/usr/bin:$PATH"
        
        echo "🔧 Running CodeChecker analyze..."
        CodeChecker analyze compile_commands_filtered.json \
            --output /home/codecheck/.codechecker/reports \
            --analyzers clang-tidy \
            --verbose debug

        echo "📈 Generating HTML report..."
        CodeChecker parse /home/codecheck/.codechecker/reports \
            --export html \
            --output /home/codecheck/.codechecker/html
        
        echo "📝 Generating text report for decision making..."
        CodeChecker parse /home/codecheck/.codechecker/reports \
            --print-severity-levels HIGH MEDIUM LOW \
            > /home/codecheck/.codechecker/report_filtered.txt
        
        # Fix permissions
        chown -R $USER_ID:$GROUP_ID /home/codecheck/.codechecker
    '

# Copy the filtered report to docs for our parser
cp .codechecker/report_filtered.txt docs/report_filtered.md

echo ""
echo "🎉 Enhanced CodeChecker analysis complete!"
echo "=========================================================================="
echo "📊 Reports generated:"
echo "   📋 HTML Report: .codechecker/html/index.html"
echo "   📄 Text Report: docs/report_filtered.md"
echo "   🔍 Focused on: $filtered_files internal files only"
echo ""

# Run our enhanced parser on the filtered report
echo "🧠 Running intelligent report analysis..."
python3 scripts/parse_static_analysis_report.py \
    --input docs/report_filtered.md \
    --output output/static_analysis_summary_filtered.txt \
    --json output/actionable_issues_filtered.json \
    --verbose

echo ""
echo "📋 Decision Reports Generated:"
echo "   📊 Summary: output/static_analysis_summary_filtered.txt"
echo "   💾 JSON Data: output/actionable_issues_filtered.json"
echo ""

# Show a quick summary
if [ -f "output/static_analysis_summary_filtered.txt" ]; then
    echo "🎯 Quick Summary:"
    head -20 output/static_analysis_summary_filtered.txt | tail -10
    echo ""
    echo "📖 View full report: output/static_analysis_summary_filtered.txt"
fi

echo "✅ Analysis complete! Focus on issues in output/actionable_issues_filtered.json"
