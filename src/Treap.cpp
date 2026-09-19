#include "Treap.hpp"

#include <random>
#include <string>

namespace {
std::mt19937& treapRng() {
    thread_local static std::mt19937 gen{42};
    return gen;
}

int randomPriority() {
    static std::uniform_int_distribution<int> dist(1, 2147483646);
    return dist(treapRng());
}

std::string treapNodeId(int key) {
    return "\"n" + std::to_string(key) + "\"";
}
}

TreapNode::TreapNode(int k) {
    key = k;
    priority = randomPriority();
    left = nullptr;
    right = nullptr;
}

Treap::Treap() {
    root = nullptr;
}

Treap::~Treap() {
    destroy(root);
    root = nullptr;
}

void Treap::destroy(TreapNode* node) {
    while (node != nullptr) {
        if (node->left != nullptr) {
            TreapNode* left = node->left;
            node->left = left->right;
            left->right = node;
            node = left;
        } else {
            TreapNode* right = node->right;
            delete node;
            node = right;
        }
    }
}

TreapNode* Treap::rightRotate(TreapNode* y) {
    if (y == nullptr || y->left == nullptr) {
        return y;
    }
    TreapNode* x = y->left;
    TreapNode* t2 = x->right;
    x->right = y;
    y->left = t2;
    return x;
}

TreapNode* Treap::leftRotate(TreapNode* x) {
    if (x == nullptr || x->right == nullptr) {
        return x;
    }
    TreapNode* y = x->right;
    TreapNode* t2 = y->left;
    y->left = x;
    x->right = t2;
    return y;
}

TreapNode* Treap::insertHelper(TreapNode* node, int key, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new TreapNode(key);
    }

    if (key == node->key) {
        inserted = false;
        return node;
    }

    if (key < node->key) {
        node->left = insertHelper(node->left, key, inserted);
        if (node->left != nullptr && node->left->priority > node->priority) {
            node = rightRotate(node);
        }
    } else {
        node->right = insertHelper(node->right, key, inserted);
        if (node->right != nullptr && node->right->priority > node->priority) {
            node = leftRotate(node);
        }
    }
    return node;
}

bool Treap::insert(int key) {
    bool inserted = false;
    root = insertHelper(root, key, inserted);
    return inserted;
}

TreapNode* Treap::removeHelper(TreapNode* node, int key, bool& removed) {
    if (node == nullptr) {
        removed = false;
        return nullptr;
    }

    if (key < node->key) {
        node->left = removeHelper(node->left, key, removed);
    } else if (key > node->key) {
        node->right = removeHelper(node->right, key, removed);
    } else {
        removed = true;
        if (node->left == nullptr) {
            TreapNode* temp = node->right;
            delete node;
            return temp;
        }
        if (node->right == nullptr) {
            TreapNode* temp = node->left;
            delete node;
            return temp;
        }
        if (node->left->priority > node->right->priority) {
            node = rightRotate(node);
            node->right = removeHelper(node->right, key, removed);
        } else {
            node = leftRotate(node);
            node->left = removeHelper(node->left, key, removed);
        }
    }
    return node;
}

bool Treap::remove(int key) {
    bool removed = false;
    root = removeHelper(root, key, removed);
    return removed;
}

bool Treap::searchHelper(TreapNode* node, int key) const {
    if (node == nullptr) {
        return false;
    }
    if (node->key == key) {
        return true;
    }
    if (key < node->key) {
        return searchHelper(node->left, key);
    }
    return searchHelper(node->right, key);
}

bool Treap::search(int key) const {
    return searchHelper(root, key);
}

int Treap::getRootKey() const {
    return (root != nullptr) ? root->key : -1;
}

int Treap::getRootPriority() const {
    return (root != nullptr) ? root->priority : -1;
}

std::optional<int> Treap::getRootKeyOpt() const {
    if (root == nullptr) {
        return std::nullopt;
    }
    return root->key;
}

std::optional<int> Treap::getRootPriorityOpt() const {
    if (root == nullptr) {
        return std::nullopt;
    }
    return root->priority;
}

bool Treap::isValidBSTHelper(TreapNode* node, long long minVal, long long maxVal) const {
    if (node == nullptr) {
        return true;
    }
    if (node->key <= minVal || node->key >= maxVal) {
        return false;
    }
    return isValidBSTHelper(node->left, minVal, node->key) &&
           isValidBSTHelper(node->right, node->key, maxVal);
}

bool Treap::isValidBST() const {
    return isValidBSTHelper(root, -9223372036854775807LL, 9223372036854775807LL);
}

bool Treap::isValidHeapHelper(TreapNode* node) const {
    if (node == nullptr) {
        return true;
    }
    if (node->left != nullptr && node->left->priority > node->priority) {
        return false;
    }
    if (node->right != nullptr && node->right->priority > node->priority) {
        return false;
    }
    return isValidHeapHelper(node->left) && isValidHeapHelper(node->right);
}

bool Treap::isValidHeap() const {
    return isValidHeapHelper(root);
}

void Treap::toDotHelper(TreapNode* node, int& nullCount, std::string& dot) const {
    if (node == nullptr) {
        return;
    }

    std::string nodeId = treapNodeId(node->key);
    std::string label = std::to_string(node->key) + "\\n(p" + std::to_string(node->priority) + ")";
    dot += "    " + nodeId + " [label=\"" + label + "\"];\n";

    if (node->left != nullptr) {
        std::string leftId = treapNodeId(node->left->key);
        dot += "    " + nodeId + " -> " + leftId + ";\n";
        toDotHelper(node->left, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }

    if (node->right != nullptr) {
        std::string rightId = treapNodeId(node->right->key);
        dot += "    " + nodeId + " -> " + rightId + ";\n";
        toDotHelper(node->right, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }
}

std::string Treap::toDot() const {
    std::string dot = "digraph Treap {\n";
    dot += "    node [shape=circle];\n";
    if (root != nullptr) {
        int nullCount = 0;
        toDotHelper(root, nullCount, dot);
    }
    dot += "}\n";
    return dot;
}
