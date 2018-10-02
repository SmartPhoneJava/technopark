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

#define ERROR -1
#define POSITION_OF_FIRST_CAPITAL_EN 65
#define POSITION_OF_LAST_CAPITAL_EN 90
#define POSITION_OF_FIRST_CAPITAL_RU 128
#define POSITION_OF_LAST_CAPITAL_RU 159

#define OFFSET 32
#define BUFFER_SIZE 32

int len(char* arr)
{
	int size = 0;
	for (char *ch = arr; *ch != 0; ch++, size++);
	return size;
}

int lowRegistr(char **arr, int arr_size, char ***save)
{
	assert(arr != NULL);
	assert(*arr != NULL);
	assert(save != NULL);
	assert(arr_size > 0);
	
	// выделим память под массив строк
	*save = (char**)calloc(arr_size, sizeof(char*));
	
	// не удалось выделить память
	if (!*save){
		return ERROR;
	}
	
	// размер строки
	int row_size = 0;
	
	// цикл по строкам
	for (int i = 0; i < arr_size; i++)
	{
		row_size = len(arr[i]);
		
		//Выделим память непосредственно под строки
		(*save)[i] = (char*)calloc(row_size + 1, sizeof(char));
		//(*save)[i][row_size] = 0;
		
		// Если не вышло
		if (!(*save)[i]){
			// Очистим память строк
			i--;
			for (; i >= 0; i--){
				free((*save)[i]);
			}
			// и память массива строк
			free(*save);
			
			return ERROR;
		}

		//цикл по строке
		for (int j = 0; j < row_size; j++)
		{
			if ((arr[i][j] >= POSITION_OF_FIRST_CAPITAL_RU) &&
			(arr[i][j] <= POSITION_OF_LAST_CAPITAL_RU))
			{
				arr[i][j] = arr[i][j] + OFFSET;
			} else if((arr[i][j]  >= POSITION_OF_FIRST_CAPITAL_EN)
				&& (arr[i][j] <= POSITION_OF_LAST_CAPITAL_EN)){
				arr[i][j] = arr[i][j] + OFFSET;
			}
			(*save)[i][j] = arr[i][j];
		}
		
	}
	return arr_size;
}

int my_getdelim(char **lineptr, int *n, int delimiter, FILE *stream)
{
	assert(lineptr != NULL);
	assert(n != NULL);
	assert(stream != NULL);
	
    int count = 0;
    char *pb = NULL;
    int c = 0;
   
    char *tmp = NULL;

    if (!*lineptr)
    {
        tmp = realloc(*lineptr, *n);
        if (!tmp)
            return -1;
        *lineptr = tmp;
    }

    pb = *lineptr;
	
    while ((c = fgetc(stream)) != EOF)// && c != delimiter)
    {
        if (count >= *n - 1)
        {   
            *n += BUFFER_SIZE;
            tmp = realloc(*lineptr, *n);
            if (!tmp)
                return -1;

            *lineptr = tmp;
            pb = tmp + count;
        }
		if (c == delimiter)
            break;
        *pb = c;
        pb++;
        count++;
    }
    
    if (count == 0)
        return -1;

    *pb = '\0';

    return count;
}

int main(int argc, char* argv[])
{
	setbuf(stdout,NULL);
      
   char **arr = (char**)calloc(BUFFER_SIZE, sizeof(char*));
   int i = 0;
   int max_size = BUFFER_SIZE;
do
{
	arr[i] = (char*)calloc(BUFFER_SIZE, sizeof(char));
}
while (my_getdelim(&arr[i++], &max_size, '\n', stdin) != -1);
   char** text = NULL;
   int lines = lowRegistr(arr, i, &text);
   if (lines == ERROR)
	   printf("[error]");
   else
   {
		for (int f = 0; f < lines; f++)
		{
			if (f != 0)
				printf("\n");
			printf("%s", text[f]);
			free(arr[f]);
			free(text[f]);
		}
   }
	free(arr);
	free(text);
	return 0;
}
