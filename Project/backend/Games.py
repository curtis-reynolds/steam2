from Utilities import Utilities

class Games: 
    def __init__(self):
        pass

    @staticmethod
    def buy_game(line_04, games_collection, user_accounts, available_games):
        utility = Utilities()
        parts = line_04.split()
        game_price = float(parts[-1])
        buyer_username = parts[-2]
        seller_username = parts[-3]
        game_name = ' '.join(parts[1:-3])  # Adjusted for transaction code and price/seller/buyer.

        # Check if the game exists in the available games file.
        with open(available_games, 'r') as file:
            games = file.readlines()

            game_exists = False
            for line in games:
                game_parts = line.strip().split()
                current_game_name = ' '.join(game_parts[0:-2])
                if game_name == current_game_name:
                    game_exists = True
                    break

            if not game_exists:
                print(f"ERROR: The game '{game_name}' does not exist in the available games collection.")
                return  # Exit the function if the game does not exist

        # Check if the buyer already owns the game in the games collection.
        with open(games_collection, 'r') as file:
            owned_games = file.readlines()

            for line in owned_games:
                if game_name in line and buyer_username in line:
                    print(f"ERROR: Buyer '{buyer_username}' already owns the game '{game_name}'.")
                    return  # Exit the function if the buyer already owns the game

        # Update user accounts.
        updated_accounts = []
        end_line_user_accounts = None
        buyer_found = False  # Flag to track if the buyer is found
        seller_found = False  # Flag to track if the seller is found
        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        for account in accounts:
            if account.strip() == "END":
                end_line_user_accounts = account
                break
            if account.strip() == "":
                continue
            try:
                username, user_type, credit = account.strip().split()
                credit = float(credit)
            except ValueError:
                print(f"WARNING: Skipping malformed account line: {account.strip()}")
                continue

            if username == buyer_username:
                buyer_found = True  # Mark the buyer as found
                if credit < game_price:
                    print(f"ERROR: User '{buyer_username}' does not have enough credit to buy the game.")
                    return  # Exit if the buyer does not have enough credit
                credit -= game_price
            elif username == seller_username:
                seller_found = True  # Mark the seller as found
                credit += game_price

            updated_accounts.append(f"{utility.format_username(username)} {user_type} {utility.format_credit(credit)}\n")

        if not seller_found:  # Check if the seller was not found after looping through all accounts
            print(f"ERROR: Seller '{seller_username}' is not found.")
            return  # Exit the function if the seller is not found

        if not buyer_found:  # Check if the buyer was not found
            print(f"ERROR: Buyer '{buyer_username}' is not found.")
            return

        with open(user_accounts, 'w') as file:
            file.writelines(updated_accounts)
            if end_line_user_accounts:
                file.write(end_line_user_accounts)

        # Proceed to update the games collection if all checks pass.
        with open(games_collection, 'r') as file:
            games = file.readlines()

        if games and games[-1].strip() == "END":
            games = games[:-1]

        games.append(f"{utility.format_game_name(game_name)} {utility.format_username_game_collection(buyer_username)}\n")
        games.append("END".ljust(42))

        with open(games_collection, 'w') as file:
            file.writelines(games)




    @staticmethod
    def sell_game(line_03, available_games):
    # Extract game name, seller, and price from the line, taking into account spaces in the game name.
        utility = Utilities()
        parts = line_03.split()
        price = parts[-1]
        seller = parts[-2]
        game_name = ' '.join(parts[1:-2])  # Skip the first part (transaction code) and the last two parts (seller and price).

        # Assuming format_game_name and format_price are functions that format the game name and price.
        game_name = utility.format_game_name(game_name)
        price = utility.format_price(price)
        seller = utility.format_username(seller)

        # Read the existing games, remove the "END" line if it exists, and prepare to append the new game.
        updated_games = []
        with open(available_games, 'r') as file:
            games = file.readlines()

        if games and games[-1].strip() == "END":
            updated_games = games[:-1]  # Remove the last "END" line if it exists.
        else:
            updated_games = games  # No "END" found, use the list as is.

        # Append the new game and the "END" line with 39 spaces.
        updated_games.append(f"{game_name} {seller} {price}\n")
        updated_games.append("END" + " " * 46)

        # Write everything back to the file.
        with open(available_games, 'w') as file:
            file.writelines(updated_games)