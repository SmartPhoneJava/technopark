#ifndef CLISTGRAPH_H
#define CLISTGRAPH_H

#include "igraph.h"
#include <list>

// Массив списков смежности
struct CListGraph : public IGraph {
public:
    CListGraph(int size);
    CListGraph(const IGraph* graph);

    ~CListGraph() override;
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::list<int>> lists_;
    int size_;

    void Create(int size) override;

    bool IsAlreadyInGraph(int from, int to) const;
};

#endif // CLISTGRAPH_H
