/*
 * Даны неотрицательные целые числа n, k и массив целых чисел из диапазона [0..109] размера n.
 * Требуется найти k-ю порядковую статистику. т.е. напечатать число, которое бы стояло на позиции с индексом k ∈[0..n-1] в отсортированном массиве.
 * Напишите нерекурсивный алгоритм.
 * Требования к дополнительной памяти: O(n).
 * Требуемое среднее время работы: O(n).
 * Функцию Partition следует реализовывать методом прохода двумя итераторами в одном направлении.
 * Описание для случая прохода от начала массива к концу:
 * Выбирается опорный элемент.
 * Опорный элемент меняется с последним элементом массива.
 * Во время работы Partition в начале массива содержатся элементы, не бОльшие опорного. Затем располагаются элементы, строго бОльшие опорного. В конце массива лежат нерассмотренные элементы. Последним элементом лежит опорный.
 * Итератор (индекс) i указывает на начало группы элементов, строго бОльших опорного.
 * Итератор j больше i, итератор j указывает на первый нерассмотренный элемент.
 * Шаг алгоритма. Рассматривается элемент, на который указывает j. Если он больше опорного, то сдвигаем j. Если он не больше опорного, то меняем a[i] и a[j] местами, сдвигаем i и сдвигаем j.
 * В конце работы алгоритма меняем опорный и элемент, на который указывает итератор i.
 *
 * Вариант 1
 * Реализуйте стратегию выбора опорного элемента “медиана трёх”. Функцию Partition реализуйте методом прохода двумя итераторами от начала массива к концу.
 */

#include <iostream>
#include "stdio.h"
#include "stdlib.h"
#include "time.h"

int statistic(int* arr, int pos, size_t size);

int main(void) {
    size_t size = 0;
    std::cin >> size;

    int position = 0;
    std::cin >> position;

    int* a = static_cast<int*>(calloc(size, sizeof(int)));
    if (a) {
        for (size_t i = 0; i < size; i++) {
            std::cin >> a[i];
        }
        std::cout << statistic(a, position, size);
        free(a);
    }
    return 0;
}

void swap(int& a1, int& a2) {
    int tmp = a1;
    a1 = a2;
    a2 = tmp;
}

int median(int* arr, int left, int middle, int right)
{
    int a = arr[left];
    int b = arr[middle];
    int c = arr[right];
    if (a > b) {
        if (c > a) {
            return left;
        }
        return (b > c) ? middle : right;
    }
    if (c > b) {
        return middle;
    }
    return (a > c) ? left : right;
}

int partition_qsort(int* a, int left, int right) {
    if (right - left <= 0) {
        return left;
    }
    int med = median(a, left, (right + left) / 2, right);
    swap(a[med], a[right]);

    const int& pivot = a[right];
    int i = left - 1;
    for (int j = left; j <= right - 1; j++) {
        if (a[j] <= pivot) {
            i++;
            swap(a[i], a[j]);
        }
    }
    swap(a[i + 1], a[right]);
    return (i + 1);
}

int statistic(int* arr, int pos, size_t size) {
    int left = 0, right = static_cast<int>(size) - 1;
    int i = pos - 1;
    while (i != pos) {
        i = partition_qsort(arr, left, right);
        if (pos < i) {
            right = i - 1;
        } else {
            left = i + 1;
        }
    }
    return arr[i];
}
