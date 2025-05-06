# DSA HW4-3 Dictionary Set Administration

## Problem Description

The description on **NTU COOL** shall be considered the latest and official one. This version is provided for reference only.

We have learned some string-matching algorithms such as Rabin-Karp and KMP in class, which efficiently find substrings within larger strings. However, these algorithms are not designed for scenarios where we need to efficiently store and retrieve multiple strings based on their shared prefixes. For example, when implementing an autocomplete system, searching for strings with a given prefix or organizing a dictionary for efficient lookups becomes crucial. Such tasks require data structures specifically designed for prefix-based operations.

In this problem, we suggest you learn a new data structure called **Trie** that can address prefix searching more efficiently. The following description of the Trie might be helpful for you.

> **Definitions**
A Trie (pronounced as "try") is a specialized tree-based data structure used for storing and retrieving strings, typically in applications where prefix-based operations are crucial. Tries are particularly efficient for tasks such as autocomplete systems, spell checkers, or dictionary management.
> Let $T$ be a Trie storing a set of strings $S=\{s_1, s_2, \ldots, s_n\}$, where each string is composed of characters from a fixed alphabet $\Sigma$. 
> * Each node in the Trie represents a character of the strings in $S$.
> * Edges between nodes indicate the sequence of characters in the strings. 
> * Strings are stored implicitly through the paths from the root node to the leaf nodes. 
> 
> For example, if $S=\{$"cat", "car", "cart", "dog"$\}$, the Trie will branch at shared prefixes like "ca" and "car".

(The explanation figure is shown on NTU COOL).

> **Key properties of a Trie**
> * Prefix representation: Every prefix of a string in $S$ is represented in $T$ by the path from the root to the corresponding node.
> * Search efficiency: Finding a string or prefix of length $m$ in a Trie takes $O(m)$ time, regardless of the number of strings stored.
> * Space efficiency: Shared prefixes among strings are stored only once.
>
> **Implementation**
A Trie is constructed by inserting strings one character at a time. Each node stores a mapping of child nodes for each character in $\Sigma$. A boolean flag indicating whether the current node marks the end of a valid string. 
To insert a string $s$ into the Trie:
> 1. Start from the root node.
> 2. For each character $c$ in $s$:
If $c$ does not exist as a child of the current node, create a new node for $c$.
> 3. Move to the child node corresponding to $c$.
> 4. After processing all characters, mark the final node as the end of a string.

> To search for a string $s$ in the Trie:
> 1. Start from the root node.
> 2. Traverse the nodes corresponding to the characters in $s$.
> 3. If all characters are found and the final node is marked as the end of a string, $s$ exists in the Trie.

> For prefix-based operations, such as finding all strings with a given prefix $p$:
> 1. Traverse the Trie to find the node corresponding to the last character of $p$.
> 2. Perform a depth-first search (DFS) from this node to collect all strings that share the prefix $p$.

In this problem, we ask you to administrate an initially empty dictionary set using Trie to support the following six operations:

* **Insert** operation: Insert the query string into the dictionary set.
* **Check** operation: Check if the query string is in the dictionary set or not.
* **Prefix_Search** operation: Find the number of strings in the dictionary set that have the query string as a prefix.
* **LCP** operation: Find the string in the dictionary set that has the **longest common prefix** with the query string. If there are multiple answers, return the string with the smallest lexicographical order.
* **Score** operation: Find the score of the query string, where the score is defined as the total length of the common prefix of all strings in the dictionary set and the query string.
* **Compress** operation: Compress the whole dictionary set by replacing each string in the dictionary set with its non-empty prefix such that all new strings are still unique (the new strings can coincide with old strings). Return the smallest possible total length of the new strings. For example, assume there are four strings "hsinmu", "hsin", hsuantien", and "hello" in the dictionary set. After the Compress operation, `hsinmu` can be compressed to `hsi`, `hsin` can be compressed to `hs`, `hsuantien` can be compressed to `h`, `hello` can be compressed to `he`. The four compressed strings will be `hsi`, `hs`, `h`, and `he`, which are all unique, and the total length is 8.

## Input

The first line contains one integer $Q$, representing the total number of operations. Each of the next $Q$ lines is given in one of the following formats:

* `1 str`: Indicating an **Insert** operation with string `str`.
* `2 str`: Indicating a **Check** operation with string `str`.
* `3 prefix`: Indicating a **Prefix_Search** operation with string `prefix`.
* `4 str`: Indicating a **LCP** operation with string `str`.
* `5 str`: Indicating a **Score** operation with string `str`.
* `6`: Indicating a **Compress** operation.

## Output

* For each **Check** operation, print "YES" (brackets not included) in uppercase English letters in a single line if `str` is in the dictionary set. Otherwise, print "NO" (brackets not included) in uppercase English letters in a single line.
* For each **Prefix_Search** operation, print a non-negative integer in a single line representing the number of strings in the dictionary set that have `str` as a prefix.
* For each **LCP** operation, print a string in a single line representing the string in the dictionary set that has the **longest common prefix** with `str`.
* For each **Score** operation, print a non-negative integer in a single line representing the score of `str`.
* For each **Compress** operation, print a non-negative integer in a single line representing the smallest possible total length of the new strings.

## Constraints

- $1\leq Q\leq 10^4$
- The length of one single input string is not greater than $10^4$.
- The total length of input strings is not greater than $3\times 10^5$.
- The inserted strings are unique.
- Each string contains only lowercase English letters.
- The **Compress** operation will only called once and always be the last one, i.e. only the last operation will possibly be **Compress**.

## Subtasks

### Subtasks 1 (10 pts)

- Only **Insert**, **Check**, and **Prefix_Search** operations are implemented.

### Subtasks 2 (20 pts)

- Only **Insert**, **Check**, **Prefix_Search**, and **LCP** operations are implemented.

### Subtasks 3 (30 pts)

- Only **Insert**, **Check**, **Prefix_Search**, **LCP**, and **Score** operations are implemented.
 
### Subtasks 4 (40 pts)

- No other constraints

## Sample Testcases

### Sample Input 1

```
7
1 hsinmu
1 hsuantien
1 dsa
1 csie
2 dsa
2 ntu
3 hs
```

### Sample Output 1

```
YES
NO
2
```

### Sample Input 2

```
8
1 hsinmu
1 hsin
1 hsuantien
1 hello
4 hsinchiji
4 hsuchihmo
5 hell
6
```

### Sample Output 2

```
hsin
hsuantien
7
8
```

### Sample Explanation 2

* `4 hsinchiji` will find `hsin` as the longest common prefix with length=4. Note that even if `hsinmu` has also longest common prefix with length=4, it is lexicographically larger than `hsin`, so the answer should be `hsin`.
* `4 hsuchihmo` will find `hsuantien` as the longest common prefix with length=3.
* `5 hell` will score 1 on `hsinmu`, score 1 on `hsin`, score 1 on `hsuantien`, and score 4 on `hello`. Hence, the total score is 7.
* After the Compress operation, `hsinmu` can be compressed to `hsi`, `hsin` can be compressed to `hs`, `hsuantien` can be compressed to `h`, `hello` can be compressed to `he`. The four compressed strings will be `hsi`, `hs`, `h`, and `he`, which are all unique, and the total length is 8. It can be proved that there isn't any compression method to have shorter total length.

### Sample Input 3

```
10
1 cat
1 car
1 cart
1 dog
2 car
3 car
4 california
4 carbon
5 cartoon
6
```

### Sample Output 3

```
YES
2
car
car
8
7
```