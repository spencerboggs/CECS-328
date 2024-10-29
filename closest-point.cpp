#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <limits>

class Point {
public:
    double x, y;
    Point(double x, double y) : x(x), y(y) {}
};

double dist(const Point& p1, const Point& p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

double bruteForce(const std::vector<Point>& P, int n) {
    double min_dist = std::numeric_limits<double>::infinity();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            double distance = dist(P[i], P[j]);
            if (distance < min_dist) {
                min_dist = distance;
            }
        }
    }
    return min_dist;
}

double stripClosest(std::vector<Point>& strip, int size, double d) {
    double min_dist = d;
    std::sort(strip.begin(), strip.end(), [](const Point& a, const Point& b) { return a.y < b.y; });

    for (int i = 0; i < size; i++) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min_dist; j++) {
            double distance = dist(strip[i], strip[j]);
            if (distance < min_dist) {
                min_dist = distance;
            }
        }
    }
    return min_dist;
}

double closestUtil(const std::vector<Point>& P, int n) {
    if (n <= 3) {
        return bruteForce(P, n);
    }

    int mid = n / 2;
    Point midPoint = P[mid];

    double dl = closestUtil(P, mid);
    double dr = closestUtil(std::vector<Point>(P.begin() + mid, P.end()), n - mid);

    double d = std::min(dl, dr);

    std::vector<Point> strip;
    for (int i = 0; i < n; i++) {
        if (std::abs(P[i].x - midPoint.x) < d) {
            strip.push_back(P[i]);
        }
    }

    return std::min(d, stripClosest(strip, strip.size(), d));
}


double closest(std::vector<Point>& P, int n) {
    std::sort(P.begin(), P.end(), [](const Point& a, const Point& b) { return a.x < b.x; });
    return closestUtil(P, n);
}

int main() {
    std::string fileName;
    std::cout << "Enter file name here: ";
    std::cin >> fileName;
    std::ifstream file(fileName);
    std::string megaSet;
    std::getline(file, megaSet);
    
    std::string filter1 = megaSet;
    filter1.erase(std::remove(filter1.begin(), filter1.end(), '{'), filter1.end());
    filter1.erase(std::remove(filter1.begin(), filter1.end(), '}'), filter1.end());
    
    std::istringstream ss(filter1);
    std::string item;
    std::vector<double> xList, yList;

    while (std::getline(ss, item, ',')) {
        if (xList.size() <= yList.size()) {
            xList.push_back(std::stod(item));
        } else {
            yList.push_back(std::stod(item));
        }
    }

    int limit = std::min(xList.size(), yList.size());
    std::vector<Point> P;

    for (int j = 0; j < limit; j++) {
        P.emplace_back(xList[j], yList[j]);
    }
    int n = P.size();

    if (n < 2) {
        std::cerr << "Error: Not enough points." << std::endl;
        return 1;
    }

    std::cout << "The smallest distance is " << closest(P, n) << std::endl;
    file.close();

    return 0;
}