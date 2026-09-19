#ifndef TREAP_HPP
#define TREAP_HPP

#include <optional>
#include <string>

struct TreapNode {
    int key;
    int priority;
    TreapNode* left;
    TreapNode* right;

    explicit TreapNode(int k);
};

class Treap {
private:
    TreapNode* root;

    void destroy(TreapNode* node);
    TreapNode* rightRotate(TreapNode* y);
    TreapNode* leftRotate(TreapNode* x);
    TreapNode* insertHelper(TreapNode* node, int key, bool& inserted);
    TreapNode* removeHelper(TreapNode* node, int key, bool& removed);
    bool searchHelper(TreapNode* node, int key) const;
    bool isValidBSTHelper(TreapNode* node, long long minVal, long long maxVal) const;
    bool isValidHeapHelper(TreapNode* node) const;

    void toDotHelper(TreapNode* node, int& nullCount, std::string& dot) const;

public:
    Treap();
    ~Treap();

    Treap(const Treap&) = delete;
    Treap& operator=(const Treap&) = delete;
    Treap(Treap&&) = delete;
    Treap& operator=(Treap&&) = delete;

    bool insert(int key);
    bool search(int key) const;
    bool remove(int key);

    int getRootKey() const;
    int getRootPriority() const;
    std::optional<int> getRootKeyOpt() const;
    std::optional<int> getRootPriorityOpt() const;
    bool isValidBST() const;
    bool isValidHeap() const;

    std::string toDot() const;
};

#endif
