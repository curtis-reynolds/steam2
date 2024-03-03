#!/bin/bash

# Ensure the script is running in the correct working directory
expectedWorkingDirectory="../Source"
cd "$expectedWorkingDirectory" || exit

# Define the path to the executable
exePath="../Source/steam2.exe"

# Define the directory containing the test cases
testCasesDir="../TestCases"

# Define paths to the required input files for steam2.exe
userAccounts="../Source/user_accounts.txt"
availableGames="../Source/availablegames.txt"
gamesCollection="../Source/gamescollection.txt"
transactionOutput="../Source/transout.atf"

# Define the output directory
outputDir="../TestCases/ResultantOutputs"

# Print current working directory and user context for debugging
echo "Current working directory: $(pwd)"
echo "Executing as user: $USER"

# Ensure output directory exists
mkdir -p "$outputDir"
# Loop through each test case directory
for categoryDir in "$testCasesDir"/*; do
    testCategory=$(basename "$categoryDir")
    echo "Testing category: $testCategory"

    # Loop through each input file in the test category directory
    for inputFile in "$categoryDir"/*.in; do
        testName=$(basename -s .in "$inputFile")
        expectedOutputFile="$categoryDir/$testName.out"

        # Compare output files with expected
        diffOutput=$(diff "$outputDir/$testName.out" "$expectedOutputFile")
        if [ $? -eq 0 ]; then
            echo "Test $inputFile passed"
        else
            echo "Test $inputFile failed"
            echo "Expected Output:<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
            cat "$expectedOutputFile"
            echo "Actual Output:>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
            cat "$outputDir/$testName.out"
            #echo "Differences:"
            #echo "$diffOutput"
        fi
    done
done