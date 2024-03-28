import os
from UserAccounts import UserAccounts
from Games import Games

class Transactions: 
    def __init__(self):
        pass

    @staticmethod
    def process_transactions(transactions, user_accounts, available_games, games_collection):
        # Check if the transactions file exists before attempting to open it
        if not os.path.exists(transactions):
            print(f"ERROR: The file {transactions} does not exist.")
            return
        
        # Create instances of the UserAccounts and Games classes
        games = Games()
        useraccounts = UserAccounts()

        # Process transactions and append them to the respective files. 
        with open(transactions, 'r') as file:
            for line in file:
                if line.startswith('01'):
                    useraccounts.create_account(line, user_accounts)
                    print('Create user account transaction')
                elif line.startswith('02'):
                    useraccounts.delete_account(line, user_accounts)
                    print('Deleted account transaction')
                elif line.startswith('03'):
                    games.sell_game(line, available_games, user_accounts)
                    print('Sell game transaction')
                elif line.startswith('04'):
                    games.buy_game(line, games_collection, user_accounts, available_games)
                    print('Buy game transaction')
                elif line.startswith('05'):
                    useraccounts.refund(line, user_accounts)
                    print('Refund transaction')
                elif line.startswith('06'):
                    useraccounts.add_credit(line, user_accounts)
                    print('Add credit transaction')
                elif line.startswith('00'):
                    print('End of transactions file')
                else:
                    print('ERROR: Invalid transaction code')
                    