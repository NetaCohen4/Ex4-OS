#!/bin/bash

# Automated Testing Script for Eulerian Circuit Project
# This script runs all tests and generates clean reports

echo "🚀 Starting Comprehensive Testing Suite"
echo "========================================"

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Check if required tools are installed
check_tools() {
    print_status "Checking required tools..."
    
    local tools=("gcc" "g++" "gcov" "lcov" "valgrind" "gprof")
    local missing_tools=()
    
    for tool in "${tools[@]}"; do
        if ! command -v "$tool" &> /dev/null; then
            missing_tools+=("$tool")
        fi
    done
    
    if [ ${#missing_tools[@]} -ne 0 ]; then
        print_warning "Missing tools: ${missing_tools[*]}"
        print_status "Installing missing tools..."
        make install-tools
    else
        print_success "All required tools are installed"
    fi
}

# Clean previous results
cleanup() {
    print_status "Cleaning previous test results..."
    make clean > /dev/null 2>&1
    print_success "Cleanup completed"
}

# Build all versions
build_all() {
    print_status "Building all versions..."
    
    if make coverage > /dev/null 2>&1; then
        print_success "Coverage version built"
    else
        print_error "Failed to build coverage version"
        return 1
    fi
    
    if make profile > /dev/null 2>&1; then
        print_success "Profile version built"
    else
        print_error "Failed to build profile version"
        return 1
    fi
    
    if make debug > /dev/null 2>&1; then
        print_success "Debug version built"
    else
        print_error "Failed to build debug version"
        return 1
    fi
    
    return 0
}

# Run code coverage test
test_coverage() {
    print_status "Running Code Coverage Analysis..."
    
    make test-coverage > /dev/null 2>&1
    if make generate-coverage > /dev/null 2>&1; then
        print_success "Code coverage analysis completed"
        if [ -f coverage_report.txt ]; then
            echo "Coverage Summary:"
            cat coverage_report.txt | grep -E "(Lines executed|Branches executed)" | sed 's/^/  /'
        fi
    else
        print_error "Code coverage analysis failed"
        return 1
    fi
    
    return 0
}

# Run performance profiling
test_profiling() {
    print_status "Running Performance Profiling..."
    
    make test-profile > /dev/null 2>&1
    if make generate-profile > /dev/null 2>&1; then
        print_success "Performance profiling completed"
        if [ -f gprof_summary.txt ]; then
            echo "Performance Summary:"
            head -15 gprof_summary.txt | tail -10 | sed 's/^/  /'
        fi
    else
        print_error "Performance profiling failed"
        return 1
    fi
    
    return 0
}

# Run memory check
test_memory() {
    print_status "Running Memory Analysis (this may take a while)..."
    
    timeout 300 make test-memcheck > /dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        print_warning "Memory check timed out (5 minutes limit)"
        return 1
    elif [ $exit_code -ne 0 ]; then
        print_error "Memory check failed"
        return 1
    fi
    
    if make generate-memcheck-report > /dev/null 2>&1; then
        print_success "Memory analysis completed"
        if [ -f memcheck_summary.txt ]; then
            echo "Memory Summary:"
            grep -A 5 "Memory Leak Summary:" memcheck_summary.txt | sed 's/^/  /'
        fi
    else
        print_error "Memory report generation failed"
        return 1
    fi
    
    return 0
}

# Run call graph analysis
test_callgraph() {
    print_status "Running Call Graph Analysis (this may take a while)..."
    
    timeout 300 make test-callgrind > /dev/null 2>&1
    local exit_code=$?
    
    if [ $exit_code -eq 124 ]; then
        print_warning "Call graph analysis timed out (5 minutes limit)"
        return 1
    elif [ $exit_code -ne 0 ]; then
        print_error "Call graph analysis failed"
        return 1
    fi
    
    if make generate-callgrind-report > /dev/null 2>&1; then
        print_success "Call graph analysis completed"
        if [ -f callgrind_summary.txt ]; then
            echo "Call Graph Summary:"
            head -20 callgrind_summary.txt | tail -10 | sed 's/^/  /'
        fi
    else
        print_error "Call graph report generation failed"
        return 1
    fi
    
    return 0
}

# Generate final report
generate_final_report() {
    print_status "Generating final comprehensive report..."
    
    local report_file="final_test_report.txt"
    
    cat > "$report_file" << EOF
COMPREHENSIVE TEST REPORT
========================
Generated: $(date)
Project: Eulerian Circuit Generator

SUMMARY:
========
EOF
    
    # Add test results
    if [ -f coverage_report.txt ]; then
        echo "" >> "$report_file"
        echo "CODE COVERAGE:" >> "$report_file"
        echo "==============" >> "$report_file"
        cat coverage_report.txt >> "$report_file"
    fi
    
    if [ -f gprof_summary.txt ]; then
        echo "" >> "$report_file"
        echo "PERFORMANCE PROFILE:" >> "$report_file"
        echo "===================" >> "$report_file"
        cat gprof_summary.txt >> "$report_file"
    fi
    
    if [ -f memcheck_summary.txt ]; then
        echo "" >> "$report_file"
        echo "MEMORY ANALYSIS:" >> "$report_file"
        echo "===============" >> "$report_file"
        cat memcheck_summary.txt >> "$report_file"
    fi
    
    if [ -f callgrind_summary.txt ]; then
        echo "" >> "$report_file"
        echo "CALL GRAPH ANALYSIS:" >> "$report_file"
        echo "===================" >> "$report_file"
        cat callgrind_summary.txt >> "$report_file"
    fi
    
    echo "" >> "$report_file"
    echo "FILES GENERATED:" >> "$report_file"
    echo "===============" >> "$report_file"
    echo "- $report_file (this file)" >> "$report_file"
    echo "- coverage_html/index.html (HTML coverage report)" >> "$report_file"
    echo "- gprof_report.txt (detailed performance data)" >> "$report_file"
    echo "- valgrind_memcheck.txt (detailed memory analysis)" >> "$report_file"
    echo "- callgrind_report.txt (detailed call graph)" >> "$report_file"
    
    print_success "Final report generated: $report_file"
}

# Main execution
main() {
    local start_time=$(date +%s)
    
    check_tools
    cleanup
    
    if ! build_all; then
        print_error "Build failed. Exiting."
        exit 1
    fi
    
    local tests_passed=0
    local tests_total=4
    
    echo ""
    echo "Running Tests:"
    echo "=============="
    
    if test_coverage; then
        ((tests_passed++))
    fi
    echo ""
    
    if test_profiling; then
        ((tests_passed++))
    fi
    echo ""
    
    if test_memory; then
        ((tests_passed++))
    fi
    echo ""
    
    if test_callgraph; then
        ((tests_passed++))
    fi
    echo ""
    
    generate_final_report
    
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    
    echo ""
    echo "========================================"
    echo "🏁 Testing Complete