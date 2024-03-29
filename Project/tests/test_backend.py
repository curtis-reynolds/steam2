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
    assert "Create user account transaction" in captured.out

#Test Case 4
def test_all_transaction_types(capsys):
    d = Path("tests/dataTC4")
    d.mkdir(parents=True, exist_ok=True)
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "currentaccounts.txt"
    available_games_file = d / "availablegames.txt"
    games_collection_file = d / "gamescollection.txt"

    transactions_file.write_text("01 User255         FS 000045.00\n02 User255         FS 000045.00\n\
03 Game Name           User123       045.00\n\
04 Game Name           TestUser        SellUser       045.00\n\
05 SellUser        TestUser        000025.00\n\
06 SellUser        FS 000045.00\n\
00                             ")
    user_accounts_file.write_text("TestUser         AA 000005.00\n\
SellUser         SS 004140.00\n\
User123          SS 000050.00\n\
END                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")
    
    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()
    
    # Define the expected output
    expected_output = """Create user account transaction
Deleted account transaction
Sell game transaction
Buy game transaction
Refund transaction
Add credit transaction
End of transactions file
"""

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))
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
    d = Path("tests/dataTC7")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "currentaccounts.txt"
    available_games_file = d / "availablegames.txt"
    games_collection_file = d / "gamescollection.txt"

    transactions_file.write_text("04 Game Name           SellUser        TestUser       045.00")
    user_accounts_file.write_text("TestUser         AA 999279.00\nSellUser         SS 001395.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    expected_output = "Game Name                 TestUser       \n"
    with open(games_collection_file, 'r') as file:
        first_line = file.readline()  # Read and discard the first line
        second_line = file.readline().strip()

    print("Captured:", repr(first_line))
    print("Expected:", repr(expected_output))
    assert expected_output in first_line

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

#Test Case 11
def test_game_name_already_exists(capsys):
    # Set up file paths
    d = Path("tests/dataTC11")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("03 Game Name            TestUser      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    captured = capsys.readouterr()

    expected_output = "ERROR: 'Game Name' already exists.\nSell game transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 12
def test_successful_sell_game():
    # Set up file paths
    d = Path("tests/dataTC12")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("03 Game Name 2          TestUser      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    expected_output = "Game Name 2               TestUser         020.00"

    # Verify the user_accounts.txt file has been updated correctly
    with open(available_games_file, 'r') as file:
        file.readline()  # Read and discard the first line
        second_line = file.readline().strip()

    print("Captured:", repr(second_line))
    print("Expected:", repr(expected_output))
    assert expected_output in second_line

#Test Case 13
def test_seller_username_not_found_sell_game(capsys):
    # Set up file paths
    d = Path("tests/dataTC13")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("03 Game Name 2          TestUser55    020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Seller 'TestUser55' not found.\nSell game transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 14
def test_price_exceeds_max_value_sell_game(capsys):
    # Set up file paths
    d = Path("tests/dataTC14")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("03 Game Name 2          TestUser    1000.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: The price exceeds maximum value.\nSell game transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 15
def test_gamename_exceeds_max_length(capsys): 
    # Set up file paths
    d = Path("tests/dataTC15")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("03 Game Name 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100 TestUser    020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: the name of the game 'Game Name 84 85 86 87 88 89 90 91 92 93 94 95 96 97 98 99 100' is too long.\nSell game transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 16
def test_username_already_exists_create_account(capsys):
    # Set up file paths
    d = Path("tests/dataTC16")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("01 TestUser         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Username 'TestUser        ' already exists.\nCreate user account transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 17
def test_successful_create_account():
    # Set up file paths
    d = Path("tests/dataTC17")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("01 TestUser2         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    expected_output = "TestUser2        AA 000000.00" 
    
    with open(user_accounts_file, 'r') as file:
        file.readline()  # Read and discard the first line
        second_line = file.readline().strip()

    print("Captured:", repr(second_line))
    print("Expected:", repr(expected_output))
    assert expected_output in second_line

#Test Case 18
def test_username_exceeds_max_length(capsys):
    # Set up file paths
    d = Path("tests/dataTC18")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("01 TestUser8485868888990919293949596979899100         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Username must be 15 characters or less.\nCreate user account transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 19
def test_usertype_invalid(capsys):
    # Set up file paths
    d = Path("tests/dataTC19")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("01 TestUser2         AB 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Invalid user type.\nCreate user account transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 20
def test_initial_credit_exceeds_max_value(capsys):
    # Set up file paths
    d = Path("tests/dataTC20")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("01 TestUser2         AA 1000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Credit exceeds maximum value.\nCreate user account transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 21
def test_username_not_found_delete_account(capsys):
    # Set up file paths
    d = Path("tests/dataTC21")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("02 TestUser2         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: User 'TestUser2' not found.\nDeleted account transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 22
def test_successful_delete_account():
    # Set up file paths
    d = Path("tests/dataTC22")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("02 TestUser         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser2        AA 000000.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    

    expected_output = "TestUser2        AA 000000.00\nEND                          "
    
    with open(user_accounts_file, 'r') as file:
        actual_output = file.read()  # Read the entire file content 

    print("Captured:", repr(actual_output))
    print("Expected:", repr(expected_output))
    assert expected_output in actual_output

#Test Case 23
def test_delete_user_useraccounts_empty(capsys): 
    # Set up file paths
    d = Path("tests/dataTC23")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("02 TestUser         AA 000000\n00                                                          ")
    user_accounts_file.write_text("")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    

    expected_output = "ERROR: No user accounts found.\nDeleted account transaction\nEnd of transactions file\n"
    
    # Capture the printed output
    captured = capsys.readouterr()

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 24
def test_account_delete_only_account():
        # Set up file paths
    d = Path("tests/dataTC24")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"
    
    # Populate files
    transactions_file.write_text("02 TestUser         AA 000000\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    

    expected_output = ""
    
    with open(user_accounts_file, 'r') as file:
        actual_output = file.read()  # Read the entire file content 

    print("Captured:", repr(actual_output))
    print("Expected:", repr(expected_output))
    assert expected_output in actual_output

#Test Case 25
def test_delete_with_existing_transactions(): 
    # Set up file paths
    d = Path("tests/dataTC25")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("02 TestUser         AA 00000\n05 TestUser2         TestUser        020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser2        AA 000000.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    expected_output = "TestUser2        AA 000000.00\n"

    # Verify the user_accounts.txt file has been updated correctly
    with open(user_accounts_file, 'r') as file:
        first_line = file.readline()  # Read and discard the first line
        second_line = file.readline().strip()

    print("Captured:", repr(first_line))
    print("Second line:", repr(second_line))
    print("Expected:", repr(expected_output))
    assert expected_output in first_line

#Test Case 26
def test_buyer_username_doesnt_exist(capsys):
        # Set up file paths
    d = Path("tests/dataTC26")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("05 TestUser35          TestUser        020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000009.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                  TestUser3      ")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: TestUser35 not found in user accounts.\nRefund transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()
    
#Test Case 27
def test_seller_username_doesnt_exist(capsys):
    # Set up file paths
    d = Path("tests/dataTC27")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("05 TestUser3           TestUser55      020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000009.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                  TestUser3      ")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: TestUser55 not found in user accounts.\nRefund transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 28
def test_refund_negative_balance_seller(capsys):
    # Set up file paths
    d = Path("tests/dataTC28")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    #Populate files
    transactions_file.write_text("05 TestUser3           TestUser        020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000009.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                  TestUser3      ")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Seller has insufficient funds for the refund.\nRefund transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Test Case 29
def test_successful_refund(): 
    # Set up file paths
    d = Path("tests/dataTC29")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    #Populate files
    transactions_file.write_text("05 TestUser3           TestUser        020.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                  TestUser3      ")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    user_accounts_content = user_accounts_file.read_text()
    user_accounts_expected = "TestUser         AA 000889.00\nTestUser3        BS 001390.00\nEND                          "
    print("Captured UserAccounts file:", repr(user_accounts_content))
    print("Expected:", repr(user_accounts_expected))
    assert user_accounts_expected == user_accounts_content
    

#Test Case 30 
def test_refund_invalid_amount(capsys):
    # Set up file paths
    d = Path("tests/dataTC30")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    #Populate files
    transactions_file.write_text("05 TestUser3           TestUser        -20.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000909.00\nTestUser3        BS 001370.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("Game Name                  TestUser3      ")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Invalid refund amount.\nRefund transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 31
def test_username_doesnt_exist(capsys):
     # Set up file paths
    d = Path("tests/dataTC31")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("06 TestUser2       AA 010.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000999.99\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file,
                                        available_games_file, games_collection_file)
    
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: User 'TestUser2' not found.\nAdd credit transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()

#Test Case 32
def test_max_credit_exceeded(capsys): 
    # Set up file paths
    d = Path("tests/dataTC32")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("06 TestUser        AA 010.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 999999.99\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Credit limit exceeded.\nAdd credit transaction\nEnd of transactions file\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    assert captured.out.strip() == expected_output.strip()


#Test Case 33
def test_successful_add_credit(): 
    # Set up file paths
    d = Path("tests/dataTC33")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("06 TestUser        AA 010.00\n00                                                          ")
    user_accounts_file.write_text("TestUser         AA 000900.00\nEND                          ")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    expected_output = "TestUser         AA 000910.00\n"

    # Verify the user_accounts.txt file has been updated correctly
    with open(user_accounts_file, 'r') as file:
        first_line = file.readline()
    
    print("Captured:", repr(first_line))
    print("Expected:", repr(expected_output))

    assert expected_output in first_line

#Test Case 34
def test_create_account_add_credit():
        # Set up file paths
    d = Path("tests/dataTC34")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "user_accounts.txt"
    available_games_file = d / "available_games.txt"
    games_collection_file = d / "games_collection.txt"

    # Populate files
    transactions_file.write_text("01 NewUser         AA 000000\n06 NewUser         AA 010.00\n00                                                          ")
    user_accounts_file.write_text("")
    available_games_file.write_text("")
    games_collection_file.write_text("")

    # Process the transaction
    Transactions.process_transactions(str(transactions_file), str(user_accounts_file), 
                                      str(available_games_file), str(games_collection_file))
    
    expected_output = "NewUser          AA 000010.00\n"
    # Verify the user_accounts.txt file has been updated correctly
    with open(user_accounts_file, 'r') as file:
        first_line = file.readline()  

    print("Captured:", repr(first_line))
    print("Expected:", repr(expected_output))
    
    assert expected_output in first_line

#Test Case 35
def test_add_negative_credit(capsys):
    transactions_file = 'tests/dataTC35/dailytransactions.txt'
    user_accounts_file = 'tests/dataTC35/currentaccounts.txt'
    available_games_file = 'tests/dataTC35/availablegames.txt'
    games_collection_file = 'tests/dataTC35/gamescollection.txt'

    Transactions.process_transactions(transactions_file, user_accounts_file, 
                                      available_games_file, games_collection_file)
         
    # Capture the printed output
    captured = capsys.readouterr()

    expected_output = "ERROR: Credit amount must be a positive number.\nAdd credit transaction\n"

    print("Captured:", repr(captured.out))
    print("Expected:", repr(expected_output))

    # Assert that the captured output matches the expected output
    assert captured.out.strip() == expected_output.strip()

#Path Coverage Tests 
    
#Path Coverage Test 1
def test_PC1_enough_credit(capsys):
    d = Path("tests/dataPC1")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "currentaccounts.txt"
    available_games_file = d / "availablegames.txt"
    games_collection_file = d / "gamescollection.txt"

    transactions_file.write_text("04 Game Name           SellUser        TestUser       045.00")
    user_accounts_file.write_text("TestUser         AA 999279.00\nSellUser         SS 001395.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

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
def test_PC2_more_than_enough_credit(capsys):
    d = Path("tests/dataPC2")
    d.mkdir(parents=True, exist_ok=True) 
    transactions_file = d / "dailytransactions.txt"
    user_accounts_file = d / "currentaccounts.txt"
    available_games_file = d/ "availablegames.txt"
    games_collection_file = d / "gamescollection.txt"

    # Populate files
    transactions_file.write_text("04 Game Name           SellUser        TestUser       045.00")
    user_accounts_file.write_text("TestUser         AA 999279.00\nSellUser         SS 001395.00\nEND                          ")
    available_games_file.write_text("Game Name                 TestUser         020.00\nEND                                              ")
    games_collection_file.write_text("")

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
def test_PC3_not_enough_credit(capsys):
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
def test_PC4_buyer_not_found(capsys):
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
def test_PC5_seller_not_found(capsys):
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
def test_PC6_game_successfully_bought(capsys):
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
def test_PC7_buy_game_already_exists(capsys): 
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

    expected_output = "ERROR: Buyer 'TestUser3' already owns the game 'Game Name'.\nBuy game transaction\nEnd of transactions file"

    assert captured.out.strip() == expected_output.strip()

#Path Coverage Test 8
def test_PC8_game_exists(capsys):
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
def test_PC9_game_price_added_to_seller_account():
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
def test_PC10_game_price_deducted_from_buyer_account():
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