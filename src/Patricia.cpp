#include "Patricia.hpp"

namespace {
std::string escapePatriciaLabel(const std::string& value) {
    std::string escaped;
    escaped.reserve(value.size());
    for (char c : value) {
        if (c == '"' || c == '\\') {
            escaped.push_back('\\');
        }
        escaped.push_back(c);
    }
    return escaped;
}
}

PatriciaNode::PatriciaNode(const std::string& pref) {
    prefix = pref;
    isEndOfWord = false;
}

Patricia::Patricia() {
    root = new PatriciaNode("");
}

Patricia::~Patricia() {
    destroy(root);
    root = nullptr;
}

void Patricia::destroy(PatriciaNode* node) {
    if (node == nullptr) {
        return;
    }
    for (PatriciaNode* child : node->children) {
        destroy(child);
    }
    node->children.clear();
    delete node;
}

int Patricia::commonPrefixLength(const std::string& s1, const std::string& s2) const {
    int i = 0;
    const int n1 = static_cast<int>(s1.length());
    const int n2 = static_cast<int>(s2.length());
    while (i < n1 && i < n2 && s1[static_cast<std::size_t>(i)] == s2[static_cast<std::size_t>(i)]) {
        i++;
    }
    return i;
}

void Patricia::insert(const std::string& key) {
    if (key.empty() || root == nullptr) {
        return;
    }
    insertHelper(root, key);
}

void Patricia::insertHelper(PatriciaNode* node, const std::string& word) {
    if (word.empty()) {
        node->isEndOfWord = true;
        return;
    }

    for (std::size_t i = 0; i < node->children.size(); i++) {
        PatriciaNode* child = node->children[i];
        int commonLen = commonPrefixLength(child->prefix, word);

        if (commonLen > 0) {
            if (commonLen == static_cast<int>(child->prefix.length())) {
                insertHelper(child, word.substr(static_cast<std::size_t>(commonLen)));
                return;
            }

            PatriciaNode* splitChild = new PatriciaNode(child->prefix.substr(static_cast<std::size_t>(commonLen)));
            splitChild->isEndOfWord = child->isEndOfWord;
            splitChild->children = child->children;

            child->prefix = child->prefix.substr(0, static_cast<std::size_t>(commonLen));
            child->isEndOfWord = false;
            child->children.clear();
            child->children.push_back(splitChild);

            if (commonLen < static_cast<int>(word.length())) {
                PatriciaNode* wordChild = new PatriciaNode(word.substr(static_cast<std::size_t>(commonLen)));
                wordChild->isEndOfWord = true;
                child->children.push_back(wordChild);
            } else {
                child->isEndOfWord = true;
            }
            return;
        }
    }

    PatriciaNode* newChild = new PatriciaNode(word);
    newChild->isEndOfWord = true;
    node->children.push_back(newChild);
}

bool Patricia::search(const std::string& key) const {
    if (key.empty() || root == nullptr) {
        return false;
    }
    return searchHelper(root, key);
}

bool Patricia::searchHelper(PatriciaNode* node, const std::string& word) const {
    if (word.empty()) {
        return node->isEndOfWord;
    }

    for (PatriciaNode* child : node->children) {
        int commonLen = commonPrefixLength(child->prefix, word);
        if (commonLen > 0) {
            if (commonLen == static_cast<int>(child->prefix.length())) {
                return searchHelper(child, word.substr(static_cast<std::size_t>(commonLen)));
            }
            return false;
        }
    }
    return false;
}

bool Patricia::startsWith(const std::string& prefix) const {
    if (root == nullptr) {
        return false;
    }
    if (root->children.empty() && !root->isEndOfWord) {
        return false;
    }
    if (prefix.empty()) {
        return true;
    }
    return startsWithHelper(root, prefix);
}

bool Patricia::startsWithHelper(PatriciaNode* node, const std::string& prefix) const {
    if (prefix.empty()) {
        return true;
    }

    for (PatriciaNode* child : node->children) {
        int commonLen = commonPrefixLength(child->prefix, prefix);
        if (commonLen > 0) {
            if (commonLen == static_cast<int>(prefix.length())) {
                return true;
            }
            if (commonLen == static_cast<int>(child->prefix.length())) {
                return startsWithHelper(child, prefix.substr(static_cast<std::size_t>(commonLen)));
            }
            return false;
        }
    }
    return false;
}

bool Patricia::remove(const std::string& key) {
    if (key.empty() || root == nullptr) {
        return false;
    }
    return removeHelper(root, key);
}

bool Patricia::removeHelper(PatriciaNode* node, const std::string& word) {
    if (word.empty()) {
        if (!node->isEndOfWord) {
            return false;
        }
        node->isEndOfWord = false;
        return true;
    }

    for (auto it = node->children.begin(); it != node->children.end(); ++it) {
        PatriciaNode* child = *it;
        int commonLen = commonPrefixLength(child->prefix, word);

        if (commonLen > 0) {
            if (commonLen != static_cast<int>(child->prefix.length())) {
                return false;
            }

            bool deleted = removeHelper(child, word.substr(static_cast<std::size_t>(commonLen)));
            if (!deleted) {
                return false;
            }

            if (!child->isEndOfWord && child->children.empty()) {
                delete child;
                node->children.erase(it);
            } else if (!child->isEndOfWord && child->children.size() == 1) {
                PatriciaNode* onlyChild = child->children[0];
                child->prefix += onlyChild->prefix;
                child->isEndOfWord = onlyChild->isEndOfWord;
                child->children = onlyChild->children;
                delete onlyChild;
            }
            return true;
        }
    }
    return false;
}

std::size_t Patricia::countNodesHelper(PatriciaNode* node) const {
    if (node == nullptr) {
        return 0;
    }
    std::size_t total = 1;
    for (PatriciaNode* child : node->children) {
        total += countNodesHelper(child);
    }
    return total;
}

std::size_t Patricia::nodeCount() const {
    return countNodesHelper(root);
}

void Patricia::toDotHelper(PatriciaNode* node, int nodeId, int& nextId, std::string& dot) const {
    for (PatriciaNode* child : node->children) {
        int childId = nextId++;
        dot += "    n" + std::to_string(childId) + " [label=\"" +
               escapePatriciaLabel(child->prefix) + "\"";
        if (child->isEndOfWord) {
            dot += ", shape=doublecircle";
        }
        dot += "];\n";
        dot += "    n" + std::to_string(nodeId) + " -> n" + std::to_string(childId) + ";\n";
        toDotHelper(child, childId, nextId, dot);
    }
}

std::string Patricia::toDot() const {
    std::string dot = "digraph Patricia {\n";
    dot += "    node [shape=box, style=rounded];\n";
    dot += "    n0 [label=\"raiz\"];\n";
    if (root != nullptr) {
        int nextId = 1;
        toDotHelper(root, 0, nextId, dot);
    }
    dot += "}\n";
    return dot;
}
