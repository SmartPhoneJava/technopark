#include <list>
#include <vector>
#include <queue>
#include <iostream>
#include <algorithm>

// Массив списков смежности
struct CListGraph  {
public:
    CListGraph(int size);

    ~CListGraph();
    void AddEdge(int from, int to);

    int VerticesCount() const;

    int AmountOfShortest(int from, int to);

private:
    std::vector<std::list<int>> lists_;
    int size_;

    std::vector<bool> used;

    std::vector<int> distances;
    std::vector<int> times;

    void Create(int size);

    void bfs(int vertex);

    bool IsGraphCorrect() const;

    bool IsIndexCorrect(int index) const;

    bool IsAlreadyInGraph(int from, int to) const;
};

int main()
{

    int vertices = 0;
    std::cin >> vertices;
    CListGraph graph(vertices);

    int ribs = 0;
    std::cin >> ribs;

    int a = 0, b = 0;
    for(int i = 0; i < ribs; i++) {
        std::cin >> a >> b;
        graph.AddEdge(a,b);
    }
    std::cin >> a >> b;
    std::cout << graph.AmountOfShortest(a, b);

    return 0;
}

int CListGraph::AmountOfShortest(int from, int to) {
    used = std::vector<bool>(size_, false);
    distances = std::vector<int>(size_, 0);
    times = std::vector<int>(size_, 1);

    bfs(from);

    int need_dist = distances[to] - 1;

    int how_much = 0;

    for (int i = 0; i < size_; i++) {
        if (distances[i] == (need_dist)) {
            if (std::find(lists_[to].begin(),
                         lists_[to].end(), i) !=
                   lists_[to].end()) {
               how_much += times[i];
            }
        }
    }
    return how_much;
}

void CListGraph::bfs(int vertex) {

    std::queue<int> q;
    q.push(vertex);

    used[vertex] = true;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        auto i = lists_[u].begin(), end = lists_[u].end();

        for (; i != end; i++) {
            int v = *i;

            if (!used[v]) {
                used[v] = true;
                distances[v] = distances[u] + 1;
                q.push(v);
                times[v] = times[u];
            } else {
                if (distances[v] == distances[u] + 1) {
                     times[v] += times[u];
                }
            }
        }
    }
}

CListGraph::~CListGraph() {
    for (auto &list : lists_) {
        list.clear();
    }
    lists_.clear();
}

CListGraph::CListGraph(int size) {
    Create(size);
}

bool CListGraph::IsIndexCorrect(int index) const {
    return (index >= 0) && (index < VerticesCount());
}

bool CListGraph::IsGraphCorrect() const {
    return VerticesCount() > 0;
}

void CListGraph::Create(int size) {
    if (size > 0) {
        size_ = size;
        lists_ = std::vector<std::list<int>>(static_cast<size_t>(size_),
                                          std::list<int>());
        used = std::vector<bool>(size_, false);
        distances = std::vector<int>(size_, 0);
    } else {
        size_ = 0;
    }
}

int CListGraph::VerticesCount() const {
    return size_;
}

bool CListGraph::IsAlreadyInGraph(int from, int to) const {
    std::list<int> list = lists_[static_cast<size_t>(from)];
    std::list<int>::iterator begin = list.begin(), end = list.end();
    return std::find(begin, end, to) != end;
}

void CListGraph::AddEdge(int from, int to) {
    if (!IsGraphCorrect()) {
        return;
    }
    if (!IsIndexCorrect(from) || !IsIndexCorrect(to) || (from == to)) {
        return;
    }

    if (!IsAlreadyInGraph(from, to)) {
        lists_[static_cast<size_t>(from)].push_back(to);
        lists_[static_cast<size_t>(to)].push_back(from);
    }
}
