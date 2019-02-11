#include "clistgraph.h"
#include <algorithm>

CListGraph::~CListGraph() {
    for (auto &list : lists_) {
        list.clear();
    }
    lists_.clear();
}

CListGraph::CListGraph(const IGraph* graph) {
    assert(graph != NULL);
    Create(graph->VerticesCount());
    Copy(graph, this);
}

CListGraph::CListGraph(int size) {
    Create(size);
}

void CListGraph::Create(int size) {
    if (size > 0) {
        size_ = size;
        lists_ = std::vector<std::list<int>>(static_cast<size_t>(size_),
                                          std::list<int>());
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
    }

}

std::vector<int> CListGraph::GetNextVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::list<int> list = lists_[static_cast<size_t>(vertex)];
    return std::vector<int>(std::begin(list), std::end(list));
}

std::vector<int> CListGraph::GetPrevVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }

    std::vector<int> prev_vertices;

    int who = 0;
    auto array_i = lists_.begin(), array_end = lists_.end();
    while (array_i != array_end) {
         auto list_i = array_i->begin(), list_end = array_i->end();
         while (list_i != list_end) {
            list_i = std::find(list_i, list_end, vertex);
            if (list_i != list_end) {
                prev_vertices.push_back(who);
                list_i++;
            }
         }
         array_i++;
         who++;
    }

    return prev_vertices;
}
