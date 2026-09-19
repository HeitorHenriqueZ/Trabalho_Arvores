#ifndef KDTREE_HPP
#define KDTREE_HPP

#include <string>
#include <vector>

inline constexpr int K = 2;

struct Point {
    int coords[K];
    Point(int x, int y);
    Point();
};

struct KDNode {
    Point point;
    KDNode* left;
    KDNode* right;

    explicit KDNode(Point pt);
};

class KDTree {
private:
    KDNode* root;

    void destroy(KDNode* node);
    KDNode* insertHelper(KDNode* node, Point pt, unsigned depth, bool& inserted);
    bool searchHelper(KDNode* node, Point pt, unsigned depth) const;
    bool arePointsEqual(Point pt1, Point pt2) const;

    KDNode* findMin(KDNode* node, int d, unsigned depth);
    KDNode* minNode(KDNode* x, KDNode* y, KDNode* z, int d);
    KDNode* removeHelper(KDNode* node, Point pt, unsigned depth, bool& removed);
    bool isValidDescendants(KDNode* node, int dim, int val, bool isLeft) const;
    bool isValidKDTreeHelper(KDNode* node, unsigned depth) const;

    long double distSquared(Point a, Point b) const;
    void nearestNeighborHelper(KDNode* node, Point target, unsigned depth,
                               KDNode*& best, long double& bestDist) const;
    void rangeSearchHelper(KDNode* node, Point low, Point high, unsigned depth,
                           std::vector<Point>& result) const;

    void toDotHelper(KDNode* node, unsigned depth, int& nullCount, std::string& dot) const;

public:
    KDTree();
    ~KDTree();

    KDTree(const KDTree&) = delete;
    KDTree& operator=(const KDTree&) = delete;
    KDTree(KDTree&&) = delete;
    KDTree& operator=(KDTree&&) = delete;

    bool insert(Point pt);
    bool search(Point pt) const;
    bool remove(Point pt);

    bool nearestNeighbor(Point target, Point& result) const;
    std::vector<Point> rangeSearch(Point low, Point high) const;

    bool isValidKDTree() const;

    std::string toDot() const;
};

#endif
