#!/bin/bash

FILE="s1.txt"


gcc ../src/mem_list.c ../src/memory.c -o memory

if [ $? -ne 0 ]; then
    echo "Failed to compile."
    exit 1
fi

./memory "$FILE"
