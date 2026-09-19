#ifndef SPLAY_HPP
#define SPLAY_HPP

#include <optional>
#include <string>

struct SplayNode {
    int key;
    SplayNode* left;
    SplayNode* right;

    explicit SplayNode(int k);
};

class SplayTree {
private:
    SplayNode* root;

    void destroy(SplayNode* node);
    SplayNode* rightRotate(SplayNode* x);
    SplayNode* leftRotate(SplayNode* x);
    SplayNode* splay(SplayNode* node, int key);
    bool isValidBSTHelper(SplayNode* node, long long minVal, long long maxVal) const;

    void toDotHelper(SplayNode* node, int& nullCount, std::string& dot) const;

public:
    SplayTree();
    ~SplayTree();

    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;
    SplayTree(SplayTree&&) = delete;
    SplayTree& operator=(SplayTree&&) = delete;

    bool insert(int key);
    bool search(int key);
    bool remove(int key);

    int getRootKey() const;
    std::optional<int> getRootKeyOpt() const;
    bool isValidBST() const;

    std::string toDot() const;
};

#endif
