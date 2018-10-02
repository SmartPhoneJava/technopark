#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define EN_SIZE 32
#define RU_SIZE 32

int len(char* arr)
{
	int size = 0;
	for (char *ch = arr; *ch != 0; ch++, size++);
	return size;
}

int function(char **arr, int arr_size, char ***save)
{
	// неверное кол-во строк 
	if (arr_size < 1)
	{
		printf("[error]");
		return 0;
	}
	// указатель на массив строк не задан
	if (!arr)
	{
		printf("[error]");
		return 0;
	}
	
	// выделим память под массив строк
	*save = (char**)calloc(arr_size, sizeof(char*));
	
	// не удалось выделить память
	if (!*save)
	{
		printf("[error]");
		return 0;
	}
	
	// размер строки
	int row_size = 0;
	
	int j = 0;
	
	// цикл по строкам
	for (int i = 0; i < arr_size; i++)
	{
		row_size = len(arr[i]);
		
		//Довыделим память непосредственно под строки
		(*save)[i] = (char*)calloc(row_size, sizeof(char));
		
		// Если не вышло
		if (!(*save)[i])
		{
			// Очистим память строк
			i--;
			for (; i >= 0; i--)
			{
				free((*save)[i]);
			}
			// и память массива строк
			free(*save);
			
			printf("[error]");
			return 0;
		}
		
		j = 0;
		//цикл по символам
		for (char *ch = arr[i]; *ch != 0; ch++, j++)
		{
			if ((*ch > 126) && (*ch < 160))
				*ch = *ch + RU_SIZE;
			else if ((*ch > 64) && (*ch < 91))
				*ch = *ch + EN_SIZE;
			(*save)[i][j] = *ch;
		}
	}
	return arr_size;
}

int main(int argc, char* argv[])
{
	setlocale(LC_ALL, "Rus");
	char** text;
	int err = function(argv, argc, &text);
	for (int i = 1; i < argc; i++)
	{
		printf("%s", text[i]);
	}
	return 0;
}