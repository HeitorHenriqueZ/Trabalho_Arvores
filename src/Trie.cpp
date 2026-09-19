#include "Trie.hpp"

#include <iomanip>
#include <sstream>

namespace {
std::string trieLabel(int value) {
    if (value >= 32 && value <= 126) {
        char c = static_cast<char>(value);
        if (c == '"' || c == '\\') {
            return std::string("\\") + c;
        }
        return std::string(1, c);
    }
    std::ostringstream out;
    out << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << value;
    return out.str();
}
}

TrieNode::TrieNode() {
    isEndOfWord = false;
    for (int i = 0; i < 256; i++) {
        children[i] = nullptr;
    }
}

Trie::Trie() {
    root = new TrieNode();
}

Trie::~Trie() {
    destroy(root);
    root = nullptr;
}

void Trie::destroy(TrieNode* node) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < 256; i++) {
        if (node->children[i] != nullptr) {
            destroy(node->children[i]);
        }
    }
    delete node;
}

void Trie::insert(const std::string& key) {
    if (key.empty() || root == nullptr) {
        return;
    }

    TrieNode* current = root;
    for (unsigned char c : key) {
        int index = static_cast<int>(c);
        if (current->children[index] == nullptr) {
            current->children[index] = new TrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = true;
}

bool Trie::search(const std::string& key) const {
    if (key.empty() || root == nullptr) {
        return false;
    }

    TrieNode* current = root;
    for (unsigned char c : key) {
        int index = static_cast<int>(c);
        if (current->children[index] == nullptr) {
            return false;
        }
        current = current->children[index];
    }
    return current->isEndOfWord;
}

bool Trie::startsWith(const std::string& prefix) const {
    if (root == nullptr) {
        return false;
    }
    if (prefix.empty()) {
        return !isEmpty(root) || root->isEndOfWord;
    }

    TrieNode* current = root;
    for (unsigned char c : prefix) {
        int index = static_cast<int>(c);
        if (current->children[index] == nullptr) {
            return false;
        }
        current = current->children[index];
    }
    return true;
}

bool Trie::isEmpty(TrieNode* node) const {
    if (node == nullptr) {
        return true;
    }
    for (int i = 0; i < 256; i++) {
        if (node->children[i] != nullptr) {
            return false;
        }
    }
    return true;
}

bool Trie::removeHelper(TrieNode* node, const std::string& key, int depth) {
    if (node == nullptr) {
        return false;
    }

    if (depth == static_cast<int>(key.length())) {
        if (!node->isEndOfWord) {
            return false;
        }
        node->isEndOfWord = false;
        return isEmpty(node);
    }

    int index = static_cast<unsigned char>(key[static_cast<std::size_t>(depth)]);
    if (node->children[index] == nullptr) {
        return false;
    }

    if (removeHelper(node->children[index], key, depth + 1)) {
        delete node->children[index];
        node->children[index] = nullptr;
        return !node->isEndOfWord && isEmpty(node);
    }
    return false;
}

bool Trie::remove(const std::string& key) {
    if (!search(key)) {
        return false;
    }
    removeHelper(root, key, 0);
    return true;
}

std::size_t Trie::countNodesHelper(TrieNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    std::size_t total = 1;
    for (int i = 0; i < 256; i++) {
        total += countNodesHelper(node->children[i]);
    }
    return total;
}

std::size_t Trie::nodeCount() const {
    return countNodesHelper(root);
}

void Trie::toDotHelper(TrieNode* node, int nodeId, int& nextId, std::string& dot) const {
    for (int i = 0; i < 256; ++i) {
        TrieNode* child = node->children[i];
        if (child == nullptr) {
            continue;
        }
        int childId = nextId++;
        dot += "    n" + std::to_string(childId) + " [label=\"" + trieLabel(i) + "\"";
        if (child->isEndOfWord) {
            dot += ", shape=doublecircle";
        }
        dot += "];\n";
        dot += "    n" + std::to_string(nodeId) + " -> n" + std::to_string(childId) + ";\n";
        toDotHelper(child, childId, nextId, dot);
    }
}

std::string Trie::toDot() const {
    std::string dot = "digraph Trie {\n";
    dot += "    node [shape=circle];\n";
    dot += "    n0 [label=\"raiz\"];\n";
    if (root != nullptr) {
        int nextId = 1;
        toDotHelper(root, 0, nextId, dot);
    }
    dot += "}\n";
    return dot;
}
