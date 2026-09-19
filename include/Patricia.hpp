#ifndef PATRICIA_HPP
#define PATRICIA_HPP

#include <cstddef>
#include <string>
#include <vector>

struct PatriciaNode {
    std::string prefix;
    bool isEndOfWord;
    std::vector<PatriciaNode*> children;

    explicit PatriciaNode(const std::string& pref = "");
};

class Patricia {
private:
    PatriciaNode* root;

    void destroy(PatriciaNode* node);
    int commonPrefixLength(const std::string& s1, const std::string& s2) const;
    void insertHelper(PatriciaNode* node, const std::string& word);
    bool searchHelper(PatriciaNode* node, const std::string& word) const;
    bool startsWithHelper(PatriciaNode* node, const std::string& prefix) const;
    bool removeHelper(PatriciaNode* node, const std::string& word);
    std::size_t countNodesHelper(PatriciaNode* node) const;
    void toDotHelper(PatriciaNode* node, int nodeId, int& nextId, std::string& dot) const;

public:
    Patricia();
    ~Patricia();

    Patricia(const Patricia&) = delete;
    Patricia& operator=(const Patricia&) = delete;
    Patricia(Patricia&&) = delete;
    Patricia& operator=(Patricia&&) = delete;

    void insert(const std::string& key);
    bool search(const std::string& key) const;
    bool startsWith(const std::string& prefix) const;
    bool remove(const std::string& key);
    std::size_t nodeCount() const;
    std::string toDot() const;
};

#endif
