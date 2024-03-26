class Utilities: 
    
    def __init__(self):
        pass

    @staticmethod
    def format_username(username):
        return username.ljust(16)
    
    @staticmethod
    def format_username_game_collection(username):
        return username.ljust(15)
    
    @staticmethod
    def format_game_name(game_name):
        return game_name.ljust(25)
    
    @staticmethod
    def format_user_type(user_type):
        return user_type.ljust(2)
    
    @staticmethod
    def format_credit(credit):
        return f"{float(credit):.2f}".rjust(9, '0')
    
    @staticmethod
    def format_price(price):
        return f"{float(price):.2f}".rjust(6, '0')