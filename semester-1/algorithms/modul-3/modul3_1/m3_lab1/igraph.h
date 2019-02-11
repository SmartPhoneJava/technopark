#ifndef IGRAPH_H
#define IGRAPH_H

#include <vector>
#include <assert.h>

struct IGraph {
public:
    virtual ~IGraph() {}

    IGraph();

    IGraph(const IGraph* graph);

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const  = 0;

    bool IsGraphCorrect() const;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;
    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;

protected:
    virtual void Create(int size);
    void Copy(const IGraph *from, IGraph *to);

    bool IsIndexCorrect(int index) const;
};

#endif // IGRAPH_H
