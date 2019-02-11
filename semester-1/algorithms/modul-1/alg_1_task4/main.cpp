/*
 * Дан массив целых чисел A[0..n), n не превосходит
 * 100 000. Так же задан размер некотрого окна
 * (последовательно расположенных элементов массива)
 *  в этом массиве k, k<=n. Требуется для каждого
 *  положения окна (от 0 и до n-k) вывести значение
 * максимума в окне. Скорость работы O(n log n), память
 *  O(n).
 */

#include <iostream>
#include <assert.h>

#define BUFFER 64

class Functor {
public:
    Functor(const int *arr, const size_t size_of_arr)
        : array(arr), size(size_of_arr) {}
    int operator()(size_t index);

private:
    const int *array;
    const size_t size;
};

class Heap {
public:
    Heap(Functor func) : functor(func), real_size(0), max_size(BUFFER),
       indexes(nullptr)
    {
        try {
            indexes = static_cast<size_t *>(calloc(max_size, sizeof(size_t)));
            if (!indexes) {
                throw - 1;
            }
        } catch (...) {
            std::cout << "error";
        }
    }
    ~Heap();

    void add(size_t number);
    size_t getMax() const;
    size_t extractMax();

    Functor functor;

private:
    size_t *indexes;
    size_t real_size;
    size_t max_size;

    int get(size_t index);
    void grow();
    void siftUp();
    void siftDown();

    void getChildrenIndexes(size_t parent, size_t &left, size_t &right) const;
};

void get_answer(Heap &heap, size_t size_of_array, size_t size_of_window);

int main() {
    size_t size_of_array = 0;
    std::cin >> size_of_array;
    if (size_of_array < 1) {
        std::cout << "error";
        return 0;
    }

    int *array = static_cast<int *>(calloc(size_of_array, sizeof(int)));
    for (size_t i = 0; i < size_of_array; i++) {
        std::cin >> array[i];
    }

    size_t size_of_window = 0;
    std::cin >> size_of_window;
    if (size_of_window < 1) {
        free(array);
        return 0;
    }
    Functor functor(array, size_of_array);

    Heap heap(functor);
    get_answer(heap, size_of_array, size_of_window);
    free(array);
    return 0;
}

void get_answer(Heap &heap, size_t size_of_array, size_t size_of_window) {
    assert(size_of_array != 0);
    assert(size_of_window != 0);

    if (size_of_array < size_of_window) {
        size_of_window = size_of_array;
    }

    for (size_t i = 0; i < size_of_window; i++) {
        heap.add(i);
    }
    size_t i = 0;
    while (i <= size_of_array - size_of_window) {
        while (heap.getMax() < i) {
            heap.extractMax();
        }
        std::cout << " " << (heap.functor)(heap.getMax());
        if (i != size_of_array - size_of_window) heap.add(i + size_of_window);
        i++;
    }
}

Heap::~Heap() {
    free(indexes);
}

void Heap::getChildrenIndexes(size_t parent, size_t &left,
                              size_t &right) const {
    left = parent;
    right = parent;

    if (2 * parent + 1 < real_size) {
        left = 2 * parent + 1;
    }
    if (2 * parent + 2 < real_size) {
        right = 2 * parent + 2;
    }
}

void swap(size_t &a, size_t &b) {
    size_t tmp = a;
    a = b;
    b = tmp;
}

int Heap::get(size_t index) {
    return (functor)(indexes[index]);
}

void Heap::siftDown() {
    size_t i = 0;
    size_t left_child = i, right_child = i;
    while (i < real_size) {
        getChildrenIndexes(i, left_child, right_child);
        if (get(i) < get(left_child) || get(i) < get(right_child)) {
            if (get(left_child) < get(right_child)) {
                swap(indexes[i], indexes[right_child]);
                i = right_child;
            } else {
                swap(indexes[i], indexes[left_child]);
                i = left_child;
            }
        } else {
            break;
        }
    }
}

void Heap::grow() {
    max_size *= 2;
    size_t *tmp = nullptr;
    try {
        tmp = static_cast<size_t *>(realloc(indexes, max_size * sizeof(size_t)));
        if (!tmp) {
            delete indexes;
            throw 0;
        }
        indexes = tmp;
    } catch (...) {
        std::cout << "error";
    }
}

void Heap::add(size_t number) {
    indexes[real_size++] = number;
    if (real_size >= max_size - 1) {
        grow();
    }
    siftUp();
}

void Heap::siftUp() {
    size_t child = real_size - 1;
    size_t parent = (child - 1) / 2;
    while (child > 0 && get(parent) < get(child)) {
        swap(indexes[child], indexes[parent]);
        child = parent;
        parent = (child - 1) / 2;
    }
}

size_t Heap::extractMax() {
    assert(indexes);
    size_t ret = indexes[0];
    indexes[0] = indexes[--real_size];
    siftDown();

    return ret;
}

size_t Heap::getMax() const {
    assert(indexes);
    return indexes[0];
}

int Functor::operator()(size_t index) {
    assert(index < size);
    return array[index];
}
