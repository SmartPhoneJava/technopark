#include <iostream>

#include "clistgraph.h"
#include "cmatrixgraph.h"
#include "csetgraph.h"
#include "carcgraph.h"

void init(IGraph& graph);

void print(IGraph& graph, int index);

void test_case1();

void init_full(IGraph& graph);

void print_full(IGraph& graph);

void test_case2();

int main()
{
    test_case1();
    //test_case2();

    return 0;
}

void test_case1() {
    std::cout << "\nCListGraph test;";
    {
        CListGraph for_init(10);
        init(for_init);
        CListGraph for_print(&for_init);
        print(for_print, 1);
    }
    std::cout << "\n\nCMatrixGraph test;";
    {
        CMatrixGraph for_init(10);
        init(for_init);
        CMatrixGraph for_print(&for_init);
        print(for_print, 1);
    }
    std::cout << "\n\nCSetGraph test;";
    {
        CSetGraph for_init(10);
        init(for_init);
        CArcGraph for_print(&for_init);
        print(for_print, 1);
    }
    std::cout << "\n\nCArcGraph test;";
    {
        CArcGraph for_init(10);
        init(for_init);
        CArcGraph for_print(&for_init);
        print(for_print, 1);
    }
}

void test_case2() {

    std::cout << "\n\n\n Full;";
    std::cout << "\nCListGraph test;";
    {
        CListGraph for_init(3);
        init_full(for_init);
        CListGraph for_print(&for_init);
        print_full(for_print);
    }
    std::cout << "\n\nCMatrixGraph test;";
    {
        CMatrixGraph for_init(3);
        init_full(for_init);
        CMatrixGraph for_print(&for_init);
        print_full(for_print);
    }
    std::cout << "\n\nCSetGraph test;";
    {
        CSetGraph for_init(3);
        init_full(for_init);
        CSetGraph for_print(&for_init);
        print_full(for_print);
    }

    std::cout << "\n\nCArcGraph test;";
    {
        CArcGraph for_init(3);
        init_full(for_init);
        CArcGraph for_print(&for_init);
        print_full(for_print);
    }
}

void init(IGraph& graph) {
    graph.AddEdge(1,1);
    graph.AddEdge(1,2);
    graph.AddEdge(1,3);
    graph.AddEdge(1,4);
    graph.AddEdge(4,1);
    graph.AddEdge(4,4);
    graph.AddEdge(5,1);
    graph.AddEdge(7,2);
    graph.AddEdge(7,1);
    graph.AddEdge(7,2);
    graph.AddEdge(7,2);

    graph.AddEdge(11,1);
    graph.AddEdge(1,11);
    graph.AddEdge(-1,1);
}

void init_full(IGraph& graph) {
    int size = graph.VerticesCount();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            graph.AddEdge(i,j);
        }
    }
}

void print_full(IGraph& graph) {
    int size = graph.VerticesCount();
    for (int i = 0; i < size; i++) {
        print(graph,i);
    }
}

void print(IGraph& graph, int index) {

    auto next = graph.GetNextVertices(index);
    auto i_next = next.begin(), end_next = next.end();

    std::cout << "\n\nexit from " << index << ":";
    while (i_next != end_next) {
        std::cout << *i_next << " ";
        i_next++;
    }

    auto prev = graph.GetPrevVertices(index);
    auto i_prev = prev.begin(), end_prev = prev.end();

    std::cout << "\ncome to " << index << ":";
    while (i_prev != end_prev) {
      std::cout << *i_prev << " ";
      i_prev++;
    }

    std::cout << "\nfinish:";
}
