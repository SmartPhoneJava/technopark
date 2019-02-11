#include <vector>
#include <iostream>

/*
//18:06
class HTree {
public:
    HTree(std::vector<std::pair<char, int>> table) {
        build(table);
    }
private:
    void build(std::vector<std::pair<char, int>> in) {
        out.clear();

        // Отсортировать in
        int umn = 1;
        int in_size = static_cast<int>(in.size());
        while (in_size != 1) {
            int j = 0;
            for (int i = 0, s = 0; i < in_size; i++) {
                // "Удаление элемента" происходит
                // установкой частотности в -1
                if (in[i].first >= 0) {
                    int mod2 = j % 2;
                    // Если элемент четный, бит 1
                    if (mod2 == 0) {
                        mod2++;
                    } else { // иначе бит 0
                        mod2--;
                        in[i - 1].second = -1;
                    }

                    // Если массив out еще не создан
                    if (i == out.size()) {
                        out.push_back(std::make_pair(
                                          in[i].first,
                                          mod2));
                    } else {
                        for (int k = s; s < j; s++) {
                            out[k].second += mod2 * umn;
                        }
                    }
                    s = i;
                    j++;
                }
            }
            in_size = j;
            umn *= 10;
        }
    }

    std::vector<std::pair<char, int>> out;
};

std::vector<std::pair<char, char*>> code(
        std::vector<std::pair<char, int>>& in) {


    // Отсортировать in
    int move = 0;
    int in_size = static_cast<int>(in.size());

    //char *mas = new char(10);
    //for (int i = 0; i < 10; i++) {
    //    mas[i] = '0';
    //}

    std::vector<std::pair<char, char*>> out;
    for (int i = 0; i < 10; i++) {
        char *mas = new char(10);
        mas[i] = '0';
        out.push_back(std::make_pair('f', mas));
    }
    //(in_size,
    //std::make_pair('-',mas));

    int real_size = in_size;
    while (real_size > 1) {
         std::cout << "\nhelp " << real_size;
        int j = 0;
        for (int i = 0, s = 0; i < in_size; i++) {
            // "Удаление элемента" происходит
            // установкой частотности в -1
            if (in[i].second >= 0) {
                int mod2 = j % 2;
                // Если элемент четный, бит 1
                if (mod2 == 0) {
                    mod2++;
                } else { // иначе бит 0
                    mod2--;
                    in[s - 1].second = -1;
                }
                //std::cout << "\n i = " <<
                //             i << ", j =" << j;
                if (!(j % 2 == 0 && i == (in_size - 1))) {
                        for (int k = s; k <= i; k++) {
                            ;
                            //out[k].second[move] = mod2;
                        }

                    s = i + 1;
                }
                j++;
            }
        }
        real_size = j;
        move++;
        if (move > 9) {
            std::cout << "i tryied...";
        }
    }

    return out;
}


class Node {
public:
    Node(std::pair<char, int> pair) :
        symbol(pair.first), key(pair.second){}

private:

    //Node *left_node;
    //Node *right_node;
    //Node *parent;
    int key;
    char symbol;
};

int main(int argc, char *argv[])
{
    std::vector<std::pair<char, int>> in;
    in.push_back(std::make_pair('q',5));
    in.push_back(std::make_pair('w',6));
    in.push_back(std::make_pair('e',6));
    in.push_back(std::make_pair('r',7));
    in.push_back(std::make_pair('t',15));
    in.push_back(std::make_pair('y',5));
    in.push_back(std::make_pair('u',6));
    in.push_back(std::make_pair('i',6));
    in.push_back(std::make_pair('o',7));
    in.push_back(std::make_pair('p',15));
    in.push_back(std::make_pair('a',5));
    in.push_back(std::make_pair('s',6));
    in.push_back(std::make_pair('d',6));
    in.push_back(std::make_pair('f',7));
    in.push_back(std::make_pair('g',15));
    in.push_back(std::make_pair('h',5));
    in.push_back(std::make_pair('j',6));
    in.push_back(std::make_pair('k',6));
    in.push_back(std::make_pair('l',7));
    in.push_back(std::make_pair('z',5));
    in.push_back(std::make_pair('x',6));
    in.push_back(std::make_pair('c',6));
    in.push_back(std::make_pair('v',7));
    in.push_back(std::make_pair('b',15));
    in.push_back(std::make_pair('n',5));
    in.push_back(std::make_pair('m',6));
    in.push_back(std::make_pair(',',6));
    in.push_back(std::make_pair('.',7));
    in.push_back(std::make_pair(';',15));
    in.push_back(std::make_pair('/',5));
    in.push_back(std::make_pair(']',6));
    in.push_back(std::make_pair('[',6));
    in.push_back(std::make_pair('{',7));
    in.push_back(std::make_pair('}',15));
    in.push_back(std::make_pair(':',5));
    in.push_back(std::make_pair('@',6));
    in.push_back(std::make_pair('"',6));
    in.push_back(std::make_pair('1',7));
    in.push_back(std::make_pair('2',7));
    in.push_back(std::make_pair('3',7));
    in.push_back(std::make_pair('4',7));
    in.push_back(std::make_pair('5',7));

    std::vector<std::pair<char, char*>> out = code(in);

    for (int i = 0; i < out.size(); i++) {
        std::cout << "\n" << out[i].first << " - ";// <<
                     //out[i].second;
    }

    int some;
    std::cin >> some;

    return 0;
}
*/

// 20:31

typedef struct Bits10
{
    unsigned value: 10; // 10 bits
} Bits10;

typedef std::pair<char, Bits10> SymbolPair;
typedef std::vector<SymbolPair> Symbols;


class Node {
public:
    Node(std::pair<char, Bits10> pair) :
        symbol(pair.first), bits(pair.second){
        left_node = right_node = nullptr;
    }

    Node(Node* node){
        left_node = node;
        bits = node->bits;
        right_node = nullptr;
        //left = node->left;
        //right = node->right;

        // Чтобы отличать составные узлы от конечных
        symbol = '_';
    }

    Node *left_node;
    Node *right_node;
    // Всегда одинаковые нет смысла хранить
    //Bits10 left;
    //Bits10 right;
    Bits10 bits;
    char symbol;
};

//18:06
class HTree {
public:
    HTree(Symbols table) {
        build(table);
    }



private:
    void build(Symbols in) {
        out.clear();

        // Отсортировать in

        size_t in_size = in.size();

        std::vector<Node*> prev;
        std::vector<Node*> curr;

        // Инициализация
        for (size_t i = 0; i < in_size; i++) {
            Bits10 b;
            b.value = 0;
            out.push_back(std::make_pair(in[i].first, b));
            curr.push_back(new Node(in[i]));
        }

        while (curr.size() > 1) {
            prev = std::move(curr);
            for (size_t i = 0; i < prev.size(); i++) {
                if ( i % 2 == 0) { // 0 2 4
                    curr.push_back(new Node(prev[i]));
                } else {
                    // Поскольку добавили итерацией назад, то обращаемся
                    // к последнему элементу
                    curr.back()->right_node = prev[i];
                    curr.back()->bits.value += prev[i]->bits.value;
                }
            }
        }
        tree = std::move(curr[0]);

        // Дерево построено
        // теперь проиниализируем таблицу кодов out

        // Да, вектора чистые, но на всякий случай, потом лучше удалю
        prev.clear();
        curr.clear();

        Symbols old_out;
        Symbols cur_out;

        curr.push_back(tree);

        int umn = 1;

        while (curr.size() != in_size) {
            prev = std::move(curr);
            for (size_t i = 0; i < prev.size(); i++) {
                if (prev[i]->left_node) { // 0 2 4
                    curr.push_back(prev[i]->left_node);
                    curr_out.push_back(std::make_pair(prev[i]->symbol, ))
                } else {
                    // Поскольку добавили итерацией назад, то обращаемся
                    // к последнему элементу
                    curr.back()->right_node = prev[i];
                    curr.back()->bits.value += prev[i]->bits.value;
                }
            }

            umn *= 10;
        }



    }
    std::vector<std::pair<char, Bits10>> out;
    Node* tree;
};

int main(int argc, char *argv[])
{
    std::vector<std::pair<char, int>> in;
    in.push_back(std::make_pair('q',5));
    in.push_back(std::make_pair('w',6));
    in.push_back(std::make_pair('e',6));
    in.push_back(std::make_pair('r',7));
    in.push_back(std::make_pair('t',15));
    in.push_back(std::make_pair('y',5));
    in.push_back(std::make_pair('u',6));
    in.push_back(std::make_pair('i',6));
    in.push_back(std::make_pair('o',7));
    in.push_back(std::make_pair('p',15));
    in.push_back(std::make_pair('a',5));
    in.push_back(std::make_pair('s',6));
    in.push_back(std::make_pair('d',6));
    in.push_back(std::make_pair('f',7));
    in.push_back(std::make_pair('g',15));
    in.push_back(std::make_pair('h',5));
    in.push_back(std::make_pair('j',6));
    in.push_back(std::make_pair('k',6));
    in.push_back(std::make_pair('l',7));
    in.push_back(std::make_pair('z',5));
    in.push_back(std::make_pair('x',6));
    in.push_back(std::make_pair('c',6));
    in.push_back(std::make_pair('v',7));
    in.push_back(std::make_pair('b',15));
    in.push_back(std::make_pair('n',5));
    in.push_back(std::make_pair('m',6));
    in.push_back(std::make_pair(',',6));
    in.push_back(std::make_pair('.',7));
    in.push_back(std::make_pair(';',15));
    in.push_back(std::make_pair('/',5));
    in.push_back(std::make_pair(']',6));
    in.push_back(std::make_pair('[',6));
    in.push_back(std::make_pair('{',7));
    in.push_back(std::make_pair('}',15));
    in.push_back(std::make_pair(':',5));
    in.push_back(std::make_pair('@',6));
    in.push_back(std::make_pair('"',6));
    in.push_back(std::make_pair('1',7));
    in.push_back(std::make_pair('2',7));
    in.push_back(std::make_pair('3',7));
    in.push_back(std::make_pair('4',7));
    in.push_back(std::make_pair('5',7));

    std::vector<std::pair<char, char*>> out = code(in);

    for (int i = 0; i < out.size(); i++) {
        std::cout << "\n" << out[i].first << " - ";// <<
                     //out[i].second;
    }

    int some;
    std::cin >> some;

    return 0;
}
