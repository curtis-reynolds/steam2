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

        #Check if the game exists in the available games collection.
        with open(available_games, 'r') as file:
            games = file.readlines()
            game_parts = games.split()
            game_name = ' '.join(game_parts[0:-2])
            if game_name not in games:
                print(f"ERROR: The game '{game_name}' does not exist in the available games collection.")
                return

        # Update user accounts.
        updated_accounts = []
        end_line_user_accounts = None
        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        for account in accounts:
            if account.strip() == "END":
                end_line_user_accounts = account  # Save the "END" line with its spaces
                break
            username, user_type, credit = account.strip().split()
            credit = float(credit)

            if username == buyer_username:
                if credit < game_price:
                    print(f"ERROR: User '{buyer_username}' does not have enough credit to buy the game.")
                    return
                credit -= game_price
            elif username == seller_username:
                credit += game_price

            updated_accounts.append(f"{utility.format_username(username)} {user_type} {utility.format_credit(credit)}\n")

        with open(user_accounts, 'w') as file:
            file.writelines(updated_accounts)
            if end_line_user_accounts:
                file.write(end_line_user_accounts)  # Re-add the "END" line

        # Update the games collection, managing the "END" line.
        with open(games_collection, 'r') as file:
            games = file.readlines()

        if games and games[-1].strip() == "END":
            games = games[:-1]  # Remove the last "END" line if present.

        games.append(f"{utility.format_game_name(game_name)} {utility.format_username_game_collection(buyer_username)}" + '\n')
        games.append("END".ljust(42))  # Re-add the "END" line with padding.

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