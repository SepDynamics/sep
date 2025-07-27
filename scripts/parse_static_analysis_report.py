#!/usr/bin/env python3
"""
Static Analysis Report Parser and Deduplicator

Parses CodeChecker static analysis reports from docs/report.md,
removes redundancy, and generates actionable decision reports.

Author: Amp
"""
import re
import json
from collections import defaultdict, Counter
from dataclasses import dataclass
from typing import List, Dict, Set
import argparse

@dataclass
class StaticAnalysisIssue:
    """Represents a single static analysis issue."""
    severity: str
    file_path: str
    line_number: int
    column: int
    message: str
    rule_id: str
    report_hash: str
    steps: List[str]
    notes: List[str] = None
    
    def __post_init__(self):
        if self.notes is None:
            self.notes = []

class StaticAnalysisParser:
    """Parser for CodeChecker static analysis reports."""
    
    def __init__(self, report_file: str = "/sep/docs/report.md"):
        self.report_file = report_file
        self.issues: List[StaticAnalysisIssue] = []
        self.pattern_counts: Dict[str, int] = defaultdict(int)
        
    def parse_report(self) -> List[StaticAnalysisIssue]:
        """Parse the static analysis report file."""
        with open(self.report_file, 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Split into individual issue blocks
        issue_blocks = self._extract_issue_blocks(content)
        
        for block in issue_blocks:
            issue = self._parse_issue_block(block)
            if issue:
                self.issues.append(issue)
                # Track pattern frequency
                self.pattern_counts[issue.rule_id] += 1
        
        return self.issues
    
    def _extract_issue_blocks(self, content: str) -> List[str]:
        """Extract individual issue blocks from the report."""
        # Pattern to match issue blocks starting with [SEVERITY]
        pattern = r'\[(?:HIGH|MEDIUM|LOW|STYLE)\].*?(?=\n\[(?:HIGH|MEDIUM|LOW|STYLE)\]|\nFound \d+ defect|\Z)'
        blocks = re.findall(pattern, content, re.DOTALL)
        return blocks
    
    def _parse_issue_block(self, block: str) -> StaticAnalysisIssue:
        """Parse a single issue block into a StaticAnalysisIssue object."""
        lines = block.strip().split('\n')
        if not lines:
            return None
            
        # Parse the first line: [SEVERITY] /path/file:line:col: message [rule-id]
        first_line = lines[0]
        severity_match = re.match(r'\[(\w+)\]', first_line)
        if not severity_match:
            return None
            
        severity = severity_match.group(1)
        
        # Extract file path, line, column, message, and rule
        main_pattern = r'\[(\w+)\] ([^:]+):(\d+):(\d+): (.*?) \[([^\]]+)\]'
        match = re.match(main_pattern, first_line)
        
        if not match:
            return None
            
        _, file_path, line_num, column, message, rule_id = match.groups()
        
        # Extract report hash
        report_hash = ""
        hash_pattern = r'Report hash: ([a-f0-9]+)'
        hash_match = re.search(hash_pattern, block)
        if hash_match:
            report_hash = hash_match.group(1)
        
        # Extract steps
        steps = []
        step_pattern = r'(\d+), ([^:]+):(\d+):(\d+): (.+)'
        for line in lines:
            step_match = re.match(r'\s*' + step_pattern, line)
            if step_match:
                steps.append(line.strip())
        
        # Extract notes (fixits)
        notes = []
        note_pattern = r'(\d+), ([^:]+):(\d+):(\d+): (.+) \(fixit\)'
        for line in lines:
            note_match = re.match(r'\s*' + note_pattern, line)
            if note_match:
                notes.append(line.strip())
        
        return StaticAnalysisIssue(
            severity=severity,
            file_path=file_path,
            line_number=int(line_num),
            column=int(column),
            message=message,
            rule_id=rule_id,
            report_hash=report_hash,
            steps=steps,
            notes=notes
        )

class ReportAnalyzer:
    """Analyzes and deduplicates static analysis issues."""
    
    def __init__(self, issues: List[StaticAnalysisIssue]):
        self.issues = issues
        
    def categorize_by_severity(self) -> Dict[str, List[StaticAnalysisIssue]]:
        """Group issues by severity level."""
        categories = defaultdict(list)
        for issue in self.issues:
            categories[issue.severity].append(issue)
        return dict(categories)
    
    def categorize_by_rule(self) -> Dict[str, List[StaticAnalysisIssue]]:
        """Group issues by rule ID."""
        categories = defaultdict(list)
        for issue in self.issues:
            categories[issue.rule_id].append(issue)
        return dict(categories)
    
    def categorize_by_file_prefix(self) -> Dict[str, List[StaticAnalysisIssue]]:
        """Group issues by file prefix (internal vs external dependencies)."""
        categories = {
            'External Dependencies': [],
            'Internal Code': []
        }
        
        external_prefixes = [
            '/sep/build/_deps/',
            '/sep/extern/',
            '/sep/third_party/'
        ]
        
        for issue in self.issues:
            is_external = any(issue.file_path.startswith(prefix) for prefix in external_prefixes)
            if is_external:
                categories['External Dependencies'].append(issue)
            else:
                categories['Internal Code'].append(issue)
        
        return categories
    
    def get_top_rule_violations(self, n: int = 10) -> List[tuple]:
        """Get the top N most frequent rule violations."""
        rule_counts = Counter(issue.rule_id for issue in self.issues)
        return rule_counts.most_common(n)
    
    def deduplicate_by_hash(self) -> List[StaticAnalysisIssue]:
        """Remove duplicate issues based on report hash."""
        seen_hashes = set()
        unique_issues = []
        
        for issue in self.issues:
            if issue.report_hash and issue.report_hash not in seen_hashes:
                unique_issues.append(issue)
                seen_hashes.add(issue.report_hash)
            elif not issue.report_hash:
                # Keep issues without hashes
                unique_issues.append(issue)
        
        return unique_issues
    
    def filter_actionable_issues(self) -> List[StaticAnalysisIssue]:
        """Filter for issues that are actionable (not in external dependencies)."""
        external_prefixes = [
            '/sep/build/_deps/',
            '/sep/extern/',
            '/sep/third_party/'
        ]
        
        actionable = []
        for issue in self.issues:
            is_external = any(issue.file_path.startswith(prefix) for prefix in external_prefixes)
            if not is_external:
                actionable.append(issue)
        
        return actionable

class DecisionReportGenerator:
    """Generates decision-focused reports from analyzed issues."""
    
    def __init__(self, analyzer: ReportAnalyzer):
        self.analyzer = analyzer
        
    def generate_summary_report(self) -> str:
        """Generate a summary report for decision making."""
        total_issues = len(self.analyzer.issues)
        severity_breakdown = self.analyzer.categorize_by_severity()
        file_breakdown = self.analyzer.categorize_by_file_prefix()
        top_violations = self.analyzer.get_top_rule_violations(10)
        actionable_issues = self.analyzer.filter_actionable_issues()
        
        report = []
        report.append("# Static Analysis Decision Report")
        report.append("=" * 50)
        report.append("")
        
        # Executive Summary
        report.append("## Executive Summary")
        report.append(f"- **Total Issues Found:** {total_issues}")
        report.append(f"- **Actionable Issues (Internal Code):** {len(actionable_issues)}")
        report.append(f"- **External Dependency Issues:** {len(file_breakdown.get('External Dependencies', []))}")
        report.append("")
        
        # Severity Breakdown
        report.append("## Severity Breakdown")
        for severity in ['HIGH', 'MEDIUM', 'LOW', 'STYLE']:
            count = len(severity_breakdown.get(severity, []))
            percentage = (count / total_issues * 100) if total_issues > 0 else 0
            report.append(f"- **{severity}:** {count} issues ({percentage:.1f}%)")
        report.append("")
        
        # Top Rule Violations
        report.append("## Top 10 Most Frequent Rule Violations")
        for i, (rule_id, count) in enumerate(top_violations, 1):
            percentage = (count / total_issues * 100) if total_issues > 0 else 0
            report.append(f"{i:2d}. **{rule_id}:** {count} occurrences ({percentage:.1f}%)")
        report.append("")
        
        # Priority Actions
        report.append("## Recommended Priority Actions")
        
        # High severity actionable issues
        high_severity_actionable = [i for i in actionable_issues if i.severity == 'HIGH']
        if high_severity_actionable:
            report.append(f"### 1. Address {len(high_severity_actionable)} HIGH severity issues in internal code")
            rule_breakdown = Counter(i.rule_id for i in high_severity_actionable)
            for rule_id, count in rule_breakdown.most_common(5):
                report.append(f"   - {rule_id}: {count} occurrences")
        
        # Medium severity with high frequency
        medium_actionable = [i for i in actionable_issues if i.severity == 'MEDIUM']
        if medium_actionable:
            report.append(f"### 2. Review {len(medium_actionable)} MEDIUM severity issues")
            rule_breakdown = Counter(i.rule_id for i in medium_actionable)
            for rule_id, count in rule_breakdown.most_common(3):
                report.append(f"   - {rule_id}: {count} occurrences")
        
        # External dependency decision
        external_count = len(file_breakdown.get('External Dependencies', []))
        if external_count > 0:
            report.append(f"### 3. External Dependencies ({external_count} issues)")
            report.append("   - Consider whether to suppress or address upstream")
            report.append("   - Most external issues are in: imgui, implot, yaml-cpp, spdlog, tbb")
        
        report.append("")
        return "\n".join(report)
    
    def generate_actionable_json(self) -> str:
        """Generate JSON report of actionable issues only."""
        actionable_issues = self.analyzer.filter_actionable_issues()
        
        # Group by file for easier fixing
        by_file = defaultdict(list)
        for issue in actionable_issues:
            by_file[issue.file_path].append({
                'severity': issue.severity,
                'line': issue.line_number,
                'column': issue.column,
                'message': issue.message,
                'rule_id': issue.rule_id,
                'report_hash': issue.report_hash,
                'steps': issue.steps,
                'notes': issue.notes
            })
        
        report_data = {
            'summary': {
                'total_actionable_issues': len(actionable_issues),
                'files_affected': len(by_file),
                'severity_breakdown': {
                    severity: len([i for i in actionable_issues if i.severity == severity])
                    for severity in ['HIGH', 'MEDIUM', 'LOW', 'STYLE']
                }
            },
            'issues_by_file': dict(by_file)
        }
        
        return json.dumps(report_data, indent=2)

def main():
    parser = argparse.ArgumentParser(description='Parse and analyze static analysis reports')
    parser.add_argument('--input', '-i', default='/sep/docs/report.md',
                       help='Input report file (default: /sep/docs/report.md)')
    parser.add_argument('--output', '-o', default='/sep/output/static_analysis_summary.txt',
                       help='Output summary file (default: /sep/output/static_analysis_summary.txt)')
    parser.add_argument('--json', '-j', default='/sep/output/actionable_issues.json',
                       help='Output JSON file for actionable issues (default: /sep/output/actionable_issues.json)')
    parser.add_argument('--verbose', '-v', action='store_true',
                       help='Verbose output')
    
    args = parser.parse_args()
    
    # Parse the report
    if args.verbose:
        print(f"Parsing static analysis report from {args.input}...")
    
    parser_obj = StaticAnalysisParser(args.input)
    issues = parser_obj.parse_report()
    
    if args.verbose:
        print(f"Parsed {len(issues)} issues")
    
    # Analyze issues
    analyzer = ReportAnalyzer(issues)
    
    # Generate reports
    report_generator = DecisionReportGenerator(analyzer)
    
    # Summary report
    summary_report = report_generator.generate_summary_report()
    with open(args.output, 'w') as f:
        f.write(summary_report)
    
    # JSON report
    json_report = report_generator.generate_actionable_json()
    with open(args.json, 'w') as f:
        f.write(json_report)
    
    if args.verbose:
        print(f"Reports generated:")
        print(f"  Summary: {args.output}")
        print(f"  JSON: {args.json}")
    
    # Print summary to console
    print(summary_report)

if __name__ == '__main__':
    main()
