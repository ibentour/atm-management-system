#include "header.h"

// Function to log in a user
int login(User *users, int user_count, int *logged_in_user_id) {
    char name[MAX_NAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    
    clear_screen();
    printf("Enter username: ");
    scanf("%49s", name);
    printf("Enter password: ");
    scanf("%49s", password);
    
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, password) == 0) {
            *logged_in_user_id = users[i].id;
            return 1; // Login successful
        }
    }
    
    printf("Invalid username or password.\n");
    return 0; // Login failed
}

// Function to register a new user
int register_user(User *users, int *user_count) {
    User new_user;
    
    clear_screen();
    // Check if maximum users limit reached
    if (*user_count >= MAX_USERS) {
        printf("Maximum user limit reached!\n");
        return 0;
    }

    // Safe input for name
    if (!safe_input("Enter your name: ", new_user.name, sizeof(new_user.name))) {
        return 0;
    }

    // Check if username already exists
    for (int i = 0; i < *user_count; i++) {
        if (strcmp(users[i].name, new_user.name) == 0) {
            printf("Username already exists. Choose a different name.\n");
            return 0;
        }
    }
    
    if (!safe_input("Enter your password: ",  new_user.password, sizeof(new_user.password))) {
        return 0;
    }
    
    
    // Assign unique ID
    new_user.id = *user_count;
    
    // Add new user
    users[*user_count] = new_user;
    (*user_count)++;
    
    // Save to file
    save_users_to_file(users, *user_count);
    
    printf("User  registered successfully!\n");
    return 1;
}