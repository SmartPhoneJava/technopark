/*
 * Дано число N < 106 и последовательность целых
 * чисел из [-231..231] длиной N.
 * Требуется построить бинарное дерево поиска, заданное наивным
 * порядком вставки. Т.е., при добавлении очередного числа K
 * в дерево с корнем root, если root→Key ≤ K, то узел K
 * добавляется в правое поддерево root; иначе в левое
 * поддерево root.
 * Выведите элементы в порядке level-order (по слоям,
 * “в ширину”).
 */

#include <vector>
#include <string>
#include <iostream>


class Node {
public:
    Node(int k) : key(k), left_node(nullptr),
        right_node(nullptr){}

    void add(int k);
    void print();
    void free_memory();
private:
    Node *left_node;
    Node *right_node;
    int key;
};

int main()
{
    int amount = 0;
    std::cin >> amount;
    amount--;

    int key;
    std::cin >> key;
    Node tree(key);

    for(int i = 0; i < amount && std::cin >> key; i++) {
        tree.add(key);
    }
    tree.print();
    tree.free_memory();

    return 0;
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
        //curr_lvl.clear();
        curr_lvl = std::move(next_lvl);
        //curr_lvl = next_lvl;
        //next_lvl.clear();
    }
}

void Node::free_memory() {
    std::vector<Node*> curr_lvl;
    std::vector<Node*> next_lvl;

    if (this->left_node) {
        curr_lvl.push_back(this->left_node);
    }
    if (this->right_node) {
        curr_lvl.push_back(this->right_node);
    }

    while (curr_lvl.size() != 0) {
        for (auto elem: curr_lvl) {
            if (elem->left_node) {
                next_lvl.push_back(elem->left_node);
            }
            if (elem->right_node) {
                next_lvl.push_back(elem->right_node);
            }
            delete elem;
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
        if (node->key <= k) {
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
