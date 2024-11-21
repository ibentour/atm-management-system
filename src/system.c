#include "header.h"

// Function to create a new account
int create_account(Account *accounts, int *account_count, User *users, int user_id) {
    // Check if we've reached maximum account limit
    if (*account_count >= MAX_ACCOUNTS) {
        printf("Maximum number of accounts reached.\n");
        return 0;
    }

    // Create a new account
    Account new_account;

    // Associate the account with the logged-in user
    new_account.user_id = users[user_id].id;
    strcpy(new_account.user_name, users[user_id].name);

    // Generate unique account ID
    new_account.account_id = generate_unique_account_id(accounts, *account_count);

    // Validate account type
    while (1) {
        if (!safe_input("Enter account type (savings/current/fixed01/fixed02/fixed03): ", 
                        new_account.type, sizeof(new_account.type))) {
            continue;
        }

        // Convert to lowercase for consistent comparison
        to_lowercase(new_account.type);

        // Check against valid account types
        if (strcmp(new_account.type, "savings") == 0 ||
            strcmp(new_account.type, "current") == 0 ||
            strcmp(new_account.type, "fixed01") == 0 ||
            strcmp(new_account.type, "fixed02") == 0 ||
            strcmp(new_account.type, "fixed03") == 0) {
            break;
        }

        printf("Invalid account type. Please choose from:\n");
        printf("- savings\n");
        printf("- current\n");
        printf("- fixed01\n");
        printf("- fixed02\n");
        printf("- fixed03\n");
    }

    // Validate initial balance
    while (1) {
        if (!safe_float_input("Enter initial balance: $", &new_account.balance)) {
            continue;
        }

        // Additional balance validation based on account type
        if (strcmp(new_account.type, "savings") == 0 && new_account.balance < 100) {
            printf("Minimum balance for savings account is $100.\n");
            continue;
        }
        else if (strcmp(new_account.type, "current") == 0 && new_account.balance < 500) {
            printf("Minimum balance for current account is $500.\n");
            continue;
        }
        else if (strncmp(new_account.type, "fixed", 5) == 0 && new_account.balance < 1000) {
            printf("Minimum balance for fixed deposit account is $1000.\n");
            continue;
        }

        break;
    }

    // Validate country
    while (1) {
        if (!safe_input("Enter country: ", new_account.country, sizeof(new_account.country))) {
            continue;
        }

        // Optional: Add country validation if needed
        if (strlen(new_account.country) > 0) {
            break;
        }
    }

    // Validate phone number
    while (1) {
        if (!safe_input("Enter phone number: ", new_account.phone, sizeof(new_account.phone))) {
            continue;
        }

        // Optional: Add phone number validation
        if (strlen(new_account.phone) > 0) {
            break;
        }
    }

    // Set deposit date to current date
    time_t now;
    time(&now);
    struct tm *current_time = localtime(&now);
    
    // Format date as DD/MM/YYYY
    snprintf(new_account.deposit_date, sizeof(new_account.deposit_date), 
             "%02d/%02d/%04d", 
             current_time->tm_mday, 
             current_time->tm_mon + 1, 
             current_time->tm_year + 1900);

    // Set date of creation (can be same as deposit date)
    strcpy(new_account.date_of_creation, new_account.deposit_date);

    // Add the new account to the accounts array
    accounts[*account_count] = new_account;
    (*account_count)++;

    // Print success message with account details
    printf("\nAccount created successfully!\n");
    printf("Account ID: %d\n", new_account.account_id);
    printf("User ID: %d\n", new_account.user_id);
    printf("User Name: %s\n", new_account.user_name);
    printf("Account Type: %s\n", new_account.type);
    printf("Initial Balance: $%.2f\n", new_account.balance);
    printf("Country: %s\n", new_account.country);
    printf("Phone: %s\n", new_account.phone);
    printf("Deposit Date: %s\n", new_account.deposit_date);

    return 1;
}

// Helper function to generate unique account ID
int generate_unique_account_id(Account *accounts, int account_count) {
    if (account_count == 0) {
        return 1001; // Start with first account ID
    }

    // Find the maximum existing account ID and add 1
    int max_id = accounts[0].account_id;
    for (int i = 1; i < account_count; i++) {
        if (accounts[i].account_id > max_id) {
            max_id = accounts[i].account_id;
        }
    }

    return max_id + 1;
}

// Function to update account information
int update_account_information(Account *accounts, int account_count, int user_id) {
    int account_id, choice;
    
    printf("Enter the account ID you want to update: ");
    scanf("%d", &account_id);
    
    // Find the specific account
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].account_id == account_id && accounts[i].user_id == user_id) {
            printf("What would you like to update?\n");
            printf("1. Country\n2. Phone Number\n");
            scanf("%d", &choice);
            
            switch(choice) {
                case 1:
                    printf("Enter new country: ");
                    scanf("%49s", accounts[i].country);
                    break;
                case 2:
                    printf("Enter new phone number: ");
                    scanf("%19s", accounts [i].phone);
                    break;
                default:
                    printf("Invalid choice.\n");
                    return 0;
            }
            // Save updated accounts to file
            save_accounts_to_file(accounts, account_count);
            printf("Account information updated successfully!\n");
            return 1;
        }
    }
    
    printf("Account not found or you do not have permission to update this account.\n");
    return 0;
}

// Function to display account details
void display_account_details(Account *accounts, int account_count, int logged_in_user_id) {
    char account_number[20];
    int found = 0;

    // Prompt for account number
    if (!safe_input("Enter account number to view details: ", account_number, sizeof(account_number))) {
        return;
    }

    // Search for the account
    for (int i = 0; i < account_count; i++) {
        if (strcmp(accounts[i].account_id, account_number) == 0) {
            // Check if the logged-in user owns the account
            if (accounts[i].user_id == logged_in_user_id) {
                clear_screen();
                printf("=== Account Details ===\n");
                printf("Account Number: %s\n", accounts[i].account_number);
                printf("Account Type: %s\n", accounts[i].account_type);
                printf("Balance: $%.2f\n", accounts[i].balance);
                printf("Creation Date: %s\n", accounts[i].creation_date);
                printf("Status: %s\n", accounts[i].is_active ? "Active" : "Inactive");
                found = 1;
                break;
            } else {
                // Account exists but not owned by the logged-in user
                printf("Error: You do not have permission to view this account.\n");
                found = 1;
                break;
            }
        }
    }

    // If account not found
    if (!found) {
        printf("Account number %s not found.\n", account_number);
    }

    // Pause to allow user to read
    printf("\nPress Enter to continue...");
    while (getchar() != '\n');  // Clear any remaining input
    getchar();  // Wait for Enter key
}

// Function to remove an account
int remove_account(Account *accounts, int *account_count, int user_id) {
    int account_id;
    printf("Enter the account ID to remove: ");
    scanf("%d", &account_id);
    
    for (int i = 0; i < *account_count; i++) {
        if (accounts[i].account_id == account_id && accounts[i].user_id == user_id) {
            // Shift accounts to remove the selected account
            for (int j = i; j < *account_count - 1; j++) {
                accounts[j] = accounts[j + 1];
            }
            (*account_count)--;
            // Save updated accounts to file
            save_accounts_to_file(accounts, *account_count);
            printf("Account removed successfully!\n");
            return 1;
        }
    }
    
    printf("Account not found or you do not have permission to remove this account.\n");
    return 0;
}

// Updated transfer_account_ownership function in system.c
int transfer_account_ownership(Account *accounts, int account_count, 
                                User *users, int user_count, int current_user_id) {
    int account_id, new_owner_id;
    int account_index = -1, new_owner_index = -1;

    // Select account to transfer
    printf("Enter the account ID you want to transfer: ");
    scanf("%d", &account_id);

    // Find the account
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].account_id == account_id && accounts[i].user_id == current_user_id) {
            account_index = i;
            break;
        }
    }

    if (account_index == -1) {
        printf("Account not found or you don't own this account.\n");
        return 0;
    }

    // List available users
    printf("Available Users:\n");
    for (int i = 0; i < user_count; i++) {
        if (users[i].id != current_user_id) {
            printf("User ID: %d, Name: %s\n", users[i].id, users[i].name);
        }
    }

    // Select new owner
    printf("Enter the User ID to transfer ownership: ");
    scanf("%d", &new_owner_id);

    // Validate new owner
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == new_owner_id) {
            new_owner_index = i;
            break;
        }
    }

    if (new_owner_index == -1) {
        printf("Invalid user ID.\n");
        return 0;
    }

    // Transfer ownership
    accounts[account_index].user_id = new_owner_id;
    strcpy(accounts[account_index].user_name, users[new_owner_index].name);

    // Save updated accounts
    save_accounts_to_file(accounts, account_count);

    printf("Account ownership transferred successfully!\n");
    return 1;
}


// Function to calculate interest based on account type
float calculate_interest(Account *account) {
    float interest_rate = 0.0;
    time_t now;
    time(&now);
    // Removed unused struct tm *current_time = localtime(&now);

    if (strcmp(account->type, "savings") == 0) {
        interest_rate = 0.07; // 7% for savings
    } else if (strcmp(account->type, "fixed01") == 0) {
        interest_rate = 0.04; // 4% for 1-year fixed
    } else if (strcmp(account->type, "fixed02") == 0) {
        interest_rate = 0.05; // 5% for 2-year fixed
    } else if (strcmp(account->type, "fixed03") == 0) {
        interest_rate = 0.08; // 8% for 3-year fixed
    } else if (strcmp(account->type, "current") == 0) {
        return 0.0; // No interest for current account
    }

    // Parse deposit date
    struct tm deposit_time = {0};
    sscanf(account->deposit_date, "%d/%d/%d", 
           &deposit_time.tm_mday, 
           &deposit_time.tm_mon, 
           &deposit_time.tm_year);
    deposit_time.tm_mon -= 1;  // months are 0-indexed
    deposit_time.tm_year -= 1900;  // years since 1900

    // Calculate monthly interest
    float monthly_interest = account->balance * (interest_rate / 12);

    return monthly_interest;
}

// Function to check account details
int check_account_details(Account *accounts, int account_count, int user_id) {
    int account_id;
    
    // Prompt for account ID
    if (!safe_int_input("Enter the account ID you want to check: ", &account_id)) {
        return 0;
    }

    // Find the account
    Account *account = NULL;
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].account_id == account_id && accounts[i].user_id == user_id) {
            account = &accounts[i];
            break;
        }
    }

    // Validate account
    if (account == NULL) {
        printf("Account not found or you don't have access to this account.\n");
        return 0;
    }

    // Validate account type
    if (strcmp(account->type, "savings") != 0 &&
        strcmp(account->type, "fixed01") != 0 &&
        strcmp(account->type, "fixed02") != 0 &&
        strcmp(account->type, "fixed03") != 0 &&
        strcmp(account->type, "current") != 0) {
        printf("Invalid account type.\n");
        return 0;
    }

    // Display account details
    printf("\nAccount Details:\n");
    printf("Account ID: %d\n", account->account_id);
    printf("Account Type: %s\n", account->type);
    printf("Balance: $%.2f\n", account->balance);
    printf("Deposit Date: %s\n", account->deposit_date);

    // Calculate and display interest
    if (strcmp(account->type, "current") == 0) {
        printf("You will not get interests because the account is of type current\n");
    } else {
        float monthly_interest = calculate_interest(account);
        printf("You will get $%.2f as interest on day %d of every month\n", 
               monthly_interest, 
               atoi(strtok(account->deposit_date, "/")));
    }

    return 1;
}

// Updated make_transaction function
int make_transaction(Account *accounts, int account_count, int user_id) {
    int account_id;
    float amount = 0;
    int account_index = -1;

    // Prompt for account ID
    if (!safe_int_input("Enter the account ID for transaction: ", &account_id)) {
        return 0;
    }

    // Find the account
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].account_id == account_id && accounts[i].user_id == user_id) {
            account_index = i;
            break;
        }
    }

    // Validate account
    if (account_index == -1) {
        printf("Account not found or you don't have access to this account.\n");
        return 0;
    }

    // Check for fixed account types
    if (strcmp(accounts[account_index].type, "fixed01") == 0 ||
        strcmp(accounts[account_index].type, "fixed02") == 0 ||
        strcmp(accounts[account_index].type, "fixed03") == 0) {
        printf("Transactions are not allowed for fixed deposit accounts.\n");
        return 0;
    }


    TransactionType trans_type = get_transaction_type();
    switch (trans_type) {
    case WITHDRAW:
        if (amount > accounts[account_index].balance) {
            printf("Insufficient funds.\n");
            return 0;
        }
        accounts[account_index].balance -= amount;
        printf("Withdrawal successful. New balance: $%.2f\n", 
               accounts[account_index].balance);
        break;
    case DEPOSIT:
        accounts[account_index].balance += amount;
        printf("Deposit successful. New balance: $%.2f\n", 
               accounts[account_index].balance);
        break;
    default:
        printf("Invalid transaction type.\n");
        return 0;
}
    if (!safe_float_input("Enter amount: $", &amount)) {
        return 0;
    } else if (amount <= 0) {
        printf("Invalid amount.\n");
        return 0;
    }

    // Save updated accounts
    save_accounts_to_file(accounts, account_count);

    return 1;
}


TransactionType get_transaction_type() {
    char transaction_type[20];
    printf("Enter transaction type (deposit/withdraw): ");
    scanf("%9s", transaction_type);
    if (strcmp(transaction_type, "withdraw") == 0) return WITHDRAW;
    if (strcmp(transaction_type, "deposit") == 0) return DEPOSIT;
    return INVALID;
}
