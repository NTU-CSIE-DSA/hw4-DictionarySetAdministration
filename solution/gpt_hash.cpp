#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_STR_LEN 10000
#define MAX_TOTAL_LEN 300000
#define HASH_SIZE 100003

typedef struct HashNode {
    char* str;
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* table[HASH_SIZE];
} HashSet;

// Utility functions for HashSet
unsigned long hashFunction(const char* str) {
    unsigned long hash = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        hash = (hash * 31 + (str[i] - 'a' + 1)) % HASH_SIZE;
    }
    return hash;
}

HashSet* createHashSet() {
    HashSet* set = (HashSet*)malloc(sizeof(HashSet));
    for (int i = 0; i < HASH_SIZE; i++) {
        set->table[i] = NULL;
    }
    return set;
}

bool hashSetInsert(HashSet* set, const char* str) {
    unsigned long hash = hashFunction(str);
    HashNode* node = set->table[hash];
    while (node) {
        if (strcmp(node->str, str) == 0) {
            return false; // Already exists
        }
        node = node->next;
    }
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->str = strdup(str);
    newNode->next = set->table[hash];
    set->table[hash] = newNode;
    return true;
}

bool hashSetContains(HashSet* set, const char* str) {
    unsigned long hash = hashFunction(str);
    HashNode* node = set->table[hash];
    while (node) {
        if (strcmp(node->str, str) == 0) {
            return true;
        }
        node = node->next;
    }
    return false;
}

// Prefix_Search operation
int countPrefix(HashSet* set, const char* prefix) {
    int count = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = set->table[i];
        while (node) {
            if (strncmp(node->str, prefix, strlen(prefix)) == 0) {
                count++;
            }
            node = node->next;
        }
    }
    return count;
}

// Longest Common Prefix (LCP) operation
void findLCP(HashSet* set, const char* query, char* result) {
    char lcp[MAX_STR_LEN + 1] = "";
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = set->table[i];
        while (node) {
            int length = 0;
            while (query[length] != '\0' && node->str[length] != '\0' && query[length] == node->str[length]) {
                length++;
            }
            if (length > strlen(lcp) || (length == strlen(lcp) && strcmp(node->str, lcp) < 0)) {
                strncpy(lcp, node->str, length);
                lcp[length] = '\0';
            }
            node = node->next;
        }
    }
    strcpy(result, lcp);
}

// Score operation
int calculateScore(HashSet* set, const char* query) {
    int score = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = set->table[i];
        while (node) {
            int length = 0;
            while (query[length] != '\0' && node->str[length] != '\0' && query[length] == node->str[length]) {
                length++;
            }
            score += length;
            node = node->next;
        }
    }
    return score;
}

// Compress operation
int compress(HashSet* set) {
    int totalLength = 0;
    for (int i = 0; i < HASH_SIZE; i++) {
        HashNode* node = set->table[i];
        while (node) {
            totalLength += strlen(node->str);
            node = node->next;
        }
    }
    return totalLength;
}

int main() {
    int Q;
    scanf("%d", &Q);

    HashSet* dictionary = createHashSet();

    for (int i = 0; i < Q; i++) {
        int operation;
        char str[MAX_STR_LEN + 1];
        scanf("%d", &operation);

        if (operation == 1) { // Insert
            scanf("%s", str);
            hashSetInsert(dictionary, str);
        } else if (operation == 2) { // Check
            scanf("%s", str);
            printf("%s\n", hashSetContains(dictionary, str) ? "YES" : "NO");
        } else if (operation == 3) { // Prefix_Search
            scanf("%s", str);
            printf("%d\n", countPrefix(dictionary, str));
        } else if (operation == 4) { // LCP
            scanf("%s", str);
            char result[MAX_STR_LEN + 1];
            findLCP(dictionary, str, result);
            printf("%s\n", result);
        } else if (operation == 5) { // Score
            scanf("%s", str);
            printf("%d\n", calculateScore(dictionary, str));
        } else if (operation == 6) { // Compress
            printf("%d\n", compress(dictionary));
        }
    }

    return 0;
}

