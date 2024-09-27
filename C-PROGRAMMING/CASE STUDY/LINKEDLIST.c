#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


struct node {
    int x;
    struct node *next;
};


//most important part ng code
struct node *head, *curr, *tail;


// Attempt to clear screen
void clearScreen() {
    system("cls || clear");
}


//the entire main menu
void displayMainMenu() {
    clearScreen();


    printf("\n\tMain Menu\n\n");
    printf(" A) Create Linear Linked List\n");
    printf(" B) Display Linear Linked List\n");
    printf(" C) Insert number to Linear Linked List\n");
    printf(" D) Delete number from Linear Linked List\n");
    printf(" E) Search number in Linear Linked List\n");
    printf(" S) Exit\n\n");
    printf(" Input > ");
}






void display_createLinkedList();
void createLinkedList();
void deleteEntireList(struct node **head);
void sortLinkedList(struct node *head);


void displayLinkedList();
void displayAddNum();
void displayRemoveNum();
void display_searchNumLinkedList();


void insertNumLinkedList(struct node **head, struct node **tail, int number);
void deleteNumLinkedList(struct node **head, struct node **tail, int number);
void searchNumLinkedList(struct node *head, int number);




//main na nag papafunction sa main menu
int main() {
    char userInput[10];


    while (1) {
        displayMainMenu();
        scanf("%s", userInput);
        getchar(); // Clear newline from buffer


        userInput[0] = toupper(userInput[0]);
        if (strlen(userInput) == 1) {
            switch (userInput[0]) {
                case 'A':
                    display_createLinkedList();
                    break;
                case 'B':
                    displayLinkedList();
                    break;
                case 'C':
                    displayAddNum();
                    break;
                case 'D':
                    displayRemoveNum();
                    break;
                case 'E':
                    display_searchNumLinkedList();
                    break;
                case 'S':
                    deleteEntireList(&head);
                    return 0;
                default:
                    printf("Invalid input. Please try again.\n");
                    getchar(); // Wait for user input before continuing
                    break;
            }
        } else {
            printf("Invalid input. Please try again.\n");
            getchar(); // Wait for user input before continuing
        }
    }


    return 0;
}




//so ito ung nag didisplay pag na select mo ung "A" which is createlinklist and meron ka nang existing na linklist
void display_createLinkedList() {
    if (head != NULL) {
        char userInput[10];
        clearScreen();
        printf("You already have a linked list. Do you want to overwrite? (Y/N): ");
        do {
            scanf("%s", userInput);
            getchar(); // Clear newline from buffer
            userInput[0] = toupper(userInput[0]);
            if (strlen(userInput) == 1 && (userInput[0] == 'Y' || userInput[0] == 'N')) {
                if (userInput[0] == 'Y') {
                    deleteEntireList(&head);
                }
                break;
            } else {
                printf("Invalid input. Please enter Y or N: ");
            }
        } while (1);
    }


    createLinkedList();
}


//ito naman ung pinaka main function ng createlinklist kumbaga ito na ung may "STOP" na part
void createLinkedList() {
    char userInput[10];
    int num;


    clearScreen();
    head = curr = tail = NULL;


    while (1) {
        curr = (struct node *) malloc(sizeof(struct node));
        printf("Enter a number or type 'stop' to finish: ");
        scanf("%s", userInput);
        getchar(); // Clear newline from buffer


        if (strcmp(userInput, "stop") == 0 || strcmp(userInput, "STOP") == 0) {
            free(curr);
            break;
        }


        num = atoi(userInput);
        if (num == 0 && strcmp(userInput, "0") != 0) {
            printf("Invalid input. Please enter a number.\n");
            free(curr);
            continue;
        }


        curr->x = num;


        if (head == NULL) {
            head = curr;
            tail = curr;
            head->next = NULL;
        } else {
            tail->next = curr;
            curr->next = NULL;
            tail = curr;
        }
    }


    sortLinkedList(head);
}


//now for this part ito ung nag sosort sa ascending order ng mga numbers
void sortLinkedList(struct node *head) {
    struct node *curr, *temp;
    int tempValue;


    if (head == NULL || head->next == NULL) {
        return;
    }


    for (curr = head; curr != NULL; curr = curr->next) {
        for (temp = curr->next; temp != NULL; temp = temp->next) {
            if (curr->x > temp->x) {
                tempValue = curr->x;
                curr->x = temp->x;
                temp->x = tempValue;
            }
        }
    }
}


//ito ung nag fufunction if mag cecreate ka ng new linklist, sha ung nag dedelete ng previous number para hnd na mag exist ung previous parts
void deleteEntireList(struct node **head) {
    struct node *temp, *curr;


    curr = *head;
    while (curr != NULL) {
        temp = curr->next;
        free(curr);
        curr = temp;
    }


    *head = NULL;
}


//ito ung nag didisplay ng numbers in ascending order to null(nothingness)
void displayLinkedList() {
    struct node *temp = head;


    clearScreen();
    if (temp == NULL) {
        printf("The list is empty.\n");
    } else {
        printf("Linked List Contents:\n");
        while (temp != NULL) {
            printf("%d -> ", temp->x);
            temp = temp->next;
        }
        printf("NULL\n");
    }
    printf("Press Enter to continue...");
    getchar(); // Wait for user input before continuing
}


//so ito naman ung part na nag aadd pa ng number if want mo pa ayan ito ung nag fufunction kapag inenter mo ung "C"
void displayAddNum() {
    char userInput[10];
    int number;


    clearScreen();
    while (1) {
        printf("Enter a number to insert or type 'stop' to finish: ");
        scanf("%s", userInput);
        getchar(); // Clear newline from buffer


        if (strcmp(userInput, "stop") == 0 || strcmp(userInput, "STOP") == 0) {
            break;
        }


        number = atoi(userInput);
        if (number == 0 && strcmp(userInput, "0") != 0) {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }


        insertNumLinkedList(&head, &tail, number);
    }
}


//so itong part na toh it will only function if nag add ka ng new number, so what it does is sinisingit nya ung sarili nya para maayos in to the ascending form
void insertNumLinkedList(struct node **head, struct node **tail, int number) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    newNode->x = number;
    newNode->next = NULL;


    if (*head == NULL) {
        *head = newNode;
        *tail = newNode;
    } else if (number < (*head)->x) {
        newNode->next = *head;
        *head = newNode;
    } else if (number > (*tail)->x) {
        (*tail)->next = newNode;
        *tail = newNode;
    } else {
        struct node *temp = *head;
        while (temp->next != NULL && temp->next->x < number) {
            temp = temp->next;
        }
        newNode->next = temp->next;
        temp->next = newNode;
    }
}


//so itong part na toh hnd pa sha ung mismong nag dedelete ng number sa list, sha lang ung parang nag hahanap or nakikipag interact dun sa user kung ano ung want nyang idelete
void displayRemoveNum() {
    char userInput[10];
    int number;


    clearScreen();
    printf("Enter a number to remove: ");
    scanf("%s", userInput);
    getchar(); // Clear newline from buffer


    number = atoi(userInput);
    if (number == 0 && strcmp(userInput, "0") != 0) {
        printf("Invalid input. Please enter a number.\n");
    } else {
        deleteNumLinkedList(&head, &tail, number);
    }


    printf("Press Enter to continue...");
    getchar(); // Wait for user input before continuing
}


// and this part ito na talaga ung nag dedelete sa number ito na ung code na nag fufunction if want mo talagang idelete ung number na want mong idelete
void deleteNumLinkedList(struct node **head, struct node **tail, int number) {
    struct node *temp = *head, *prev = NULL;


    if (temp != NULL && temp->x == number) {
        *head = temp->next;
        free(temp);
        printf("The number %d is successfully deleted.\n", number);
        return;
    }


    while (temp != NULL && temp->x != number) {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL) {
        printf("The number you're trying to delete does not exist.\n");
        return;
    }


    prev->next = temp->next;
    free(temp);


    if (*tail == temp) {
        *tail = prev;
    }


    printf("The number %d is successfully deleted.\n", number);
}


//so same function toh dun sa delete ito lang ung parang nakikipag interact sa user kung ano ung number na want mong ipahanap
void display_searchNumLinkedList() {
    char userInput[10];
    int number;


    clearScreen();
    printf("Enter a number to search: ");
    scanf("%s", userInput);
    getchar(); // Clear newline from buffer


    number = atoi(userInput);
    if (number == 0 && strcmp(userInput, "0") != 0) {
        printf("Invalid input. Please enter a number.\n");
    } else {
        searchNumLinkedList(head, number);
    }


    printf("Press Enter to continue...");
    getchar(); // Wait for user input before continuing
}


//while this one ito na talaga ung code na nag fufunction para hanapin ung number na inenter mo if its already existing on the list or hind pa
void searchNumLinkedList(struct node *head, int number) {
    struct node *temp = head;


    while (temp != NULL) {
        if (temp->x == number) {
            printf("Number %d is found in the list.\n", number);
            return;
        }
        temp = temp->next;
    }


    printf("Number %d is not found in the list.\n", number);
}
