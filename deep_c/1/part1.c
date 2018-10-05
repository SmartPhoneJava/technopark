/*
Submit a solution for A- (Variant 6)
Задача A-6. Задача о приведении строк к нижнему регистру символов
Time limit:	14 s
Memory limit:	64 M
Составить программу построчной обработки текста. Суть обработки - приведение
каждого символа в каждой строке к нижнему регистру.

Программа считывает входные данные со стандартного ввода, и печатает результат в
стандартный вывод.

Процедура приведения к нижнему регистру должна быть оформлена в виде отдельной
функции, которой на вход подается массив строк (который необходимо обработать),
количество переданных строк, а также указатель на переменную, в которой
необходимо разместить результат - массив уже обработанных строк.
В качестве возвращаемого значения функция должна возвращать количество строк,
содержащихся в результирующем массиве.

Программа должна уметь обрабатывать ошибки - такие как неверные входные
данные(отсутствие входных строк) или ошибки выделения памяти и т.п.
В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и
завершать выполнение программы.

*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#include <assert.h>

#define NO_SYMBOLS 0
#define ERROR -1

#define BUFFER_SIZE 32
#define ARRAY_SIZE 4

typedef char** string_ptr;

size_t get_string_length(char arr[]);

char downcase_letter(char letter);

size_t downcase_text(char* arr[], const size_t arr_size, string_ptr* save);

size_t get_string(char** lineptr, size_t* n, int delimiter, FILE* stream);

void print_text(string_ptr text, const size_t text_size);

void clean_text(string_ptr text, const size_t text_size);

void free_all_memory(string_ptr first_ptr, string_ptr second_ptr, size_t string_ptr_size);

/* Загрузить строки, возвращает количество строк или
 0 в случае ошибки */
size_t load_text(string_ptr text);

int main(int argc, char* argv[]) {
    string_ptr arr = (string_ptr)calloc(BUFFER_SIZE, sizeof(char*));
    if (arr == NULL) {
        return 0;
    }
    size_t text_size = load_text(arr);

    if (text_size == 0) {
        return 0;
    }

    string_ptr text = NULL;
    int lines = downcase_text(arr, text_size, &text);

    print_text(text, lines);

    free_all_memory(arr, text, lines);

    return 0;
}

size_t get_string_length(char arr[]) {
    assert(arr != NULL);
    if (arr == NULL) {
        return 0;
    }

    size_t size = 0;
    for (char *ch = arr; *ch != '\0'; ch++, size++)
        ;
    return size;
}

char downcase_letter(char letter) {

    char return_symbol = letter;

    if ((return_symbol >= 'A') && (return_symbol <= 'Z')) {
        return_symbol -= ('Z' - 'z');
    }
    return return_symbol;
}

size_t downcase_text(char* arr[], const size_t arr_size, string_ptr* save) {
    assert(arr != NULL);
    assert(*arr != NULL);
    assert(save != NULL);
    assert(arr_size > 0);

    if ((arr == NULL) || (*arr == NULL) || (arr_size <= 0) || (save == NULL)) {
        return ERROR;
    }

    // выделим память под массив строк
    *save = (string_ptr)calloc(arr_size, sizeof(char*));

    if (!*save) {
        return ERROR;
    }
    size_t row_size = 0;

    for (size_t i = 0; i < arr_size; i++) {
        row_size = get_string_length(arr[i]);

        //Выделим память непосредственно под строки
        (*save)[i] = (char*)calloc(row_size + 1, sizeof(char));

        // Если не вышло
        if (!(*save)[i]) {
            clean_text(*save, i);
            return ERROR;
        }

        // Преобразование заглавных символов к прописным
        for (int j = 0; j < row_size; j++) {
            (*save)[i][j] = downcase_letter(arr[i][j]);
        }
    }
    return arr_size;
}

size_t get_string(char** lineptr, size_t* n, int delimiter, FILE* stream) {
    assert(lineptr != NULL);
    assert(n != NULL);
    assert(stream != NULL);

    if ((lineptr == NULL) || (n == NULL) || (stream == NULL)) {
        return ERROR;
    }

    size_t count = NO_SYMBOLS;
    char* pb = NULL;
    char c = 0;

    char* tmp = NULL;

    if (!*lineptr) {
        tmp = realloc(*lineptr, *n);
        if (!tmp) {
            free(*lineptr);
            return ERROR;
        }
        *lineptr = tmp;
    }

    pb = *lineptr;

    while ((c = fgetc(stream)) != EOF) {
        if (count >= *n - 1) {
            *n += BUFFER_SIZE;
            tmp = realloc(*lineptr, *n);
            if (!tmp) {
                free(*lineptr);
                count = NO_SYMBOLS;
                break;
            }
            *lineptr = tmp;
            pb = tmp + count;
        }
        if (c == delimiter) {
            break;
        }
        *pb = c;
        pb++;
        count++;
    }

    if (count == NO_SYMBOLS) {
        return ERROR;
    }

    *pb = '\0';

    return count;
}

void print_text(string_ptr text, const size_t text_size) {
    assert(text != NULL);

    if ((text_size < 0) || (text == NULL)) {
        printf("[error]");
        return;
    }
    for (size_t i = 0; i < text_size; i++) {
        if (i != 0) {
            printf("\n");
        }
        printf("%s", text[i]);
    }
}

void clean_text(string_ptr text, const size_t text_size) {
    for (size_t i = 0; i < text_size; i++) {
        free(text[i]);
    }
    free(text);
}

void free_all_memory(string_ptr first_ptr, string_ptr second_ptr, size_t string_ptr_size) {
    clean_text(first_ptr, string_ptr_size);
    clean_text(second_ptr, string_ptr_size);
}

size_t load_text(string_ptr text) {
    assert(text != NULL);
    if (text == NULL) {
        return 0;
    }

    size_t i = 0;
    size_t max_size = BUFFER_SIZE;
    do {
        text[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));
        if (text[i] == NULL) {
            clean_text(text, i);
            i = 0;
            break;
        }
    } while (get_string(&text[i++], &max_size, '\n', stdin) != ERROR);
    return i;
}