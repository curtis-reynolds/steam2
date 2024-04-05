# Steam 2

## Project Overview
This project aims to create a Digital Distribution Service similar to Steam, where users can buy and sell games through a console-based application. The project is split into two main components:

- **Front End**: A C++ console application handling user interactions, including login/logout, buying/selling games, and managing user accounts.
- **Back End**: A batch processor (written in Python or Java) that updates the master games file and user accounts based on daily transactions.

## Run Instructions

### Compiling the Project
To compile the Steam 2 Front End, ensure you have a C++ compiler installed (e.g., GCC). Navigate to Project/src and run the following command in the terminal:

```bash
g++ main.cpp -o GameDistributionSystem
```

### Running the Project
To run the Steam 2 Front End, ensure that you are still inside Project/src and run the following command in the terminal:

```bash
GameDistributionSystem ../data/currentaccounts.txt ../data/availablegames.txt ../data/gamescollection.txt ../data/dailytransactions.txt
```

### Running the Back End Tests
- Navigate to the Project folder in your terminal
- Type 'pytest' in your terminal and hit enter 

## Features

### Front End
- **Login/Logout**: Secure access to user-specific functionalities.
- **Create/Delete User**: Admins can add or remove users, with unique constraints on usernames and credit limits.
- **Sell/Buy Games**: Users can list games for sale or purchase available games.
- **Refund and Add Credit**: Admins can issue refunds and add credit to user accounts, with constraints to prevent misuse.

### Back End
- **Batch Processing**: Runs at 12:00am daily to update user accounts and game listings based on transactions recorded by the Front End.

## Development and Testing
- Development follows XP (Extreme Programming) practices, with periodic adaptations to project requirements.
- Testing includes a comprehensive suite of test cases covering all functionalities and edge cases, ensuring robustness and reliability.

## Contribution Guidelines
- Contributors are encouraged to follow project coding standards and contribute to test case development.
- For major changes, please open an issue first to discuss what you would like to change.

## Acknowledgments
- Thanks to all contributors and the project management team for their support and coordination.
