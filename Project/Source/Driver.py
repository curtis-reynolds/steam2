import Transactions

def main():
    #Main function to load, process, and append transactions.
    user_accounts_file = 'Project/Source/user_accounts1.txt'
    available_games_file = 'Project/Source/availablegames1.txt'
    games_collection_file = 'Project/Source/gamescollection1.txt'
    daily_transactions_file = 'Project/Source/daily_transactions.txt'

    # Process transactions and append them to the respective files
    transactions = Transactions()
    transactions.process_transactions(daily_transactions_file, user_accounts_file, available_games_file, games_collection_file)

if __name__ == '__main__':
    main()