#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define N 300005
#define W 26
#define rep(i, n) for(long long i = 0; i < n; i++)

// Priority queue implementation
typedef struct {
    int* heap;
    int size;
    int capacity;
} PriorityQueue;

void initPQ(PriorityQueue* pq, int capacity) {
    pq->heap = (int*)malloc(sizeof(int) * capacity);
    pq->size = 0;
    pq->capacity = capacity;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(PriorityQueue* pq, int idx) {
    int largest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < pq->size && pq->heap[left] > pq->heap[largest])
        largest = left;

    if (right < pq->size && pq->heap[right] > pq->heap[largest])
        largest = right;

    if (largest != idx) {
        swap(&pq->heap[idx], &pq->heap[largest]);
        heapify(pq, largest);
    }
}

void push(PriorityQueue* pq, int val) {
    if (pq->size == pq->capacity) {
        pq->capacity *= 2;
        pq->heap = (int*)realloc(pq->heap, sizeof(int) * pq->capacity);
    }

    int i = pq->size;
    pq->heap[i] = val;
    pq->size++;

    while (i != 0 && pq->heap[(i - 1) / 2] < pq->heap[i]) {
        swap(&pq->heap[i], &pq->heap[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int pop(PriorityQueue* pq) {
    if (pq->size <= 0)
        return -1;

    if (pq->size == 1) {
        pq->size--;
        return pq->heap[0];
    }

    int root = pq->heap[0];
    pq->heap[0] = pq->heap[pq->size - 1];
    pq->size--;
    heapify(pq, 0);

    return root;
}

int top(PriorityQueue* pq) {
    if (pq->size <= 0)
        return -1;
    return pq->heap[0];
}

bool empty(PriorityQueue* pq) {
    return pq->size == 0;
}

void freePQ(PriorityQueue* pq) {
    free(pq->heap);
    pq->size = 0;
    pq->capacity = 0;
}

// Global variables
int n, ch[N][W], cnt[N], prefixsum[N], pos;
PriorityQueue pq[N];

void Initialize() {
    memset(ch, 0, sizeof(ch));
    memset(cnt, 0, sizeof(cnt));
    memset(prefixsum, 0, sizeof(prefixsum));
    pos = 0;

    for (int i = 0; i < N; i++) {
        initPQ(&pq[i], 100);
    }
}

void Insert(char* s) {
    int now = 0;
    prefixsum[0]++;

    for (int i = 0; s[i]; i++) {
        int to = s[i] - 'a';
        if (ch[now][to] == 0) {
            ch[now][to] = ++pos;
        }
        now = ch[now][to];
        prefixsum[now]++;
    }
    cnt[now]++;
}

bool Check(char* s) {
    int now = 0;

    for (int i = 0; s[i]; i++) {
        int to = s[i] - 'a';
        if (ch[now][to] == 0) {
            return false;
        }
        now = ch[now][to];
    }

    return cnt[now] > 0;
}

int PrefixSearch(char* s) {
    int now = 0;

    for (int i = 0; s[i]; i++) {
        int to = s[i] - 'a';
        if (ch[now][to] == 0) {
            return 0;
        }
        now = ch[now][to];
    }

    return prefixsum[now];
}

void LCP(char* s, char* result) {
    int now = 0;
    int lcpIndex = 0;
    
    // Match as much of the input string as possible
    for (int i = 0; s[i]; i++) {
        int to = s[i] - 'a';
        if (ch[now][to] == 0) {
            break;
        }
        result[lcpIndex++] = s[i];
        now = ch[now][to];
    }
    
    // If no common prefix or we've reached a complete word, we're done
    if (cnt[now] > 0) {
        result[lcpIndex] = '\0';
        return;
    }
    
    // Continue finding the lexicographically smallest path
    while (1) {
        // If we've found a complete word, stop
        if (cnt[now] > 0) {
            result[lcpIndex] = '\0';
            return;
        }
        
        // Find the lexicographically smallest child
        int smallestChild = -1;
        for (int i = 0; i < 26; i++) {
            if (ch[now][i]) {
                smallestChild = i;
                break;
            }
        }
        
        // If no children, we're done
        if (smallestChild == -1) {
            result[lcpIndex] = '\0';
            return;
        }
        
        // Add the smallest child to the result
        result[lcpIndex++] = 'a' + smallestChild;
        now = ch[now][smallestChild];
    }
}

long long Score(char* s) {
    long long ret = 0;
    int pre = prefixsum[0];
    int now = 0;
    int dep = 0;

    for (int i = 0; s[i]; i++) {
        int to = s[i] - 'a';
        if (ch[now][to] == 0) {
            break;
        }

        now = ch[now][to];
        if (prefixsum[now] != pre) {
            ret += (dep * (pre - prefixsum[now]));
        }

        dep++;
        pre = prefixsum[now];
    }

    ret += dep * prefixsum[now];
    return ret;
}

void Compress(int now, int dep) {
    for (int i = 0; i < 26; i++) {
        if (ch[now][i]) {
            int nxt = ch[now][i];
            Compress(nxt, dep + 1);

            while (!empty(&pq[nxt])) {
                int val = pop(&pq[nxt]);
                push(&pq[now], val);
            }
        }
    }

    if (!empty(&pq[now]) && now != 0 && cnt[now] == 0) {
        pop(&pq[now]);
        push(&pq[now], dep);
    } else {
        push(&pq[now], dep);
    }
}

void CleanupPQs() {
    for (int i = 0; i <= pos; i++) {
        freePQ(&pq[i]);
    }
}

int main() {
    Initialize();

    int Q;
    scanf("%d", &Q);

    char s[N];
    char result[N];

    for (int i = 0; i < Q; i++) {
        int ty;
        scanf("%d", &ty);

        if (ty == 1) {
            scanf("%s", s);
            Insert(s);
        } else if (ty == 2) {
            scanf("%s", s);
            if (Check(s)) {
                printf("YES\n");
            } else {
                printf("NO\n");
            }
        } else if (ty == 3) {
            scanf("%s", s);
            printf("%d\n", PrefixSearch(s));
        } else if (ty == 4) {
            scanf("%s", s);
            LCP(s, result);
            printf("%s\n", result);
        } else if (ty == 5) {
            scanf("%s", s);
            printf("%lld\n", Score(s));
        } else if (ty == 6) {
            Compress(0, 0);
            int ans = 0;
            while (!empty(&pq[0])) {
                ans += pop(&pq[0]);
            }
            printf("%d\n", ans);
        }
    }

    CleanupPQs();
    return 0;
}
