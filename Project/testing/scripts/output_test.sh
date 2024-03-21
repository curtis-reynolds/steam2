#!/bin/bash

# Set the paths for input and output directories
input_dir="inputs"
output_dir="actual_outputs"

# Define an array of exclusion patterns (stuff that we don't want to save in the output files)
exclusion_patterns=(
	"Enter username: "
	"Enter user type (1-admin, 2-full-standard, 3-buy-standard, 4-sell-standard):" 
	"Enter the username to delete: "
	"Enter the game name (up to 25 characters): "
	"Enter the price for the game (up to \$999.99): "
	"Enter the game name you want to buy: "
	"Enter the seller'\''s username: "
	"Enter the buyer'\''s username: "
	"Enter the amount of credit to transfer: "
	"Enter the username: "
	"Enter the amount of credit to add: "
)

# Create the output directory if it doesn't exist
mkdir -p "../$output_dir"

# Change to the input directory
cd "../$input_dir" || exit

# Iterate through each input file
iteration_count=0
for input_file in *.inp; do
    # Extract the file name without extension
    filename=$(basename -- "$input_file")
    filename_noext="${filename%.*}"
    echo "running output test for $filename_noext"

    ((iteration_count++))

    # Run the C++ program with input file and save output to temporary file
    if [ $iteration_count -eq 47 ]; then # For the 47th iteration (the empty list of games test), use an empty gamescollection file
        ../../src/distribution-system ../test_data/currentaccounts_test.txt ../test_data/availablegames_test_empty.txt ../test_data/gamescollection_test.txt ../test_data/dailytransactions_test.txt < <(cat "$input_file"; echo "exit") > temp_output.txt 2>&1
    else
        ../../src/distribution-system ../test_data/currentaccounts_test.txt ../test_data/availablegames_test.txt ../test_data/gamescollection_test.txt ../test_data/dailytransactions_test.txt < <(cat "$input_file"; echo "exit") > temp_output.txt 2>&1
    fi

    # For the the list of games or list of user tests, capture the entire output without filtering
    if [ $iteration_count -eq 48 ] || [ $iteration_count -eq 49 ] || [ $iteration_count -eq 51 ]; then
        cat "temp_output.txt" > "../$output_dir/$filename_noext.out"
    else
        # For other iterations, apply the usual filtering logic
        # Exclude specific patterns from each line and save the result to the output file
        sed_cmd="sed -n 's/Enter transaction code (or '\''exit'\'' to quit): \(.*\)/\1/p' temp_output.txt"
        for pattern in "${exclusion_patterns[@]}"; do
            sed_cmd+=" | sed 's/$pattern//g'"
        done

        eval "$sed_cmd" > "../$output_dir/$filename_noext.out"
    fi

    # Empty transactions.txt for the next run
    echo -n "" > ../test_data/dailytransactions_test.txt

    # Clean up temporary file
    rm temp_output.txt
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

echo -e "Output tests completed.\n"

