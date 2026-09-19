#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

#include "KDTree.hpp"
#include "Patricia.hpp"
#include "Splay.hpp"
#include "Treap.hpp"
#include "Trie.hpp"

using namespace std;
using namespace std::chrono;
using Clock = steady_clock;

namespace {

mt19937& rng() {
    static mt19937 gen(42);
    return gen;
}

string randomString(int length) {
    static const string alphabet = "abcdefghijklmnopqrstuvwxyz";
    uniform_int_distribution<int> dist(0, static_cast<int>(alphabet.size()) - 1);
    string res;
    res.reserve(static_cast<size_t>(length));
    for (int i = 0; i < length; i++) {
        res.push_back(alphabet[static_cast<size_t>(dist(rng()))]);
    }
    return res;
}

long long elapsedMs(const Clock::time_point& start,
                    const Clock::time_point& end) {
    return duration_cast<milliseconds>(end - start).count();
}

vector<string> uniqueRandomStrings(int n) {
    unordered_set<string> seen;
    vector<string> unique;
    unique.reserve(static_cast<size_t>(n));
    while (static_cast<int>(unique.size()) < n) {
        string w = randomString(5 + static_cast<int>(rng()() % 10));
        if (seen.insert(w).second) {
            unique.push_back(w);
        }
    }
    return unique;
}

vector<int> uniqueRandomInts(int n, int low, int high) {
    uniform_int_distribution<int> dist(low, high);
    unordered_set<int> seen;
    vector<int> nums;
    nums.reserve(static_cast<size_t>(n));
    while (static_cast<int>(nums.size()) < n) {
        int v = dist(rng());
        if (seen.insert(v).second) {
            nums.push_back(v);
        }
    }
    return nums;
}

vector<Point> uniqueRandomPoints(int n, int low, int high) {
    uniform_int_distribution<int> dist(low, high);
    unordered_set<long long> seen;
    vector<Point> pts;
    pts.reserve(static_cast<size_t>(n));
    while (static_cast<int>(pts.size()) < n) {
        Point p(dist(rng()), dist(rng()));
        uint64_t u0 = static_cast<uint32_t>(p.coords[0]);
        uint64_t u1 = static_cast<uint32_t>(p.coords[1]);
        long long key = static_cast<long long>((u0 << 32) ^ u1);
        if (seen.insert(key).second) {
            pts.push_back(p);
        }
    }
    return pts;
}

void writeRow(ofstream& csv, const string& name, int n, long long ins, long long busca, long long rem) {
    csv << name << ',' << n << ',' << ins << ',' << busca << ',' << rem << '\n';
}

void benchmarkTriePatricia(int n, ofstream& csv, ofstream& memCsv) {
    vector<string> words = uniqueRandomStrings(n);
    Trie trie;
    Patricia pat;

    auto start = Clock::now();
    for (const string& w : words) {
        trie.insert(w);
    }
    auto end = Clock::now();
    long long trieIns = elapsedMs(start, end);

    start = Clock::now();
    for (const string& w : words) {
        pat.insert(w);
    }
    end = Clock::now();
    long long patIns = elapsedMs(start, end);

    const size_t trieNodes = trie.nodeCount();
    const size_t patNodes = pat.nodeCount();

    start = Clock::now();
    for (const string& w : words) {
        volatile bool found = trie.search(w);
        (void)found;
    }
    end = Clock::now();
    long long trieBusca = elapsedMs(start, end);

    start = Clock::now();
    for (const string& w : words) {
        volatile bool found = pat.search(w);
        (void)found;
    }
    end = Clock::now();
    long long patBusca = elapsedMs(start, end);

    start = Clock::now();
    for (const string& w : words) {
        trie.remove(w);
    }
    end = Clock::now();
    long long trieRem = elapsedMs(start, end);

    start = Clock::now();
    for (const string& w : words) {
        pat.remove(w);
    }
    end = Clock::now();
    long long patRem = elapsedMs(start, end);

    cout << "Trie Insercao (" << n << "): " << trieIns << " ms\n";
    cout << "Patricia Insercao (" << n << "): " << patIns << " ms\n";
    cout << "Trie Busca (" << n << "): " << trieBusca << " ms\n";
    cout << "Patricia Busca (" << n << "): " << patBusca << " ms\n";
    cout << "Trie Remocao (" << n << "): " << trieRem << " ms\n";
    cout << "Patricia Remocao (" << n << "): " << patRem << " ms\n";
    cout << "Nos Trie/Patricia (" << n << "): " << trieNodes << "/" << patNodes << "\n";

    writeRow(csv, "Trie", n, trieIns, trieBusca, trieRem);
    writeRow(csv, "Patricia", n, patIns, patBusca, patRem);
    memCsv << "Trie," << n << "," << trieNodes << "\n";
    memCsv << "Patricia," << n << "," << patNodes << "\n";
}

void benchmarkSplayTreap(int n, ofstream& csv, ofstream& distCsv) {

    vector<int> nums = uniqueRandomInts(n, 1, 10000000);

    SplayTree splay;
    Treap treap;

    auto start = Clock::now();
    for (int num : nums) {
        splay.insert(num);
    }
    auto end = Clock::now();
    long long splayIns = elapsedMs(start, end);

    start = Clock::now();
    for (int num : nums) {
        treap.insert(num);
    }
    end = Clock::now();
    long long treapIns = elapsedMs(start, end);

    start = Clock::now();
    for (int num : nums) {
        volatile bool found = splay.search(num);
        (void)found;
    }
    end = Clock::now();
    long long splayBusca = elapsedMs(start, end);

    start = Clock::now();
    for (int num : nums) {
        volatile bool found = treap.search(num);
        (void)found;
    }
    end = Clock::now();
    long long treapBusca = elapsedMs(start, end);

    start = Clock::now();
    for (int num : nums) {
        splay.remove(num);
    }
    end = Clock::now();
    long long splayRem = elapsedMs(start, end);

    start = Clock::now();
    for (int num : nums) {
        treap.remove(num);
    }
    end = Clock::now();
    long long treapRem = elapsedMs(start, end);

    cout << "Splay Insercao (" << n << "): " << splayIns << " ms\n";
    cout << "Treap Insercao (" << n << "): " << treapIns << " ms\n";
    cout << "Splay Busca (" << n << "): " << splayBusca << " ms\n";
    cout << "Treap Busca (" << n << "): " << treapBusca << " ms\n";
    cout << "Splay Remocao (" << n << "): " << splayRem << " ms\n";
    cout << "Treap Remocao (" << n << "): " << treapRem << " ms\n";

    writeRow(csv, "Splay", n, splayIns, splayBusca, splayRem);
    writeRow(csv, "Treap", n, treapIns, treapBusca, treapRem);

    if (n <= 100000) {
        SplayTree splayOrd;
        Treap treapOrd;
        start = Clock::now();
        for (int i = 1; i <= n; i++) {
            splayOrd.insert(i);
        }
        end = Clock::now();
        long long splayOrdIns = elapsedMs(start, end);

        start = Clock::now();
        for (int i = 1; i <= n; i++) {
            treapOrd.insert(i);
        }
        end = Clock::now();
        long long treapOrdIns = elapsedMs(start, end);

        start = Clock::now();
        for (int i = 1; i <= n; i++) {
            volatile bool found = splayOrd.search(i);
            (void)found;
        }
        end = Clock::now();
        long long splayOrdBusca = elapsedMs(start, end);

        start = Clock::now();
        for (int i = 1; i <= n; i++) {
            volatile bool found = treapOrd.search(i);
            (void)found;
        }
        end = Clock::now();
        long long treapOrdBusca = elapsedMs(start, end);

        distCsv << "ordenado,Splay," << n << "," << splayOrdIns << "," << splayOrdBusca << "\n";
        distCsv << "ordenado,Treap," << n << "," << treapOrdIns << "," << treapOrdBusca << "\n";
        cout << "Ordenado Splay/Treap insert (" << n << "): " << splayOrdIns << "/" << treapOrdIns << " ms\n";
        cout << "Ordenado Splay/Treap busca (" << n << "): " << splayOrdBusca << "/" << treapOrdBusca << " ms\n";
    }

    if (n <= 100000) {
        SplayTree splayLocal;
        Treap treapLocal;
        for (int num : nums) {
            splayLocal.insert(num);
            treapLocal.insert(num);
        }
        const int hot = std::max(1, n / 5);
        bernoulli_distribution hotAccess(0.8);
        uniform_int_distribution<int> hotIndex(0, hot - 1);
        uniform_int_distribution<int> allIndex(0, n - 1);
        vector<int> queries;
        queries.reserve(static_cast<size_t>(n));
        for (int i = 0; i < n; i++) {
            if (hotAccess(rng())) {
                queries.push_back(nums[static_cast<size_t>(hotIndex(rng()))]);
            } else {
                queries.push_back(nums[static_cast<size_t>(allIndex(rng()))]);
            }
        }

        start = Clock::now();
        for (int q : queries) {
            volatile bool found = splayLocal.search(q);
            (void)found;
        }
        end = Clock::now();
        long long splayLocalBusca = elapsedMs(start, end);

        start = Clock::now();
        for (int q : queries) {
            volatile bool found = treapLocal.search(q);
            (void)found;
        }
        end = Clock::now();
        long long treapLocalBusca = elapsedMs(start, end);

        distCsv << "localidade_80_20,Splay," << n << ",0," << splayLocalBusca << "\n";
        distCsv << "localidade_80_20,Treap," << n << ",0," << treapLocalBusca << "\n";
        cout << "Localidade 80/20 Splay/Treap busca (" << n << "): "
             << splayLocalBusca << "/" << treapLocalBusca << " ms\n";
    }
}

void benchmarkKDTree(int n, ofstream& csv, ofstream& spatialCsv) {
    vector<Point> pts = uniqueRandomPoints(n, 0, 1000000);
    KDTree kd;

    auto start = Clock::now();
    for (const Point& pt : pts) {
        kd.insert(pt);
    }
    auto end = Clock::now();
    long long kdIns = elapsedMs(start, end);

    start = Clock::now();
    for (const Point& pt : pts) {
        volatile bool found = kd.search(pt);
        (void)found;
    }
    end = Clock::now();
    long long kdBusca = elapsedMs(start, end);

    int nnQueries = min(n, 10000);
    vector<Point> queryPts = uniqueRandomPoints(nnQueries, 0, 1000000);

    start = Clock::now();
    for (const Point& qpt : queryPts) {
        Point result;
        volatile bool found = kd.nearestNeighbor(qpt, result);
        (void)found;
    }
    end = Clock::now();
    long long kdNN = elapsedMs(start, end);

    uniform_int_distribution<int> baseDist(0, 990000);
    int rangeQueries = min(n, 10000);
    vector<pair<Point, Point>> rectangles;
    rectangles.reserve(static_cast<size_t>(rangeQueries));
    for (int i = 0; i < rangeQueries; i++) {
        int x0 = baseDist(rng());
        int y0 = baseDist(rng());
        rectangles.emplace_back(Point(x0, y0), Point(x0 + 10000, y0 + 10000));
    }

    start = Clock::now();
    for (const auto& rect : rectangles) {
        volatile size_t cnt = kd.rangeSearch(rect.first, rect.second).size();
        (void)cnt;
    }
    end = Clock::now();
    long long kdRange = elapsedMs(start, end);

    start = Clock::now();
    for (const Point& pt : pts) {
        kd.remove(pt);
    }
    end = Clock::now();
    long long kdRem = elapsedMs(start, end);

    cout << "KDTree Insercao (" << n << "): " << kdIns << " ms\n";
    cout << "KDTree Busca (" << n << "): " << kdBusca << " ms\n";
    cout << "KDTree NN (" << nnQueries << " queries): " << kdNN << " ms\n";
    cout << "KDTree Range (" << rangeQueries << " queries): " << kdRange << " ms\n";
    cout << "KDTree Remocao (" << n << "): " << kdRem << " ms\n";

    writeRow(csv, "KDTree", n, kdIns, kdBusca, kdRem);
    spatialCsv << "NearestNeighbor," << n << "," << nnQueries << "," << kdNN << "\n";
    spatialCsv << "RangeSearch," << n << "," << rangeQueries << "," << kdRange << "\n";
}

}

int main() {
    ofstream csv("data/benchmark_results.csv");
    ofstream memCsv("data/benchmark_memory.csv");
    ofstream distCsv("data/benchmark_distributions.csv");
    ofstream spatialCsv("data/benchmark_spatial.csv");

    if (!csv.is_open() || !memCsv.is_open() || !distCsv.is_open() || !spatialCsv.is_open()) {
        cerr << "Erro ao abrir arquivos de saída em data/\n";
        return 1;
    }

    csv << "Estrutura,N,Insercao_ms,Busca_ms,Remocao_ms\n";
    memCsv << "Estrutura,N,Nos\n";
    distCsv << "Distribuicao,Estrutura,N,Insercao_ms,Busca_ms\n";
    spatialCsv << "Operacao,N,Consultas,Tempo_ms\n";

    const vector<int> tamanhos = {10000, 100000, 500000};

    for (int n : tamanhos) {
        cout << "===============================================\n";
        cout << "Executando benchmarks para N = " << n << "\n";
        cout << "===============================================\n";

        cout << "\n=== Benchmarks de Strings (Trie e Patricia) ===\n";
        benchmarkTriePatricia(n, csv, memCsv);

        cout << "\n=== Benchmarks de Inteiros (Splay e Treap) ===\n";
        benchmarkSplayTreap(n, csv, distCsv);

        cout << "\n=== Benchmarks de Pontos Espaciais (KD-Tree) ===\n";
        benchmarkKDTree(n, csv, spatialCsv);
        cout << "\n";
    }

    csv.close();
    memCsv.close();
    distCsv.close();
    spatialCsv.close();
    return 0;
}
