#include "igraph.h"
#include <algorithm>
#include <assert.h>

void IGraph::Copy(const IGraph *from, IGraph *to) {
    assert(from != NULL);
    assert(to != NULL);

    int lists_amount = from->VerticesCount();

    for (int i = 0; i < lists_amount; i++) {
        std::vector<int> one_list = from->GetNextVertices(i);
        int size_list = static_cast<int>(one_list.size());
        for (int j = 0; j < size_list; j++) {
            to->AddEdge(i, one_list[static_cast<size_t>(j)]);
        }
    }
}

void IGraph::Create(int size){};

IGraph::IGraph(){};

bool IGraph::IsIndexCorrect(int index) const {
    return (index >= 0) && (index < VerticesCount());
}

bool IGraph::IsGraphCorrect() const {
    return VerticesCount() > 0;
}

IGraph::IGraph(const IGraph* graph) {
    assert(graph != NULL);
    Create(graph->VerticesCount());
    Copy(graph, this);
}
