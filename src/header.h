#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#define MAX_LINE_LENGTH 500
#define MAX_ACCOUNTS 100
#define MAX_USERS 100
#define MAX_NAME_LENGTH 50
#define MAX_PASSWORD_LENGTH 50

// User struct definition
typedef struct {
    int id;
    char name[20];
    char password[50];
    Account account_ids[50];
} User;

// Account structure definition
typedef struct {
    int id;
    char user_name[MAX_NAME_LENGTH];
    char date_of_creation[20];
    char country[15];
    char phone[15];
    float balance;
    char type[20];
    char deposit_date [20];
} Account;

typedef enum {
    WITHDRAW,
    DEPOSIT,
    INVALID
} TransactionType;

TransactionType get_transaction_type();

// Authentication function prototypes
int login(User *users, int user_count, int *logged_in_user_id);
int register_user(User *users, int *user_count);

// Account management function prototypes
int create_account(Account *accounts, int *account_count, User *users, int user_id);
int generate_unique_account_id(Account *accounts, int account_count);
int update_account_information(Account *accounts, int account_count, int user_id);
void display_account_details(Account *accounts, int account_count);
int make_transaction(Account *accounts, int account_count, int user_id);
int remove_account(Account *accounts, int *account_count, int user_id);
int transfer_account_ownership(Account *accounts, int account_count, User *users, int user_count, int current_user_id);

// File handling function prototypes
void save_users_to_file(User *users, int user_count);
void save_accounts_to_file(Account *accounts, int account_count);
int load_users_from_file(User *users);
int load_accounts_from_file(Account *accounts);


int is_valid_input(const char *input);
int safe_input(const char *prompt, char *result, size_t max_length);
int safe_float_input(const char *prompt, float *result);
int safe_int_input(const char *prompt, int *result);
int safe_confirm(const char *prompt);
void to_lowercase(char *str);
// char* trim_whitespace(char *str);
// int is_alphabetic(const char *str);
// int is_numeric(const char *str);

void clear_screen();

#endif