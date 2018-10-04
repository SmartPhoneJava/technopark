/*
Задача B-1. Калькулятор для числовых множеств
Time limit:	14 s
Memory limit:	64 M
Разработайте программу-калькулятор, умеющую вычислять арифметические выражения над множествами натуральных чисел. Входные данные (выражение с описанием множеств и производимых над ними операций) подаются на стандартный поток ввода программы, результат вычислений должен подаваться на стандартный поток вывода. 

Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность множеств, '()' - задание приоритета вычислений.
Множества задаются в виде [ el1, el2, ... ], где элементами являются обычные натуральные числа.
Множества могут быть пустыми.
Между операциями, множествами и элементами множеств может быть любое количество пробелов.

При вычислениях должны учитываться приоритеты операций (в том числе, заданные вручную при помощи круглых скобочек).

Результирующее множество должно выводиться без пробелов, в отсортированном порядке.
В случае, если введенное выражение содержит ошибку, необходимо вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы. 
*/

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define NO_ERROR 0
#define WRONG_INPUT -2 
#define MEMORY_ERROR -1

#define ERROR_NUMBER_NOT_IN_SET -3
#define ERROR_COMMA_NOT_AFTER_NUMBER -4
#define ERROR_OPERATOR_IN_SET -5
#define ERROR_OPERATOR_WITHOUT_SETS -6
#define ERROR_SQUERE_END_BEFORE_START -7
#define ERROR_CIRCLE_END_BEFORE_START -8
#define ERROR_CIRCLE_IN_SQUERE -9
#define ERROR_NO_SQUERE_END -10
#define ERROR_NO_CIRCLE_END -11
#define ERROR_WRONG_SYMBOL -12
#define ERROR_WRONG_NUMBER_OF_OPERATORS -13
#define ERROR_COMMA_WITHOUT_NUMBER -14
#define ERROR_NO_SQUERE_IN_CIRCLE -15

#define BUFFER_SIZE 32

typedef struct NumericSet
{
	int *numbers;
	size_t max_size;
	size_t real_size;
} NumericSet;

// Обменять местами двух целочисленных значений
void swap(int *a, int *b);

// Печатать множество
void printSet(const NumericSet *ns);

// Создать множество
NumericSet* createSet(int *error);

// Очистить множество
void clearSet(NumericSet* ns);

// Добавить элемент в множество
int pushToSet(NumericSet *ns, int num);

// Добавить в мноежство to элементы из множества for
void addFromSetToSet(NumericSet *to, const NumericSet *from, int *error);

// Переместить множество
NumericSet* moveSet(NumericSet *from);

// Получить пересечение множеств(с очисткой)
NumericSet *getUnionOfSets(NumericSet *A, NumericSet *B, int* error);

// Получить пересечение множеств(с очисткой)
NumericSet *getIntersectionOfSets(NumericSet *A, NumericSet *B, int *error);

// получить разность множеств(с очисткой)
NumericSet* getSubtractionOfSets(NumericSet *A, NumericSet *B, int *error);

// Получить размер числа
size_t getNumberSize(int number);

// Вставить число в строку
void insertNumberInString(int number, char *begin[]);

// Вставить множество в строку
void numericSetToChar(NumericSet *ns, char *begin, char *end);

// Проверить строку на корректность
char* checkString(const char* string, size_t size, int *error);

// Получить ответ
void getAnswer(char* string);

void unit_tests();

int getString(char **lineptr, size_t *n, int delimiter, FILE *stream);

int main()
{
	char *buffer = NULL, *string = NULL;
	int error = NO_ERROR;
	size_t max_size = BUFFER_SIZE;
	while (getString(&buffer, &max_size, '\n', stdin) == NO_ERROR)
	{
		if (buffer == NULL)
		{
			break;
		}
		string =  checkString(buffer, strlen(buffer), &error);
		free(buffer);
		if (string != NULL)
		{
			getAnswer(string);
		}
		else
		{
			printf("[error]");
			break;
		}
		free(string);
	}
	
	return 0;
}

void swap(int *a, int *b)
{
	assert(a != NULL);
	assert(b != NULL);
	
	if ((a == NULL) || (b == NULL))
	{
		return;
	}
	
	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void printSet(const NumericSet *ns)
{	
	assert(ns != NULL);
	
	if (ns == NULL)
	{
		return;
	}
	
	size_t size = ns->real_size;
	if (size == 0)
	{
		printf("[]");
		return;
	}
	printf("[%d", ns->numbers[0]);
	for(size_t i = 1; i < size; i++)
	{
		printf(",%d", ns->numbers[i]);
	}
	printf("]");
}

NumericSet* createSet(int *error)
{
	assert(error != NULL);
	
	if (error == NULL)
	{
		return NULL;
	}
	
	NumericSet* ns = (NumericSet*)calloc(1, sizeof(NumericSet));
	if (ns == NULL)
	{
		*error = MEMORY_ERROR;
		return NULL;
	}
	
	ns->max_size = 2;
	ns->real_size = 0;
	ns->numbers = (int*)calloc(ns->max_size, sizeof(int));
	if (ns->numbers == NULL)
	{
		free(ns);
		*error = MEMORY_ERROR;
		return NULL;
	}
	return ns;
}

void clearSet(NumericSet* ns)
{
	if (ns != NULL)
	{
		free(ns->numbers);
		free(ns);
	}
}

int pushToSet(NumericSet *ns, int num)
{
	assert(ns != NULL);
	assert(num > 0);
	assert(ns->numbers != NULL);
	assert(ns->real_size >= 0);
	assert(ns->max_size >= 0);
	
	if ((ns == NULL) || (num <= 0) || (ns->numbers == NULL) ||
		(ns->real_size < 0) || (ns->max_size <= 0))
	{
		return WRONG_INPUT;
	}
	
	// Если числовое множество не содержит элементов
	if (ns->real_size == 0)
	{
		ns->numbers[0] = num;
		ns->real_size++;
		return NO_ERROR;
	}
	
	size_t rsize = ns->real_size;
	
	// Если числовое множество полностью заполнилось
	if (rsize >=  ns->max_size)
	{
		ns->max_size *= 2;
		ns->numbers = (int*)realloc(ns->numbers, ns->max_size * sizeof(int));
		if (ns->numbers == NULL)
		{
			return MEMORY_ERROR;
		}
	}
	for(size_t i = 0; i < rsize; i++)
	{
		if (num == ns->numbers[i])
		{
			// Поскольку числовому множеству не нужны дубли
			// просто не будем их вставлять
			return NO_ERROR;
		}
		// Для удобства работы с числовым множеством, пусть оно будет
		// отсортированным по возрастанию
		if (num < ns->numbers[i])
		{
			swap(&num, &(ns->numbers[i]));
		}
	}
	// Оставшийся элемент самый большой, помещаем его в конец
	ns->numbers[ns->real_size++] = num;
	return NO_ERROR;
}

void addFromSetToSet(NumericSet *to, const NumericSet *from, int *error)
{
	assert(to != NULL);
	assert(from != NULL);
	assert(error != NULL);
	
	if (error == NULL)
	{
		return;
	}
	
	if ((from == NULL) || (to == NULL))
	{
		*error = WRONG_INPUT;
		return;
	}
	
	int size = from->real_size;
	for (int i = 0; i < size; i++)
	{
		*error = pushToSet(to, from->numbers[i]); 
		if (*error != NO_ERROR)
		{
			break;
		}
	}
}

NumericSet* moveSet(NumericSet *from)
{	
	NumericSet *ret = from;
	from = NULL;
	return ret;
}

NumericSet *getUnionOfSets(NumericSet *A, NumericSet *B, int* error)
{
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);
	
	if (error == NULL)
	{
		return NULL;
	}
	
	if ((A == NULL) || (B == NULL))
	{
		*error = WRONG_INPUT;
		return NULL;
	}
	
	NumericSet *ret = createSet(error);
	if (*error != NO_ERROR)
	{
		return NULL;
	}
	addFromSetToSet(ret, A, error);
	addFromSetToSet(ret, B, error);
	clearSet(A);
	clearSet(B);
	return ret;
}	

NumericSet *getIntersectionOfSets(NumericSet *A, NumericSet *B, int *error)
{
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);

	if (error == NULL)
	{
		return NULL;
	}
	
	if ((A == NULL) || (B == NULL))
	{
		*error = WRONG_INPUT;
		return NULL;
	}
	
	NumericSet *ret = createSet(error);
	if (*error != NO_ERROR)
	{
		return NULL;
	}
	
	size_t sizeA = A->real_size, sizeB = B->real_size;
	size_t iA = 0, iB = 0;
	
	while ((iA < sizeA) && (iB < sizeB))
	{
		if (A->numbers[iA] < B->numbers[iB])
		{
			iA++;
			continue;
		}
		if (A->numbers[iA] > B->numbers[iB])
		{
			iB++;
			continue;
		}
		*error = pushToSet(ret, A->numbers[iA]);
		if (*error)
		{
			clearSet(ret);
			return NULL;
		}
		iB++;
		iA++;
	}
	
	clearSet(A);
	clearSet(B);
	return ret;
}

NumericSet* getSubtractionOfSets(NumericSet *A, NumericSet *B, int *error)
{
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);
	
	if (error == NULL)
	{
		return NULL;
	}
	
	if ((A == NULL) || (B == NULL))
	{
		*error = WRONG_INPUT;
		return NULL;
	}
	
	NumericSet *ret = createSet(error);
	if (*error != NO_ERROR)
	{
		return NULL;
	}
	
	size_t sizeA = A->real_size, sizeB = B->real_size;
	size_t iA = 0, iB = 0;
	while ((iA < sizeA) && (iB < sizeB))
	{
		if (A->numbers[iA] < B->numbers[iB])
		{
			iA++;
			continue;
		}
		if (A->numbers[iA] > B->numbers[iB])
		{
			*error = pushToSet(ret, B->numbers[iB]);
			if (*error)
			{
				clearSet(ret);
				return NULL;
			}
			iB++;
			continue;
		}
		iB++;
		iA++;
	}
	
	// Если не все элементы из множества B успели залететь
	// Например если размер B гораздо больше А
	while(iB < sizeB)
	{
		*error = pushToSet(ret, A->numbers[iB]);
		if (*error != NO_ERROR)
		{
			clearSet(ret);
			return NULL;
		}
		iB++;
	}
	
	clearSet(A);
	clearSet(B);
	return ret;
}

size_t getNumberSize(int number)
{	
	size_t size = 0;
	while (number != 0)
	{
		number /= 10;
		size++;
	}
	return size;
}

void insertNumberInString(int number, char *begin[])
{
	assert(begin != NULL);
	assert(*begin != NULL);
	
	if ((begin == NULL) || (*begin == NULL))
	{
		return;
	}
	
	size_t size = getNumberSize(number);
	
	for(size_t i = size; i > 0; i--)
	{
		*((*begin) + i - 1) = number % 10 + '0';
		number /= 10;
	}
	
	*begin = *begin + size;
}

void numericSetToChar(NumericSet *ns, char *begin, char *end)
{
	assert(ns != NULL);
	assert(begin != NULL);
	assert(end != NULL);
	
	if ((begin == NULL) || (end == NULL) || (ns == NULL))
	{
		return;
	}

	*(begin++) = '[';
	*end = ']';
	
	int size = ns->real_size;
	for (int i = 0; i < size; i++)
	{
		if (i != 0)
		{
			*(begin++) = ',';
		}
		insertNumberInString(ns->numbers[i], &begin);
	}
	while (end > begin)
	{
		*(begin++) = ' ';
	}
}

char* checkString(const char* string, size_t size, int *error)
{	
	assert(string != NULL);
	assert(error != NULL);
	assert(size > 0);
	
	if (error == NULL)
	{
		return NULL;
	}
	
	if ((string == NULL) || (size == 0))
	{
		*error = WRONG_INPUT;
		return NULL;
	}

	int afterNumber = 0; // находится ли символ после числа
	// Находится ли множество за оператором
	// -1 - первый элемент, 1 - находится за оператором, 0 - не находится
	int afterOperator = -1; 
	
	int afterLeftSquare = 0; // после '['
	int afterLeftCircle = 0; // после '('
	int squareInCircle = 1; // Круглые скобки содержат в себе множества
	
	int setAmount = 0; //количество множеств
	int operatorAmount = 0; //количество операторов
	int afterComma = 0; // находится ли символ за запятой
	
	for (size_t i = 0; i < size; i++)
	{
		// Если встречен пробел
		if (string[i] == ' '){}
		// Если встречена цифра
		else if ((string[i] >= '0') &&
			(string[i] <= '9'))
		{
			// Если число вне скобок
			if (afterLeftSquare == 0)
			{
				*error = ERROR_NUMBER_NOT_IN_SET;
				break;
			}
			afterComma = 0; // следующий символ  не находится за запятой
			afterNumber = 1; // следующий символ находится за числом
		}
		// Если символ ','
		else if (string[i] == ',')
		{
			// Если запятая не идет после числа
			if (afterNumber == 0)
			{
				*error = ERROR_COMMA_NOT_AFTER_NUMBER;
				break;
			}
			afterNumber = 0; // следующий символ не находится за числом
			afterComma = 1; // следующий символ находится за запятой
		}
		// Если обнаружен один из операторов
		else if ((string[i] == 'U') ||
		(string[i] == '^') || (string[i] == '\\'))
		{
			// Если оператор идет после '['
			if (afterLeftSquare == 1)
			{
				*error = ERROR_OPERATOR_IN_SET;
				break;
			}
			operatorAmount++; // Инкремент количества операторов
			afterOperator = 1;
		}
		// Если символ '['
		else if (string[i] == '[')
		{
			// Если множество не за оператором
			if (afterOperator == 0)
			{
				*error = ERROR_OPERATOR_WITHOUT_SETS;
				break;
			}
			afterLeftSquare = 1; // Последующие символы после [
			afterOperator = 0; // Последующие символы не за оператором
			setAmount++; // Инкремент количества множеств
			if (afterLeftCircle > 0)
				squareInCircle = 1; // Обнаружено множество в скобках
		}
		else if (string[i] == ']')
		{
			// Если конец множества идёт сразу после запятой
			if (afterComma == 1)
			{
				*error = ERROR_COMMA_WITHOUT_NUMBER;
				break;
			}
			// Если ']' идет первее '['
			if (afterLeftSquare == 0)
			{
				*error = ERROR_SQUERE_END_BEFORE_START;
				break;
			}
			afterLeftSquare = 0; // Дальше символы идут вне множества
		}
		else if (string[i] == '(')
		{
			// Если круглая скобка внутри квадратной
			if (afterLeftSquare == 1)
			{
				*error = ERROR_CIRCLE_IN_SQUERE;
				break;
			}
			afterLeftCircle++; // Последующие символы после (
			squareInCircle = 0; // Скобки не содержат множеств
		}
		else if (string[i] == ')')
		{
			// Количество скобок уменьшилось
			afterLeftCircle--;
			// Если ')' идет первее '('
			if (afterLeftCircle < 0)
			{
				*error = ERROR_CIRCLE_END_BEFORE_START;
				break;
			}
			// Если еще не было обьявлено ни одного множества
			if (afterOperator == -1)
			{
				*error = ERROR_NO_SQUERE_IN_CIRCLE ;
				break;
			}
			afterOperator = 0; // Следующее множество не за оператором
		}
		else
		{
			printf("\n delete it %c", string[i]);
			// если запрещенный символ
			*error = ERROR_WRONG_SYMBOL;
			break;;
		}
	}
	
	if (*error != NO_ERROR)
	{
		return NULL;
	}
	
	// Если одна из круглых скобок не закрылась
	if (afterLeftCircle > 0)
	{
		*error = ERROR_NO_CIRCLE_END;
		return NULL;
	}
	
	// Если одна из квадратных скобок не закрылась
	if (afterLeftSquare > 0)
	{
		*error = ERROR_NO_SQUERE_END;
		return NULL;
	}
	
	// Операторов всегда на 1 меньше чем множеств(например [] U [] ^ [])
	// Если это не так, то значит входные данные не верны
	if (setAmount - operatorAmount != 1)
	{
		*error = ERROR_WRONG_NUMBER_OF_OPERATORS;
		return NULL;
	}
	
	// Круглые скобки не содержут в себе множеств
	if (squareInCircle == 0)
	{
		*error = ERROR_NO_SQUERE_IN_CIRCLE;
		return NULL;
	}
		
	char* ret = (char*)calloc(size + 2,
			sizeof(struct NumericSet*));
	if (!ret)
	{
		*error = MEMORY_ERROR;
		return NULL;
	}
	ret[0] = '(';
	ret[size + 1] = ')';
	for (size_t i = 0; i < size; i++)
	{
		ret[i + 1] = string[i];
	}
	return ret;
}

/*
int canApplyOPeration(const int priority, char )
{
	return (((priority == 0) && (operator == '^')) ||
			((priority == 1) && (operator == 'U')) ||
			((priority == 2) && (operator == '\\')));
}
*/

void getAnswer(char* string)
{
	assert(string != NULL);
	
	if (string == NULL)
	{
		return;
	}
	
	int strsize = strlen(string);
	
	int number = 0, begin = 0;
	
	int error = NO_ERROR;
	
	NumericSet *first = NULL, *second = NULL, *result = NULL;
	
	char operator = ' ';
	
	char* localBegin = NULL;
	
	int circleBrackets = 1;
	
	// Возможные операции
	char operations[] = "^U\\";
	
	while (circleBrackets > 0)
	{
		circleBrackets = 0;
		for (size_t i = 0; i < strsize; i++)
		{
			if (string[i] == '(')
			{
				circleBrackets++;
				begin = i;
			}
			else if (string[i] == ')')
			{
				for (char* currentOperator = operations; 
				*currentOperator != '\0'; currentOperator++)
				{
					for (char* letter = string + begin; letter < string + i; letter++)
					{
						if (*letter == '[')
						{
							first = createSet(&error);
							if (error != NO_ERROR)
							{
								break;
							}
							number = 0; 
							
							if (!(*currentOperator == operator))
							{
								localBegin = letter;
							}
						}
						else if (*letter == ']')
						{
							if (number != 0)
							{
								error = pushToSet(first, number);
								if (error != NO_ERROR)
								{
									break;
								}
							}
							number = 0;
							
							if (*currentOperator == operator)
							{
								switch(operator)
								{
									case 'U':
									{
										result = getUnionOfSets(first, second, &error);
										break;
									}
									case '\\':
									{
										result = getSubtractionOfSets(first, second,
																		&error);
										break;
									}
									case '^':
									{
										result = getIntersectionOfSets(first, second,
																		&error);
										break;
									}
								}
								if (error != NO_ERROR)
									break;
								
								second = moveSet(result);
								if (error != NO_ERROR)
									break;
								
								numericSetToChar(second, localBegin, letter);
								
								operator = ' ';
							}
							else 
							{
								if (second != NULL)
								{
									clearSet(second);
								}
								
								second = moveSet(first);
								if (error != NO_ERROR)
									break;
							}
						}
						else if (*letter == ',')
						{
							if (pushToSet(first, number) != NO_ERROR)
							{
								break;
							}
							number = 0;
						}
						else if ((*letter >= '0') &&
							(*letter <= '9'))
						{
							number *= 10;
							number += (*letter - '0');
						}
						else if (*letter == ' ')
						{
							;
						}
						else
						{
							operator = *letter;
						}
					}
				}
				numericSetToChar(second, string + begin, string + i);
				break;
			}	
		}
	}
	if (error != NO_ERROR)
	{
		printf("[error]");
	}
	else
	{
		printSet(second);
	}
	clearSet(second);
}

void printResult(int real_error, int my_error, char* string, char* name)
{
	printf("\n %s", name);
	if (real_error == my_error)
		printf(" OK");
	else
		printf(" Failed. Found %d, but need %d", my_error, real_error);
}

void case_error1()
{
	char* stroka = "(";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NO_CIRCLE_END, error, string, stroka);
}

void case_error2()
{
	char* stroka = "()";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error3()
{
	char* stroka = "[1,2,3,]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_COMMA_WITHOUT_NUMBER, error, string, stroka);
}

void case_error4()
{
	char* stroka = "[1,,3]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_COMMA_NOT_AFTER_NUMBER, error, string, stroka);
}

void case_error5()
{
	char* stroka = "[1]3";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NUMBER_NOT_IN_SET, error, string, stroka);
}

void case_error6()
{
	char* stroka = "[1]()";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error7()
{
	char* stroka = "()[1]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error8()
{
	char* stroka = "[[1]]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}

void case_error9()
{
	char* stroka = "[[1] U]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}
void case_error10()
{
	char* stroka = "[1,2,3,4";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_NO_SQUERE_END, error, string, stroka);
}
void case_error11()
{
	char* stroka = "[1,2,3,4] U][";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_SQUERE_END_BEFORE_START, error, string, stroka);
}
void case_error12()
{
	char* stroka = "([1,2,3,4] U))U([])";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_CIRCLE_END_BEFORE_START, error, string, stroka);
}
void case_error13()
{
	char* stroka = "([1,2,3,4] U)[]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}
void case_error14()
{
	char* stroka = "     ";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(ERROR_WRONG_NUMBER_OF_OPERATORS, error, string, stroka);
}

void error_tests()
{
	printf("\n\n Error tests of checkString()\n");
	case_error1();
	case_error2();
	case_error3();
	case_error4();
	case_error5();
	case_error6();
	case_error7();
	case_error8();
	case_error9();
	case_error10();
	case_error11();
	case_error12();
	case_error13();
	case_error14();
}

void case_right5()
{
	char* stroka = "([]U([]\\([])^([] ^   ([] U []))))";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(NO_ERROR , error, string, stroka);
}
void case_right6()
{
	char* stroka = "([1])";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(NO_ERROR  , error, string, stroka);
}
void case_right7()
{
	char* stroka = "(([1]))";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(NO_ERROR , error, string, stroka);
}
void case_right8()
{
	char* stroka = "[]U[]";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(NO_ERROR , error, string, stroka);
}
void case_right9()
{
	char* stroka = "([])U([])";
	int error = NO_ERROR;
	int strsize = strlen(stroka);
	char* string =  checkString(stroka, strsize, &error);
	printResult(NO_ERROR , error, string, stroka);
}

void right_tests()
{
	printf("\n\n Right tests of checkString()\n");
	case_right5();
	case_right6();
	case_right7();
	case_right8();
	case_right9();
}

void unit_tests()
{
	error_tests();
	right_tests();
}

int getString(char **lineptr, size_t *n, int delimiter, FILE *stream)
{
	assert(lineptr != NULL);
	assert(n != NULL);
	assert(stream != NULL);
	
	if ((lineptr == NULL) || (n== NULL) || (stream == NULL))
	{
		return WRONG_INPUT;
	}
	
    size_t count = 0;
    char *pb = NULL;
    char c = 0;
   
    char *tmp = NULL;

    if (*lineptr == NULL)
    {
        tmp = (char*)calloc(*n, sizeof(char));
        if (!tmp)
		{
			free(*lineptr);
            return MEMORY_ERROR;
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
				count = 0;
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
	
	if (count == 0)
	{
		return EOF;
	}

    *pb = '\0';

    return NO_ERROR;
}