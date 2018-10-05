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
#define LINES_COUNT 4

typedef char** string_ptr;

size_t get_string_length(char string[]);

char downcase_letter(char letter);

size_t downcase_text(char* lines[], const size_t lines_count, string_ptr* downcase_text);

size_t get_string(int delimiter, FILE* stream, size_t* n, char** lineptr);

void print_text(string_ptr text, const size_t text_size);

void clean_text(string_ptr text, const size_t text_size);

void free_all_memory(string_ptr first_ptr, string_ptr second_ptr, size_t string_ptr_size);

/* Загрузить строки, возвращает количество строк или
 0 в случае ошибки */
size_t load_text(string_ptr *text, int* error);

int main(int argc, char* argv[]) {
    string_ptr arr = NULL; //(char**)calloc(BUFFER_SIZE, sizeof(char*));
    //if (arr == NULL) {
     //   return 0;
    //}
	
	int error = 0;
	
    size_t text_size = load_text(&arr, &error);

    if (error == ERROR) {
        return 0;
    }

    string_ptr text = NULL;
    int lines = downcase_text(arr, text_size, &text);

    print_text(text, lines);

    free_all_memory(arr, text, lines);

    return 0;
}

size_t get_string_length(char string[]) {
    assert(string != NULL);
    if (string == NULL) {
        return 0;
    }

    size_t size = 0;
    for (char *ch = string; *ch != '\0'; ch++, size++)
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

size_t downcase_text(char* lines[], const size_t lines_count,
						string_ptr* downcase_text) {
    assert(lines != NULL);
    assert(*lines != NULL);
    assert(downcase_text != NULL);
    assert(lines_count > 0);

    if ((lines == NULL) || (*lines == NULL) || (lines_count <= 0) || (downcase_text == NULL)) {
        return ERROR;
    }

    // выделим память под массив строк
    *downcase_text = (string_ptr)calloc(lines_count, sizeof(char*));

    if (!*downcase_text) {
        return ERROR;
    }
    size_t row_size = 0;

    for (size_t i = 0; i < lines_count; i++) {
        row_size = get_string_length(lines[i]);

        //Выделим память непосредственно под строки
        (*downcase_text)[i] = (char*)calloc(row_size + 1, sizeof(char));

        // Если не вышло
        if (!(*downcase_text)[i]) {
            clean_text(*downcase_text, i);
            return ERROR;
        }

        // Преобразование заглавных символов к прописным
        for (int j = 0; j < row_size; j++) {
            (*downcase_text)[i][j] = downcase_letter(lines[i][j]);
        }
    }
    return lines_count;
}

size_t get_string(int delimiter, FILE* stream, size_t* n, char** lineptr) {
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

size_t load_text(string_ptr *lines, int* error) {
    assert(lines != NULL);
    if (lines == NULL) {
		*error = ERROR;
        return 0;
    }
	
	size_t text_size = LINES_COUNT;
	string_ptr text = (string_ptr)calloc(text_size, sizeof(char*));
	
	string_ptr tmp = NULL;
	if (text == NULL) {
		*error = ERROR;
		return 0;
	}
	
    size_t i = 0;
    size_t max_size = BUFFER_SIZE;
    do {
		if (i >= text_size - 1) {
			text_size *= 2;
			tmp = realloc(text, text_size * sizeof(string_ptr));
			if (tmp == NULL) {
				*error = ERROR;
				clean_text(text, i);
				i = 0;
				break;
			}
			text = tmp;
		}
        text[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));
        if (text[i] == NULL) {
            clean_text(text, i);
			*error = ERROR;
            i = 0;
            break;
        }
    } while (get_string('\n', stdin, &max_size, &text[i++]) != ERROR);
	if (*error != ERROR) {
		*lines = text;
	}
    return i;
}