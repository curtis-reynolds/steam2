import sys
import os
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..', 'backend')))
import pytest 
from unittest.mock import patch, mock_open
from Transactions import Transactions

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