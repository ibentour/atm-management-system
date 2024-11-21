#include "header.h"

void save_users_to_file(User *users, int user_count) {
    FILE *file = fopen("data/users.txt", "w");
    if (!file) {
        printf("Error opening users file for writing\n");
        return;
    }
    
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%d %s %s\n", 
                users[i].id, 
                users[i].name, 
                users[i].password);
    }
    
    fclose(file);
}

void save_accounts_to_file(Account *accounts, int account_count) {
    FILE *file = fopen("data/records.txt", "w");
    if (!file) {
        printf("Error opening accounts file for writing\n");
        return;
    }
    
    for (int i = 0; i < account_count; i++) {
        fprintf(file, "%d %d %s %d %s %s %s %.2f %s\n", 
                accounts[i].id, 
                accounts[i].user_id, 
                accounts[i].user_name,
                accounts[i].account_id,
                accounts[i].date_of_creation,
                accounts[i].country,
                accounts[i].phone,
                accounts[i].balance,
                accounts[i].type);
    }
    
    fclose(file);
}

int load_users_from_file(User *users) {
    FILE *file = fopen("data/users.txt", "r");
    int count = 0;
    
    if (!file) {
        printf("No existing users file found.\n");
        return 0;
    }
    
    while (fscanf(file, "%d %s %s", 
                  &users[count].id,  users[count].name, 
                  users[count].password) != EOF) {
        count++;
    }
    
    fclose(file);
    return count;
}

int load_accounts_from_file(Account *accounts) {
    FILE *file = fopen("data/records.txt", "r");
    int count = 0;
    
    if (!file) {
        printf("No existing accounts file found.\n");
        return 0;
    }
    
    while (fscanf(file, "%d %d %s %d %s %s %s %f %s", 
                  &accounts[count].id, 
                  &accounts[count].user_id, 
                  accounts[count].user_name, 
                  &accounts[count].account_id, 
                  accounts[count].date_of_creation, 
                  accounts[count].country, 
                  accounts[count].phone, 
                  &accounts[count].balance, 
                  accounts[count].type) != EOF) {
        count++;
    }
    
    fclose(file);
    return count;
}



// ----------------------------------------------------------------------------------


int is_valid_input(const char *input) {
    // Check if input is NULL
    if (input == NULL) {
        return 0;
    }

    // Check if input is empty string
    if (strlen(input) == 0) {
        return 0;
    }

    // Check if input is only whitespace
    int is_whitespace = 1;
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isspace((unsigned char)input[i])) {
            is_whitespace = 0;
            break;
        }
    }

    return !is_whitespace;
}

// Safe integer input function
int safe_int_input(const char *prompt, int *result) {
    char input[50];
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        
        // Read input as a string
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            clearerr(stdin);
            clearerr(stdin);
            continue;
        }
        
        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;
        
        // Check for empty input
        if (strlen(input) == 0) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        
        // Convert string to integer
        errno = 0;
        long parsed_value = strtol(input, &endptr, 10);
        
        // Check for conversion errors
        if (*endptr != '\0') {
            printf("Invalid input. Please enter a valid integer.\n");
            continue;
        }
        
        // Check for overflow/underflow
        if (errno == ERANGE) {
            printf("Number out of range. Please try again.\n");
            continue;
        }
        
        // Store result and return success
        *result = (int)parsed_value;
        return 1;
    }
}

// Safe float input function
int safe_float_input(const char *prompt, float *result) {
    char input[50];
    char *endptr;
    
    while (1) {
        printf("%s", prompt);
        
        // Read input as a string
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            clearerr(stdin);
            continue;
        }
        
        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;
        
        // Check for empty input
        if (strlen(input) == 0) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        
        // Convert string to float
        errno = 0;
        float parsed_value = strtof(input, &endptr);
        
        // Check for conversion errors
        if (*endptr != '\0') {
            printf("Invalid input. Please enter a valid number.\n");
            continue;
        }
        
        // Check for overflow/underflow
        if (errno == ERANGE) {
            printf("Number out of range. Please try again.\n");
            continue;
        }
        
        // Store result and return success
        *result = parsed_value;
        return 1;
    }
}

// Safe string input function
int safe_input(const char *prompt, char *result, size_t max_length) {
    while (1) {
        printf("%s", prompt);
        
        // Read input as a string
        if (fgets(result, max_length, stdin) == NULL) {
            printf("Input error. Please try again.\n");
            clearerr(stdin);
            continue;
        }
        
        // Remove newline character if present
        result[strcspn(result, "\n")] = 0;
        
        // Check for empty input
        if (strlen(result) == 0) {
            printf("Input cannot be empty. Please try again.\n");
            continue;
        }
        
        return 1;
    }
}

// Safe confirmation input function
int safe_confirm(const char *prompt) {
    char input[10];
    
    while (1) {
        printf("%s", prompt);
        
        // Read input as a string
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Input error. Please try again.\n");
            clearerr(stdin);
            continue;
        }
        
        // Remove newline character if present
        input[strcspn(input, "\n")] = 0;
        
        // Convert to lowercase for case-insensitive comparison
        for (int i = 0; input[i]; i++) {
            input[i] = tolower((unsigned char)input[i]);
        }
        
        // Check input
        if (strcmp(input, "y") == 0 || strcmp(input, "yes") == 0) {
            return 1;
        }
        else if (strcmp(input, "n") == 0 || strcmp(input, "no") == 0) {
            return 0;
        }
        
        printf("Invalid input. Please enter 'y' or 'n'.\n");
    }
}

void to_lowercase(char *str) {
    if (str == NULL) return;
    
    for (int i = 0; str[i]; i++) {
        str[i] = tolower((unsigned char)str[i]);
    }
}

void clear_screen() {
    printf("\033[2J");    // Clear entire screen
    printf("\033[H");     // Move cursor to top-left corner
}