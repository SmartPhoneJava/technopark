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

#define STRING_SIZE 4
#define LINES_COUNT 10

typedef char (*string_ptr)[STRING_SIZE];

size_t get_string_length(char string[]);

char downcase_letter(char letter);

// Возвращает количество строк или ошибку
size_t downcase_lines(const string_ptr lines, const size_t lines_count, string_ptr* downcase_text);

int get_string(FILE* stream, string_ptr str_ptr);

int print_text(const string_ptr lines, const size_t lines_size);

void clean_input_text(string_ptr loaded_text);

void clean_downcase_text(string_ptr downcase_text);

// Загрузить строки из файла или потока ввода, возвращает наличие ошибок
int load_text(FILE* stream, string_ptr *blocks, size_t *blocks_count);

int main() {	
	// Текст, приходящий на вход. Память выделяется в load_text()
	string_ptr input_text = NULL;	
	size_t lines_count = 0;
	
	string_ptr downcase_text = NULL;
	
	int error = load_text(stdin, &input_text, &lines_count);
	if (error != ERROR) {
		lines_count = downcase_lines(input_text, lines_count, &downcase_text);
	}
	
	if (lines_count != ERROR) {
		error = print_text(downcase_text, lines_count);
	}
	
	if (error == ERROR) {
		printf("[error]");
		return 0;
	}

	clean_input_text(input_text);
	clean_downcase_text(downcase_text);
	return 0;
}

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
	*downcase_text = (string_ptr)calloc(lines_count * STRING_SIZE, sizeof(char));

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

int get_string(FILE* stream, string_ptr str_ptr) {
	assert(str_ptr != NULL);
	assert(stream != NULL);

	if ((str_ptr == NULL) || (stream == NULL)) {
		return ERROR;
	}

	size_t count = NO_SYMBOLS;
	char* pb = NULL;
	char c = 0;

	pb = *str_ptr;

	while (count < STRING_SIZE - 1 && (c = fgetc(stream)) != EOF) {
		*pb = c;
		pb++;
		count++;
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

int load_text(FILE* stream, string_ptr *blocks, size_t *blocks_count) {
	assert(blocks != NULL);
	assert(stream != NULL);
	assert(blocks_count != NULL);
	if ((blocks == NULL) || (stream == NULL) || (blocks_count == NULL)) {
		return ERROR;
	}
	
	size_t max_lines_count = LINES_COUNT;
	*blocks = (string_ptr)calloc(max_lines_count * STRING_SIZE, sizeof(char));
	
	if (*blocks == NULL) {
		return ERROR;
	}
	
	string_ptr tmp = NULL;
	size_t i = 0;
	int error = NO_ERROR;
	while (get_string(stream, &((*blocks)[i])) && error == NO_ERROR)
	{
		i++;
		if (i >= max_lines_count - 1) {
			max_lines_count *= 2;
			tmp = realloc(*blocks, max_lines_count * STRING_SIZE * sizeof(char));
			if (tmp == NULL) {
				error = ERROR;
			} else {
				*blocks = tmp;
			}
		}
	}
	*blocks_count = i;
	if (error == ERROR) {
		clean_input_text(*blocks);
	}
	return error;
}