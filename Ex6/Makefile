CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -fprofile-arcs -ftest-coverage -g -O0
SOURCES = server.cpp client.cpp
TARGETS = server client

all: $(TARGETS)

server: server.cpp
	$(CXX) $(CXXFLAGS) -o server server.cpp

client: client.cpp
	$(CXX) $(CXXFLAGS) -o client client.cpp

debug: server client

wait-for-server:
	@echo "=== Waiting for server to start ==="
	@while ! grep -q "Server listening on port 8080" server.log 2>/dev/null; do sleep 0.2; done


wait-for-client-connection:
	@echo "=== Waiting for client to successfully connect ==="
	@timeout 30s sh -c '\
		while ! printf "3\n3\n1\n" | ./client > /dev/null 2>&1; do \
			sleep 1; \
		done; \
		echo "Client successfully connected!"; \
	'


# ================================
# Valgrind / Memcheck / Callgrind / Helgrind
# עם שרת ברקע ולקוח עם קלט מוכן
# ================================

# ----- Memcheck -----
memcheck: debug
	@rm -f server.log
	@echo "=== Running Valgrind Memcheck (Server + Client) ==="
	# מריצים את השרת ברקע
	valgrind --leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=valgrind_memcheck_server.txt \
    	timeout 20s ./server > server.log 2>&1 &
		$(MAKE) wait-for-server; \
		$(MAKE) wait-for-client-connection; \
		valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind_memcheck_client.txt \
			sh -c "printf '3\n3\n1\n' | ./client"; 
			sh -c "printf '5\n5\n1\n' | ./client";
		sleep 5; 
	@echo "Memcheck completed."

# ----- Callgrind -----
callgrind: debug
	@rm -f server.log
	@echo "=== Running Valgrind Callgrind (Server + Client) ==="
	valgrind --tool=callgrind \
		--callgrind-out-file=callgrind_server.out \
		timeout 20s ./server > server.log 2>&1 &
		$(MAKE) wait-for-server; \
		$(MAKE) wait-for-client-connection; \
		valgrind --tool=callgrind \
			--callgrind-out-file=callgrind_client.out \
			sh -c "printf '3\n3\n1\n' | ./client"; \
			sh -c "printf '5\n5\n1\n' | ./client";
		sleep 5; 
	@echo "Callgrind completed." 

# ----- Helgrind -----
helgrind: server client
	@echo "=== Running Valgrind Helgrind (Server + Client) ==="
	# הרצת השרת ברקע
	valgrind --tool=helgrind --log-file=helgrind_server.txt \
		timeout 20s ./server > server.log 2>&1 &
	$(MAKE) wait-for-server
	$(MAKE) wait-for-client-connection
	valgrind --tool=helgrind --log-file=helgrind_client1.txt \
		sh -c "printf '3\n3\n1\n' | ./client"
	valgrind --tool=helgrind --log-file=helgrind_client2.txt \
		sh -c "printf '5\n5\n1\n' | ./client"

	@echo "Helgrind completed."

# Build with coverage
coverage: $(SOURCES)
	$(CXX) $(CXXFLAGS) --coverage -g -o $(TARGET) $(SOURCES)

# Coverage run with server + client
coverage-run: coverage server client
	@rm -f server.log
	@echo "=== Running Coverage Tests (Server + Client) ==="
	# מריצים את השרת ברקע עם timeout
	timeout 20s ./server > server.log 2>&1 &
	$(MAKE) wait-for-server
	$(MAKE) wait-for-client-connection
	# מריצים מספר תרחישים של הלקוח
	sh -c "printf '3\n3\n1\n' | ./client"
	sh -c "printf '5\n5\n1\n' | ./client"
	sh -c "printf '2\n2\n1\n' | ./client"
	@echo "=== Coverage run finished, now you can run 'make generate-coverage' ==="


# ----- Run all -----
all_valgrind: helgrind memcheck callgrind 
	@echo "=== All Valgrind tests completed (Memcheck + Callgrind + Helgrind on Client + Server) ==="


clean:
	rm -f $(TARGETS) server.log

clean-all:
	rm -f $(TARGETS) valgrind_memcheck_*.txt callgrind_*.out helgrind_*.txt *.gcno *.gcda server.log callgrind.*
		

