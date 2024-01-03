#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ADMIN "admin"
#define ADMIN_PASS "1234"

#define ROLL_NUMBER_LENGTH 10
#define HASH_TABLE_SIZE 5
int table_size = HASH_TABLE_SIZE;

// students voting information
typedef struct Block {
    char roll_number[ROLL_NUMBER_LENGTH + 1];
    int vote;
    struct Block* next;
} Block;

typedef struct Block* block;

typedef struct HashTable {
    int size;
    block* table;
} HashTable;

typedef struct Candidate {
    char name[20];
} Candidate;

 // Create a hash table
HashTable* hashtable(){
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    hash_table->size = table_size;
    hash_table->table = (Block**)malloc(table_size * sizeof(Block*));
    return hash_table;
}

// upto here

struct noderec {
    char name[50];
    struct noderec *next;
};

typedef struct noderec *node;
typedef struct noderec *header;

header students, voters;
node new, temp, start, last, prev;

// Stack structure
struct StackNode {
    char name[50];
    struct StackNode *next;
};

typedef struct StackNode *Stack;

// Global undo and redo stacks
Stack undoStack = NULL;
Stack redoStack = NULL;



// Function prototypes
void push(Stack *s, char *name);
char *pop(Stack *s);
int isStackEmpty(Stack s);

void createList(header *h) {
    *h = (struct noderec *)malloc(sizeof(struct noderec));
    (*h)->next = NULL;
    start = last = NULL;
}

int isEmpty(header h) {
    return (h->next == NULL);
}

void insertEnd(header h, char *name) {
    new = (struct noderec *)malloc(sizeof(node));
    strcpy(new->name, name);
    new->next = NULL;
    if (isEmpty(h)) {
        h->next = new;
        start = last = new;
    } else {
        last->next = new;
        last = new;
    }
}

void deleteList(header h) {
    if (!isEmpty(h)) {
        temp = start;
        prev = start;
        while (temp != NULL) {
            prev = temp;
            temp = temp->next;
            free(prev);
        }
        free(temp);
    } else {
        printf("List is already empty\n");
    }
}

void display(header h) {
    if (!isEmpty(h)) {
        temp = start;
        while (temp != NULL) {
            printf("%s\t", temp->name);
            temp = temp->next;
        }
    } else {
        printf("List is empty\n");
    }
    printf("\n");
}

void admin();

void after_log();

int checkAdminPass(char *password) {
    return (strcmp(ADMIN_PASS, password) == 0);
}

int checkAdminName(char *username) {
    return (strcmp(ADMIN, username) == 0);
}

void addVoters(header h) {
    printf("Enter the name of the voter:");
    char voter_name[50];
    scanf("%s", voter_name);
    insertEnd(h, voter_name);
    push(&undoStack, voter_name);
    // Clear the redo stack when a new operation is performed
    while (!isStackEmpty(redoStack)) {
        pop(&redoStack);
    }
    after_log();
}

void undo(header h) {
    if (!isStackEmpty(undoStack)) {
        // Pop from the undo stack and push onto the redo stack
        char *removedName = pop(&undoStack);
        push(&redoStack, removedName);

        // Clear the list and reinsert the names from the undo stack
        deleteList(h);
        Stack tempStack = NULL;
        while (!isStackEmpty(undoStack)) {
            char *name = pop(&undoStack);
            insertEnd(h, name);
            push(&tempStack, name);
        }

        // Restore the undo stack
        while (!isStackEmpty(tempStack)) {
            char *name = pop(&tempStack);
            push(&undoStack, name);
        }

        printf("Undo: %s removed\n", removedName);
        after_log();
    } else {
        printf("Undo stack is empty\n");
        after_log();
    }
}

void redo(header h) {
    if (!isStackEmpty(redoStack)) {
        // Pop from the redo stack and push onto the undo stack
        char *restoredName = pop(&redoStack);
        push(&undoStack, restoredName);

        // Clear the list and reinsert the names from the undo stack
        deleteList(h);
        Stack tempStack = NULL;
        while (!isStackEmpty(undoStack)) {
            char *name = pop(&undoStack);
            insertEnd(h, name);
            push(&tempStack, name);
        }

        // Restore the undo stack
        while (!isStackEmpty(tempStack)) {
            char *name = pop(&tempStack);
            push(&undoStack, name);
        }

        printf("Redo: %s restored\n", restoredName);
        after_log();
    } else {
        printf("Redo stack is empty\n");
        after_log();
    }
}

void display_candidates(header h) {
    display(h);
    after_log();
}
void display_voters(header h){
    display(h);
}

void push(Stack *s, char *name) {
    struct StackNode *newNode = (struct StackNode *)malloc(sizeof(struct StackNode));
    strcpy(newNode->name, name);
    newNode->next = *s;
    *s = newNode;
}

char *pop(Stack *s) {
    if (isStackEmpty(*s)) {
        return NULL;
    }

    struct StackNode *temp = *s;
    char *name = (char *)malloc(sizeof(char) * 50);
    strcpy(name, temp->name);
    *s = temp->next;
    free(temp);
    return name;
}

int isStackEmpty(Stack s) {
    return s == NULL;
}

int hash(char* roll_number, int table_size) {
    int hash_value = 0;
    for (int i = 0; i < ROLL_NUMBER_LENGTH; i++) {
        hash_value += roll_number[i];
    }
    return hash_value % table_size;
}

// creating a new block to store the rollnumber and vote of the candidate. 

Block* createBlock(char* roll_number, int vote) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    strncpy(new_block->roll_number, roll_number, ROLL_NUMBER_LENGTH);
    new_block->roll_number[ROLL_NUMBER_LENGTH] = '\0';  // Null-terminate the roll number
    new_block->vote = vote;
    new_block->next = NULL;
    return new_block;
}

// this function will insert the created block to the hash table .

void insertBlock(HashTable* hash_table, Block* new_block) {
    int index = hash(new_block->roll_number, hash_table->size);

    // Check if the roll number already exists in the linked list
    Block* current_block = hash_table->table[index];

    while (current_block != NULL) {
        if (strcmp(current_block->roll_number, new_block->roll_number) == 0) {
            // Roll number already exists, mark the block as "DELETED"
            current_block->vote = -1;  // Assuming -1 indicates "DELETED"
            return;
        }
        current_block = current_block->next;
    }

    // Insert at the beginning of the linked list at the index
    new_block->next = hash_table->table[index];
    hash_table->table[index] = new_block;
}

void calculateVoteResults(HashTable* hash_table, int* vote_results) {
    for (int i = 0; i < hash_table->size; i++) {
        Block* current_block = hash_table->table[i];
        while (current_block != NULL) {
            if (current_block->vote != -1) {  // Exclude "DELETED" votes
                vote_results[current_block->vote - 1]++;  // Assuming votes start from 1
            }
            current_block = current_block->next;
        }
    }
}

/*
void displayVoteResults(int* vote_results,header h, int num_candidates) {
    printf("Vote Results:\n");
    for (int i = 0; i < num_candidates; i++) {
        // Find the index with the maximum votes
        int max_index = 0;
        for (int j = 1; j < num_candidates; j++) {
            if (vote_results[j] > vote_results[max_index]) {
                max_index = j;
            }
        }
        // Display candidate name and vote count
        printf("%s: %d votes\n", h->name, vote_results[max_index]);
        // Set the vote count of the maximum voted candidate to -1 to ignore in the next iteration
        vote_results[max_index] = -1;
    }
}
*/
void displayVoteResults(int* vote_results, header h, int num_candidates) {
    printf("Vote Results:\n");
    node current_node = h->next; // Start from the first node after the header

    while (current_node != NULL && num_candidates > 0) {
        // Find the index with the maximum votes
        int max_index = 0;
        for (int j = 1; j < num_candidates; j++) {
            if (vote_results[j] > vote_results[max_index]) {
                max_index = j;
            }
        }
        // Display candidate name and vote count
        printf("%s: %d votes\n", current_node->name, vote_results[max_index]);
        // Set the vote count of the maximum voted candidate to -1 to ignore in the next iteration
        vote_results[max_index] = -1;

        current_node = current_node->next;
        num_candidates--;
    }
}


void displayHashTable(HashTable* hash_table) {
    for (int i = 0; i < hash_table->size; i++) {
        printf("Index %d:", i);
        Block* current_block = hash_table->table[i];
        while (current_block != NULL) {
            printf(" -> [%s, %d]", current_block->roll_number, current_block->vote);
            current_block = current_block->next;
        }
        printf(" -> NULL\n");
    }
}

void displayScammers(HashTable* hash_table) {
    printf("Scammer Details:\n");
    for (int i = 0; i < hash_table->size; i++) {
        Block* current_block = hash_table->table[i];
        while (current_block != NULL) {
            if (current_block->vote == -1) {
                printf("[%s, %d]\n", current_block->roll_number, current_block->vote);
            }
            current_block = current_block->next;
        }
    }
}
void student();
HashTable* table; 
// declaring the table as a global variable .
void menudriven(){
    printf("Welcome to the pool!!\n");
    printf("Lets start the voting!!\n");
    printf("select!!\n");
    printf("1. Admin\n");
    printf("2. Student\n");
    int option;
    scanf("%d", &option);
    switch (option) {
    case 1:
        //table = hashtable();  
        admin();
        break;
    case 2:
    // Call the student function here
        student();
        break;
    default:
        break;
    }

}


void admin() {
    printf("Welcome to the admin Portal!!\n");
    char username[50], password[50];
    printf("Enter your username:\n");
    scanf("%s", username);
    printf("Enter your password:\n");
    scanf("%s", password);
    if (checkAdminName(username) && checkAdminPass(password)) {
        after_log();
    } else {
        printf("Either Username or Password is Incorrect!!\n");
    }
}


void add_cand_voter(char * rollnumber,int vote){
// creating a block .
    block  new_block = createBlock(rollnumber,vote);
    //inserting the block into hash table 
     // table = hashtable();  
    insertBlock(table,new_block);


    printf("!!!!!!your vote has been recorded!!!!!\n");
    printf("!!!!thank you!!!!!!!\n ");
    menudriven();


    //displayVoteResults()
    //displayHashTable(table);


}

void student(){
    printf("Welcome to the student student portal !!\n");
    char rollnumber[12] ;
    int vote;
    printf("enter your rollnumber \n");
    scanf("%s",rollnumber);
    // display the candidates
    printf("select any one of the following candidates\n");
    display_voters(voters);
    scanf("%d",&vote);
    //we need to add the rollnumber and vote to the candidates using hash, linked list.

    add_cand_voter(rollnumber,vote);
}



void after_log() {
    printf("What you wanna do?\n");
    printf("1. Add candidates\n");
    printf("2. Undo\n");
    printf("3. Redo\n");
    printf("4. Display candidates\n");
    printf("5. Start Voting\n");
    printf("6. End Voting\n");
    printf("7. Exit to menu\n");
    int admin_option;
    int num_candidates = 5;
    int vote_results[num_candidates];
    scanf("%d", &admin_option);
    switch (admin_option) {
    case 1:
        addVoters(voters);
        break;
    case 2:
        undo(voters);
        break;
    case 3:
        redo(voters);
        break;
    case 4:
        display_candidates(voters);
        break;
    case 5:
        menudriven();
        break;
    case 6:
        // need to dispaly vote results.
        // Initialize vote results array
       printf("*************************************************************\n");
        for (int i = 0; i < num_candidates; i++) {
        vote_results[i] = 0;
        }
        calculateVoteResults(table, vote_results);

        displayVoteResults(vote_results,voters, num_candidates);
        
        printf("the hash table");
        displayHashTable(table);

        
        displayScammers(table);
        break;
    
    default:
        break;
    }
}



int main() {
    createList(&students);
    createList(&voters);
    table = hashtable();  
    menudriven();

    /*printf("Welcome to the pool!!\n");
    printf("Lets start the voting!!\n");
    printf("select!1\n");
    printf("1. Admin\n");
    printf("2. Student\n");
    int option;
    scanf("%d", &option);
    switch (option) {
    case 1:
        admin();
        break;
    case 2:
        // Call the student function here
        break;
    default:
        break;
    }*/
    return 0;
}