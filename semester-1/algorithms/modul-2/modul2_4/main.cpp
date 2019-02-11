/*
 * Дано число N и N строк. Каждая строка содержащит команду
 * добавления или удаления натуральных чисел, а также запрос на
 * получение k-ой порядковой статистики. Команда добавления числа
 * A задается положительным числом A, команда удаления числа A
 * задается отрицательным числом “-A”. Запрос на получение k-ой
 * порядковой статистики задается числом k. Требуемая скорость
 * выполнения запроса - O(log n).
 */

#include <iostream>
#include <vector>

class Node {
public:
    Node(int k) : left_node(nullptr),
        right_node(nullptr), height(1), key(k),
        left_size(0), right_size(0){}

    void print();
    void free_memory();
    unsigned char getHeight(Node *node) const;
    Node* remove(Node* node, int k);

    Node* insert(Node* node, int k);

    int stat(int index);

private:
    Node* balance(Node* node);
    void fixHeight(Node* node);
    int balanceFactor(Node* node);

    Node* removeMin(Node* node);
    Node* findMin(Node* node);

    Node* rotateRight(Node* p);
    Node* rotateLeft(Node* p);

    int getNodeAmount(Node *node);

    Node *left_node;
    Node *right_node;
    unsigned char height;
    int left_size;
    int right_size;
    int key;
};

int main()
{
    int size = 0;
    std::cin >> size;

    int num;
    int index;

    Node *tree = nullptr;

    for (int i = 0; i < size && std::cin >> num >> index; i++) {
        if (num > 0) {
            if (!tree) {
                tree = new Node(num);
            } else {
                tree = tree->insert(tree, num);
            }
        } else {
            tree = tree->remove(tree, -num);
        }
        if (tree) {
            //tree->print();
            std::cout << tree->stat(index) << "\n";
        }
    }
    if (tree) {
        tree->free_memory();
        free(tree);
    }

    return 0;
}

Node* Node::insert(Node* node, int k) // вставка ключа k в дерево с корнем p
{
    if(!node) {
        return new Node(k);
    }
    if(k < node->key) {
        node->left_node = insert(node->left_node,k);
    } else {
        node->right_node = insert(node->right_node,k);
    }
    return balance(node);
}

Node* Node::balance(Node* node) {
    fixHeight(node);
    if(balanceFactor(node) == 2) {
        if( balanceFactor(node->right_node) < 0 ) {
            node->right_node = rotateRight(node->right_node);
        }
        return rotateLeft(node);
    }
    if(balanceFactor(node) == -2)
    {
        if( balanceFactor(node->left_node) > 0  )
            node->left_node = rotateLeft(node->left_node);
        return rotateRight(node);
    }
    return node;
}

Node* Node::rotateRight(Node* p) {
    Node* q = p->left_node;
    p->left_node = q->right_node;
    q->right_node = p;
    fixHeight(p);
    fixHeight(q);
    return q;
}

Node* Node::rotateLeft(Node* q) {
    Node* p = q->right_node;
    q->right_node = p->left_node;
    p->left_node = q;
    fixHeight(q);
    fixHeight(p);
    return p;
}

unsigned char Node::getHeight(Node *node) const {
    if (!node) {
        return 0;
    } else {
        return node->height;
    }
}

void Node::fixHeight(Node* node)
{
    unsigned char hl = getHeight(node->left_node);
    unsigned char hr = getHeight(node->right_node);
    if (hl > hr) {
        node->height = hl;
    } else {
        node->height = hr;
    }


    node->left_size = getNodeAmount(node->left_node);
    node->right_size = getNodeAmount(node->right_node);

    node->height = node->height + 1;
}

int Node::balanceFactor(Node* node) {
    if (node) {
        return getHeight(node->right_node) - getHeight(node->left_node);
    }
    return 0;
}

Node* Node::findMin(Node* node) {
    if (node->left_node) {
        return findMin(node->left_node);
    }
    return node;
}

Node* Node::removeMin(Node* node) {
    if(!node->left_node) {
        return node->right_node;
    }
    node->left_node = removeMin(node->left_node);
    return balance(node);
}

Node* Node::remove(Node* node, int k) {
    if (!node) {
        return nullptr;
    }
    if (k < node->key) {
        node->left_node = remove(node->left_node, k);
    } else if (k > node->key) {
        node->right_node = remove(node->right_node, k);
    } else {
        Node* left = node->left_node;
        Node* right = node->right_node;
        delete node;
        if (!right) {
            return left;
        }

        Node *min = findMin(right);
        min->right_node = removeMin(right);
        min->left_node = left;
        return balance(min);
    }
    return balance(node);
}

void Node::print() {
    std::vector<Node*> curr_lvl;
    std::vector<Node*> next_lvl;

    curr_lvl.push_back(this);
    std::cout << "\n Tree \n";
    while (curr_lvl.size() != 0) {
        for (auto elem: curr_lvl) {
            if (elem->left_node) {
                next_lvl.push_back(elem->left_node);
            }
            if (elem->right_node) {
                next_lvl.push_back(elem->right_node);
            }
            for(int a = 0; a < elem->left_size; a++){
                std::cout << "   ";
            }
            std::cout << elem->key << "(" << elem->left_size <<
                      "," << elem->right_size << ")";
            for(int a = 0; a < elem->right_size; a++){
                std::cout << "   ";
            }
        }
        std::cout << "\n";
        curr_lvl.clear();
        curr_lvl = next_lvl;
        next_lvl.clear();
    }
}

int Node::stat(int index) {
    Node *move = this;
    if (index > (left_size + right_size)) {
        return -1;
    }

    // Индексы левой части предка и самого предка
    int parent_left_indexes = 0;

    while (move) {
        if (move->left_size + parent_left_indexes == index) {
            return move->key;
        } else if (move->left_size + parent_left_indexes < index) {
            parent_left_indexes += move->left_size + 1;
            move = move->right_node;
        } else {
            move = move->left_node;
        }
    }
    return -1;
}

int Node::getNodeAmount(Node *node) {
    if (!node) {
        return 0;
    }
    return 1 + node->left_size + node->right_size;
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
            free(elem);
        }
        curr_lvl.clear();
        curr_lvl = next_lvl;
        next_lvl.clear();
    }
}
