#ifndef CLISTGRAPH_H
#define CLISTGRAPH_H

#include <list>
#include <vector>

// Массив списков смежности
struct CListGraph  {
public:
    CListGraph(int size);

    ~CListGraph();
    void AddEdge(int from, int to);

    int VerticesCount() const;

    std::vector<int> GetNextVertices(int vertex) const;
    std::vector<int> GetPrevVertices(int vertex) const;

private:
    std::vector<std::list<int>> lists_;
    int size_;

    void Create(int size);

    bool IsAlreadyInGraph(int from, int to) const;
};

#endif // CLISTGRAPH_H
