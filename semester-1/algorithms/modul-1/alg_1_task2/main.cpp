/*
Дан отсортированный массив различных целых чисел A[0..n-1] и
 массив целых чисел B[0..m-1]. Для каждого элемента массива B[i]
 найдите минимальный индекс элемента массива A[k], ближайшего по
 значению к B[i]. n ≤ 110000, m ≤ 1000. Время работы поиска для каждого
 элемента B[i]: O(log(k)).
*/

#include <iostream>
#include <cmath>
#include <assert.h>

size_t find_element(const int elem, const int* arr, const size_t size) {
    assert(arr);
    assert(size > 0);

    if (size == 1)
        return 0;

    size_t a = 0, b = 1;
    while (b < size && arr[b] <= elem) {
        a = b;
        b *= 2;
    }

    if (b >= size) {
        b = size - 1;
    }
    size_t middle = a + ((b - a) >> 1);
    while (b - a > 1) {
        if (elem > arr[middle]) {
            a = middle;
        } else {
            b = middle;
        }
        middle = a + ((b - a) >> 1);
    }

    if (abs(elem - arr[a]) <= abs(elem - arr[b])) {
        return a;
    }
    return b;
}

void get_answer(const int* A, const size_t a_size, const int* B,
                const size_t b_size) {
    assert(A);
    assert(a_size > 0);
    assert(B);
    assert(b_size > 0);

    for (size_t i = 0; i < b_size; i++) {
        std::cout << " " << find_element(B[i], A, a_size);
    }
}

int main() {
    size_t size_a = 0, size_b = 0;

    std::cin >> size_a;
    if (size_a < 1) {
        std::cout << "error";
        return 0;
    }
    int* arr_a = static_cast<int*>(malloc(size_a * sizeof(int)));
    if (!arr_a) {
        std::cout << "error";
        return 0;
    }
    for (size_t i = 0; i < size_a; i++) {
        std::cin >> arr_a[i];
    }

    std::cin >> size_b;
    if (size_b < 1) {
        free(arr_a);
        std::cout << "error";
        return 0;
    }
    int* arr_b = static_cast<int*>(malloc(size_b * sizeof(int)));
    if (!arr_b) {
        free(arr_a);
        std::cout << "error";
        return 0;
    }
    for (size_t i = 0; i < size_b; i++) {
        std::cin >> arr_b[i];
    }

    get_answer(arr_a, size_a, arr_b, size_b);

    free(arr_a);
    free(arr_b);
    return 0;
}
