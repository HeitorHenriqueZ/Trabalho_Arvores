#include <filesystem>
#include <fstream>
#include <string>

#include "KDTree.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "Trie.hpp"

namespace {
void writeDot(const std::string& name, const std::string& content) {
    std::ofstream file("docs/diagramas/dot/" + name + ".dot");
    file << content;
}
}

int main() {
    std::filesystem::create_directories("docs/diagramas/dot");

    Trie trie;
    trie.insert("roma");
    writeDot("trie_1", trie.toDot());
    trie.insert("romano");
    writeDot("trie_2", trie.toDot());
    trie.remove("roma");
    writeDot("trie_3", trie.toDot());

    Patricia patricia;
    patricia.insert("roma");
    patricia.insert("romano");
    writeDot("patricia_1", patricia.toDot());
    patricia.insert("rato");
    writeDot("patricia_2", patricia.toDot());
    patricia.remove("romano");
    writeDot("patricia_3", patricia.toDot());

    SplayTree splay;
    splay.insert(10);
    splay.insert(20);
    splay.insert(30);
    writeDot("splay_1", splay.toDot());
    splay.search(10);
    writeDot("splay_2", splay.toDot());
    splay.remove(20);
    writeDot("splay_3", splay.toDot());

    Treap treap;
    treap.insert(50);
    writeDot("treap_1", treap.toDot());
    treap.insert(30);
    treap.insert(40);
    writeDot("treap_2", treap.toDot());
    treap.remove(treap.getRootKey());
    writeDot("treap_3", treap.toDot());

    KDTree kd;
    kd.insert(Point(3, 6));
    kd.insert(Point(17, 15));
    writeDot("kdtree_1", kd.toDot());
    kd.insert(Point(13, 15));
    writeDot("kdtree_2", kd.toDot());
    kd.remove(Point(3, 6));
    writeDot("kdtree_3", kd.toDot());
}
