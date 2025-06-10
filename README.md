## Solution

* Subtask 1: Refer to problem statement.
* Subtask 2: While performing `dfs`, walk through the smallest lexicographical path after the first character fails to match the query string. Notice that if the last node matching the prefix of the query string is also a string in the dictionary set, it should be immediately returned.
* Subtask 3: While performing `prefix_search()`, you can maintain the counts for every prefix string in $O(1)$. While performing `score()`, you only need to update the score by adding depth $\cdot$ diff when the counts of prefix strings differ.
* Subtask 4: I apologize for underestimating the complexity of this subtask. Here, we first define an "**occupied**" node as the ending character of one of the strings in the dictionary set. A "compress" operation for a string involves moving an occupied node to a non-occupied node along its path to the root. Thus, when traversing the Trie bottom-up:
    * If we visit an occupied node, pass its depth and its children's depths to the parent.
    * If we visit a non-occupied node, which can be compressed into another deeper node, find the largest depth among its children and modify it to the current depth.
The final answer will be the sum of the depths at the root node.
This method can be implemented using a max-heap, so the total time complexity of the compression operation will be $O\left(\sum |S| \log \sum |S|\right)$.

Sample solution:

```c=
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
            DFS(Trie -> children[i]);
        }
    }
    return ;
}

int main(){
    int q;

    Node *Trie = createNode();

    int type;
    char inputStr[MAXL];
    for (int i=0; i<q; i++){
        if (type != 6){
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
                compressedLength += Trie -> heap[j];
            }
            printf("%d\n", compressedLength);
        }
    }

    return 0;
}

```
