#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#define MAXL 10004

typedef struct node {
    struct node *children[26];
    int asPrefix;
    bool isEnd;
    int *heap;
    int heapSize;
} Node;

void swap(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return ;
}

Node *createNode(){
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode){
        perror("Malloc trie node failed");
        exit(1);
    }

    for (int i=0; i<26; i++){
        newNode -> children[i] = NULL;
    }
    newNode -> asPrefix = 0;
    newNode -> isEnd = false;
    newNode -> heap = NULL;
    newNode -> heapSize = 0;
    return newNode;
}

void Insert(Node *Trie, char *str){
    Node *tmp = Trie;
    int len = strlen(str);
    for (int i=0; i<len; i++){
        int index = str[i] - 'a';
        if (!tmp -> children[index]){
            tmp -> children[index] = createNode();
        }
        tmp -> asPrefix++;
        tmp = tmp -> children[index];
    }
    assert(!tmp -> isEnd);
    tmp -> asPrefix++;
    tmp -> isEnd = true;
    return ;
}

Node *Check(Node *Trie, char *str){
    Node *tmp = Trie;
    int len = strlen(str);
    bool flag = true;
    for (int i=0; i<len; i++){
        int index = str[i] - 'a';
        if (!tmp -> children[index]){
            flag = false;
            break;
        }
        tmp = tmp -> children[index];
    }
    return flag ? tmp : NULL;
}

void PrefixSearch(Node *Trie, char *str){
    Node *target = Check(Trie, str);
    printf("%d\n", target ? target -> asPrefix : 0);
    return ;
}

void LCP(Node *Trie, char *str){
    Node *tmp = Trie;
    char lcp[MAXL];
    int lcpIndex = 0;
    int len = strlen(str);
    for (int i=0; i<len; i++){
        int index = str[i] - 'a';
        if (!tmp -> children[index]){
            break;
        }
        lcp[lcpIndex++] = str[i];
        tmp = tmp -> children[index];
    }
    while (!tmp -> isEnd){
        // fprintf(stderr, "Current substring: %s\n", lcp);
        bool flag = true;
        for (int i=0; i<26; i++){
            if (tmp -> children[i]){
                flag = false;
                lcp[lcpIndex++] = 'a' + i;
                tmp = tmp -> children[i];
                break;
            }
        }
        if (flag){
            break;
        }
    }
    // fprintf(stderr, "Final substring: %s\n", lcp);
    lcp[lcpIndex] = '\0';
    printf("%s\n", lcp);
    return ;
}

void Score(Node *Trie, char *str){
    Node *tmp = Trie;
    int score = 0;
    int len = strlen(str);
    for (int i=0; i<len; i++){
        int index = str[i] - 'a';
        if (!tmp -> children[index]){
            break;
        }
        tmp = tmp -> children[index];
        score += tmp -> asPrefix;
    }
    printf("%d\n", score);
    return ;
}

void BubbleUp(Node *cur, int index){
    int parent = (index - 1) / 2;
    if (index > 0 && cur -> heap[index] > cur -> heap[parent]){
        swap(&cur -> heap[index], &cur -> heap[parent]);
        BubbleUp(cur, parent);
    }
    return ;
}

void Heapify(Node *cur, int index){
    int largest = index, left = 2 * index + 1, right = 2 * index + 2;
    if (left < cur -> heapSize && cur -> heap[left] > cur -> heap[largest]){
        largest = left;
    }
    if (right < cur -> heapSize && cur -> heap[right] > cur -> heap[largest]){
        largest = right;
    }
    if (largest != index){
        swap(&cur -> heap[index], &cur -> heap[largest]);
        Heapify(cur, largest);
    }
    return ;
}

void Compress(Node *Trie, int depth){
    Node *tmp = Trie;
    int heapCapacity = 1;
    for (int i=0; i<26; i++){
        if (tmp -> children[i]){
            Node *child = tmp -> children[i];
            Compress(child, depth + 1);
            heapCapacity += child -> heapSize;
        }
    }
    tmp -> heap = (int *) malloc(sizeof(int) * heapCapacity);
    if (!tmp -> heap){
        perror("Malloc heap failed");
        exit(1);
    }
            
    for (int i=0; i<26; i++){
        if (tmp -> children[i]){
            Node *child = tmp -> children[i];
            while (child -> heapSize > 0){
                tmp -> heap[tmp -> heapSize++] = child -> heap[0];
                BubbleUp(tmp, tmp -> heapSize - 1);

                child -> heapSize--;
                swap(&child -> heap[0], &child -> heap[child -> heapSize]);
                Heapify(child, 0);
            }
            free(child -> heap);
        }
    }

    if (tmp -> heapSize > 0 && depth > 0 && !tmp -> isEnd){
        tmp -> heapSize--;
        swap(&tmp -> heap[0], &tmp -> heap[tmp -> heapSize]);
        Heapify(tmp, 0);

        tmp -> heap[tmp -> heapSize++] = depth;
        BubbleUp(tmp, tmp -> heapSize - 1);
    }
    else {
        tmp -> heap[tmp -> heapSize++] = depth;
        BubbleUp(tmp, tmp -> heapSize - 1);
    }

    return ;
}

void DFS(Node *Trie){
    if (!Trie){
        return ;
    }
    for (int i=0; i<26; i++){
        if (Trie -> children[i]){
            fprintf(stderr, "(%c, %d)\n", 'a' + i, Trie -> children[i] -> asPrefix);
            DFS(Trie -> children[i]);
        }
    }
    return ;
}

int main(){
    int q;
    assert(scanf("%d", &q) == 1);

    Node *Trie = createNode();

    int type;
    char inputStr[MAXL];
    for (int i=0; i<q; i++){
        assert(scanf("%d", &type) == 1);
        if (type != 6){
            assert(scanf("%s", inputStr) == 1);
            if (type == 1){
                Insert(Trie, inputStr);
            }
            else if (type == 2){
                Node *target = Check(Trie, inputStr);
                printf("%s\n", (target && target -> isEnd) ? "YES" : "NO");
            }
            else if (type == 3){
                PrefixSearch(Trie, inputStr);
            }
            else if (type == 4){
                LCP(Trie, inputStr);
            }
            else if (type == 5){
                Score(Trie, inputStr);
            }
            else {
                perror("Invalid type");
                exit(1);
            }
        }
        else {
            Compress(Trie, 0);
            int compressedLength = 0;
            for (int j=0; j<Trie -> heapSize; j++){
                // fprintf(stderr, "%d ", Trie -> heap[j]);
                compressedLength += Trie -> heap[j];
            }
            // fprintf(stderr, "\n");
            printf("%d\n", compressedLength);
        }
    }

    // DFS(Trie);

    return 0;
}

