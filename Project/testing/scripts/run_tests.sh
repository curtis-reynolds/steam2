#!/bin/bash

# Empty the outputs directory to populate it with new results
output_dir="../actual_outputs"

# Check if the directory exists
if [ -d "$output_dir" ]; then
    # Remove all files and subdirectories within the directory
    rm -rf "$output_dir"/*

    echo -e "Directory '$output_dir' has been emptied.\n"
else
    echo -e "Error: Directory '$output_dir' not found.\n"
fi


# Run the first script
./output_test.sh

# Check the exit status of the first script
if [ $? -eq 0 ]; then
    # If the first script succeeded, run the second script
    ./transaction_file_test.sh
else
    # If the first script failed, print an error message
    echo "Error: output_test.sh failed."
fi

echo "All tests completed"

