#include "csetgraph.h"
#include <algorithm>

CSetGraph::~CSetGraph() {
    for (auto &set : sets_) {
        set.clear();
    }
    sets_.clear();
}

CSetGraph::CSetGraph(const IGraph* graph) {
    assert(graph != NULL);
    Create(graph->VerticesCount());
    Copy(graph, this);
}

CSetGraph::CSetGraph(int size) {
    Create(size);
}

void CSetGraph::Create(int size) {
    if (size > 0) {
        size_ = size;
        sets_ = std::vector<std::unordered_set<int>>(static_cast<size_t>(size_),
                                          std::unordered_set<int>());
    } else {
        size_ = 0;
    }
}

int CSetGraph::VerticesCount() const {
    return size_;
}

bool CSetGraph::IsAlreadyInGraph(int from, int to) const {
    std::unordered_set<int> set = sets_[static_cast<size_t>(from)];
    std::unordered_set<int>::iterator begin = set.begin(), end = set.end();
    return std::find(begin, end, to) != end;
}

void CSetGraph::AddEdge(int from, int to) {
    if (!IsGraphCorrect()) {
        return;
    }
    if (!IsIndexCorrect(from) || !IsIndexCorrect(to) || (from == to)) {
        return;
    }

    if (!IsAlreadyInGraph(from, to)) {
        sets_[static_cast<size_t>(from)].insert(to);
    }

}

std::vector<int> CSetGraph::GetNextVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::unordered_set<int> set = sets_[static_cast<size_t>(vertex)];
    return std::vector<int>(std::begin(set), std::end(set));
}

std::vector<int> CSetGraph::GetPrevVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }

    std::vector<int> prev_vertices;

    int who = 0;
    auto array_i = sets_.begin(), array_end = sets_.end();
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
