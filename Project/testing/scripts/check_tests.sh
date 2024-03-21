#!/bin/bash

# ANSI color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

# Check if the inputs directory exists
if [ ! -d "../inputs" ]; then
    echo -e "${RED}Error:${NC} Inputs directory not found."
    exit 1
fi

cd "../inputs" || exit 1

echo -e "${GREEN}Checking outputs and transactions:${NC}\n"

passed_tests=0
total_tests=0

for i in *; do
    filename=$(basename -- "$i")
    filename_noext="${filename%.*}"

    # echo -e "${GREEN}Checking test $filename_noext${NC}"
    
    # Check both output and transaction files
    if diff_output=$(diff "../actual_outputs/$filename_noext.out" "../expected_outputs/$filename_noext.out" 2>&1) &&
       diff_transaction=$(diff "../actual_outputs/$filename_noext.atf" "../expected_outputs/$filename_noext.etf" 2>&1); then
        echo -e "${GREEN}Test $filename_noext passed${NC}"
        ((passed_tests++))
    else
        echo -e "${RED}Test $filename_noext failed${NC}"
        
        if [ -n "$diff_output" ]; then
            echo -e "${RED}Output Differences:${NC}\n$diff_output"
        fi
        
        if [ -n "$diff_transaction" ]; then
            echo -e "${RED}Transaction Differences:${NC}\n$diff_transaction"
        fi
    fi

    ((total_tests++))
done

echo -e "\n${GREEN}Total tests passed: $passed_tests/$total_tests${NC}"

cd ".."

echo -e "Total tests passed $passed_tests/$total_tests Done." > test_results.log
