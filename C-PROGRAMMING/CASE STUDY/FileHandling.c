#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


#define MAX_ACCOUNTS 100
#define MAX_HISTORY 100
#define ADMIN_ACCOUNT_NUMBER 1234567890
#define ADMIN_PIN 999999


struct transaction {
    char transCode[10];
    float amount;
};


struct accounts {
    long number;
    char name[50];
    int pin;
    float accBalance;
    struct transaction history[MAX_HISTORY];
    int transactionCount;
};


struct accounts user[MAX_ACCOUNTS];
int accountCount = 0;


void saveAccounts();
void loadAccounts();
void pressEnterToContinue();
void clearScreen();
void clearInputBuffer();
void adminMenu();
void menu();
void option(char opt);
void newAcc();
void signIn();
void exitProgram();
void validatePin(long number, int pin);
void transMenu(int userIndex);
void transOption(char opt, int userIndex);
void BalInq(int userIndex);
void dep(int userIndex);
void withdr(int userIndex);
void viewAllAccounts();
void transferMoney();
bool accountNumberExists(long number);


void saveAccounts() {
    FILE *file = fopen("account.dat", "wb");
    if (file == NULL) {
        printf("Error opening file for saving accounts.\n");
        return;
    }
    fwrite(&accountCount, sizeof(int), 1, file);
    fwrite(user, sizeof(struct accounts), accountCount, file);
    fclose(file);
}


void loadAccounts() {
    FILE *file = fopen("account.dat", "rb");
    if (file == NULL) {
        return; // No data to load
    }
    fread(&accountCount, sizeof(int), 1, file);
    fread(user, sizeof(struct accounts), accountCount, file);
    fclose(file);
}


void pressEnterToContinue() {
    printf("Press Enter to continue...");
    while (getchar() != '\n');
    getchar();
}


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void viewAllAccounts() {
    clearScreen();
    printf("\n\tALL ACCOUNTS:\n");
    printf("\t--------------\n");


    for (int i = 0; i < accountCount; i++) {
        printf("\tAccount Number: %ld\n", user[i].number);
        printf("\tName: %s\n", user[i].name);
        printf("\tBalance: %.2f\n", user[i].accBalance);
        printf("\tTransactions: %d\n", user[i].transactionCount);
        printf("\t--------------\n");
    }


    pressEnterToContinue();
    adminMenu();
}


void transferMoney() {
    long fromAccount, toAccount;
    float amount;
    int fromIndex = -1, toIndex = -1;


    clearScreen();
    printf("\n\tTRANSFER MONEY\n");
    printf("\t-----------------\n");


    printf("Enter the account number to transfer from:\n");
    scanf("%ld", &fromAccount);
    printf("Enter the account number to transfer to:\n");
    scanf("%ld", &toAccount);


    for (int i = 0; i < accountCount; i++) {
        if (user[i].number == fromAccount) {
            fromIndex = i;
        }
        if (user[i].number == toAccount) {
            toIndex = i;
        }
    }


    if (fromIndex == -1 || toIndex == -1) {
        printf("Invalid account number.\n");
        pressEnterToContinue();
        clearScreen();
        adminMenu();
        return;
    }


    printf("Enter the amount to transfer:\n");
    scanf("%f", &amount);


    if (user[fromIndex].accBalance < amount) {
        printf("Insufficient funds.\n");
        pressEnterToContinue();
        clearScreen();
        adminMenu();
        return;
    }


    user[fromIndex].accBalance -= amount;
    user[toIndex].accBalance += amount;


    strcpy(user[fromIndex].history[user[fromIndex].transactionCount].transCode, "transfer");
    user[fromIndex].history[user[fromIndex].transactionCount].amount = amount;
    user[fromIndex].transactionCount++;


    strcpy(user[toIndex].history[user[toIndex].transactionCount].transCode, "received");
    user[toIndex].history[user[toIndex].transactionCount].amount = amount;
    user[toIndex].transactionCount++;


    printf("Transfer successful.\n");
    saveAccounts();
    pressEnterToContinue();
    clearScreen();
    adminMenu();
}


void menu() {
    char opt;
    clearScreen();


    printf("\n\n\t\t AUTOMATED TELLER MACHINE in C \n");
    printf("\n\n\t ------------------------------------------------------------------------------------------");
    printf("\n\n\t a. Create an account.");
    printf("\n\n\t b. Already have an account? Sign in!");
    printf("\n\n\t c. Exit.");
    printf("\n\n\t | Select an option: ");
    scanf(" %c", &opt);
    option(opt);
}


void option(char opt) {
    switch (opt) {
        case 'a':
            newAcc();
            break;
        case 'b':
            signIn();
            break;
        case 'c':
            exitProgram();
            break;
        default:
            clearScreen();
            printf("Invalid option.\n\n");
            menu();
            break;
    }
}


bool accountNumberExists(long number) {
    for (int i = 0; i < accountCount; i++) {
        if (user[i].number == number) {
            return true;
        }
    }
    return false;
}


void newAcc() {
    clearScreen();
    printf("\n\t | CREATE NEW ACCOUNT\n");
    printf("\t ---------------------\n");


    struct accounts newAcc;
    newAcc.transactionCount = 0;


    printf("\t | Enter account number: \n\t >>> ");
    scanf("%ld", &newAcc.number);


    if (accountNumberExists(newAcc.number)) {
        printf("\n\t >>> ACCOUNT NUMBER ALREADY EXISTS! Please try again.\n");
        pressEnterToContinue();
        menu();
        return;
    }


    printf("\t | Enter name: \n\t >>> ");
    clearInputBuffer();
    fgets(newAcc.name, sizeof(newAcc.name), stdin);
    newAcc.name[strcspn(newAcc.name, "\n")] = '\0';
    printf("\t | Enter a pin: \n\t >>> ");
    scanf("%d", &newAcc.pin);
    printf("\t | Enter amount to deposit: \n\t >>> ");
    scanf("%f", &newAcc.accBalance);


    user[accountCount] = newAcc;
    accountCount++;


    strcpy(user[accountCount-1].history[user[accountCount-1].transactionCount].transCode, "dep");
    user[accountCount-1].history[user[accountCount-1].transactionCount].amount = newAcc.accBalance;
    user[accountCount-1].transactionCount++;


    saveAccounts();


    printf("\n\t >>> ACCOUNT CREATED SUCCESSFULLY!\n");
    pressEnterToContinue();
    menu();
}


void signIn() {
    clearScreen();
    printf("\n\t | SIGN IN\n");


    long number;
    int pin;
    printf("\n\t | Enter your account number:\n\t >>> ");
    scanf("%ld", &number);
    printf("\t | Enter your pin:\n\t >>> ");
    scanf("%d", &pin);


    validatePin(number, pin);
}


void exitProgram() {
    clearScreen();
    printf("\n\t\t >>> Exiting the program...\n");
    saveAccounts();
    exit(0);
}


void validatePin(long number, int pin) {
    int i;
    int attempts = 3;


    while (attempts > 0) {
        for (i = 0; i < accountCount; i++) {
            if (user[i].number == number && user[i].pin == pin) {
                printf("\n\t |  VALID PIN! This account belongs to %s!\n", user[i].name);
                pressEnterToContinue();
                if (number == ADMIN_ACCOUNT_NUMBER && pin == ADMIN_PIN) {
                    adminMenu();
                } else {
                    transMenu(i);
                }
                return;
            }
        }
        if (number == ADMIN_ACCOUNT_NUMBER && pin == ADMIN_PIN) {
            printf("\n\t |  VALID PIN! This account belongs to Admin!\n");
            pressEnterToContinue();
            adminMenu();
            return;
        }
        attempts--;
        clearScreen();
        if (attempts > 0) {
            printf("\n    | INVALID PIN! You have %d out of 3 tries left.\n", attempts);
            printf("\n | Enter your account number:\n\t>>> ");
            scanf("%ld", &number);
            printf(" | Enter your pin:\n\t>>> ");
            scanf("%d", &pin);
        }
    }


    clearScreen();
    printf(" >>> Card is captured! Please visit your bank branch to retrieve your card.\n");
    pressEnterToContinue();
    clearScreen();
    menu();
}


void transMenu(int userIndex) {
    char opt;
    clearScreen();


    printf("\n\t\tTRANSACTION MENU\n");
    printf("\t\t----------------\n");
    printf("a. BALANCE INQUIRY\n");
    printf("b. DEPOSIT\n");
    printf("c. WITHDRAWAL\n");
    printf("d. Quit\n");
    printf(" | Select an option: ");
    scanf(" %c", &opt);


    transOption(opt, userIndex);
}


void transOption(char opt, int userIndex) {
    switch (opt) {
        case 'a':
            BalInq(userIndex);
            break;
        case 'b':
            dep(userIndex);
            break;
        case 'c':
            withdr(userIndex);
            break;
        case 'd':
            menu();
            break;
        default:
            clearScreen();
            printf("Invalid option.\n\n");
            transMenu(userIndex);
            break;
    }
}


void BalInq(int userIndex) {
    clearScreen();
    printf("\n\n\t BALANCE INQUIRY\n\n");
    printf(" | Your current balance is: %.2f\n", user[userIndex].accBalance);
    pressEnterToContinue();
    transMenu(userIndex);
}


void dep(int userIndex) {
    clearScreen();
    printf("\n\n\t DEPOSIT\n\n");


    float depositAmount;
    printf(" | Enter amount to deposit:\n\t>>> ");
    scanf("%f", &depositAmount);


    user[userIndex].accBalance += depositAmount;


    strcpy(user[userIndex].history[user[userIndex].transactionCount].transCode, "dep");
    user[userIndex].history[user[userIndex].transactionCount].amount = depositAmount;
    user[userIndex].transactionCount++;


    saveAccounts();


    printf("\n | DEPOSIT SUCCESSFUL!\n");
    pressEnterToContinue();
    transMenu(userIndex);
}


void withdr(int userIndex) {
    clearScreen();
    printf("\n\n\t WITHDRAWAL\n\n");


    float withdrawalAmount;
    printf(" | Enter amount to withdraw:\n\t>>> ");
    scanf("%f", &withdrawalAmount);


    if (withdrawalAmount > user[userIndex].accBalance) {
        printf("\n | INSUFFICIENT BALANCE!\n");
        pressEnterToContinue();
        transMenu(userIndex);
        return;
    }


    user[userIndex].accBalance -= withdrawalAmount;


    strcpy(user[userIndex].history[user[userIndex].transactionCount].transCode, "withdr");
    user[userIndex].history[user[userIndex].transactionCount].amount = withdrawalAmount;
    user[userIndex].transactionCount++;


    saveAccounts();


    printf("\n | WITHDRAWAL SUCCESSFUL!\n");
    pressEnterToContinue();
    transMenu(userIndex);
}


void adminMenu() {
    char opt;
    clearScreen();


    printf("ADMIN MENU\n\n");
    printf("a. View All Accounts\n");
    printf("b. Transfer Money\n");
    printf("c. Quit\n");
    printf(" | Select an option: ");
    scanf(" %c", &opt);


    switch (opt) {
        case 'a':
            viewAllAccounts();
            break;
        case 'b':
            transferMoney();
            break;
        case 'c':
            menu();
            break;
        default:
            clearScreen();
            printf("Invalid option.\n\n");
            adminMenu();
            break;
    }
}


int main() {
    loadAccounts();
    menu();
    return 0;
}

