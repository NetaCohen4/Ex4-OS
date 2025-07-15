#!/bin/bash
set -e

EXE=./graph

# איפוס כיסוי
make cov

V_EULER=5
E_EULER=10
S_EULER=1

V_NOEULER=4
E_NOEULER=5
S_NOEULER=2

echo "== 🧠 Valgrind Memcheck =="
valgrind --leak-check=full --log-file=valgrind.out $EXE -v $V_EULER -e $E_EULER -s $S_EULER

echo "== 🔍 Valgrind Callgrind =="
valgrind --tool=callgrind --callgrind-out-file=callgrind.out $EXE -v $V_EULER -e $E_EULER -s $S_EULER

echo "== ✅ Coverage with both scenarios =="

# שתי ריצות שונות לאותו הקובץ בינארי לצורך כיסוי ענפי קוד שונים
$EXE -v $V_EULER -e $E_EULER -s $S_EULER           # אמור להחזיר שיש מעגל אוילר
$EXE -v $V_NOEULER -e $E_NOEULER -s $S_NOEULER     # אמור להחזיר שאין מעגל כזה

echo -n "בדיקה עם Eulerian: "
if $EXE -v $V_EULER -e $E_EULER -s $S_EULER | grep -q "Eulerian"; then
    echo "SUCCESS"
else
    echo "FAIL"
fi

echo -n "בדיקה בלי Eulerian: "
if $EXE -v $V_NOEULER -e $E_NOEULER -s $S_NOEULER | grep -q "Not Eulerian"; then
    echo "SUCCESS"
else
    echo "FAIL"
fi

# עכשיו להריץ gcov אחרי ששני המסלולים כוסו
gcov graph.cpp

echo "== 📊 דוחות כיסוי קוד (graph.cpp.gcov) =="

# תיאור קצר איפה להסתכל
echo "פלט בדיקות ההרצה רץ ישירות על הטרמינל."
echo "כיסוי קוד – ב-gcov: graph.cpp.gcov"

echo "== 📈 gprof Profile =="
make prof
$EXE -v $V_EULER -e $E_EULER -s $S_EULER
gprof $EXE gmon.out > gprof.txt
cat gprof.txt

echo "== 🔗 Callgrind Snapshot =="
echo "נתוני callgrind בקובץ callgrind.out"

rm -f $(TARGET) *.o *.gcda *.gcno *.h.gcov *.tcc.gcov gmon.out new.gcov 
