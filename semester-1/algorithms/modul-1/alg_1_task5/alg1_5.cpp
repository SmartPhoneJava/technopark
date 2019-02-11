/*В супермаркете решили оптимизировать показ рекламы.
 *  Известно расписание прихода и
 *  ухода покупателей (два целых числа).
 *  Каждому покупателю необходимо показать минимум 2
 *  рекламы. Рекламу можно транслировать только в
 *  целочисленные моменты времени. Покупатель
 *  может видеть рекламу от момента прихода до момента
 *  ухода из магазина. В каждый момент
 *  времени может показываться только одна реклама.
 *  Считается, что реклама показывается
 *  мгновенно. Если реклама показывается в момент ухода
 *  или прихода, то считается, что
 *  посетитель успел её посмотреть.
 * Требуется определить минимальное число показов рекламы.
 * */

#include <iostream>
#include <assert.h>
#include <time.h>

#define BUFFER 64

typedef struct Pair {
public:
    int start;
    int end;
    int saw = 0;

    Pair(int start_ = 0, int end_ = 0) : start(start_), end(end_), saw(0){}
    bool operator<(Pair pair);

    friend std::ostream& operator<< (std::ostream &out, const Pair &pair);
} Pair;

class PairArray {
public:
    void grow();
    void add(Pair pair);

    PairArray();
    ~PairArray(){free(pairs);}

    Pair* pairs;

    size_t real_size;
    size_t max_size;

    void answer();

    friend std::ostream& operator<< (std::ostream &out, const PairArray &parr);
};

int main() {
    std::ios::sync_with_stdio(false);
    int amount_of_pairs;
    std::cin >> amount_of_pairs;
    PairArray parr;
    for (int i = 0; i < amount_of_pairs; i++) {
        int a, b;
        std::cin >> a >> b;
        parr.add(Pair(a,b));
    }
    parr.answer();
    return 0;
}

bool Pair::operator<(Pair pair) {
    return (this->end < pair.end) || ((this->end == pair.end) && (this->start > pair.start));
}

void PairArray::grow() {
    max_size *= 2;
    Pair *tmp = nullptr;
    try {
        tmp = static_cast<Pair*>(realloc(pairs, max_size * sizeof(Pair)));
        if (!tmp) {
            delete pairs;
            throw 0;
        }
        pairs = tmp;
    } catch (...) {
        std::cout << "error";
    }
}

PairArray::PairArray() {
    real_size = 0;
    max_size = BUFFER;
    pairs = nullptr;
    try {
        pairs = static_cast<Pair *>(calloc(max_size, sizeof(Pair)));
        if (!pairs) {
            throw - 1;
        }
    } catch (...) {
        std::cout << "error";
    }
}

void swap(Pair &a, Pair &b) {
    Pair tmp = a;
    a = b;
    b = tmp;
}

int Partition(Pair* a, size_t n ) {
    if( n <= 1 ) {
        return 0;
    }

    srand(time(NULL));
    int random = rand() % (n - 1);
    swap(a[random], a[n - 1]);
    const Pair& pivot = a[n - 1];
    int i = 0, j = n - 2;
    while( i <= j ) {
        for( ; a[i] < pivot; ++i ) {}
        for( ; j >= 0 && !( a[j] < pivot ); --j) {}
        if( i < j ) {
            swap(a[i++], a[j--] );
        }
    }
    swap( a[i], a[n - 1] );
    return i;
}

void QuickSort(Pair* arr, size_t n) {
    size_t part = Partition(arr, n);
    if (part > 0) {
        QuickSort(arr, part);
    }
    if (part + 1 < n) {
        QuickSort(arr + part + 1, n - (part + 1));
    }
}

void PairArray::add(Pair pair) {
    pairs[real_size++] = pair;
    if (real_size >= max_size - 1) {
        grow();
    }
}

void PairArray::answer() {
    QuickSort(pairs,real_size);
    int answers = 2;

    int end = pairs[0].end;
    int start = end - 1;

    for (int i = 1; i < real_size; i++) {
        if (end <= pairs[i].end && start >= pairs[i].start) {
            // Если следующая пара времен 'вложена' в текущую
            // например текущая [4,7], а следующая [4,5]
            continue;
        } else if (end >= pairs[i].start) {
            // Если текущая пара 'пересекается' со следующей
            // [2,4] и [3,5]
            start = end;
            end = pairs[i].end;
            answers += 1;
        } else  {
            // Если следующая пара времен не пересекается с текущей
            // [12, 15], а следующая [16,20]
            end = pairs[i].end;
            start = end - 1;
            answers += 2;
        }
    }
    std::cout << answers;
}


std::ostream& operator<< (std::ostream &out, const Pair &pair) {
    std::cout << "\n" << pair.start << " " << pair.end;
    return out;
}

std::ostream& operator<< (std::ostream &out, const PairArray &parr) {
    for(size_t i = 0; i < parr.real_size; i++) {
        std::cout << parr.pairs[i];
    }
    return out;
}
