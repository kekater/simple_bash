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
"VAR int test_0_grep.txt test_2_grep.txt"
"VAR int test_1_grep.txt test_2_grep.txt"
"VAR int test_2_grep.txt test_2_grep.txt"
"VAR int test_3_grep.txt test_2_grep.txt"
"VAR int test_4_grep.txt test_2_grep.txt"
"VAR int test_5_grep.txt test_2_grep.txt"
"VAR in test_6_grep.txt test_2_grep.txt"
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

# 2  pair parameters
for var1 in i v c l n h o
do
    for var2 in v c l n h o
    do
        if [ $var1 != $var2 ]
        then
            for i in "${tests[@]}"
            do
                var="-$var1$var2"
                testing $i
            done
        fi
    done
done

echo -e "\033[31mFAIL: $FAIL\033[0m"
echo -e "\033[32mSUCCESS: $SUCCESS\033[0m"
echo "ALL: $COUNTER"
