/*
Submit a solution for A- (Variant 6)
Задача A-6. Задача о приведении строк к нижнему регистру символов
Time limit:	14 s
Memory limit:	64 M
Составить программу построчной обработки текста. Суть обработки - приведение каждого символа в каждой строке к нижнему регистру.

Программа считывает входные данные со стандартного ввода, и печатает результат в стандартный вывод.

Процедура приведения к нижнему регистру должна быть оформлена в виде отдельной функции, которой на вход подается массив строк (который необходимо обработать), количество переданных строк, а также указатель на переменную, в которой необходимо разместить результат - массив уже обработанных строк. 
В качестве возвращаемого значения функция должна возвращать количество строк, содержащихся в результирующем массиве. 

Программа должна уметь обрабатывать ошибки - такие как неверные входные данные(отсутствие входных строк) или ошибки выделения памяти и т.п. 
В случае возникновения ошибки нужно выводить об этом сообщение "[error]" и завершать выполнение программы. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <stddef.h>
#include <string.h>

#include <assert.h>

#define NO_SYMBOLS 0
#define ERROR -1

#define BUFFER_SIZE 32
#define ARRAY_SIZE 4

typedef char** string_ptr; 

size_t len(char arr[]);

size_t lowRegistr(char* arr[], const size_t arr_size, string_ptr *save);

size_t getString(char **lineptr, size_t *n, int delimiter, FILE *stream);

void printText(char* text[], const size_t kolvo);

void cleanText(char* text[], const size_t size);

void cleanTexts(char* arr[], char* text[], size_t size);

/* Загрузить строки, возвращает количество строк или
 -1, если не выделилась память */
size_t loadText(string_ptr text);

int main(int argc, char* argv[])
{
	//setbuf(stdout,NULL);
	
	string_ptr arr = (char**)calloc(BUFFER_SIZE, sizeof(char*));
	size_t text_size = loadText(arr);
	
	if (text_size == ERROR)
	{
		return 0;
	}
	
	string_ptr text;
	int lines = lowRegistr(arr, text_size, &text);
	
	printText(text, lines);
	
	cleanTexts(arr, text, lines);
	
	return 0;
}

size_t len(char arr[])
{
	assert(arr != NULL);
	if (arr == NULL)
	{
		return 0;
	}
	
	size_t size = 0;
	for (char *ch = arr; *ch != '\0'; ch++, size++);
	return size;
}

size_t lowRegistr(char* arr[], const size_t arr_size, string_ptr *save)
{
	assert(arr != NULL);
	assert(*arr != NULL);
	assert(save != NULL);
	assert(arr_size > 0);
	
	if ((arr == NULL) || (*arr == NULL) || (arr_size <= 0)|| (save == NULL))
	{
		return ERROR;
	}
	
	// выделим память под массив строк
	*save = (char**)calloc(arr_size, sizeof(char*));
	
	if (!*save)
	{
		return ERROR;
	}
	size_t row_size = 0;
	wchar_t letter_a = L'а';
	wchar_t letter_A = L'А';
	wchar_t letter_Ja = L'Я';

	for (size_t i = 0; i < arr_size; i++)
	{
		row_size = len(arr[i]);
		
		//Выделим память непосредственно под строки
		(*save)[i] = (char*)calloc(row_size + 1, sizeof(char));
		
		// Если не вышло
		if (!(*save)[i])
		{
			cleanText(*save, i);
			return ERROR;
		}
		
		// Преобразование заглавных символов к прописным
		for (int j = 0; j < row_size; j++)
		{
			if ((arr[i][j] >= letter_A) &&
			(arr[i][j] <= letter_Ja))
			{
				printf("hmmmm");
				arr[i][j] = arr[i][j] - (letter_A - letter_a);
			} else if((arr[i][j]  >= 'A')
				&& (arr[i][j] <= 'Z')){
				arr[i][j] = arr[i][j] - ('Z' - 'z');
			}
			(*save)[i][j] = arr[i][j];
		}
		
	}
	return arr_size;
}

size_t getString(char **lineptr, size_t *n, int delimiter, FILE *stream)
{
	assert(lineptr != NULL);
	assert(n != NULL);
	assert(stream != NULL);
	
	if ((lineptr == NULL) || (n== NULL) || (stream == NULL))
	{
		return ERROR;
	}
	
    size_t count = NO_SYMBOLS;
    char *pb = NULL;
    char c = 0;
   
    char *tmp = NULL;

    if (!*lineptr)
    {
        tmp = realloc(*lineptr, *n);
        if (!tmp)
		{
			free(*lineptr);
            return ERROR;
		}
        *lineptr = tmp;
    }

    pb = *lineptr;
	
    while ((c = fgetc(stream)) != EOF)
    {
        if (count >= *n - 1)
        {   
            *n += BUFFER_SIZE;
            tmp = realloc(*lineptr, *n);
            if (!tmp)
			{
				free(*lineptr);
				count = NO_SYMBOLS;
                break;
			}
			*lineptr = tmp;
			pb = tmp + count;
		}
		if (c == delimiter)
		{
            break;
		}
        *pb = c;
        pb++;
        count++;
    }
	
	if (count == NO_SYMBOLS)
	{
		return ERROR;
	}

    *pb = '\0';

    return count;
}

void printText(char* text[], const size_t kolvo)
{
	assert(text != NULL);
	
	if ((kolvo < 0) || (text == NULL))
	{
		printf("[error]");
		return;
	}
	for (size_t i = 0; i < kolvo; i++)
	{
		if (i != 0)
		{
			printf("\n");
		}
		printf("%s", text[i]);
	}
}

void cleanText(char* text[], const size_t size)
{
	for (size_t i = 0; i < size; i++)
	{
		free(text[i]);
	}
	free(text);
}

void cleanTexts(char* arr[], char* text[], const size_t size)
{
	cleanText(arr, size);
	cleanText(text, size);
}

size_t loadText(string_ptr text)
{
	assert(text != NULL);
	if (text == NULL)
	{
		return ERROR;
	}
	
	size_t i = 0;
	size_t max_size = BUFFER_SIZE;
	do
	{
		text[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));
		if (text[i] == NULL)
		{
			cleanText(text, i);
			i = 0;
			break;
		}
	}
	while (getString(&text[i++], &max_size, '\n', stdin) != ERROR);
	if (i == 0)
		return ERROR;
	return i;
}