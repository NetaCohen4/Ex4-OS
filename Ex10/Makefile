SHELL := /bin/bash
EXS := ../Ex4 ../Ex6 ../Ex7 ../Ex8 ../Ex9
REPORT_DIR := valgrind_reports

prepare:
	@mkdir -p $(REPORT_DIR)

valgrind_all: prepare
	@for d in $(EXS); do \
	  if [ -d "$$d" ]; then \
	    echo "=== Running all Valgrind tools in $$d ==="; \
	    if [ -f "$$d/Makefile" ] || [ -f "$$d/makefile" ]; then \
	      (cd "$$d" && make all_valgrind) || echo "  (make all_valgrind failed in $$d)"; \
	      (cd "$$d" && make clean) || echo "  (make clean failed in $$d)"; \
	    else \
	      echo "  (no Makefile in $$d — skipping)"; \
	    fi; \
	    for f in $$d/valgrind_* $$d/callgrind_*; do \
	      if [ -f "$$f" ]; then \
	        mv -f "$$f" "$(REPORT_DIR)/$$(basename $$d)_$$(basename $$f)"; \
	      fi; \
	    done; \
	  else \
	    echo "  (directory $$d not found — skipping)"; \
	  fi; \
	done
	@echo "All Valgrind runs completed. Reports are in $(REPORT_DIR)/"

clean-all:
	@rm -rf $(REPORT_DIR)
	@echo "Removed $(REPORT_DIR)/"
