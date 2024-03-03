# Ensure the script is running in the correct working directory
$expectedWorkingDirectory = "../Source"
Set-Location -Path $expectedWorkingDirectory

# Define the path to the executable
$exePath = "..\Source\steam2.exe"

# Define the directory containing the test cases
$testCasesDir = "..\TestCases"

# Define paths to the required input files for steam2.exe
$userAccounts = "..\Source\user_accounts.txt"
$availableGames = "..\Source\availablegames.txt"
$gamesCollection = "..\Source\gamescollection.txt"
$transactionOutput = "..\Source\transout.atf"

# Print current working directory and user context for debugging
Write-Host "Current working directory: $(Get-Location)"
Write-Host "Executing as user: $env:USERNAME"

# Iterate over each test case directory (e.g., Login, Logout, etc.)
Get-ChildItem $testCasesDir -Directory | ForEach-Object {
    $testCategory = $_.Name
    Write-Host "Testing category: $testCategory"

    # Iterate over each .in file in the test category directory
    Get-ChildItem $_.FullName -Filter "*.in" | ForEach-Object {
        $inputFile = $_.FullName
        $expectedOutputFile = $inputFile.Replace(".in", ".out")
        $testName = $_.BaseName

        # Execute the application with input from the .in file and capture the output
        $output = Get-Content $inputFile | & $exePath $userAccounts $availableGames $gamesCollection $transactionOutput

        # Read the expected output from the .out file
        $expectedOutput = Get-Content $expectedOutputFile -Raw

        # Compare the actual output to the expected output
        if ($output -eq $expectedOutput) {
            Write-Host "Category '$testCategory' Test '$testName': PASS"
        } else {
            Write-Host "Category '$testCategory' Test '$testName': FAIL"
            Write-Host "Expected Output: $expectedOutput"
            Write-Host "Actual Output: $output"
        }
    }
}