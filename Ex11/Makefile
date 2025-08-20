SHELL := /bin/bash

# Adjust the list of exercise directories if yours are named differently
EXS := ../Ex6 ../Ex7 ../Ex8 ../Ex9

COVERAGE_DIR := coverage_reports

prepare:
	@mkdir -p $(COVERAGE_DIR)

coverage_all: prepare
	@for d in $(EXS); do \
	  if [ -d "$$d" ]; then \
	    if [ -f "$$d/Makefile" ] || [ -f "$$d/makefile" ]; then \
	      echo "=== Running coverage in $$d ==="; \
	      if (cd "$$d" && make coverage-run > /dev/null 2>&1 \
	                     && make clean > /dev/null 2>&1); then \
	        echo "  [SUCCESS] Coverage completed in $$d"; \
	      else \
	        echo "  [FAIL] Coverage failed in $$d"; \
	      fi; \
	      if [ -d "$$d/coverage_html" ]; then \
	        cp -r "$$d/coverage_html" "$(COVERAGE_DIR)/$$(basename $$d)_coverage_html" >/dev/null 2>&1; \
	      fi; \
	      if [ -f "$$d/coverage_report.txt" ]; then \
	        cp "$$d/coverage_report.txt" "$(COVERAGE_DIR)/$$(basename $$d)_coverage_report.txt" >/dev/null 2>&1; \
	      fi; \
	    else \
	      echo "  (no Makefile in $$d — skipping)"; \
	    fi; \
	  else \
	    echo "  (directory $$d not found — skipping)"; \
	  fi; \
	done
	@echo "All Coverage runs completed. Reports are in $(COVERAGE_DIR)/"

clean-all:
	@rm -rf $(COVERAGE_DIR)
	@echo "Removed $(COVERAGE_DIR)/"
