#!/bin/bash
set -e

V=6
E=10
S=42
EXE=./graph

echo "== 🧠 Running Valgrind Memcheck =="
valgrind --leak-check=full --log-file=valgrind.out $EXE -v $V -e $E -s $S
cat valgrind.out

echo "== 🔍 Running Valgrind Callgrind =="
valgrind --tool=callgrind --callgrind-out-file=callgrind.out $EXE -v $V -e $E -s $S
echo "Callgrind data saved to callgrind.out"

echo "== 📈 Running gprof =="
make prof
$EXE -v $V -e $E -s $S
gprof $EXE gmon.out > gprof.txt
cat gprof.txt

echo "== ✅ Running coverage =="
make cov
$EXE -v $V -e $E -s $S
gcov graph.cpp
