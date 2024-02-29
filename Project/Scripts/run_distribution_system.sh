#!/bin/bash

# Change to the input directory
cd inputs || exit

# Loop through each file in the input directory
for i in *; do
    # Check if the file is a regular file
    if [[ -f $i ]]; then
        # Run distribution-system with the appropriate input and output files
        distribution-system currentaccounts.txt availablegames.txt gamescollection.txt "../outputs/${i%.atf}.out" < "$i"
    fi
done