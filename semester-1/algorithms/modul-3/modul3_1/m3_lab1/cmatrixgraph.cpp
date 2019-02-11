#include "cmatrixgraph.h"
#include <algorithm>

CMatrixGraph::~CMatrixGraph() {
    for (auto &vector : matrix_) {
        vector.clear();
    }
    matrix_.clear();
}

CMatrixGraph::CMatrixGraph(const IGraph* graph) {
    assert(graph != NULL);
    Create(graph->VerticesCount());
    Copy(graph, this);
}

CMatrixGraph::CMatrixGraph(int size) {
    Create(size);
}

void CMatrixGraph::Create(int size) {
    if (size > 0) {
        size_ = size;
        size_t sizet = static_cast<size_t>(size);
        matrix_ = std::vector<std::vector<int>>(sizet,
                                                std::vector<int>(sizet, 0));
    } else {
        size_ = 0;
    }
}

int CMatrixGraph::VerticesCount() const {
    return size_;
}

void CMatrixGraph::AddEdge(int from, int to) {
    if (!IsGraphCorrect()) {
        return;
    }
    if (!IsIndexCorrect(from) || !IsIndexCorrect(to) || (from == to)) {
        return;
    }

    matrix_[static_cast<size_t>(from)][static_cast<size_t>(to)] = 1;
}

std::vector<int> CMatrixGraph::GetNextVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::vector<int> next_vertices;
    for (int i = 0; i < size_; i++) {
        if (matrix_[static_cast<size_t>(vertex)][static_cast<size_t>(i)] > 0) {
            next_vertices.push_back(i);
        }
    }
    return next_vertices;
}

std::vector<int> CMatrixGraph::GetPrevVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::vector<int> next_vertices;
    for (int i = 0; i < size_; i++) {
        if (matrix_[static_cast<size_t>(i)][static_cast<size_t>(vertex)] > 0) {
            next_vertices.push_back(i);
        }
    }
    return next_vertices;
}
