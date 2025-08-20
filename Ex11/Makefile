SHELL := /bin/bash
EXS := ../Ex4 ../Ex6 ../Ex7 ../Ex8 ../Ex9

coverage_all:
	@for d in $(EXS); do \
		if [ -d "$$d" ]; then \
			if [ -f "$$d/Makefile" ] || [ -f "$$d/makefile" ]; then \
				echo "=== Running coverage in $$d ==="; \
				( cd "$$d" && make coverage-run > /dev/null 2>&1 \
				             && make clean > /dev/null 2>&1 ); \
			else \
				echo "  (no Makefile in $$d — skipping)"; \
			fi; \
		else \
			echo "  (directory $$d not found — skipping)"; \
		fi; \
	done
	@echo "All Coverage runs completed."
