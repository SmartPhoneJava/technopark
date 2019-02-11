#ifndef CARCGRAPH_H
#define CARCGRAPH_H

#include "igraph.h"
#include <algorithm>

#define NO_ELEMENT -1

// Массив пар, отсортированный по убыванию
struct CArcGraph : public IGraph {
public:
    CArcGraph(int size);
    CArcGraph(const IGraph* graph);

    ~CArcGraph() override;
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> pairs_;
    int size_;

    void Create(int size) override;

    // -1 если левое больше, 1 если правое, 0 если равны
    int Compare(std::pair<int, int> pair1,
                std::pair<int, int> pair2) const;

    bool IsNoElement(int i) const;

    void InsertElement(size_t start, std::pair<int, int> insert);
};

#endif // CARCGRAPH_H
