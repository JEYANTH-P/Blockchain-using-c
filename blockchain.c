#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROLL_NUMBER_LENGTH 10
#define HASH_TABLE_SIZE 5

// Define a structure for a block in the blockchain
typedef struct Block {
    int index;
    char roll_number[ROLL_NUMBER_LENGTH + 1];
    int vote;
    char timestamp[20];
    struct Block* next;
} Block;

// Define the hash table structure
typedef struct HashTable {
    int size;
    Block** table;
    int* hashed_values;  // Array to store hashed values
} HashTable;

// Define the structure for candidate information
typedef struct Candidate {
    char name[20];
} Candidate;

// Hash function
int hash(char* roll_number, int table_size) {
    int hash_value = 0;
    for (int i = 0; i < ROLL_NUMBER_LENGTH; i++) {
        hash_value += roll_number[i];
    }
    return hash_value % table_size;
}

// Create a new block for the blockchain
Block* createBlock(int index, char* roll_number, int vote, char* timestamp) {
    Block* new_block = (Block*)malloc(sizeof(Block));
    new_block->index = index;
    strncpy(new_block->roll_number, roll_number, ROLL_NUMBER_LENGTH);
    new_block->roll_number[ROLL_NUMBER_LENGTH] = '\0';  // Null-terminate the roll number
    new_block->vote = vote;
    strncpy(new_block->timestamp, timestamp, 20);
    new_block->next = NULL;
    return new_block;
}

// Insert a block into the hash table
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

    // Update the hashed value when inserting a new block
    if (hash_table->hashed_values[index] == 0) {
        hash_table->hashed_values[index] = hash(new_block->roll_number, hash_table->size);
    }
}

// Calculate the total number of votes for each person
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

// Display the vote results in descending order
void displayVoteResults(int* vote_results, Candidate* candidates, int num_candidates) {
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
        printf("%s: %d votes\n", candidates[max_index].name, vote_results[max_index]);
        // Set the vote count of the maximum voted candidate to -1 to ignore in the next iteration
        vote_results[max_index] = -1;
    }
}

// Display the contents of the hash table
void displayHashTable(HashTable* hash_table) {
    for (int i = 0; i < hash_table->size; i++) {
        printf("Index %d:", i);
        Block* current_block = hash_table->table[i];
        while (current_block != NULL) {
            printf(" -> [%s, %d, %s]", current_block->roll_number, current_block->vote, current_block->timestamp);
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
                printf("[%s, %d, %s]\n", current_block->roll_number, current_block->vote, current_block->timestamp);
            }
            current_block = current_block->next;
        }
    }
}


int main() {
    // Set the size of the hash table
    int table_size = HASH_TABLE_SIZE;
    int num_candidates = 5;

    // Create a hash table
    HashTable* hash_table = (HashTable*)malloc(sizeof(HashTable));
    hash_table->size = table_size;
    hash_table->table = (Block**)malloc(table_size * sizeof(Block*));
    hash_table->hashed_values = (int*)malloc(table_size * sizeof(int));

    // Initialize each slot in the hash table to NULL
    for (int i = 0; i < table_size; i++) {
        hash_table->table[i] = NULL;
        hash_table->hashed_values[i] = 0;  // Initialize hashed values to 0
    }

    // Create an array of candidates
    Candidate candidates[num_candidates];
    strcpy(candidates[0].name, "Candidate A");
    strcpy(candidates[1].name, "Candidate B");
    strcpy(candidates[2].name, "Candidate C");
    strcpy(candidates[3].name, "Candidate D");
    strcpy(candidates[4].name, "Candidate E");

    // Initialize vote results array
    int vote_results[num_candidates];
    for (int i = 0; i < num_candidates; i++) {
        vote_results[i] = 0;
    }

    // Simulate voters giving votes

            Block* new_block = createBlock(1, "2022115064", 2, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
             new_block = createBlock(1, "2022115064", 4, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
             new_block = createBlock(1, "2022115062", 1, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
             new_block = createBlock(1, "2022115061", 5, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
             new_block = createBlock(1, "2022115016", 5, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
             new_block = createBlock(1, "2022115060", 5, "2023-01-01 10:00:00");
            insertBlock(hash_table, new_block);
        

    // Calculate and display the vote results
    calculateVoteResults(hash_table, vote_results);
    displayVoteResults(vote_results, candidates, num_candidates);
    

    // Display the contents of the hash table
    displayHashTable(hash_table);
    displayScammers(hash_table);
    // Free allocated memory
    for (int i = 0; i < table_size; i++) {
        Block* current_block = hash_table->table[i];
        while (current_block != NULL) {
            Block* temp = current_block;
            current_block = current_block->next;
            free(temp);
        }
    }


    free(hash_table->table);
    free(hash_table->hashed_values);
    free(hash_table);

    return 0;
}
