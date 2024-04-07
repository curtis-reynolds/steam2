#!/bin/bash

# Base directories for each day
days=("monday" "tuesday" "wednesday" "thursday" "friday")

# Preset locations for the accounts, collection, and available games files
accounts_file="../data/currentaccounts.txt"
collection_file="../data/gamescollection.txt"
available_games_file="../data/availablegames.txt"

# Loop through each day, running daily.sh with the day's directory
for day in "${days[@]}"; do
	echo "Processing $day..."

	# Define actual and expected output directories for the current day
	actual_output_dir="${day}/actual_output"
	expected_output_dir="${day}/expected_output"

	# Ensure the output directories exist
	mkdir -p "$actual_output_dir"
	mkdir -p "$expected_output_dir"

	# Call daily.sh with dynamically determined paths
	./daily.sh "${day}/input" "$actual_output_dir" "$expected_output_dir" "$accounts_file" "$collection_file" "$available_games_file"
done

echo "Weekly process complete."
