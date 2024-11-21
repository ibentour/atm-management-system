#include "header.h"

void display_menu() {
    printf("\n--- ATM Management System ---\n");
    printf("1. Login\n");
    printf("2. Register\n");
    printf("3. Exit\n");
    printf("Enter your choice: ");
}

void display_logged_in_menu() {
    // clear_screen();
    printf("\n--- Account Management ---\n");
    printf("1. Create New Account\n");
    printf("2. List Owned Accounts\n");
    printf("3. Update Account Information\n");
    printf("4. Check Account Details\n");
    printf("5. Make Transaction\n");
    printf("6. Remove Account\n");
    printf("7. Transfer Account Ownership\n");
    printf("8. Logout\n");
    printf("Enter your choice: ");
}

int list_owned_accounts(Account *accounts, int account_count, int user_id) {
    int found = 0;
    clear_screen();
    printf("\nYour Accounts:\n");
    printf("ID\tType\t\tBalance\n");
    
    for (int i = 0; i < account_count; i++) {
        if (accounts[i].user_id == user_id) {
            printf("%d\t%s\t\t$%.2f\n", 
                   accounts[i].account_id, 
                   accounts[i].type, 
                   accounts[i].balance);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No accounts found.\n");
    }
    
    return found;
}

int main() {
    User users[MAX_USERS];
    Account accounts[MAX_ACCOUNTS];
    int user_count = 0, account_count = 0;
    int choice, logged_in_user_id = -1;
    // char input[10];  // Buffer for input
    
    // Load existing users and accounts
    user_count = load_users_from_file(users);
    account_count = load_accounts_from_file(accounts);
    
    while (1) {
        if (logged_in_user_id == -1) {
            // Not logged in
            display_menu();
            
            // Use safe input for menu choice
            if (!safe_int_input("Enter your choice: ", &choice)) {
                continue;  // If input is invalid, restart loop
            }
            
            switch(choice) {
                case 1:
                    if (login(users, user_count, &logged_in_user_id)) {
                        printf("Login successful!\n");
                    }
                    break;
                case 2:
                    register_user(users, &user_count);
                    break;
                case 3:
                    printf("Exiting...\n");
                    
                    // Save users and accounts before exiting
                    save_users_to_file(users, user_count);
                    save_accounts_to_file(accounts, account_count);
                    
                    return 0;
                default:
                    printf("Invalid choice. Try again.\n");
            }
        } else {
            // Logged in
            display_logged_in_menu();
            
            // Use safe input for menu choice
            if (!safe_int_input("Enter your choice: ", &choice)) {
                continue;  // If input is invalid, restart loop
            }
            
            switch(choice) {
                case 1:
                    create_account(accounts, &account_count, users, logged_in_user_id);
                    break;
                case 2:
                    list_owned_accounts(accounts, account_count, logged_in_user_id);
                    break;
                case 3:
                    update_account_information(accounts, account_count, logged_in_user_id);
                    break;
                case 4:
                    display_account_details(accounts, account_count);
                    break;
                case 5:
                    make_transaction(accounts, account_count, logged_in_user_id);
                    break;
                case 6:
                    remove_account(accounts, &account_count, logged_in_user_id);
                    break;
                case 7:
                    transfer_account_ownership(accounts, account_count, users, user_count, logged_in_user_id);
                    break;
                case 8:
                    // Prompt for confirmation before logging out
                    if (safe_confirm("Are you sure you want to log out? (y/n): ")) {
                        logged_in_user_id = -1;
                        printf("Logged out successfully.\n");
                    }
                    break;
                default:
                    printf("Invalid choice. Try again.\n");
            }
        }
    }
    
    return 0;
}