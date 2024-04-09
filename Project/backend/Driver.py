from Transactions import Transactions
import os 

def main():
    #Main function to load, process, and append transactions.
    user_accounts_file = '../data/currentaccounts.txt'
    available_games_file = '../data/availablegames.txt'
    games_collection_file = '../data/gamescollection.txt'
    daily_transactions_file = '../data/dailytransactions.txt'

    # Process transactions and append them to the respective files
    transactions = Transactions()
    transactions.process_transactions(daily_transactions_file, user_accounts_file, available_games_file, games_collection_file)

if __name__ == '__main__':
    main()
