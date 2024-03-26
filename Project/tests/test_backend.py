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
            transactions.process_transactions('fake/daily_transactions.txt', 'fake/user_accounts.txt', 'fake/available_games.txt', 'fake/games_collection.txt')

    # Capture the output
    captured = capsys.readouterr()
    
    # Assert that the error message for an invalid transaction code is part of the output
    assert "ERROR: Invalid transaction code" in captured.out