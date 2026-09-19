#ifndef TRIE_HPP
#define TRIE_HPP

#include <cstddef>
#include <string>

struct TrieNode {
    TrieNode* children[256];
    bool isEndOfWord;

    TrieNode();
};

class Trie {
private:
    TrieNode* root;

    void destroy(TrieNode* node);
    bool removeHelper(TrieNode* node, const std::string& key, int depth);
    bool isEmpty(TrieNode* node) const;
    std::size_t countNodesHelper(TrieNode* node) const;
    void toDotHelper(TrieNode* node, int nodeId, int& nextId, std::string& dot) const;

public:
    Trie();
    ~Trie();

    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
    Trie(Trie&&) = delete;
    Trie& operator=(Trie&&) = delete;

    void insert(const std::string& key);
    bool search(const std::string& key) const;
    bool startsWith(const std::string& prefix) const;
    bool remove(const std::string& key);
    std::size_t nodeCount() const;
    std::string toDot() const;
};

#endif
