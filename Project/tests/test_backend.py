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

# #Test Case 4
# def test_all_transaction_types(capsys):
#     # Simulate the contents of the transactions file
#     mock_file_contents = """01 User255         FS 000045.00
# 02 User255         FS 000045.00
# 03 Starcraft           User123       045.00
# 04 Starcraft           TestUser        SellUser       045.00
# 05 SellUser        TestUser        000025.00
# 06 SellUser        FS 000045.00
# 00                             
# """
#     expected_output = """Created user account
# Deleted user account
# Sell game transaction
# Buy game transaction
# Refund transaction
# Add credit transaction
# End of transactions file
# """
#     # Mock the open function and os.path.exists
#     with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
#          patch("os.path.exists", return_value=True):
#         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
#                                               '../data/availablegames.txt', '../data/gamescollection.txt')
    
#     # Capture the printed output
#     captured = capsys.readouterr()
    
#     # Assert that the captured output matches the expected output
#     assert captured.out.strip() == expected_output.strip()
    
# #Test Case 5
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
# def test_insufficient_buyer_credit(capsys): 
#     # Simulate the contents of the transactions file
#     mock_file_contents = "04 Starcraft           TestUser        SellUser       045.00"

#     with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
#           patch("os.path.exists", return_value=True):
#          Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
#                                                '../data/availablegames.txt', '../data/gamescollection.txt')
         
#     # Capture the printed output
#     captured = capsys.readouterr()

#     # Assert that the captured output matches the expected output
#     assert captured.out.strip() == "ERROR: User 'TestUser' does not have enough credit to buy the game."
    
#Test Case 7
def successful_purchase_test(capsys): 
    # Simulate the contents of the transactions file
    mock_file_contents = "04 Starcraft           TestUser        SellUser       045.00"

    with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
          patch("os.path.exists", return_value=True):
         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                               '../data/availablegames.txt', '../data/gamescollection.txt')
         
    # Capture the printed output
    captured = capsys.readouterr()

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == "Buy game transaction"

#Test Case 8
def buyer_username_not_found_test(capsys): 
    # Simulate the contents of the transactions file
    mock_file_contents = "04 Starcraft           TestUser        SellUser       045.00"

    with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
          patch("os.path.exists", return_value=True):
         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                               '../data/availablegames.txt', '../data/gamescollection.txt')
         
    # Capture the printed output
    captured = capsys.readouterr()

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == "ERROR: User 'TestUser' not found."

#Test Case 9
def seller_username_not_found(capsys): 
    # Simulate the contents of the transactions file
    mock_file_contents = "04 Starcraft           TestUser        SellUser       045.00"

    with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
          patch("os.path.exists", return_value=True):
         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                               '../data/availablegames.txt', '../data/gamescollection.txt')
         
    # Capture the printed output
    captured = capsys.readouterr()

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == "ERROR: User 'SellUser' not found."

#Test Case 10
def game_doesnt_exist_test(capsys): 
    # Simulate the contents of the transactions file
    mock_file_contents = "04 Starcraft           TestUser        SellUser       045.00"

    with patch("builtins.open", mock_open(read_data=mock_file_contents)), \
          patch("os.path.exists", return_value=True):
         Transactions.process_transactions('../data/dailytransactions.txt', '../data/currentaccounts.txt', 
                                               '../data/availablegames.txt', '../data/gamescollection.txt')
         
    # Capture the printed output
    captured = capsys.readouterr()

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == "ERROR: The game 'Starcraft' does not exist in the available games collection."

#Path Coverage Tests 
#Path Coverage Test 1