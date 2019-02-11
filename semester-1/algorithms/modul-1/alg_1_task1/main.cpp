/*
В круг выстроено N человек, пронумерованных числами от 1 до N.
Будем исключать каждого k-ого до тех пор, пока не уцелеет только один
 человек.

Например, если N=10, k=3, то сначала умрет 3-й, потом 6-й, затем 9-й,
 затем 2-й, затем 7-й, потом 1-й, потом 8-й, за ним - 5-й, и потом 10-й. Таким
образом, уцелеет 4-й.

Необходимо определить номер уцелевшего.

N, k ≤ 10000.
*/

#include "stdlib.h"
#include <assert.h>
#include <iostream>

typedef struct Node {
    int num;
    Node *next;
    Node *prev;

    Node(int num, Node *start, Node *last);
    ~Node();
} Node;

Node::Node(int num, Node *start, Node *last) {
    if (last != nullptr) {
        last->next = this;
    }
    if (start != nullptr) {
        start->prev = this;
    }
    this->num = num;
    this->next = start;
    this->prev = last;
}

Node::~Node() {
    Node *start = next;
    Node *end = prev;
    end->next = start;
    start->prev = end;
}

int get_answer(int people_amount, int next) {
    assert(people_amount > 0);
    assert(next > 0);

    Node *start = new Node(1, nullptr, nullptr);
    Node *end = new Node(2, start, start);
    for (int i = 3; i <= people_amount; i++) {
        end = new Node(i, start, end);
    }
    Node *circle = start;
    while (circle != circle->next) {
        for (int i = 0; i < next - 1; i++) {
            circle = circle->next;
        }
        Node *save = circle->next;
        delete circle;
        circle = save;
    }
    int ret = circle->num;
    delete circle;
    return ret;
}

int main() {
    int people = 10, next = 3;
    std::cin >> people >> next;
    if (people < 1 || next < 1) {
        std::cout << "error";
        return 0;
    }
    std::cout << get_answer(people, next);

    return 0;
}
