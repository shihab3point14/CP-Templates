/**
 * Trie (Prefix Tree) for lowercase English letters
 * Insert: O(Length of word)
 * Search: O(Length of word)
 * Space: O(Sum of lengths of all words * Alphabet Size)
 */
struct Trie {
    struct Node {
        Node* child[26];
        int count;      // Number of words ending at this node
        int prefix_cnt; // Number of words sharing this prefix

        Node() {
            for (int i = 0; i < 26; i++) child[i] = nullptr;
            count = 0;
            prefix_cnt = 0;
        }
    };

    Node* root;

    Trie() {
        root = new Node();
    }

    // Inserts a word into the trie
    void insert(const string& s) {
        Node* curr = root;
        for (char c : s) {
            int idx = c - 'a';
            if (curr->child[idx] == nullptr) {
                curr->child[idx] = new Node();
            }
            curr = curr->child[idx];
            curr->prefix_cnt++;
        }
        curr->count++;
    }

    // Returns the number of times this exact word exists
    int search(const string& s) {
        Node* curr = root;
        for (char c : s) {
            int idx = c - 'a';
            if (curr->child[idx] == nullptr) return 0;
            curr = curr->child[idx];
        }
        return curr->count;
    }

    // Returns how many words start with prefix s
    int count_prefix(const string& s) {
        Node* curr = root;
        for (char c : s) {
            int idx = c - 'a';
            if (curr->child[idx] == nullptr) return 0;
            curr = curr->child[idx];
        }
        return curr->prefix_cnt;
    }
};