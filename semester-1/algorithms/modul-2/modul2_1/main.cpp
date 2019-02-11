/*
 * Реализуйте структуру данных типа “множество строк” на
 * основе динамической хеш-таблицы с открытой адресацией.
 * Хранимые строки непустые и состоят из строчных латинских букв.
 * Хеш-функция строки должна быть реализована с помощью
 * вычисления значения многочлена методом Горнера.
 * Начальный размер таблицы должен быть равным 8-ми.
 * Перехеширование выполняйте при добавлении элементов в
 * случае, когда коэффициент заполнения таблицы достигает 3/4.
 * Структура данных должна поддерживать операции добавления
 * строки в множество, удаления строки из множества и
 * проверки принадлежности данной строки множеству.
 * 1_2. Для разрешения коллизий используйте двойное
 * хеширование.
*/

#include <vector>
#include <string>
#include <iostream>

#define DEFAULT_SIZE 8

class HashTtable {
public:
    HashTtable(int size) : N(0), M(size) {
        std::vector<std::string> array(M, "");
        this->array = array;
        A = 179;
    }

    bool add(std::string str);
    bool find(std::string str);
    bool remove(std::string str);
private:
    int N; // количество занятых ячеек(включая deleted)
    int M; // максимальное количество ячеек;
    int A;
    std::vector<std::string> array;

    bool isNil(int i) const;

    bool isDelete(int i) const;

    bool rehash();

    int h(int h1_, int h2_, int i) const;

    int h1(std::string str, int m) const;

    int h2(std::string str) const;

};

int main(int argc, char *argv[])
{
    HashTtable ht(DEFAULT_SIZE);

    char s;
    std::string str;
    bool result = false;;
    while (std::cin >> s && std::cin >> str) {
        switch(s) {
        case '+':
            result = ht.add(str);
            break;
        case '-':
            result = ht.remove(str);
            break;
        case '?':
            result = ht.find(str);
            break;
        default:
            result = false;
        }
        if (result) {
            std::cout << "OK\n";
        } else {
            std::cout << "FAIL\n";
        }
    }

    return 0;
}

int HashTtable::h1(std::string str, int m) const{
    int hash = 0;
    for (char elem :str) {
        hash = (hash * A + elem) % m;
    }

    return hash;
}

// Всегда возвращает нечетные значения
int HashTtable::h2(std::string str) const {
    return (str[0] + !(str[0] % 2));
}

bool HashTtable::isNil(int i) const{
    return array[i].size() == 0;
}

bool HashTtable::isDelete(int i) const{
    return (array[i] == "delete");
}

bool HashTtable::add(std::string str) {
    int h1_(h1(str, M));
    int h2_(h2(str));

    int i_found_deleted_place = -1;

    for(int i = 0; i < M; ++i ) {
        int j = h(h1_, h2_, i);
        if(isNil(j)) {
            N = N + 1;
            array[j] = str;

            if (double(N)/double(M) >= 0.75) {
                return rehash();
            }
            return true;
        } else if (isDelete(j)) {
            i_found_deleted_place = j;
        } else if (str == array[j]) {
            return false;
        }
    }
    if (i_found_deleted_place > 0) {
        array[i_found_deleted_place] = str;

        if (double(N)/double(M) >= 0.75) {
            return rehash();
        }
        return true;
    }
    return false;
}

bool HashTtable::find(std::string str) {
    int h1_(h1(str, M));
    int h2_(h2(str));

    for( int i = 0; i < M; ++i ) {
        int j = h(h1_, h2_, i);
        if(isNil(j)) {
            return false;
        } else if (str == array[j]) {
            return true;
        }
    }
    return false;
}

bool HashTtable::remove(std::string str) {
    int h1_(h1(str, M));
    int h2_(h2(str));

    for(int i = 0; i < M; ++i ) {
        int j = h(h1_, h2_, i);
        if(isNil(j)) {
            return false;
        } else if (str == array[j]) {
            array[j] = "deleted";
            return true;
        }
    }
    return false;
}

bool HashTtable::rehash() {
    M = M * 2;
    std::vector<std::string> array_save = array;
    std::vector<std::string> new_array(M);
    array = new_array;
    N = 0;
    for (auto elem : array_save) {
        if ((elem.size() != 0) && (elem != "deleted")) {
            if (!(add(elem))) {
                return false;
            }
        }
    }
    return true;
}

int HashTtable::h(int h1_, int h2_, int i) const {
    return (h1_ + i * h2_) % M;
}
