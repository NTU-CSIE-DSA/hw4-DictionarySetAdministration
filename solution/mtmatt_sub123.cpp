#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORD_LEN 10004

typedef struct trie_node {
  struct trie_node *ch[ALPHABET_SIZE];
  int ct_pass;
  int ct_end;
  char *min_str;
  int *heap;
  int heap_sz;
} TrieNode;

TrieNode *create_node() {
  TrieNode *node = (TrieNode *)malloc(sizeof(TrieNode));
  if (node == NULL)
    return NULL;
  for (int i = 0; i < ALPHABET_SIZE; i++)
    node->ch[i] = NULL;
  node->ct_pass = 0;
  node->ct_end = 0;
  node->min_str = NULL;
  node->heap = NULL;
  node->heap_sz = 0;
  return node;
}

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

TrieNode *rt;
char *global_min = NULL;

void insert(char *word) {
  TrieNode *node = rt;
  for (int i = 0; word[i]; i++) {
    int idx = word[i] - 'a';
    if (!node->ch[idx])
      node->ch[idx] = create_node();
    node = node->ch[idx];
    node->ct_pass++;
    if (!node->min_str || strcmp(word, node->min_str) < 0)
      node->min_str = word;
  }
  node->ct_end++;
  if (!global_min || strcmp(word, global_min) < 0)
    global_min = word;
}

bool check(char *word) {
  TrieNode *node = rt;
  for (int i = 0; word[i]; i++) {
    int idx = word[i] - 'a';
    if (!node->ch[idx])
      return false;
    node = node->ch[idx];
  }
  return node->ct_end > 0;
}

int prefix_search(char *prefix) {
  TrieNode *node = rt;
  for (int i = 0; prefix[i]; i++) {
    int idx = prefix[i] - 'a';
    if (!node->ch[idx])
      return 0;
    node = node->ch[idx];
  }
  return node->ct_pass;
}

char *lcp(char *word) {
  TrieNode *node = rt;
  TrieNode *deepest_node = rt;
  int max_depth = 0;
  int depth = 0;
  for (int i = 0; word[i]; i++) {
    int idx = word[i] - 'a';
    if (!node->ch[idx])
      break;
    node = node->ch[idx];
    depth++;
    if (node->ct_pass > 0) {
      deepest_node = node;
      max_depth = depth;
    }
  }
  if (max_depth == 0)
    return global_min;
  static char lcpResult[MAX_WORD_LEN];
  strncpy(lcpResult, word, max_depth);
  lcpResult[max_depth] = '\0';
  node = deepest_node;
  while (node->ct_end == 0) {
    bool found = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
      if (node->ch[i]) {
        lcpResult[max_depth++] = 'a' + i;
        node = node->ch[i];
        found = true;
        break;
      }
    }
    if (!found)
      break;
  }
  lcpResult[max_depth] = '\0';
  return lcpResult;
}

int score(char *word) {
  TrieNode *node = rt;
  int sum = 0;
  for (int i = 0; word[i]; i++) {
    int idx = word[i] - 'a';
    if (!node->ch[idx])
      break;
    node = node->ch[idx];
    sum += node->ct_pass;
  }
  return sum;
}

int main() {
  rt = create_node();

  int q;
  scanf("%d", &q);
  int op;
  char str[MAX_WORD_LEN];
  for (int i = 0; i < q; ++i) {
    scanf("%d", &op);
    switch (op) {
    case 1:
      scanf("%s", str);
      insert(strdup(str));
      break;
    case 2:
      scanf("%s", str);
      printf("%s\n", check(str) ? "YES" : "NO");
      break;
    case 3:
      scanf("%s", str);
      printf("%d\n", prefix_search(str));
      break;
    case 4:
      scanf("%s", str);
      printf("%s\n", lcp(str));
      break;
    case 5:
      scanf("%s", str);
      printf("%d\n", score(str));
      break;
    }
  }
}
