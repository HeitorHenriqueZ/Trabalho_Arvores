#include "Splay.hpp"

#include <string>

namespace {
std::string splayNodeId(int key) {
    return "\"n" + std::to_string(key) + "\"";
}
}

SplayNode::SplayNode(int k) {
    key = k;
    left = nullptr;
    right = nullptr;
}

SplayTree::SplayTree() {
    root = nullptr;
}

SplayTree::~SplayTree() {
    destroy(root);
    root = nullptr;
}

void SplayTree::destroy(SplayNode* node) {
    while (node != nullptr) {
        if (node->left != nullptr) {
            SplayNode* left = node->left;
            node->left = left->right;
            left->right = node;
            node = left;
        } else {
            SplayNode* right = node->right;
            delete node;
            node = right;
        }
    }
}

SplayNode* SplayTree::rightRotate(SplayNode* x) {
    if (x == nullptr || x->left == nullptr) {
        return x;
    }
    SplayNode* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

SplayNode* SplayTree::leftRotate(SplayNode* x) {
    if (x == nullptr || x->right == nullptr) {
        return x;
    }
    SplayNode* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

SplayNode* SplayTree::splay(SplayNode* node, int key) {
    if (node == nullptr || node->key == key) {
        return node;
    }

    if (node->key > key) {
        if (node->left == nullptr) {
            return node;
        }

        if (node->left->key > key) {
            node->left->left = splay(node->left->left, key);
            node = rightRotate(node);
        } else if (node->left->key < key) {
            node->left->right = splay(node->left->right, key);
            if (node->left->right != nullptr) {
                node->left = leftRotate(node->left);
            }
        }
        return (node->left == nullptr) ? node : rightRotate(node);
    }

    if (node->right == nullptr) {
        return node;
    }

    if (node->right->key > key) {
        node->right->left = splay(node->right->left, key);
        if (node->right->left != nullptr) {
            node->right = rightRotate(node->right);
        }
    } else if (node->right->key < key) {
        node->right->right = splay(node->right->right, key);
        node = leftRotate(node);
    }
    return (node->right == nullptr) ? node : leftRotate(node);
}

bool SplayTree::insert(int key) {
    if (root == nullptr) {
        root = new SplayNode(key);
        return true;
    }

    root = splay(root, key);
    if (root->key == key) {
        return false;
    }

    SplayNode* newNode = new SplayNode(key);
    if (root->key > key) {
        newNode->right = root;
        newNode->left = root->left;
        root->left = nullptr;
    } else {
        newNode->left = root;
        newNode->right = root->right;
        root->right = nullptr;
    }
    root = newNode;
    return true;
}

bool SplayTree::search(int key) {
    if (root == nullptr) {
        return false;
    }
    root = splay(root, key);
    return root->key == key;
}

bool SplayTree::remove(int key) {
    if (root == nullptr) {
        return false;
    }

    root = splay(root, key);
    if (root->key != key) {
        return false;
    }

    SplayNode* temp = root;
    if (root->left == nullptr) {
        root = root->right;
    } else {
        root = splay(root->left, key);
        root->right = temp->right;
    }
    delete temp;
    return true;
}

int SplayTree::getRootKey() const {
    if (root == nullptr) {
        return -1;
    }
    return root->key;
}

std::optional<int> SplayTree::getRootKeyOpt() const {
    if (root == nullptr) {
        return std::nullopt;
    }
    return root->key;
}

bool SplayTree::isValidBSTHelper(SplayNode* node, long long minVal, long long maxVal) const {
    if (node == nullptr) {
        return true;
    }
    if (node->key <= minVal || node->key >= maxVal) {
        return false;
    }
    return isValidBSTHelper(node->left, minVal, node->key) &&
           isValidBSTHelper(node->right, node->key, maxVal);
}

bool SplayTree::isValidBST() const {
    return isValidBSTHelper(root, -9223372036854775807LL, 9223372036854775807LL);
}

void SplayTree::toDotHelper(SplayNode* node, int& nullCount, std::string& dot) const {
    if (node == nullptr) {
        return;
    }

    std::string nodeId = splayNodeId(node->key);
    dot += "    " + nodeId + " [label=\"" + std::to_string(node->key) + "\"];\n";

    if (node->left != nullptr) {
        std::string leftId = splayNodeId(node->left->key);
        dot += "    " + nodeId + " -> " + leftId + ";\n";
        toDotHelper(node->left, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }

    if (node->right != nullptr) {
        std::string rightId = splayNodeId(node->right->key);
        dot += "    " + nodeId + " -> " + rightId + ";\n";
        toDotHelper(node->right, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }
}

std::string SplayTree::toDot() const {
    std::string dot = "digraph SplayTree {\n";
    dot += "    node [shape=circle];\n";
    if (root != nullptr) {
        int nullCount = 0;
        toDotHelper(root, nullCount, dot);
    }
    dot += "}\n";
    return dot;
}
