#include <iostream>
#include <cassert>
#include <climits>

#include "../include/Trie.hpp"
#include "../include/Patricia.hpp"
#include "../include/Splay.hpp"
#include "../include/Treap.hpp"
#include "../include/KDTree.hpp"

using namespace std;

void testTrie()
{
    cout << "Testando Trie..." << endl;

    {
        Trie trie;
        trie.insert("casa");
        trie.insert("casamento");
        trie.insert("carro");

        assert(trie.search("casa") == true);
        assert(trie.search("casamento") == true);
        assert(trie.search("carro") == true);
        assert(trie.search("cas") == false);

        assert(trie.remove("casa") == true);
        assert(trie.search("casa") == false);
        assert(trie.search("casamento") == true);
    }

    {
        Trie emptyTrie;
        assert(emptyTrie.search("") == false);
        assert(emptyTrie.search("algo") == false);
        assert(emptyTrie.startsWith("") == false);
        assert(emptyTrie.startsWith("a") == false);
        assert(emptyTrie.remove("algo") == false);
        assert(emptyTrie.remove("") == false);
    }

    {
        Trie trie;
        trie.insert("banana");
        trie.insert("bandana");
        trie.insert("bandeira");

        assert(trie.startsWith("") == true);
        assert(trie.startsWith("b") == true);
        assert(trie.startsWith("ba") == true);
        assert(trie.startsWith("ban") == true);
        assert(trie.startsWith("band") == true);
        assert(trie.startsWith("banda") == true);
        assert(trie.startsWith("bandan") == true);
        assert(trie.startsWith("banana") == true);
        assert(trie.startsWith("bandana") == true);
        assert(trie.startsWith("bandeira") == true);

        assert(trie.startsWith("c") == false);
        assert(trie.startsWith("bar") == false);
        assert(trie.startsWith("bandanas") == false);
        assert(trie.startsWith("bananinha") == false);
    }

    {
        Trie trie;
        trie.insert("teste");
        trie.insert("teste");
        trie.insert("teste");
        assert(trie.search("teste") == true);
        assert(trie.remove("teste") == true);
        assert(trie.search("teste") == false);
        assert(trie.remove("teste") == false);
    }

    {
        Trie trie;
        trie.insert("porta");
        assert(trie.remove("porto") == false);
        assert(trie.remove("por") == false);
        assert(trie.remove("portas") == false);
        assert(trie.remove("janela") == false);
        assert(trie.search("porta") == true);
    }

    {
        Trie trie;
        trie.insert("pote");
        trie.insert("potinho");
        trie.insert("potezao");

        assert(trie.remove("potezao") == true);
        assert(trie.search("potezao") == false);
        assert(trie.search("pote") == true);
        assert(trie.search("potinho") == true);

        assert(trie.remove("pote") == true);
        assert(trie.search("pote") == false);
        assert(trie.search("potinho") == true);
        assert(trie.startsWith("pot") == true);
        assert(trie.startsWith("pote") == false);

        assert(trie.remove("potinho") == true);
        assert(trie.search("potinho") == false);
        assert(trie.startsWith("pot") == false);

        trie.insert("porta");
        trie.insert("portaria");
        assert(trie.remove("porta") == true);
        assert(trie.search("porta") == false);
        assert(trie.search("portaria") == true);
        assert(trie.startsWith("porta") == true);
        assert(trie.remove("portaria") == true);
        assert(trie.search("portaria") == false);
        assert(trie.startsWith("porta") == false);
    }

    {
        Trie trie;
        trie.insert("roma");
        trie.insert("romano");
        string dot = trie.toDot();
        assert(dot.find("digraph Trie") != string::npos);
        assert(dot.find("doublecircle") != string::npos);
    }

    cout << "Trie OK!" << endl;
}

void testPatricia()
{
    cout << "Testando Patricia..." << endl;

    {
        Patricia pat;
        pat.insert("roma");
        pat.insert("romano");
        pat.insert("romeno");
        pat.insert("rato");

        assert(pat.search("roma") == true);
        assert(pat.search("romano") == true);
        assert(pat.search("romeno") == true);
        assert(pat.search("rato") == true);
        assert(pat.search("rom") == false);

        assert(pat.remove("romano") == true);
        assert(pat.search("romano") == false);
        assert(pat.search("roma") == true);
        assert(pat.search("romeno") == true);
    }

    {
        Patricia emptyPat;
        assert(emptyPat.search("") == false);
        assert(emptyPat.search("teste") == false);
        assert(emptyPat.startsWith("") == false);
        assert(emptyPat.startsWith("t") == false);
        assert(emptyPat.remove("") == false);
        assert(emptyPat.remove("teste") == false);
    }

    {
        Patricia pat;
        pat.insert("computador");
        pat.insert("computacao");
        pat.insert("compilador");
        pat.insert("composto");

        assert(pat.startsWith("") == true);
        assert(pat.startsWith("c") == true);
        assert(pat.startsWith("co") == true);
        assert(pat.startsWith("com") == true);
        assert(pat.startsWith("comp") == true);
        assert(pat.startsWith("compu") == true);
        assert(pat.startsWith("computa") == true);
        assert(pat.startsWith("computador") == true);
        assert(pat.startsWith("computacao") == true);
        assert(pat.startsWith("compilador") == true);
        assert(pat.startsWith("composto") == true);

        assert(pat.startsWith("computadores") == false);
        assert(pat.startsWith("computadx") == false);
        assert(pat.startsWith("cor") == false);
        assert(pat.startsWith("x") == false);
    }

    {
        Patricia pat;
        pat.insert("abc");
        pat.insert("abc");
        assert(pat.search("abc") == true);
        assert(pat.remove("abc") == true);
        assert(pat.search("abc") == false);
        assert(pat.remove("abc") == false);
    }

    {
        Patricia pat;
        pat.insert("casamento");
        pat.insert("casa");
        pat.insert("casarao");
        pat.insert("castelo");

        assert(pat.search("casamento") == true);
        assert(pat.search("casa") == true);
        assert(pat.search("casarao") == true);
        assert(pat.search("castelo") == true);
        assert(pat.search("cas") == false);
        assert(pat.search("casar") == false);
    }

    {
        Patricia pat;
        pat.insert("testing");
        pat.insert("tester");

        assert(pat.remove("tester") == true);
        assert(pat.search("tester") == false);
        assert(pat.search("testing") == true);
        assert(pat.startsWith("testi") == true);

        assert(pat.remove("testing") == true);
        assert(pat.search("testing") == false);
        assert(pat.startsWith("test") == false);
    }

    {
        Patricia pat;
        pat.insert("gato");
        assert(pat.remove("gat") == false);
        assert(pat.remove("gatos") == false);
        assert(pat.remove("cachorro") == false);
        assert(pat.search("gato") == true);
    }

    {
        Patricia pat;
        pat.insert("roma");
        pat.insert("romano");
        pat.insert("rato");
        string dot = pat.toDot();
        assert(dot.find("digraph Patricia") != string::npos);
        assert(dot.find("oma") != string::npos);
        assert(dot.find("ato") != string::npos);
    }

    cout << "Patricia OK!" << endl;
}

void testSplay()
{
    cout << "Testando Splay..." << endl;

    {
        SplayTree splay;
        splay.insert(10);
        splay.insert(20);
        splay.insert(30);

        assert(splay.search(10) == true);
        assert(splay.getRootKey() == 10);

        assert(splay.search(30) == true);
        assert(splay.getRootKey() == 30);

        splay.remove(20);
        assert(splay.search(20) == false);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree emptySplay;
        assert(emptySplay.search(10) == false);
        emptySplay.remove(10);
        assert(emptySplay.getRootKey() == -1);
        assert(!emptySplay.getRootKeyOpt().has_value());
        assert(emptySplay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(50);
        assert(splay.getRootKey() == 50);
        assert(splay.isValidBST() == true);
        assert(splay.search(50) == true);
        assert(splay.search(99) == false);
        assert(splay.getRootKey() == 50);
        splay.remove(50);
        assert(splay.search(50) == false);
        assert(splay.getRootKey() == -1);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        assert(splay.insert(100) == true);
        assert(splay.insert(100) == false);
        assert(splay.getRootKey() == 100);
        assert(splay.isValidBST() == true);
        assert(splay.remove(100) == true);
        assert(splay.search(100) == false);
        assert(splay.getRootKey() == -1);
    }

    {
        SplayTree splay;
        splay.insert(20);
        splay.insert(10);
        assert(splay.getRootKey() == 10);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(10);
        splay.insert(20);
        assert(splay.getRootKey() == 20);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(50);
        splay.insert(40);
        splay.insert(30);
        splay.insert(20);
        splay.insert(10);
        assert(splay.getRootKey() == 10);
        assert(splay.isValidBST() == true);

        assert(splay.search(50) == true);
        assert(splay.getRootKey() == 50);
        assert(splay.isValidBST() == true);

        assert(splay.search(10) == true);
        assert(splay.getRootKey() == 10);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(30);
        splay.insert(10);
        splay.insert(20);
        assert(splay.getRootKey() == 20);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(10);
        splay.insert(30);
        splay.insert(20);
        assert(splay.getRootKey() == 20);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        vector<int> chaves = {50, 25, 75, 10, 35, 60, 90};
        for (int k : chaves)
        {
            splay.insert(k);
            assert(splay.isValidBST() == true);
        }

        splay.remove(10);
        assert(splay.search(10) == false);
        assert(splay.isValidBST() == true);

        splay.remove(25);
        assert(splay.search(25) == false);
        assert(splay.isValidBST() == true);

        int currentRoot = splay.getRootKey();
        splay.remove(currentRoot);
        assert(splay.search(currentRoot) == false);
        assert(splay.isValidBST() == true);

        assert(splay.search(999) == false);
        assert(splay.isValidBST() == true);
        splay.remove(999);
        assert(splay.isValidBST() == true);
    }

    {
        SplayTree splay;
        splay.insert(-10);
        splay.insert(20);
        string dot = splay.toDot();
        assert(dot.find("digraph SplayTree") != string::npos);
        assert(dot.find("-10") != string::npos);
    }

    cout << "Splay OK!" << endl;
}

void testTreap()
{
    cout << "Testando Treap..." << endl;

    {
        Treap treap;
        treap.insert(50);
        treap.insert(30);
        treap.insert(20);
        treap.insert(40);
        treap.insert(70);

        assert(treap.search(40) == true);
        assert(treap.search(20) == true);
        assert(treap.search(90) == false);

        treap.remove(30);
        assert(treap.search(30) == false);
        assert(treap.search(40) == true);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);
    }

    {
        Treap emptyTreap;
        assert(emptyTreap.search(10) == false);
        emptyTreap.remove(10);
        assert(emptyTreap.getRootKey() == -1);
        assert(emptyTreap.getRootPriority() == -1);
        assert(!emptyTreap.getRootKeyOpt().has_value());
        assert(!emptyTreap.getRootPriorityOpt().has_value());
        assert(emptyTreap.isValidBST() == true);
        assert(emptyTreap.isValidHeap() == true);
    }

    {
        Treap treap;
        treap.insert(42);
        assert(treap.getRootKey() == 42);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);
        assert(treap.search(42) == true);
        treap.remove(42);
        assert(treap.search(42) == false);
        assert(treap.getRootKey() == -1);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);
    }

    {
        Treap treap;
        assert(treap.insert(33) == true);
        assert(treap.insert(33) == false);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);
        assert(treap.search(33) == true);
        assert(treap.remove(33) == true);
        assert(treap.search(33) == false);
        assert(treap.remove(33) == false);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);
    }

    {
        Treap treap;
        vector<int> valores = {50, 15, 75, 8, 25, 60, 95, 3, 12, 20, 30, 85, 100};

        for (int v : valores)
        {
            treap.insert(v);
            assert(treap.isValidBST() == true);
            assert(treap.isValidHeap() == true);
        }

        for (int v : valores)
        {
            assert(treap.search(v) == true);
        }

        treap.remove(999);
        treap.remove(-10);
        assert(treap.isValidBST() == true);
        assert(treap.isValidHeap() == true);

        for (int v : valores)
        {
            treap.remove(v);
            assert(treap.search(v) == false);
            assert(treap.isValidBST() == true);
            assert(treap.isValidHeap() == true);
        }

        assert(treap.getRootKey() == -1);
    }

    {
        Treap treap;
        for (int i = 0; i < 200; i++)
        {
            treap.insert((i * 37) % 500);
            assert(treap.isValidBST() == true);
            assert(treap.isValidHeap() == true);
        }
        for (int i = 0; i < 100; i++)
        {
            treap.remove((i * 37) % 500);
            assert(treap.isValidBST() == true);
            assert(treap.isValidHeap() == true);
        }
    }

    {
        Treap treap;
        treap.insert(-5);
        treap.insert(8);
        string dot = treap.toDot();
        assert(dot.find("digraph Treap") != string::npos);
        assert(dot.find("-5") != string::npos);
    }

    cout << "Treap OK!" << endl;
}

void testKDTree()
{
    cout << "Testando KDTree..." << endl;

    {
        KDTree kd;
        kd.insert(Point(3, 6));
        kd.insert(Point(17, 15));
        kd.insert(Point(13, 15));
        kd.insert(Point(6, 12));
        kd.insert(Point(9, 1));
        kd.insert(Point(2, 7));
        kd.insert(Point(10, 19));

        assert(kd.search(Point(10, 19)) == true);
        assert(kd.search(Point(13, 15)) == true);
        assert(kd.search(Point(10, 20)) == false);

        kd.remove(Point(13, 15));
        assert(kd.search(Point(13, 15)) == false);
        assert(kd.isValidKDTree() == true);
    }

    {
        KDTree emptyKD;
        assert(emptyKD.search(Point(0, 0)) == false);
        emptyKD.remove(Point(0, 0));
        assert(emptyKD.isValidKDTree() == true);
    }

    {
        KDTree kd;
        kd.insert(Point(15, 25));
        assert(kd.search(Point(15, 25)) == true);
        assert(kd.search(Point(15, 26)) == false);
        assert(kd.search(Point(14, 25)) == false);
        assert(kd.isValidKDTree() == true);
        kd.remove(Point(15, 25));
        assert(kd.search(Point(15, 25)) == false);
        assert(kd.isValidKDTree() == true);
    }

    {
        KDTree kd;
        assert(kd.insert(Point(8, 8)) == true);
        assert(kd.insert(Point(8, 8)) == false);
        assert(kd.search(Point(8, 8)) == true);
        assert(kd.isValidKDTree() == true);
        assert(kd.remove(Point(8, 8)) == true);
        assert(kd.search(Point(8, 8)) == false);
        assert(kd.remove(Point(8, 8)) == false);
        assert(kd.isValidKDTree() == true);
    }

    {
        KDTree kd;
        kd.insert(Point(30, 40));
        kd.insert(Point(10, 50));

        assert(kd.isValidKDTree() == true);
        kd.remove(Point(30, 40));
        assert(kd.search(Point(30, 40)) == false);
        assert(kd.search(Point(10, 50)) == true);
        assert(kd.isValidKDTree() == true);
    }

    {
        KDTree kd;
        vector<Point> pontos = {
            Point(30, 40), Point(5, 25), Point(70, 70),
            Point(10, 12), Point(50, 30), Point(35, 45),
            Point(80, 60), Point(55, 20), Point(20, 15)
        };

        for (const auto& pt : pontos)
        {
            kd.insert(pt);
            assert(kd.isValidKDTree() == true);
        }

        for (const auto& pt : pontos)
        {
            assert(kd.search(pt) == true);
        }

        kd.remove(Point(20, 15));
        assert(kd.search(Point(20, 15)) == false);
        assert(kd.isValidKDTree() == true);

        kd.remove(Point(70, 70));
        assert(kd.search(Point(70, 70)) == false);
        assert(kd.isValidKDTree() == true);
        assert(kd.search(Point(80, 60)) == true);

        kd.remove(Point(30, 40));
        assert(kd.search(Point(30, 40)) == false);
        assert(kd.isValidKDTree() == true);

        kd.remove(Point(999, 999));
        assert(kd.isValidKDTree() == true);

        for (const auto& pt : pontos)
        {
            kd.remove(pt);
            assert(kd.search(pt) == false);
            assert(kd.isValidKDTree() == true);
        }
    }

    {
        KDTree kd;
        vector<Point> quadPoints = {
            Point(-10, 20), Point(-30, -40), Point(15, -25), Point(0, 0), Point(-5, 12)
        };
        for (const auto& pt : quadPoints)
        {
            kd.insert(pt);
            assert(kd.isValidKDTree() == true);
        }
        for (const auto& pt : quadPoints)
        {
            assert(kd.search(pt) == true);
        }
        for (const auto& pt : quadPoints)
        {
            kd.remove(pt);
            assert(kd.search(pt) == false);
            assert(kd.isValidKDTree() == true);
        }
    }

    {
        KDTree kd;

        Point result;
        assert(kd.nearestNeighbor(Point(0, 0), result) == false);

        kd.insert(Point(2, 3));
        kd.insert(Point(5, 4));
        kd.insert(Point(9, 6));
        kd.insert(Point(4, 7));
        kd.insert(Point(8, 1));
        kd.insert(Point(7, 2));

        assert(kd.nearestNeighbor(Point(5, 5), result) == true);
        assert(result.coords[0] == 5 && result.coords[1] == 4);

        assert(kd.nearestNeighbor(Point(9, 6), result) == true);
        assert(result.coords[0] == 9 && result.coords[1] == 6);

        assert(kd.nearestNeighbor(Point(100, 100), result) == true);

        assert(result.coords[0] == 9 && result.coords[1] == 6);

        assert(kd.nearestNeighbor(Point(8, 2), result) == true);

        long long d1 = (long long)(result.coords[0] - 8) * (result.coords[0] - 8) +
                       (long long)(result.coords[1] - 2) * (result.coords[1] - 2);
        assert(d1 <= 1);
    }

    {
        KDTree kd;
        kd.insert(Point(INT_MIN, 0));
        kd.insert(Point(INT_MAX, 1));
        Point result;
        assert(kd.nearestNeighbor(Point(INT_MAX, 0), result) == true);
        assert(result.coords[0] == INT_MAX && result.coords[1] == 1);
    }

    {
        KDTree kd;

        vector<Point> res = kd.rangeSearch(Point(0, 0), Point(100, 100));
        assert(res.empty());

        kd.insert(Point(2, 3));
        kd.insert(Point(5, 4));
        kd.insert(Point(9, 6));
        kd.insert(Point(4, 7));
        kd.insert(Point(8, 1));
        kd.insert(Point(7, 2));

        res = kd.rangeSearch(Point(0, 0), Point(10, 10));
        assert(res.size() == 6);

        res = kd.rangeSearch(Point(9, 6), Point(9, 6));
        assert(res.size() == 1);
        assert(res[0].coords[0] == 9 && res[0].coords[1] == 6);

        res = kd.rangeSearch(Point(50, 50), Point(100, 100));
        assert(res.empty());

        res = kd.rangeSearch(Point(6, 0), Point(10, 3));

        assert(res.size() == 2);

        res = kd.rangeSearch(Point(0, 3), Point(5, 8));

        assert(res.size() == 3);

        res = kd.rangeSearch(Point(5, 4), Point(9, 6));

        assert(res.size() == 2);
    }

    {
        KDTree kd;
        kd.insert(Point(3, 6));
        kd.insert(Point(17, 15));
        kd.insert(Point(13, 15));

        string dot = kd.toDot();
        assert(!dot.empty());
        assert(dot.find("digraph") != string::npos);
        assert(dot.find("(3,6)") != string::npos);
        assert(dot.find("(17,15)") != string::npos);
        assert(dot.find("(13,15)") != string::npos);
    }

    cout << "KDTree OK!" << endl;
}

int main()
{
    cout << "===============================================" << endl;
    cout << "Iniciando Bateria Completa de Testes e Casos de Borda" << endl;
    cout << "===============================================" << endl;

    testTrie();
    testPatricia();
    testSplay();
    testTreap();
    testKDTree();

    cout << "===============================================" << endl;
    cout << "Todos os testes passaram com 100% de sucesso!" << endl;
    cout << "===============================================" << endl;
    return 0;
}
