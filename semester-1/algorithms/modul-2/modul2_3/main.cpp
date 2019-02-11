/*
 * Дано число N < 106 и последовательность пар целых чисел из
 *  [-231, 231] длиной N. Построить декартово дерево из N узлов,
 *  характеризующихся парами чисел (Xi, Yi). Каждая пара чисел
 *  (Xi, Yi) определяет ключ Xi и приоритет Yi в декартовом
 *  дереве. Добавление узла в декартово дерево выполняйте
 *  второй версией алгоритма, рассказанного на лекции: При
 *  добавлении узла (x, y) выполняйте спуск по ключу до узла
 *  P с меньшим приоритетом. Затем разбейте найденное поддерево
 *  по ключу x так, чтобы в первом поддереве все ключи меньше x,
 *  а во втором больше или равны x. Получившиеся два дерева
 *  сделайте дочерними для нового узла (x, y). Новый узел
 *  вставьте на место узла P. Построить также наивное дерево
 *  поиска по ключам Xi. Равные ключи добавляйте в правое
 *  поддерево. Вычислить количество узлов в самом широком
 *  слое декартового дерева и количество узлов в самом широком
 *  слое наивного дерева поиска. Вывести их разницу. Разница
 *  может быть отрицательна.
 */

#include <iostream>
#include <vector>
#include <queue>

class Treap {
public:
    Treap(int k, int pr) : left_node(nullptr),
        right_node(nullptr), key(k), priority(pr){}

    ~Treap();

    void print();

    Treap* insert(int k, int pr);

    Treap* merge(Treap* left, Treap* right);

    void split(Treap* current, int key,
               Treap*& left, Treap*& right);

     int bfs();

private:
    Treap *left_node;
    Treap *right_node;
    int key;
    int priority;
};

class Node {
public:
    Node(int k) : key(k), left_node(nullptr),
        right_node(nullptr){}

    ~Node();

    void add(int k);
    void print();

    int bfs();

private:
    Node *left_node;
    Node *right_node;
    int key;
};


int main()
{
    int size = 0;
    std::cin >> size;

    if (size == 0) {
        return 0;
    }

    int key;
    int pr;

    Node *node = nullptr;
    Treap *treap = nullptr;

    for (int i = 0; i < size && std::cin >> key >> pr; i++) {
        if (!node) {
            node = new Node(key);
            treap = new Treap(key, pr);
        } else {
            node->add(key);
            treap = treap->insert(key, pr);
        }
    }
    std::cout << (treap->bfs() - node->bfs());

    delete node;
    delete treap;

    return 0;
}

Treap* Treap::insert(int k, int pr) {
    int choose = 1;
    Treap *node = this;
    Treap *parent = nullptr;
    while (node && node->priority >= pr) {
        parent = node;
        if (node->key >= k) {
            choose = 1;
            node = node->left_node;
        } else {
            choose = 2;
            node = node->right_node;
        }
    }

    Treap *T1 = nullptr, *T2 = nullptr;
    split(node, k, T1, T2);
    Treap *insert = new Treap(k, pr);
    insert->left_node = T1;
    insert->right_node = T2;

    if (!parent) {
        return insert;
    }
    node = insert;
    if (choose == 1) {
        parent->left_node = insert;
    } else {
         parent->right_node = insert;
    }

    return this;
}

void Treap::split(Treap* current, int key,
                 Treap*& left, Treap*& right) {
    if (!current) {
        left = right = nullptr;
    } else if( current->key <= key ) {
        split(current->right_node, key,
              current->right_node, right );
        left = current;
    } else {
        split(current->left_node, key,
              left, current->left_node );
        right = current;
    }
}

Treap* Treap::merge(Treap* left, Treap* right) {
    if (left == nullptr) {
        return right;
    }
    if (right == nullptr) {
        return left;
    }

    if (left->priority > right->priority ) {
        left->right_node = merge(left->right_node, right);
        return left;
    } else {
        right->left_node = merge(left, right->left_node);
        return right;
    }
}

void Node::print() {
    std::vector<Node*> curr_lvl;
    std::vector<Node*> next_lvl;

    curr_lvl.push_back(this);

    while (curr_lvl.size() != 0) {
        for (auto elem: curr_lvl) {
            if (elem->right_node) {
                next_lvl.push_back(elem->right_node);
            }
            if (elem->left_node) {
                next_lvl.push_back(elem->left_node);
            }
            std::cout << elem->key << " ";
        }
        curr_lvl.clear();
        curr_lvl = next_lvl;
        next_lvl.clear();
    }
}

void Node::add(int k) {
    Node *node = this;
    bool inserted = false;

    while (!inserted) {
        if (node->key < k) {
            if (!node->left_node) {
                inserted = true;
                node->left_node = new Node(k);
            } else {
                node = node->left_node;
            }
        } else {
            if (!node->right_node) {
                inserted = true;
                node->right_node = new Node(k);
            } else {
                node = node->right_node;
            }
        }
    }
}

// Breadth-first search
int Node::bfs() {
    std::vector<Node*> curr_lvl;
    std::vector<Node*> next_lvl;

    curr_lvl.push_back(this);

    int max_width = 0;

    while (curr_lvl.size() != 0) {
        for (auto elem: curr_lvl) {
            if (elem->right_node) {
                next_lvl.push_back(elem->right_node);
            }
            if (elem->left_node) {
                next_lvl.push_back(elem->left_node);
            }
        }
        if (curr_lvl.size() > max_width) {
            max_width = curr_lvl.size();
        }
        curr_lvl = std::move(next_lvl);
    }
    return max_width;
};

int Treap::bfs() {
    std::vector<Treap*> curr_lvl;
    std::vector<Treap*> next_lvl;

    curr_lvl.push_back(this);

    int max_width = 0;

    while (curr_lvl.size() != 0) {
        for (auto elem: curr_lvl) {
            if (elem->right_node) {
                next_lvl.push_back(elem->right_node);
            }
            if (elem->left_node) {
                next_lvl.push_back(elem->left_node);
            }
        }
        if (curr_lvl.size() > max_width) {
            max_width = curr_lvl.size();
        }
        curr_lvl = std::move(next_lvl);
    }
    return max_width;
};

Node::~Node() {
    if (left_node) {
        delete left_node;
    }
    if (right_node) {
        delete right_node;
    }
}

Treap::~Treap() {
    if (left_node) {
        delete left_node;
    }
    if (right_node) {
        delete right_node;
    }
}
