#include "carcgraph.h"

CArcGraph::~CArcGraph() {
    pairs_.clear();
}

CArcGraph::CArcGraph(const IGraph* graph) {
    assert(graph != NULL);
    Create(graph->VerticesCount());
    Copy(graph, this);
}

CArcGraph::CArcGraph(int size) {
    Create(size);
}

void CArcGraph::Create(int size) {
    if (size > 0) {
        size_ = size;
        size_t sizet = static_cast<size_t>(size);
        pairs_ = std::vector<std::pair<int, int>>(sizet * sizet,
                                                  std::make_pair(NO_ELEMENT, NO_ELEMENT));
    } else {
        size_ = 0;
    }
}

int CArcGraph::VerticesCount() const {
    return size_;
}

int CArcGraph::Compare(std::pair<int, int> left,
            std::pair<int, int> right) const {
    const int left_is_more = -1;
    const int equally = 0;
    const int right_is_more = 1;

    if (left.first == right.first) {
        if (left.second == right.second) {
            return equally;
        }
        if (left.second > right.second) {
            return left_is_more;
        }
    } else if (left.first > right.first){
         return left_is_more;
    }
    return right_is_more;
}

bool CArcGraph::IsNoElement(int i) const {
    if (i < 0 || i > size_ * size_) {
        return true;
    }
    return Compare(pairs_[static_cast<size_t>(i)],
                   std::make_pair<int, int>(NO_ELEMENT, NO_ELEMENT)) == 0;
}

#include <iostream>
void CArcGraph::InsertElement(size_t start, std::pair<int, int> insert) {
    size_t i = start;
    size_t size = size_ * size_;

    std::pair<int, int> save;
    std::pair<int, int> prev = pairs_[i];

    for(; i < size - 1 && !IsNoElement(i + 1); i++) {
        save = pairs_[i + 1];
        pairs_[i + 1] = prev;
        prev = save;
    }
    pairs_[i + 1] = prev;
    pairs_[start] = insert;
}

void CArcGraph::AddEdge(int from, int to) {
    if (!IsGraphCorrect()) {
        return;
    }
    if (!IsIndexCorrect(from) || !IsIndexCorrect(to) || (from == to)) {
        return;
    }
    std::pair<int, int> new_pair = std::make_pair(from, to);

    const int right_is_more = 1;
    size_t i = 0;
    for (i = 0; Compare(new_pair, pairs_[i]) == right_is_more; i++);
    int size = size_ * size_;
    if (i < static_cast<size_t>(size)) {
        if (Compare(new_pair, pairs_[i]) == -1) {
            InsertElement(i, new_pair);
        }
    }
    //std::cout << "\nnow it is";
    //for (i = 0; i < size; i++) {
    //    std::cout << "(" << pairs_[i].first << "," << pairs_[i].second << ") ";
    //}
}

std::vector<int> CArcGraph::GetNextVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::vector<int> next_vertices;
    int size = size_ * size_;
    int i = 0;
    for (; i < size && pairs_[i].first > vertex; i++){}
    for (; i < size && pairs_[i].first == vertex; i++){
        next_vertices.push_back(pairs_[i].second);
    }
    return next_vertices;
}

std::vector<int> CArcGraph::GetPrevVertices(int vertex) const {
    if (!IsGraphCorrect() || !IsIndexCorrect(vertex)) {
        return std::vector<int>();
    }
    std::vector<int> prev_vertices;
    int size = size_ * size_;
    for (int i = 0; i < size && !IsNoElement(i); i++){
        if (pairs_[i].second == vertex) {
            prev_vertices.push_back(pairs_[i].first);
        }
    }
    return prev_vertices;
}
