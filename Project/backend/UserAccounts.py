from Utilities import Utilities

class UserAccounts: 
    def __init__(self):
        pass

    @staticmethod
    def create_account(line_01, user_accounts):
    # Create a new user account and append it to the user_accounts list.
    # Getting the information from the line passed to the function.
        utility = Utilities()
        username = line_01.split()[1]  # line_01 represents the line in the transactions file that starts with '01'.
        user_type = line_01.split()[2]
        credit = line_01.split()[3]
        formatted_username = utility.format_username(username)
        formatted_credit = utility.format_credit(credit)

        # First, check if the username has already been taken by reading the file.
        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        for line in accounts:
            if username == line.split()[0]: 
                print(f"ERROR: Username '{formatted_username}' already exists.")
                return

        # Now, remove the "END" line, append the new user account, and re-add "END".
        if accounts and accounts[-1].strip() == "END":
            accounts = accounts[:-1]  # Remove the last "END" line.

        # Append the new account and "END" line.
        with open(user_accounts, 'w') as file:
            file.writelines(accounts)  # Write back all lines except the last "END" line.
            file.write(f"{formatted_username} {user_type} {formatted_credit}\n")  # Append the new user account.
            file.write("END" + " " * 25)  # Re-add the "END" line at the end.

    @staticmethod
    def delete_account(line_02, user_accounts):
        # Delete a user account from the user_accounts list.
        # Extract the username to delete from the provided line.
        username = line_02.split()[1]

        # Initialize an empty list to hold all accounts except the one to delete.
        updated_accounts = []

        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        # Filter out the account to delete.
        for account in accounts:
            if not account.startswith(username):
                updated_accounts.append(account)

        # Write the updated list back to the file.
        with open(user_accounts, 'w') as file:
            file.writelines(updated_accounts)

    @staticmethod
    def refund(line_05, user_accounts):
        utility = Utilities()
        parts = line_05.split()
        buyer_username, seller_username = parts[1], parts[2]
        amount = float(parts[3])  # Convert amount to float for arithmetic operations.

        if amount < 0:
            print("ERROR: Invalid refund amount.")
            return

        updated_accounts = []
        end_line_user_accounts = None
        buyer_found = False
        seller_found = False
        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        # Handle the "END" line if present.
        if accounts and accounts[-1].strip() == "END":
            end_line_user_accounts = accounts.pop().strip()

        for account in accounts:
            if account.strip() == "":
                continue  # Skip empty lines.
            username, user_type, credit = account.strip().split()
            credit = float(credit)

            # Check if the current line is for the buyer or the seller
            if username == buyer_username:
                buyer_found = True
            elif username == seller_username:
                seller_found = True

        # Only proceed if both buyer and seller are found in the accounts file
        if not buyer_found or not seller_found:
            missing_username = buyer_username if not buyer_found else seller_username
            print(f"ERROR: {missing_username} not found in user accounts.")
            return

        # Process the refund if both buyer and seller are found
        for account in accounts:
            username, user_type, credit = account.strip().split()
            credit = float(credit)

            if username == seller_username and credit - amount < 0:
                print("ERROR: Seller has insufficient funds for the refund.")
                return

            if username == buyer_username:
                credit += amount  # Refund to buyer.
            elif username == seller_username:
                credit -= amount  # Chargeback from seller.

            # Reformat the credit with two decimal places.
            updated_accounts.append(f"{utility.format_username(username)} {user_type} {utility.format_credit(credit)}\n")

        # Re-add the "END" line if it was present.
        if end_line_user_accounts:
            updated_accounts.append(end_line_user_accounts + " " * 26)

        with open(user_accounts, 'w') as file:
            file.writelines(updated_accounts)


    @staticmethod
    def add_credit(line, user_accounts):
        utility = Utilities()
        parts = line.split()
        username = parts[1]
        additional_credit = float(parts[3]) 

        if additional_credit < 0:
            print("ERROR: Credit amount must be a positive number.")
            return
        
        with open(user_accounts, 'r') as file:
            accounts = file.readlines()
            if username in accounts: 
                print(f"ERROR: User '{username}' not found.")
                return

        updated_accounts = []
        user_found = False
        end_line = None

        with open(user_accounts, 'r') as file:
            accounts = file.readlines()

        # Check if the last line is "END" and remove it if present.
        if accounts and accounts[-1].strip() == "END":
            end_line = accounts.pop().strip()

        for account in accounts:
            if account.strip() == "":
                continue  # Skip empty lines.

            account_parts = account.strip().split()
            if len(account_parts) < 3:
                # Skip any malformed or incomplete lines.
                continue

            account_username, user_type, current_credit = account_parts[0], account_parts[1], float(account_parts[2])

            if current_credit + additional_credit > 999999.99:
                print("ERROR: Credit limit exceeded.")
                return

            if account_username == username:
                current_credit += additional_credit
                user_found = True  # Mark that the user has been found and updated.

            updated_account = f"{utility.format_username(account_username)} {user_type} {utility.format_credit(current_credit)}"
            updated_accounts.append(updated_account + '\n')

        if not user_found:
            print(f"ERROR: User '{username}' not found.")
            return

        # Re-add the "END" line if it was present.
        if end_line:
            updated_accounts.append(end_line + " " * 26)

        with open(user_accounts, 'w') as file:
            file.writelines(updated_accounts)