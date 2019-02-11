#ifndef CSETGRAPH_H
#define CSETGRAPH_H

#include <unordered_set>

#include "igraph.h"

// Массив хэш-таблиц
struct CSetGraph : public IGraph {
public:
    CSetGraph(int size);
    CSetGraph(const IGraph* graph);

    ~CSetGraph() override;
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;
    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> sets_;
    int size_;

    void Create(int size) override;

    bool IsAlreadyInGraph(int from, int to) const;
};


#endif // CSETGRAPH_H
