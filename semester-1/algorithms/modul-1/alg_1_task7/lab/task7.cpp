/*
Дан массив неотрицательных целых 64-разрядных чисел. 
Количество чисел не больше 1000000. Отсортировать массив
 методом MSD по битам (бинарный QuickSort).
*/

#include <iostream>

#define UUL_SIZE 64

typedef unsigned long long ull;

short digit(ull A, int offset) {
    return ((A >> offset) & (1));
}

void quicksort_binary(ull a[], int l, int r, int w);

void print(ull a[], size_t size);

int main(void) {
    size_t size = 0;
    std::cin >> size;
    ull* array = static_cast<ull*>(calloc(size, sizeof(ull)));
    if (array) {
        for (size_t i = 0; i < size; i++) {
            std::cin >> array[i];
        }
        quicksort_binary(array, 0, size - 1, UUL_SIZE - 1);
        print(array, size);
        free(array);
    }
    return 0;
}

void print(ull arr[], size_t size) {
     std::cout << "\n";
    for (size_t i = 0; i < size; i++){
        std::cout << arr[i] << " ";
    }
}

void swap(ull& a1, ull& a2) {
    ull tmp = a1;
    a1 = a2;
    a2 = tmp;
}

void quicksort_binary(ull a[], int l, int r, int w) {
	int i = l, j = r;
    if (r <= l || w < 0) {
		return;
	}
    while (i < j) {
        while ((digit(a[i], w) == 0) && (i < j)) {
            i = i + 1;
        }
        while ((digit(a[j], w) == 1) && (j > i)) {
            j = j - 1;
        }
        if (i < j) {
            swap(a[i], a[j]);
        }
    }
    /* Если бит крайнего правого элемента массива равен нулю,
     * значит в данном разряде у всех элементов массива были нули,
     * в таком случае j = r. И тогде рекурсивно будут вызваны сортировки
     * от l до r - 1 и от r до r. Но сортировка от r до r ничего не
     * отсортирует, а от l до r - 1 отсортирует на 1 элемент меньше
     * реального количества элементов, поэтому инкрементируем j, и теперь
     * рекурсивано вызываются сортировки от l до r и от r+1 до r,
     * теперь первая сортировка пройдется по всему массиву.
     */
    if (digit(a[r], w) == 0) {
        j++;
    }
    quicksort_binary(a, l, j-1, (w - 1));
    quicksort_binary(a, j, r, (w - 1));
}
