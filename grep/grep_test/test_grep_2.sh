#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0
DIFF_RES=""

declare -a tests=(
"VAR int test_0_grep.txt"
"VAR int test_1_grep.txt"
"VAR int test_2_grep.txt"
"VAR int test_3_grep.txt"
"VAR int test_4_grep.txt"
"VAR int test_5_grep.txt"
"VAR in test_6_grep.txt"
)

declare -a extra=(
"-f test_4_grep.txt test_0_grep.txt"    
"-f test_4_grep.txt test_1_grep.txt"
"-f test_4_grep.txt test_2_grep.txt"
"-f test_4_grep.txt test_3_grep.txt"
"-f test_4_grep.txt test_4_grep.txt"
"-f test_4_grep.txt test_5_grep.txt"
"-f test_4_grep.txt test_6_grep.txt"
)

testing()
{
    t=$(echo $@ | sed "s/VAR/$var/")
    ./s21_grep $t > test_s21_grep.log
    grep $t > test_sys_grep.log
    DIFF_RES="$(diff -s test_s21_grep.log test_sys_grep.log)"
    (( COUNTER++ ))
    if [ "$DIFF_RES" == "Files test_s21_grep.log and test_sys_grep.log are identical" ]
    then
      (( SUCCESS++ ))
      echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[32msuccess\033[0m grep $t"
    else
      (( FAIL++ ))
      echo -e "\033[31m$FAIL\033[0m/\033[32m$SUCCESS\033[0m/$COUNTER \033[31mfail\033[0m grep $t"
    fi
    rm test_s21_grep.log test_sys_grep.log
}

# 1 parameter
for var1 in h s o
do
    for i in "${tests[@]}"
    do
        var="-$var1"
        testing $i
    done
done

# special
for i in "${extra[@]}"
do
    var="-"
    testing $i
done

echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
