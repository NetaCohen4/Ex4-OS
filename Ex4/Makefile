CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -fprofile-arcs -g
TARGET = graph
SOURCES = graph.cpp

# Default build
$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Build with code coverage support
coverage: $(SOURCES)
	$(CXX) $(CXXFLAGS) --coverage -g -o $(TARGET) $(SOURCES)

# Build with profiling support
profile: $(SOURCES)
	$(CXX) $(CXXFLAGS) -pg -g -o $(TARGET)_profile $(SOURCES)

# Build with debug info for valgrind (no optimization)
debug: $(SOURCES)
	$(CXX) $(CXXFLAGS) -g -O0 -o $(TARGET)_debug $(SOURCES)

# Code Coverage Test
coverage-run: coverage
	@echo "=== Running Code Coverage Tests ==="
	@echo "Testing various scenarios..."
	# Small connected graph with Eulerian circuit
	./$(TARGET) -v 4 -e 4 -s 123
	# Larger graph
	./$(TARGET) -v 6 -e 9 -s 456
	# Edge cases
	./$(TARGET) -v 3 -e 2 -s 789
	./$(TARGET) -v 2 -e 1 -s 111
	./$(TARGET) -v 5 -e 10 -s 222
	./$(TARGET) -v 3 -e 3 -s 1
	@echo "Coverage data collected. Generating report..."

generate-coverage:
	@echo "=== Generating Coverage Report ==="
	gcov $(SOURCES) > coverage_summary.txt 2>&1
	lcov --capture --directory . --output-file coverage.info > /dev/null 2>&1
	genhtml coverage.info --output-directory coverage_html --quiet
	@echo "Coverage Report Summary:" > coverage_report.txt
	@echo "========================" >> coverage_report.txt
	@grep "Lines executed" coverage_summary.txt >> coverage_report.txt || echo "No line coverage data" >> coverage_report.txt
	@grep "Branches executed" coverage_summary.txt >> coverage_report.txt || echo "No branch coverage data" >> coverage_report.txt
	@echo "" >> coverage_report.txt
	@echo "Detailed HTML report: coverage_html/index.html" >> coverage_report.txt
	@echo "Coverage report saved to: coverage_report.txt"
	@echo "HTML report available at: coverage_html/index.html"

# Performance Profiling Test
test-profile: profile
	@echo "=== Running Performance Profiling ==="
	@echo "Testing with different graph sizes for performance analysis..."
	# Small graphs
	./$(TARGET)_profile -v 10 -e 20 -s 100
	./$(TARGET)_profile -v 15 -e 30 -s 200
	./$(TARGET)_profile -v 3 -e 3 -s 1
	# Medium graphs
	./$(TARGET)_profile -v 50 -e 100 -s 300
	./$(TARGET)_profile -v 100 -e 200 -s 400
	# Large graphs
	./$(TARGET)_profile -v 200 -e 500 -s 500
	@echo "Profiling data collected. Generating report..."

generate-profile:
	@echo "=== Generating Profile Report ==="
	gprof ./$(TARGET)_profile gmon.out > gprof_report.txt 2>/dev/null
	@echo "Performance Profile Report" > gprof_summary.txt
	@echo "=========================" >> gprof_summary.txt
	@echo "" >> gprof_summary.txt
	@echo "Top 10 Functions by Time:" >> gprof_summary.txt
	@echo "------------------------" >> gprof_summary.txt
	@awk '/^  %   cumulative   self/{flag=1; print; next} flag && NF>=6 && NR<=25{print} flag && NF<6{exit}' gprof_report.txt >> gprof_summary.txt || echo "No profiling data available" >> gprof_summary.txt
	@echo "" >> gprof_summary.txt
	@echo "Call Graph Summary:" >> gprof_summary.txt
	@echo "-----------------" >> gprof_summary.txt
	@awk '/^Call graph/{flag=1} flag && /^index/{print; getline; print; flag=0}' gprof_report.txt >> gprof_summary.txt || echo "No call graph data" >> gprof_summary.txt
	@echo "" >> gprof_summary.txt
	@echo "Full detailed report: gprof_report.txt" >> gprof_summary.txt
	@echo "Profile summary saved to: gprof_summary.txt"
	@echo "Full report available at: gprof_report.txt"

# Memory Check with Valgrind
memcheck: debug
	@echo "=== Running Valgrind Memory Check ==="
	@echo "This may take a while... Testing memory safety"
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind_memcheck.txt \
		./$(TARGET)_debug -v 10 -e 15 -s 123
		./$(TARGET)_debug -v 3 -e 3 -s 1
	@echo "Memory check completed."

generate-memcheck-report:
	@echo "=== Generating Memory Check Summary ==="
	@echo "Valgrind Memory Check Report" > memcheck_summary.txt
	@echo "============================" >> memcheck_summary.txt
	@echo "" >> memcheck_summary.txt
	@echo "Memory Leak Summary:" >> memcheck_summary.txt
	@echo "-------------------" >> memcheck_summary.txt
	@grep -A 10 "HEAP SUMMARY" valgrind_memcheck.txt >> memcheck_summary.txt || echo "No heap summary found" >> memcheck_summary.txt
	@echo "" >> memcheck_summary.txt
	@echo "Error Summary:" >> memcheck_summary.txt
	@echo "-------------" >> memcheck_summary.txt
	@grep "ERROR SUMMARY" valgrind_memcheck.txt >> memcheck_summary.txt || echo "No error summary found" >> memcheck_summary.txt
	@echo "" >> memcheck_summary.txt
	@echo "Memory Errors:" >> memcheck_summary.txt
	@echo "-------------" >> memcheck_summary.txt
	@grep -E "(Invalid|Mismatched|Uninitialized)" valgrind_memcheck.txt >> memcheck_summary.txt || echo "No memory errors found" >> memcheck_summary.txt
	@echo "" >> memcheck_summary.txt
	@echo "Full detailed report: valgrind_memcheck.txt" >> memcheck_summary.txt
	@echo "Memory check summary saved to: memcheck_summary.txt"

# Call Graph Analysis with Callgrind
callgrind: debug
	@echo "=== Running Valgrind Callgrind ==="
	@echo "This will be slow but thorough..."
	valgrind --tool=callgrind \
		--callgrind-out-file=callgrind.out \
		./$(TARGET)_debug -v 20 -e 30 -s 12345
		./$(TARGET)_debug -v 3 -e 3 -s 1
	@echo "Call graph analysis completed."

helgrind: debug
	@echo "=== Running Valgrind Helgrind ==="
	valgrind --tool=helgrind \
	     --log-file=helgrind_graph.txt \
	    ./$(TARGET)_debug -v 20 -e 30 -s 12345
		./$(TARGET)_debug -v 3 -e 3 -s 1
	@echo "Helgrind analysis completed."

all_valgrind: helgrind memcheck callgrind 
	@echo "=== All Valgrind tests completed (Memcheck + Callgrind + Helgrind on Client + Server) ==="

generate-callgrind-report:
	@echo "=== Generating Call Graph Report ==="
	callgrind_annotate callgrind.out > callgrind_report.txt 2>/dev/null
	@echo "Call Graph Analysis Report" > callgrind_summary.txt
	@echo "=========================" >> callgrind_summary.txt
	@echo "" >> callgrind_summary.txt
	@echo "Function Call Statistics:" >> callgrind_summary.txt
	@echo "------------------------" >> callgrind_summary.txt
	@head -30 callgrind_report.txt | tail -20 >> callgrind_summary.txt || echo "No call graph data available" >> callgrind_summary.txt
	@echo "" >> callgrind_summary.txt
	@echo "Most Called Functions:" >> callgrind_summary.txt
	@echo "--------------------" >> callgrind_summary.txt
	@grep -E "^[0-9,]+ +[0-9,]+ +.*::" callgrind_report.txt | head -10 >> callgrind_summary.txt || echo "No function call data" >> callgrind_summary.txt
	@echo "" >> callgrind_summary.txt
	@echo "Full detailed report: callgrind_report.txt" >> callgrind_summary.txt
	@echo "To view graphically: kcachegrind callgrind.out" >> callgrind_summary.txt
	@echo "Call graph summary saved to: callgrind_summary.txt"

# Install required tools
install-tools:
	@echo "=== Installing Required Tools ==="
	@echo "Updating package list..."
	@sudo apt-get update > /dev/null 2>&1
	@echo "Installing tools..."
	@sudo apt-get install -y gcc g++ gdb valgrind lcov gcov > /dev/null 2>&1
	@echo "✓ Tools installed successfully"
	@echo "Optional: sudo apt-get install kcachegrind (for visual call graphs)"

# Run all tests in sequence
test-all: coverage profile debug
	@echo "=== Running Complete Test Suite ==="
	make coverage-run
	make generate-coverage
	make test-profile
	make generate-profile
	make memcheck
	make generate-memcheck-report
	make callgrind
	make generate-callgrind-report
	@echo "=== All tests completed ==="
	make show-results

# Show comprehensive results summary
show-results:
	@echo ""
	@echo "=========================================="
	@echo "           TEST RESULTS SUMMARY"
	@echo "=========================================="
	@echo ""
	@echo "📊 CODE COVERAGE:"
	@if [ -f coverage_report.txt ]; then \
		cat coverage_report.txt; \
	else \
		echo "  ❌ Not available - run 'make coverage-run generate-coverage'"; \
	fi
	@echo ""
	@echo "⚡ PERFORMANCE PROFILE:"
	@if [ -f gprof_summary.txt ]; then \
		cat gprof_summary.txt; \
	else \
		echo "  ❌ Not available - run 'make test-profile generate-profile'"; \
	fi
	@echo ""
	@echo "🔍 MEMORY CHECK:"
	@if [ -f memcheck_summary.txt ]; then \
		cat memcheck_summary.txt; \
	else \
		echo "  ❌ Not available - run 'make memcheck generate-memcheck-report'"; \
	fi
	@echo ""
	@echo "📈 CALL GRAPH:"
	@if [ -f callgrind_summary.txt ]; then \
		cat callgrind_summary.txt; \
	else \
		echo "  ❌ Not available - run 'make callgrind generate-callgrind-report'"; \
	fi
	@echo ""
	@echo "=========================================="

# Quick test with small dataset
quick-test: $(TARGET)
	@echo "=== Quick Functionality Test ==="
	@echo "Testing basic functionality..."
	./$(TARGET) -v 4 -e 4 -s 123
	@echo ""
	./$(TARGET) -v 6 -e 5 -s 456
	@echo "Quick test completed ✓"

# Clean all generated files
clean-all:
	@echo "Cleaning up generated files..."
	@rm -f $(TARGET) $(TARGET)_* 
	@rm -f *.gcov *.gcda *.gcno coverage.info gmon.out callgrind.out *.txt
	@rm -rf coverage_html
	@echo "✓ Cleanup completed"

help:
	@echo "=== Available Targets ==="
	@echo ""
	@echo "🔨 Build Targets:"
	@echo "  make                    - Build normal version"
	@echo "  make coverage           - Build with coverage support"
	@echo "  make profile            - Build with profiling support"
	@echo "  make debug              - Build with debug info"
	@echo ""
	@echo "🧪 Test Targets:"
	@echo "  make quick-test         - Quick functionality test"
	@echo "  make coverage-run      - Run code coverage analysis"
	@echo "  make test-profile       - Run performance profiling"
	@echo "  make memcheck      - Run memory leak detection"
	@echo "  make callgrind     - Run call graph analysis"
	@echo "  make test-all           - Run all tests"
	@echo ""
	@echo "📊 Report Generation:"
	@echo "  make generate-coverage      - Generate coverage report"
	@echo "  make generate-profile       - Generate performance report"
	@echo "  make generate-memcheck-report - Generate memory report"
	@echo "  make generate-callgrind-report - Generate call graph report"
	@echo "  make show-results           - Show all test results"
	@echo ""
	@echo "🛠  Utilities:"
	@echo "  make install-tools      - Install required testing tools"
	@echo "  make clean              - Remove all generated files"
	@echo "  make help               - Show this help"

.PHONY: clean coverage profile debug coverage-run test-profile memcheck callgrind test-all generate-coverage generate-profile generate-memcheck-report generate-callgrind-report show-results install-tools quick-test help