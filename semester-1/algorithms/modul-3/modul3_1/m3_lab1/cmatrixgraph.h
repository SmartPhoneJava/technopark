#ifndef CMATRIXGRAPH_H
#define CMATRIXGRAPH_H

#include "igraph.h"

// Матрица смежности
struct CMatrixGraph : public IGraph {
public:
    CMatrixGraph(int size);
    CMatrixGraph(const IGraph* graph);

    ~CMatrixGraph() override;
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> matrix_;
    int size_;

    void Create(int size) override;
};

#endif // CMATRIXGRAPH_H
