CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
TARGET = graph
SRC = graph.cpp

# כיסוי קוד
COV_FLAGS = -fprofile-arcs -ftest-coverage
GCOV_TOOL = gcov

# gprof
PROF_FLAGS = -pg

# ברירת מחדל
all: $(TARGET) prof cov coverage

# רגיל
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

# לפרופיילינג gprof
prof: $(SRC)
	$(CXX) $(CXXFLAGS) $(PROF_FLAGS) -o $(TARGET) $(SRC)

# לכיסוי קוד gcov
cov: $(SRC)
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) -o $(TARGET) $(SRC)

coverage:
	$(GCOV_TOOL) *.cpp

# ניקוי
clean:
	rm -f $(TARGET) *.o *.gcda *.gcno *.gcov gmon.out valgrind.out callgrind.out.*

