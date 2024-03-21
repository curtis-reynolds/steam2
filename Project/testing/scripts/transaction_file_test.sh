#!/bin/bash

# Set the paths for input and output directories
input_dir="inputs"
output_dir="actual_outputs"

# Create the output directory if it doesn't exist
mkdir -p "../$output_dir"

# Change to the input directory
cd "../$input_dir" || exit

# Iterate through each input file
for input_file in *.inp; do
    # Extract the file name without extension
    filename=$(basename -- "$input_file")
    filename_noext="${filename%.*}"
    echo "running daily transaction test for $filename_noext"
    
    # Ensure the output file is created, even if empty
    touch "../$output_dir/$filename_noext.atf"

    # Run the C++ program to save transactions
    ../../src/distribution-system ../test_data/currentaccounts_test.txt ../test_data/availablegames_test.txt ../test_data/gamescollection_test.txt "../$output_dir/$filename_noext.atf" < <(cat "$input_file"; echo "logout"; echo "exit") > /dev/null 2>&1

done

# Return to the original directory
cd ..

# Empty and populate the "currentaccounts_test.txt" file with the default users
echo -n "" > "./test_data/currentaccounts_test.txt"
echo "admin___________AA_000000.00" >> "./test_data/currentaccounts_test.txt"
echo "nonadmin________FS_000000.00" >> "./test_data/currentaccounts_test.txt"
echo "buystdwcredit___FS_005000.00" >> "./test_data/currentaccounts_test.txt"
echo "END_________________________" >> "./test_data/currentaccounts_test.txt"

# Empty and populate the "availablegames_test.txt" file with the default users
echo -n "" > "./test_data/availablegames_test.txt"
echo "existinggame_______________sellstd_________020.00" >> "./test_data/availablegames_test.txt"
echo "existinggame2______________buystdwcredit___050.00" >> "./test_data/availablegames_test.txt"
echo "END______________________________________________" >> "./test_data/availablegames_test.txt"

# Empty "availablegames_test.txt"
echo -n "" > "./test_data/gamescollection_test.txt"

echo -e "Daily transaction tests completed.\n"
