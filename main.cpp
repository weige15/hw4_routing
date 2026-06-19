#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numeric>
#include <string>
#include <tuple>
#include <vector>

namespace {

constexpr int64_t kMaxPoints = 10000000;
constexpr int64_t kCoordLimit = 1000000000;

struct Point {
    int32_t x;
    int32_t y;
    uint32_t id;
};

struct Edge {
    uint32_t u;
    uint32_t v;
    int64_t w;
};

class DSU {
public:
    explicit DSU(size_t n) : parent_(n), size_(n, 1) {
        std::iota(parent_.begin(), parent_.end(), 0);
    }

    uint32_t find(uint32_t v) {
        uint32_t root = v;
        while (parent_[root] != root) {
            root = parent_[root];
        }
        while (parent_[v] != v) {
            uint32_t next = parent_[v];
            parent_[v] = root;
            v = next;
        }
        return root;
    }

    bool unite(uint32_t a, uint32_t b) {
        uint32_t ra = find(a);
        uint32_t rb = find(b);
        if (ra == rb) {
            return false;
        }
        if (size_[ra] < size_[rb]) {
            std::swap(ra, rb);
        }
        parent_[rb] = ra;
        size_[ra] += size_[rb];
        return true;
    }

private:
    std::vector<uint32_t> parent_;
    std::vector<uint32_t> size_;
};

class FastScanner {
public:
    explicit FastScanner(const char* path) : file_(std::fopen(path, "rb")) {}

    ~FastScanner() {
        if (file_ != nullptr) {
            std::fclose(file_);
        }
    }

    bool is_open() const {
        return file_ != nullptr;
    }

    bool read_int(int64_t& out) {
        int c = read_char();
        while (c != EOF && c <= ' ') {
            c = read_char();
        }
        if (c == EOF) {
            return false;
        }

        int sign = 1;
        if (c == '-' || c == '+') {
            sign = (c == '-') ? -1 : 1;
            c = read_char();
        }
        if (c < '0' || c > '9') {
            failed_ = true;
            return false;
        }

        int64_t value = 0;
        while (c >= '0' && c <= '9') {
            value = value * 10 + (c - '0');
            c = read_char();
        }
        if (c != EOF && c > ' ') {
            failed_ = true;
            return false;
        }

        out = sign * value;
        return true;
    }

    bool failed() const {
        return failed_;
    }

private:
    int read_char() {
        if (file_ == nullptr) {
            return EOF;
        }
        if (pos_ == len_) {
            len_ = std::fread(buffer_, 1, sizeof(buffer_), file_);
            pos_ = 0;
            if (len_ == 0) {
                return EOF;
            }
        }
        return static_cast<unsigned char>(buffer_[pos_++]);
    }

    std::FILE* file_ = nullptr;
    char buffer_[1 << 20] = {};
    size_t pos_ = 0;
    size_t len_ = 0;
    bool failed_ = false;
};

int64_t manhattan_distance(const Point& a, const Point& b) {
    int64_t dx = static_cast<int64_t>(a.x) - static_cast<int64_t>(b.x);
    int64_t dy = static_cast<int64_t>(a.y) - static_cast<int64_t>(b.y);
    if (dx < 0) {
        dx = -dx;
    }
    if (dy < 0) {
        dy = -dy;
    }
    return dx + dy;
}

void add_edge(const std::vector<Point>& points, uint32_t u, uint32_t v, std::vector<Edge>& edges) {
    if (u == v) {
        return;
    }
    edges.push_back(Edge{u, v, manhattan_distance(points[u], points[v])});
}

bool read_points(const char* path, std::vector<Point>& points, std::string& error) {
    FastScanner scanner(path);
    if (!scanner.is_open()) {
        error = "failed to open input file";
        return false;
    }

    int64_t n = 0;
    if (!scanner.read_int(n) || scanner.failed()) {
        error = "missing or malformed point count";
        return false;
    }
    if (n < 1 || n > kMaxPoints) {
        error = "point count is outside assignment bounds";
        return false;
    }

    points.clear();
    points.reserve(static_cast<size_t>(n));
    for (int64_t i = 0; i < n; ++i) {
        int64_t x = 0;
        int64_t y = 0;
        if (!scanner.read_int(x) || !scanner.read_int(y) || scanner.failed()) {
            error = "missing or malformed point coordinate";
            return false;
        }
        if (x < -kCoordLimit || x > kCoordLimit || y < -kCoordLimit || y > kCoordLimit) {
            error = "point coordinate is outside assignment bounds";
            return false;
        }
        points.push_back(Point{static_cast<int32_t>(x), static_cast<int32_t>(y), static_cast<uint32_t>(i)});
    }

    return true;
}

bool compute_mst_total(size_t n, std::vector<Edge>& edges, int64_t& total, std::string& error) {
    total = 0;
    if (n <= 1) {
        return true;
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return std::tie(a.w, a.u, a.v) < std::tie(b.w, b.u, b.v);
    });

    DSU dsu(n);
    size_t accepted = 0;
    for (const Edge& edge : edges) {
        if (edge.u >= n || edge.v >= n) {
            error = "candidate edge endpoint is out of range";
            return false;
        }
        if (dsu.unite(edge.u, edge.v)) {
            total += edge.w;
            ++accepted;
            if (accepted == n - 1) {
                return true;
            }
        }
    }

    error = "candidate graph is disconnected";
    return false;
}

void generate_candidates(const std::vector<Point>& points, std::vector<Edge>& edges) {
    const size_t n = points.size();
    if (n <= 1) {
        return;
    }

    if (n <= (std::numeric_limits<size_t>::max() - edges.size()) / 4) {
        edges.reserve(edges.size() + 4 * n);
    }

    std::vector<int64_t> wx(n);
    std::vector<int64_t> wy(n);
    std::vector<uint32_t> order(n);
    for (size_t i = 0; i < n; ++i) {
        wx[i] = points[i].x;
        wy[i] = points[i].y;
        order[i] = static_cast<uint32_t>(i);
    }

    for (int pass = 0; pass < 4; ++pass) {
        std::sort(order.begin(), order.end(), [&](uint32_t a, uint32_t b) {
            return std::make_tuple(wx[a] + wy[a], wx[a], wy[a], a) <
                   std::make_tuple(wx[b] + wy[b], wx[b], wy[b], b);
        });

        std::map<int64_t, uint32_t, std::greater<int64_t>> active;
        for (uint32_t id : order) {
            for (auto it = active.lower_bound(wx[id]); it != active.end();) {
                const uint32_t other = it->second;
                if (wx[id] - wx[other] > wy[id] - wy[other]) {
                    break;
                }
                add_edge(points, id, other, edges);
                it = active.erase(it);
            }
            active[wx[id]] = id;
        }

        if (pass == 0 || pass == 2) {
            wx.swap(wy);
        } else if (pass == 1) {
            for (int64_t& x : wx) {
                x = -x;
            }
        }
    }
}

bool optimized_mst_total(const std::vector<Point>& points, int64_t& total, std::string& error) {
    std::vector<Edge> edges;
    generate_candidates(points, edges);
    return compute_mst_total(points.size(), edges, total, error);
}

bool solve_rmst_total(const std::vector<Point>& points, int64_t& total, std::string& error) {
    return optimized_mst_total(points, total, error);
}

bool write_total(const char* path, int64_t total, std::string& error) {
    std::ofstream out(path);
    if (!out) {
        error = "failed to open output file";
        return false;
    }

    out << total << '\n';
    out.close();
    if (!out) {
        error = "failed to write output file";
        return false;
    }
    return true;
}

int run(const char* input_path, const char* output_path) {
    std::vector<Point> points;
    std::string error;

    if (!read_points(input_path, points, error)) {
        std::cerr << "Input error: " << error << '\n';
        return 2;
    }

    int64_t total = 0;
    if (!solve_rmst_total(points, total, error)) {
        std::cerr << "Solver error: " << error << '\n';
        return 3;
    }

    if (!write_total(output_path, total, error)) {
        std::cerr << "Output error: " << error << '\n';
        return 4;
    }

    return 0;
}

}  // namespace

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input file> <output file>\n";
        return 1;
    }

    try {
        return run(argv[1], argv[2]);
    } catch (const std::exception& ex) {
        std::cerr << "Fatal error: " << ex.what() << '\n';
        return 5;
    }
}
