#!/bin/bash

# Usage check
if [ "$#" -ne 6 ]; then
	echo "Usage: $0 <Input Directory> <Actual Output Directory> <Expected Output Directory> <Accounts File> <Games Collection File> <Available Games File>"
	exit 1
fi

# Command line arguments
input_dir="$1"
actual_output_dir="$2"
expected_output_dir="$3"
accounts_file="$4"
collection_file="$5"
available_games_file="$6"

# Ensure actual output directory exists
mkdir -p "$actual_output_dir"

# Loop through all .in files in the input directory
for in_file in "$input_dir"/*.in; do
  # Extract filename without extension
  base_name=$(basename "$in_file" .in)
  # Define output filename for .out files
  out_file="$actual_output_dir/$base_name.out"
 
  # Run the C++ program with the necessary files
  cat "$in_file" | GameDistributionSystem ../data/currentaccounts.txt ../data/availablegames.txt ../data/gamescollection.txt ../data/dailytransactions.txt > "$out_file"
done

# Merge all .dt files into Merged.dt
cat "$input_dir"/*.dt > Merged.dt

# Run backend.py on the merged file
python Driver.py Merged.dt

# Define the log file path
diff_log_file="diff_log.txt"

# Ensure the log file is empty at the start
> "$diff_log_file"

# Loop through all .out files in the actual output directory for comparison
for out_file in "$actual_output_dir"/*.out; do
  # Extract filename for comparison
  base_name=$(basename "$out_file")
  # Compare .out files and append to log
  echo "Comparing $base_name with expected output..." >> "$diff_log_file"
  diff "$out_file" "$expected_output_dir/$base_name" >> "$diff_log_file" 2>&1
done

# Compare the Merged.dt file and append to log
echo "Comparing Merged.dt with expected output..." >> "$diff_log_file"
diff Merged.dt "$expected_output_dir/Merged.dt" >> "$diff_log_file" 2>&1

# Compare the three base files against their expected counterparts and append to log
echo "Comparing base files with expected outputs..." >> "$diff_log_file"
diff "$accounts_file" "$expected_output_dir/$(basename "$accounts_file")" >> "$diff_log_file" 2>&1
diff "$collection_file" "$expected_output_dir/$(basename "$collection_file")" >> "$diff_log_file" 2>&1
diff "$available_games_file" "$expected_output_dir/$(basename "$available_games_file")" >> "$diff_log_file" 2>&1
