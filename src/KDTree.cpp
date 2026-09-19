#include "KDTree.hpp"

#include <limits>
#include <sstream>

Point::Point(int x, int y) {
    coords[0] = x;
    coords[1] = y;
}

Point::Point() {
    coords[0] = 0;
    coords[1] = 0;
}

KDNode::KDNode(Point pt) {
    point = pt;
    left = nullptr;
    right = nullptr;
}

KDTree::KDTree() {
    root = nullptr;
}

KDTree::~KDTree() {
    destroy(root);
    root = nullptr;
}

void KDTree::destroy(KDNode* node) {
    while (node != nullptr) {
        if (node->left != nullptr) {
            KDNode* left = node->left;
            node->left = left->right;
            left->right = node;
            node = left;
        } else {
            KDNode* right = node->right;
            delete node;
            node = right;
        }
    }
}

bool KDTree::arePointsEqual(Point pt1, Point pt2) const {
    for (int i = 0; i < K; ++i) {
        if (pt1.coords[i] != pt2.coords[i]) {
            return false;
        }
    }
    return true;
}

KDNode* KDTree::insertHelper(KDNode* node, Point pt, unsigned depth, bool& inserted) {
    if (node == nullptr) {
        inserted = true;
        return new KDNode(pt);
    }

    if (arePointsEqual(node->point, pt)) {
        inserted = false;
        return node;
    }

    unsigned cd = depth % static_cast<unsigned>(K);
    if (pt.coords[cd] < node->point.coords[cd]) {
        node->left = insertHelper(node->left, pt, depth + 1, inserted);
    } else {
        node->right = insertHelper(node->right, pt, depth + 1, inserted);
    }
    return node;
}

bool KDTree::insert(Point pt) {
    bool inserted = false;
    root = insertHelper(root, pt, 0, inserted);
    return inserted;
}

bool KDTree::searchHelper(KDNode* node, Point pt, unsigned depth) const {
    if (node == nullptr) {
        return false;
    }
    if (arePointsEqual(node->point, pt)) {
        return true;
    }

    unsigned cd = depth % static_cast<unsigned>(K);
    if (pt.coords[cd] < node->point.coords[cd]) {
        return searchHelper(node->left, pt, depth + 1);
    }
    return searchHelper(node->right, pt, depth + 1);
}

bool KDTree::search(Point pt) const {
    return searchHelper(root, pt, 0);
}

KDNode* KDTree::minNode(KDNode* x, KDNode* y, KDNode* z, int d) {
    KDNode* res = x;
    if (y != nullptr && (res == nullptr || y->point.coords[d] < res->point.coords[d])) {
        res = y;
    }
    if (z != nullptr && (res == nullptr || z->point.coords[d] < res->point.coords[d])) {
        res = z;
    }
    return res;
}

KDNode* KDTree::findMin(KDNode* node, int d, unsigned depth) {
    if (node == nullptr) {
        return nullptr;
    }

    unsigned cd = depth % static_cast<unsigned>(K);
    if (cd == static_cast<unsigned>(d)) {
        if (node->left == nullptr) {
            return node;
        }
        return findMin(node->left, d, depth + 1);
    }
    return minNode(node,
                   findMin(node->left, d, depth + 1),
                   findMin(node->right, d, depth + 1),
                   d);
}

KDNode* KDTree::removeHelper(KDNode* node, Point pt, unsigned depth, bool& removed) {
    if (node == nullptr) {
        removed = false;
        return nullptr;
    }

    unsigned cd = depth % static_cast<unsigned>(K);

    if (arePointsEqual(node->point, pt)) {
        removed = true;
        if (node->right != nullptr) {
            KDNode* min = findMin(node->right, static_cast<int>(cd), depth + 1);
            node->point = min->point;
            node->right = removeHelper(node->right, min->point, depth + 1, removed);
        } else if (node->left != nullptr) {
            KDNode* min = findMin(node->left, static_cast<int>(cd), depth + 1);
            node->point = min->point;
            node->right = removeHelper(node->left, min->point, depth + 1, removed);
            node->left = nullptr;
        } else {
            delete node;
            return nullptr;
        }
        return node;
    }

    if (pt.coords[cd] < node->point.coords[cd]) {
        node->left = removeHelper(node->left, pt, depth + 1, removed);
    } else {
        node->right = removeHelper(node->right, pt, depth + 1, removed);
    }
    return node;
}

bool KDTree::remove(Point pt) {
    bool removed = false;
    root = removeHelper(root, pt, 0, removed);
    return removed;
}

bool KDTree::isValidDescendants(KDNode* node, int dim, int val, bool isLeft) const {
    if (node == nullptr) {
        return true;
    }
    if (isLeft && node->point.coords[dim] >= val) {
        return false;
    }
    if (!isLeft && node->point.coords[dim] < val) {
        return false;
    }
    return isValidDescendants(node->left, dim, val, isLeft) &&
           isValidDescendants(node->right, dim, val, isLeft);
}

bool KDTree::isValidKDTreeHelper(KDNode* node, unsigned depth) const {
    if (node == nullptr) {
        return true;
    }
    unsigned cd = depth % static_cast<unsigned>(K);
    if (!isValidDescendants(node->left, static_cast<int>(cd), node->point.coords[cd], true)) {
        return false;
    }
    if (!isValidDescendants(node->right, static_cast<int>(cd), node->point.coords[cd], false)) {
        return false;
    }
    return isValidKDTreeHelper(node->left, depth + 1) &&
           isValidKDTreeHelper(node->right, depth + 1);
}

bool KDTree::isValidKDTree() const {
    return isValidKDTreeHelper(root, 0);
}

long double KDTree::distSquared(Point a, Point b) const {
    long double dist = 0;
    for (int i = 0; i < K; ++i) {
        long double diff = static_cast<long double>(a.coords[i]) -
                           static_cast<long double>(b.coords[i]);
        dist += diff * diff;
    }
    return dist;
}

void KDTree::nearestNeighborHelper(KDNode* node, Point target, unsigned depth,
                                   KDNode*& best, long double& bestDist) const {
    if (node == nullptr) {
        return;
    }

    long double d = distSquared(node->point, target);
    if (d < bestDist) {
        bestDist = d;
        best = node;
    }

    unsigned cd = depth % static_cast<unsigned>(K);
    long double diff = static_cast<long double>(target.coords[cd]) -
                       static_cast<long double>(node->point.coords[cd]);

    KDNode* first  = (diff < 0) ? node->left : node->right;
    KDNode* second = (diff < 0) ? node->right : node->left;

    nearestNeighborHelper(first, target, depth + 1, best, bestDist);

    if (diff * diff < bestDist) {
        nearestNeighborHelper(second, target, depth + 1, best, bestDist);
    }
}

bool KDTree::nearestNeighbor(Point target, Point& result) const {
    if (root == nullptr) {
        return false;
    }
    KDNode* best = nullptr;
    long double bestDist = std::numeric_limits<long double>::max();
    nearestNeighborHelper(root, target, 0, best, bestDist);
    if (best != nullptr) {
        result = best->point;
        return true;
    }
    return false;
}

void KDTree::rangeSearchHelper(KDNode* node, Point low, Point high, unsigned depth,
                               std::vector<Point>& result) const {
    if (node == nullptr) {
        return;
    }

    bool inRange = true;
    for (int i = 0; i < K; ++i) {
        if (node->point.coords[i] < low.coords[i] || node->point.coords[i] > high.coords[i]) {
            inRange = false;
            break;
        }
    }
    if (inRange) {
        result.push_back(node->point);
    }

    unsigned cd = depth % static_cast<unsigned>(K);

    if (low.coords[cd] <= node->point.coords[cd]) {
        rangeSearchHelper(node->left, low, high, depth + 1, result);
    }
    if (high.coords[cd] >= node->point.coords[cd]) {
        rangeSearchHelper(node->right, low, high, depth + 1, result);
    }
}

std::vector<Point> KDTree::rangeSearch(Point low, Point high) const {
    std::vector<Point> result;
    rangeSearchHelper(root, low, high, 0, result);
    return result;
}

void KDTree::toDotHelper(KDNode* node, unsigned depth, int& nullCount, std::string& dot) const {
    if (node == nullptr) {
        return;
    }

    unsigned cd = depth % static_cast<unsigned>(K);
    std::string label = "(" + std::to_string(node->point.coords[0]) + "," +
                        std::to_string(node->point.coords[1]) + ")";
    std::string dimLabel = (cd == 0) ? "x" : "y";
    std::string nodeId = "\"n" + std::to_string(node->point.coords[0]) + "_" +
                         std::to_string(node->point.coords[1]) + "\"";

    dot += "    " + nodeId + " [label=\"" + label + "\\n[" + dimLabel + "]\"];\n";

    if (node->left != nullptr) {
        std::string leftId = "\"n" + std::to_string(node->left->point.coords[0]) + "_" +
                             std::to_string(node->left->point.coords[1]) + "\"";
        dot += "    " + nodeId + " -> " + leftId + ";\n";
        toDotHelper(node->left, depth + 1, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }

    if (node->right != nullptr) {
        std::string rightId = "\"n" + std::to_string(node->right->point.coords[0]) + "_" +
                              std::to_string(node->right->point.coords[1]) + "\"";
        dot += "    " + nodeId + " -> " + rightId + ";\n";
        toDotHelper(node->right, depth + 1, nullCount, dot);
    } else {
        std::string nullId = "null" + std::to_string(nullCount++);
        dot += "    " + nullId + " [shape=point];\n";
        dot += "    " + nodeId + " -> " + nullId + ";\n";
    }
}

std::string KDTree::toDot() const {
    std::string dot = "digraph KDTree {\n";
    dot += "    node [shape=ellipse];\n";
    if (root != nullptr) {
        int nullCount = 0;
        toDotHelper(root, 0, nullCount, dot);
    }
    dot += "}\n";
    return dot;
}
