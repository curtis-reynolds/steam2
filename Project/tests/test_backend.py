import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'backend')))
import pytest 
from unittest.mock import patch, mock_open
from Transactions import Transactions
from pathlib import Path

#Test Case 1
def test_missing_daily_transactions_file(capsys):
    with patch('os.path.exists', return_value=False):
        transactions = Transactions()
        transactions.process_transactions('fake/daily_transactions.txt', 'fake/user_accounts.txt', 'fake/available_games.txt', 'fake/games_collection.txt')
    
    captured = capsys.readouterr()
    assert "ERROR: The file fake/daily_transactions.txt does not exist." in captured.out


#Test Case 2
def test_empty_daily_transactions_file(capsys):
    # Mock the open function to simulate an empty file
    with patch('builtins.open', mock_open(read_data="")):
        # Assuming os.path.exists will return True, as we're focusing on the file's content here
        with patch('os.path.exists', return_value=True):
            transactions = Transactions()
            transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                              '../data/availablegames.txt', '../data/gamescollection.txt')

    # Capture the output
    captured = capsys.readouterr()
    
    # Assert that the error message for an invalid transaction code is part of the output
    assert captured.out == ""

#Test Case 3
def test_valid_transaction_code_01(capsys):
    with patch('os.path.exists', return_value=True), \
         patch('builtins.open', mock_open(read_data="01 User255         FS 000045.00")):
        Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                              '../data/availablegames.txt', '../data/gamescollection.txt')
    captured = capsys.readouterr()
    assert "Created user account" in captured.out

#Test Case 4
def test_all_transaction_types(capsys):
    # Assuming the test runs from the project root, adjust paths accordingly
    transactions_file = 'tests/dataTC4/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC4/currentaccounts.txt'
    available_games_file = 'tests/dataTC4/availablegames.txt'
    games_collection_file = 'tests/dataTC4/gamescollection.txt'
    
    # No need to mock, as we're using actual files
    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()
    
    # Define the expected output
    expected_output = """Created user account
Deleted user account
Sell game transaction
Buy game transaction
Refund transaction
Add credit transaction
End of transactions file
"""
    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()
    
#Test Case 5
def test_invalid_transaction_code(capsys):
    # Simulate the contents of the transactions file
    mock_file_contents = "09 User255         FS 000045.00"

    with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
          patch("os.path.exists", return_value=True):
         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                               '../data/availablegames.txt', '../data/gamescollection.txt')
         
    # Capture the printed output
    captured = capsys.readouterr()

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == "ERROR: Invalid transaction code"

#Test Case 6
def test_insufficient_buyer_credit(capsys): 
    transactions_file = 'tests/dataTC6/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC6/currentaccounts.txt'
    available_games_file = 'tests/dataTC6/availablegames.txt'
    games_collection_file = 'tests/dataTC6/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()
    
    expected_output = "ERROR: User 'TestUser' does not have enough credit to buy the game.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 7
def test_successful_purchase(capsys): 
    transactions_file = 'tests/dataTC7/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC7/currentaccounts.txt'
    available_games_file = 'tests/dataTC7/availablegames.txt'
    games_collection_file = 'tests/dataTC7/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "Buy game transaction"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Test Case 8
def test_buyer_username_not_found(capsys): 
    transactions_file = 'tests/dataTC8/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC8/currentaccounts.txt'
    available_games_file = 'tests/dataTC8/availablegames.txt'
    games_collection_file = 'tests/dataTC8/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Buyer 'jgfufhrg' is not found.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Test Case 9
def test_seller_username_not_found(capsys): 
    transactions_file = 'tests/dataTC9/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC9/currentaccounts.txt'
    available_games_file = 'tests/dataTC9/availablegames.txt'
    games_collection_file = 'tests/dataTC9/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Seller 'djfugifh' is not found.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Test Case 10
def test_game_doesnt_exist_test(capsys): 
    transactions_file = 'tests/dataTC10/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC10/currentaccounts.txt'
    available_games_file = 'tests/dataTC10/availablegames.txt'
    games_collection_file = 'tests/dataTC10/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: The game 'Starcraft' does not exist in the available games collection.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Tests 
    
#Path Coverage Test 1
def test_PC1_enough_credit(capsys):
    transactions_file = 'tests/dataPC1/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC1/currentaccounts.txt'
    available_games_file = 'tests/dataPC1/availablegames.txt'
    games_collection_file = 'tests/dataPC1/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "Buy game transaction"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 2
def test_more_than_enough_credit(capsys):
    transactions_file = 'tests/dataPC2/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC2/currentaccounts.txt'
    available_games_file = 'tests/dataPC2/availablegames.txt'
    games_collection_file = 'tests/dataPC2/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "Buy game transaction"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 3
def test_not_enough_credit(capsys):
    transactions_file = 'tests/dataPC3/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC3/currentaccounts.txt'
    available_games_file = 'tests/dataPC3/availablegames.txt'
    games_collection_file = 'tests/dataPC3/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()
    
    expected_output = "ERROR: User 'TestUser' does not have enough credit to buy the game.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 4
def test_buyer_not_found(capsys):
    transactions_file = 'tests/dataPC4/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC4/currentaccounts.txt'
    available_games_file = 'tests/dataPC4/availablegames.txt'
    games_collection_file = 'tests/dataPC4/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Buyer 'jgfufhrg' is not found.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 5
def test_seller_not_found(capsys):
    transactions_file = 'tests/dataPC5/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC5/currentaccounts.txt'
    available_games_file = 'tests/dataPC5/availablegames.txt'
    games_collection_file = 'tests/dataPC5/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Seller 'djfugifh' is not found.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 6
def test_game_successfully_bought(capsys):
    d = Path("tests/dataPC6")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("04 Game Name           TestUser        TestUser3      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("END")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    # Verify the user_accounts.txt file has been updated correctly
    with open(games_collection_file, 'r') as file:
        first_line = file.readline().strip()
    assert "Game Name                 TestUser3" in first_line

#Path Coverage Test 7
def test_buy_game_already_exists(capsys): 
    # Set up file paths
    d = Path("tests/dataPC7")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    transactions_file.write_text("04 Game Name           TestUser        TestUser3      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                 TestUser3      \nEND                                       ")

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                    available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Buyer '{buyer_username}' already owns the game '{game_name}'."

    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 8
def test_game_exists(capsys):
    transactions_file = 'tests/dataPC8/dailytransactions.txt'
    user_accounts_file = 'tests/dataPC8/currentaccounts.txt'
    available_games_file = 'tests/dataPC8/availablegames.txt'
    games_collection_file = 'tests/dataPC8/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: The game 'Starcraft' does not exist in the available games collection.\nBuy game transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()
 
#Path Coverage Test 9
def test_game_price_added_to_seller_account():
    # Set up file paths
    d = Path("tests/dataPC9")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("04 Game Name           TestUser        TestUser3      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    # Verify the user_accounts.txt file has been updated correctly
    with open(user_accounts_file, 'r') as file:
        first_line = file.readline().strip()
    assert "TestUser         AA 000929.00" in first_line
    
#Path Coverage Test 10
def test_game_price_deducted_from_buyer_account():
    # Set up file paths
    d = Path("tests/dataPC10")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("04 Game Name           TestUser        TestUser3      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    # Verify the user_accounts.txt file has been updated correctly
    with open(user_accounts_file, 'r') as file:
        file.readline()  # Read and discard the first line
        second_line = file.readline().strip()
    assert "TestUser3        BS 001350.00" in second_line