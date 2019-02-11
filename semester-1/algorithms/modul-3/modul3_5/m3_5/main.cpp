#include <QCoreApplication>
#include <vector>
#include <iostream>

#define NO_PARENT -1

/*
class Node {
public:
    Node(Node* parent, int index, int length) {
        Create(parent, index, length);
    }
    Node* GetRoot();
private:
    Node* parent_;
    int index_;
    int length_;

    void Create(Node* parent, int index, int length) :
        parent_(parent), index_(index), length_(length){}
};
*/

class DSU {
public:
    DSU(size_t size);

    int Find(int u);

    int BadFind(int u) const;

    bool Merge(int u, int v);

    void print() {
        std::cout << "\nlook at presents:";
        for (int i = 0; i < parents_.size(); i++) {

        }
    }

private:
    std::vector<int> parents_;
    std::vector<int> rangs_;
};

// Возвращаем представителя множества
int DSU::Find(int u) {
    int i = u;
    //while (parents_[i] != NO_PARENT) {
    //    i = parents_[i];
    //}

    if (parents_[u] != NO_PARENT) {
        return parents_[u];
    }
    return parents_[u] = Find(parents_[u]);
}

// Возвращаем представителя множества
int DSU::BadFind(int u) const {
    int i = u;
    while (parents_[i] != NO_PARENT) {
        i = parents_[i];
    }
    return i;
}

bool DSU::Merge(int u, int v) {
    int present_u = Find(u);
    int present_v = Find(v);
    if (present_u != present_v) {
        if (rangs_[present_u] > rangs_[present_v]) {
            parents_[present_v] = present_u;
        } else if (rangs_[present_u] < rangs_[present_v]) {
            parents_[present_u] = present_v;
        } else {
            parents_[present_u] = present_v;
            ++rangs_[present_v];
        }
        return true;
    }
    return false;
}

DSU::DSU(size_t size) {
    parents_ = std::vector<int>(size, -1);
    rangs_ = std::vector<int>(size, 1);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    return a.exec();
}
