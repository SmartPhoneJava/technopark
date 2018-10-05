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
#define NO_ERROR 1
#define ERROR 0

#define STRING_SIZE 32
#define LINES_COUNT 4

typedef char (*string_ptr)[STRING_SIZE];

size_t get_string_length(char string[]);

char downcase_letter(char letter);

size_t downcase_lines(const string_ptr lines, const size_t lines_count, string_ptr* downcase_text);

int get_string(FILE* stream, string_ptr lineptr);

int print_text(const string_ptr lines, const size_t lines_size);

void clean_input_text(string_ptr loaded_text);

void clean_downcase_text(string_ptr downcase_text);

// Загрузить строки из файла или потока ввода, возвращает количество строк
size_t load_text(string_ptr *text, FILE* f);

int main(int argc, char* argv[]) {	
	// Для отладки
	FILE *f = stdin;
	setbuf(stdout, NULL);
	if (argc > 1) {
		f = fopen(argv[1], "r");
		if (!f) {
			printf("No file with name %s", argv[1]);
			return 0;
		}
	}
	// Текст, приходящий на вход. Память выделяется в load_text()
    string_ptr input_text = NULL;	
    size_t lines_count = load_text(&input_text, f);
	if (argc > 1) {
		fclose(f);
	}
	
    string_ptr downcase_text = NULL;
	
	if (lines_count != ERROR)
	{
		lines_count = downcase_lines(input_text, lines_count, &downcase_text);
	}
	
	if (lines_count != ERROR)
	{
		lines_count = print_text(downcase_text, lines_count);
	}
	
	 if (lines_count == ERROR) {
		printf("[error]");
        return 0;
    }

	clean_input_text(input_text);
	clean_downcase_text(downcase_text);
    return 0;
}

// ERROR - теперь 0
size_t get_string_length(char string[]) {
    assert(string != NULL);
    if (string == NULL) {
        return ERROR;
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

size_t downcase_lines(string_ptr lines, const size_t lines_count,
						string_ptr* downcase_text) {
    assert(lines != NULL);
    assert(*lines != NULL);
    assert(downcase_text != NULL);
    assert(lines_count > 0);

    if ((lines == NULL) || (*lines == NULL) || (lines_count <= 0) || (downcase_text == NULL)) {
        return ERROR;
    }

    // выделим память под массив строк
    *downcase_text = (string_ptr)calloc(lines_count * STRING_SIZE, sizeof(char)
						);

    if (!*downcase_text) {
        return ERROR;
    }
	
	int string_size = 0;

    for (size_t i = 0; i < lines_count; i++) {
		
		string_size = get_string_length(lines[i]);

        // Преобразование заглавных символов к прописным
        for (size_t j = 0; j < string_size; j++) {
            (*downcase_text)[i][j] = downcase_letter(lines[i][j]);
        }
    }
    return lines_count;
}

int get_string(FILE* stream, string_ptr lineptr) {
    assert(lineptr != NULL);
    assert(stream != NULL);

    if ((lineptr == NULL) || (stream == NULL)) {
        return ERROR;
    }

    size_t count = NO_SYMBOLS;
    char* pb = NULL;
    char c = 0;

    pb = *lineptr;

    while ((c = fgetc(stream)) != EOF && count != STRING_SIZE - 1) {
        *pb = c;
        pb++;
        count++;
		if (c == '\n') {
            break;
        }
    }

    if (count == NO_SYMBOLS) {
        return ERROR;
    }

    *pb = '\0';

    return NO_ERROR;
}

int print_text(string_ptr lines, const size_t lines_count) {
    assert(lines != NULL);
	assert(lines_count > 0);

    if ((lines_count == 0) || (lines == NULL)) {
        return ERROR;
    }
    for (size_t i = 0; i < lines_count; i++) {
        printf("%s", lines[i]);
    }
	return NO_ERROR;
}

void clean_input_text(string_ptr loaded_text)
{
	free(loaded_text);
}

void clean_downcase_text(string_ptr downcase_text)
{
	free(downcase_text);
}

size_t load_text(string_ptr *lines, FILE* stream) {
    assert(lines != NULL);
    if (lines == NULL) {
        return ERROR;
    }
	
	size_t lines_count = LINES_COUNT;
	string_ptr text = (string_ptr)calloc(lines_count * STRING_SIZE, sizeof(char));
	
	string_ptr tmp = NULL;
	if (text == NULL) {
		return ERROR;
	}
	
    size_t i = 0;
    do {
		if (i >= lines_count - 1) {
			lines_count *= 2;
			tmp = realloc(text, lines_count * STRING_SIZE * sizeof(char));
			if (tmp == NULL) {
				clean_input_text(text);
				i = ERROR;
				break;
			}
			text = tmp;
		}
        if (text[i] == NULL) {
            clean_input_text(text);
            i = ERROR;
            break;
        }
		i++;
    } while (get_string(stream, &text[i]));
	if (i != ERROR) {
		*lines = text;
		return --i;
	}
    return ERROR;
}