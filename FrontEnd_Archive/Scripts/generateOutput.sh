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

        # Run the program with the input file
        echo "Running test $inputFile"
        echo "Outputting to $outputDir/$testName.out"
        "$exePath" "$userAccounts" "$availableGames" "$gamesCollection" "$transactionOutput" < "$inputFile" > "$outputDir/$testName.out"

        # Remove newlines from new output file
        awk 'NR > 1 { print prev } { prev = $0 } END { printf "%s", prev }' "$outputDir/$testName.out" > "$outputDir/$testName.tmp" && mv "$outputDir/$testName.tmp" "$outputDir/$testName.out"
    done
done
