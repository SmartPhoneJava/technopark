/*
Задача B-1. Калькулятор для числовых множеств
Time limit:	14 s
Memory limit:	64 M
Разработайте программу-калькулятор, умеющую вычислять арифметические выражения
над множествами натуральных чисел. Входные данные (выражение с описанием
множеств и производимых над ними операций) подаются на стандартный поток ввода
программы, результат вычислений должен подаваться на стандартный поток вывода.

Поддерживаемые операции: 'U' - объединение, '^' - пересечение, '\' - разность
множеств, '()' - задание приоритета вычислений.
Множества задаются в виде [ el1, el2, ... ], где элементами являются обычные
натуральные числа.
Множества могут быть пустыми.
Между операциями, множествами и элементами множеств может быть любое количество
пробелов.

При вычислениях должны учитываться приоритеты операций (в том числе, заданные
вручную при помощи круглых скобочек).

Результирующее множество должно выводиться без пробелов, в отсортированном
порядке.
В случае, если введенное выражение содержит ошибку, необходимо вывести в поток
стандартного вывода сообщение "[error]" и завершить выполнение программы.
*/

#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

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

typedef struct numericSet_t {
	int *numbers;
	size_t max_size;
	size_t real_size;
} numericSet_t;

// Обменять местами двух целочисленных значений
void swap(int *a, int *b);

// Печатать множество
void set_print(const numericSet_t *ns);

// Создать множество
numericSet_t *set_create(int *error);

// Очистить множество
void set_clear(numericSet_t *ns);

// Добавить элемент в множество
int set_add(numericSet_t *ns, int num);

// Добавить в множество to элементы из множества for
void set_add_from_set(const numericSet_t *from, numericSet_t *to, int *error);

// Получить пересечение множеств(с очисткой)
numericSet_t *get_union_of_sets(numericSet_t *A, numericSet_t *B, int *error);

// Получить пересечение множеств(с очисткой)
numericSet_t *get_intersection_of_sets(numericSet_t *A, numericSet_t *B, int *error);

// получить разность множеств(с очисткой)
numericSet_t *get_subtraction_of_sets(numericSet_t *A, numericSet_t *B, int *error);

// Получить размер числа
size_t get_number_size(int number);

// Вставить число в строку
void insert_number_in_string(int number, char *begin[]);

// Вставить множество в строку
void numeric_set_to_char(numericSet_t *ns, char *begin, char *end);

// Проверить строку на корректность
int check_string(const char string[]);

// Перевыделяет память для строки и добавляет круглые скобки 
// в начало и конец строки
// Возвращает код ошибки
int add_circle_brackets_to_string(char* string[]);

// Преобразует строку с несколькими множествами в строку с одним множеством
// В случае неудачи возвращает код ошибки, иначе NO_ERROR
int merge_all_sets_in_string_into_one(char string[]);

// Распечатать строку без пробелов
void print_without_spaces(const char string[]);

void unit_tests();

int get_string(char **lineptr, size_t *n, int delimiter, FILE *stream);

int main() {
	//unit_tests();
	
	char *buffer = NULL;
	int error = NO_ERROR;
	size_t max_size = BUFFER_SIZE;
	while (get_string(&buffer, &max_size, '\n', stdin) == NO_ERROR && error == NO_ERROR) {
		error = check_string(buffer);
		if (error == NO_ERROR) {
			error = add_circle_brackets_to_string(&buffer);
			if (error == NO_ERROR) {
				error = merge_all_sets_in_string_into_one(buffer);
				if (error != NO_ERROR) {
					printf("[error]");
				} else {
					print_without_spaces(buffer);
				}
			} else {
				printf("[error]");
			}
		} else {
			printf("[error]");
		}
		free(buffer);
	}
	
	return 0;
}

void swap(int *a, int *b) {
	assert(a != NULL);
	assert(b != NULL);

	if ((a == NULL) || (b == NULL)) {
		return;
	}

	int tmp = *a;
	*a = *b;
	*b = tmp;
}

void set_print(const numericSet_t *ns) {
	assert(ns != NULL);

	if (ns == NULL) {
		return;
	}

	size_t size = ns->real_size;
	if (size == 0) {
		printf("[]");
		return;
	}
	printf("[%d", ns->numbers[0]);
	for (size_t i = 1; i < size; i++) {
		printf(",%d", ns->numbers[i]);
	}
	printf("]");
}

numericSet_t *set_create(int *error) {
	assert(error != NULL);

	if (error == NULL) {
		return NULL;
	}

	numericSet_t *ns = (numericSet_t *)calloc(1, sizeof(numericSet_t));
	if (ns == NULL) {
		*error = MEMORY_ERROR;
		return NULL;
	}

	ns->max_size = 2;
	ns->real_size = 0;
	ns->numbers = (int *)calloc(ns->max_size, sizeof(int));
	if (ns->numbers == NULL) {
		free(ns);
		*error = MEMORY_ERROR;
		return NULL;
	}
	return ns;
}

void set_clear(numericSet_t *ns) {
	if (ns != NULL) {
		free(ns->numbers);
		free(ns);
	}
}

int set_add(numericSet_t *ns, int num) {
	assert(ns != NULL);
	assert(num > 0);

	if ((ns == NULL) || (num <= 0)) {
		return WRONG_INPUT;
	}

	// Если числовое множество не содержит элементов
	if (ns->real_size == 0) {
		ns->numbers[0] = num;
		ns->real_size++;
		return NO_ERROR;
	}

	size_t rsize = ns->real_size;

	// Если числовое множество полностью заполнилось
	if (rsize >= ns->max_size) {
		ns->max_size *= 2;
		int* buffer = (int *)realloc(ns->numbers, ns->max_size * sizeof(int));
		if (buffer == NULL) {
			return MEMORY_ERROR;
		}
		ns->numbers = buffer;
	}
	for (size_t i = 0; i < rsize; i++) {
		if (num == ns->numbers[i]) {
			// Поскольку числовому множеству не нужны дубли
			// просто не будем их вставлять
			return NO_ERROR;
		}
		// Для удобства работы с числовым множеством, пусть оно будет
		// отсортированным по возрастанию
		if (num < ns->numbers[i]) {
			swap(&num, &(ns->numbers[i]));
		}
	}
	// Оставшийся элемент самый большой, помещаем его в конец
	ns->numbers[ns->real_size++] = num;
	return NO_ERROR;
}

void set_add_from_set(const numericSet_t *from, numericSet_t *to, int *error) {
	assert(to != NULL);
	assert(from != NULL);
	assert(error != NULL);

	if (error == NULL) {
		return;
	}

	if ((from == NULL) || (to == NULL)) {
		*error = WRONG_INPUT;
		return;
	}

	size_t size = from->real_size;
	for (size_t i = 0; (i < size) && (*error == NO_ERROR); i++) {
		*error = set_add(to, from->numbers[i]);
	}
}

numericSet_t *get_union_of_sets(numericSet_t *A, numericSet_t *B, int *error) {
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);

	if (error == NULL) {
		return NULL;
	}

	if ((A == NULL) || (B == NULL)) {
		*error = WRONG_INPUT;
		return NULL;
	}

	numericSet_t *ret = set_create(error);
	if (*error != NO_ERROR) {
		return NULL;
	}
	set_add_from_set(A, ret, error);
	set_add_from_set(B, ret, error);
	set_clear(A);
	set_clear(B);
	return ret;
}

numericSet_t *get_intersection_of_sets(numericSet_t *A, numericSet_t *B, int *error) {
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);

	if (error == NULL) {
		return NULL;
	}

	if ((A == NULL) || (B == NULL)) {
		*error = WRONG_INPUT;
		return NULL;
	}

	// Проверка error не требуется, поскольку цикл ниже
	// имеет условие *error == NO_ERROR
	numericSet_t *ret = set_create(error);

	size_t sizeA = A->real_size, sizeB = B->real_size;
	size_t iA = 0, iB = 0;

	while ((iA < sizeA) && (iB < sizeB)  && (*error == NO_ERROR)) {
		if (A->numbers[iA] < B->numbers[iB]) {
			iA++;
		} else if (A->numbers[iA] > B->numbers[iB]) {
			iB++;
		} else {
			*error = set_add(ret, A->numbers[iA]);
			iB++;
			iA++;
		}
	}
	
	if (*error != NO_ERROR) {
		set_clear(ret);
		return NULL;
	}

	set_clear(A);
	set_clear(B);
	return ret;
}

numericSet_t *get_subtraction_of_sets(numericSet_t *A, numericSet_t *B, int *error) {
	assert(A != NULL);
	assert(B != NULL);
	assert(error != NULL);

	if (error == NULL) {
		return NULL;
	}

	if ((A == NULL) || (B == NULL)) {
		*error = WRONG_INPUT;
		return NULL;
	}

	size_t sizeA = A->real_size, sizeB = B->real_size;
	size_t iA = 0, iB = 0;
	
	numericSet_t *ret = set_create(error);

	while ((iA < sizeA) && (iB < sizeB) && (*error == NO_ERROR)) {
		if (A->numbers[iA] < B->numbers[iB]) {
			iA++;
		} else if (A->numbers[iA] > B->numbers[iB]) {
			*error = set_add(ret, B->numbers[iB]);
			iB++;
		} else {
			iB++;
			iA++;
		}
	}

	// Если не все элементы из множества B успели залететь
	// Например если размер B гораздо больше А
	while ((iB < sizeB) && (*error == NO_ERROR)) {
		*error = set_add(ret, A->numbers[iB]);
		iB++;
	}
	
	if (*error != NO_ERROR) {
		set_clear(ret);
		return NULL;
	}

	set_clear(A);
	set_clear(B);
	return ret;
}

size_t get_number_size(int number) {
	size_t size = 0;
	while (number != 0) {
		number /= 10;
		size++;
	}
	return size;
}

void insert_number_in_string(int number, char *begin[]) {
	assert(begin != NULL);
	assert(*begin != NULL);

	if ((begin == NULL) || (*begin == NULL)) {
		return;
	}

	size_t size = get_number_size(number);

	for (size_t i = size; i > 0; i--) {
		*((*begin) + i - 1) = number % 10 + '0';
		number /= 10;
	}

	*begin = *begin + size;
}

void numeric_set_to_char(numericSet_t *ns, char *begin, char *end) {
	assert(ns != NULL);
	assert(begin != NULL);
	assert(end != NULL);

	if ((begin == NULL) || (end == NULL) || (ns == NULL)) {
		return;
	}

	*(begin++) = '[';
	*end = ']';

	int size = ns->real_size;
	for (int i = 0; i < size; i++) {
		if (i != 0) {
			*(begin++) = ',';
		}
		insert_number_in_string(ns->numbers[i], &begin);
	}
	while (end > begin) {
		*(begin++) = ' ';
	}
}

bool is_symbol_number(char symbol) {
	return symbol >= '0' && symbol <= '9';
}

bool is_symbol_operator(char symbol) {
	return symbol == 'U' || symbol == '^' || symbol == '\\';
}

// Обновить флаги проверок после обнаружения в строке цифры и провести 
// проверку может ли цифра находится в этом месте строки
void work_with_symbol_number(const bool after_left_square, bool *after_comma,
				 bool *after_number, int *error) {	
	// Если число вне скобок
	if (after_left_square == false) {
		*error = ERROR_NUMBER_NOT_IN_SET;
	}
	*after_comma = false;  // следующий символ  не находится за запятой
	*after_number = true;  // следующий символ находится за числом
}

// Обновить флаги проверок после обнаружения в строке запятой и провести 
// проверку может ли запятая находится в этом месте строки
void work_with_symbol_comma(bool *after_number, bool *after_comma, int *error) {
	// Если запятая не идет после числа
	if (*after_number == false) {
		*error = ERROR_COMMA_NOT_AFTER_NUMBER;
	}
	*after_number = false;  // следующий символ не находится за числом
	*after_comma = true;  // следующий символ находится за запятой
}

// Обновить флаги проверок после обнаружения в строке одного из операторов
// и провести проверку может ли оператор находится в этом месте строки
void work_with_symbol_operator(const bool after_left_square, int *operator_amount,
				   int *after_operator, int *error) {
	// Если оператор идет после '['
	if (after_left_square == true) {
		*error = ERROR_OPERATOR_IN_SET;
	}
	*operator_amount = *operator_amount + 1;  // Инкремент количества операторов
	*after_operator = 1;
}

void work_left_square_bracket(const int circle_brackets_amount, int *after_operator,
							bool *after_left_square, bool *square_in_circle,
							int *set_amount, int *error) {
	// Если множество не за оператором
	if (*after_operator == 0) {
		*error = ERROR_OPERATOR_WITHOUT_SETS;
	}
	*after_left_square = true;  // Последующие символы после [
	*after_operator = 0;  // Последующие символы не за оператором
	*set_amount = *set_amount + 1;  // Инкремент количества множеств
	if (circle_brackets_amount > 0) {
		*square_in_circle = true;  // Обнаружено множество в скобках
	}
}

void work_right_square_bracket(const bool after_comma, bool *after_left_square,
							 int *error) {
	// Если конец множества идёт сразу после запятой
	if (after_comma == true) {
		*error = ERROR_COMMA_WITHOUT_NUMBER;
	}
	// Если ']' идет первее '['
	if (*after_left_square == false) {
		*error = ERROR_SQUERE_END_BEFORE_START;
	}
	*after_left_square = false;  // Дальше символы идут вне множества
}

void work_left_circle_bracket(const bool after_left_square, bool *square_in_circle,
							int *circle_brackets_amount, int *error) {
	// Если круглая скобка внутри квадратной
	if (after_left_square == 1) {
		*error = ERROR_CIRCLE_IN_SQUERE;
	}
	// Последующие символы после '('
	*circle_brackets_amount = *circle_brackets_amount + 1;
	*square_in_circle = 0;  // Скобки не содержат множеств
}

void work_right_circle_bracket(int *circle_brackets_amount, int *after_operator,
							 int *error) {
	// Количество скобок уменьшилось
	*circle_brackets_amount = *circle_brackets_amount - 1;
	// Если ')' идет первее '('
	if (*circle_brackets_amount < 0) {
		*error = ERROR_CIRCLE_END_BEFORE_START;
	}
	// Если еще не было обьявлено ни одного множества
	if (*after_operator == -1) {
		*error = ERROR_NO_SQUERE_IN_CIRCLE;
	}
	*after_operator = 0;  // Следующее множество не за оператором
}

size_t get_string_size(const char string[])
{
	size_t size = 0;
	while (string[size] != '\0') {
		size++;
	}
	return size;
}

int add_circle_brackets_to_string(char* string[]) {
	size_t size = get_string_size(*string);
	
	char* tmp = realloc(*string, size + 2);
	if (!tmp) {
		return MEMORY_ERROR;
	}
	*string = tmp;
	
	for (size_t i = size; i > 0; i--) {
		(*string)[i] = (*string)[i - 1];
	}
	(*string)[0] = '(';
	(*string)[size + 1] = ')';
	
	return NO_ERROR;
}

int check_string(const char *string) {
	assert(string != NULL);

	if (string == NULL) {
		return WRONG_INPUT;
	}
	
	size_t size = strlen(string);

	bool after_number = false;  // находится ли символ после числа
	// Находится ли множество за оператором
	// -1 - первый элемент, 1 - находится за оператором, 0 - не находится
	int after_operator = -1;
	bool after_comma = false;  // находится ли символ за запятой

	bool after_left_square = false;  // после '['
	bool square_in_circle = true;  // Круглые скобки содержат в себе множества

	int circle_brackets_amount = 0;  // Количсетво круглых скобок
	int set_amount = 0;             //количество множеств
	int operator_amount = 0;        //количество операторов
	
	int error = NO_ERROR;
	
	// Именованные константы
	const char symbol_space = ' ';
	const char symbol_comma = ',';
	const char symbol_left_square_bracket = '[';
	const char symbol_right_square_bracket = ']';
	const char symbol_left_circle_bracket = '(';
	const char symbol_right_circle_bracket = ')';

	for (const char *symbol = string; *symbol != '\0' && error == NO_ERROR; symbol++) {
		if (*symbol == symbol_space) {
		} else if (is_symbol_number(*symbol)) {
			work_with_symbol_number(after_left_square, &after_comma, &after_number, &error);
		} else if (*symbol == symbol_comma) {
			work_with_symbol_comma(&after_number, &after_comma, &error);
		} else if (is_symbol_operator(*symbol)) {
			work_with_symbol_operator(after_left_square, &operator_amount, &after_operator, &error);
		} else if (*symbol == symbol_left_square_bracket) {
			work_left_square_bracket(circle_brackets_amount, &after_operator,
								   &after_left_square, &square_in_circle, &set_amount,
								   &error);
		} else if (*symbol == symbol_right_square_bracket) {
			work_right_square_bracket(after_comma, &after_left_square, &error);
		} else if (*symbol == symbol_left_circle_bracket) {
			work_left_circle_bracket(after_left_square, &square_in_circle,
								   &circle_brackets_amount, &error);
		} else if (*symbol == symbol_right_circle_bracket) {
			work_right_circle_bracket(&circle_brackets_amount, &after_operator, &error);
		} else {  // если запрещенный символ
			error = ERROR_WRONG_SYMBOL;
		}
	}

	// Если одна из круглых скобок не закрылась
	if (circle_brackets_amount > 0) {
		error = ERROR_NO_CIRCLE_END;
	}
	// Если одна из квадратных скобок не закрылась
	else if (after_left_square > 0) {
		error = ERROR_NO_SQUERE_END;
	}
	// Операторов всегда на 1 меньше чем множеств(например [] U [] ^ [])
	// Если это не так, то значит входные данные не верны
	else if (set_amount - operator_amount != 1) {
		error = ERROR_WRONG_NUMBER_OF_OPERATORS;
	}
	// Круглые скобки не содержут в себе множеств
	else if (square_in_circle == false) {
		error = ERROR_NO_SQUERE_IN_CIRCLE;
	}
	
	return error;
}

numericSet_t *apply_operation(char operator, numericSet_t *first, numericSet_t *second,
						   int *error) {
	assert(first != NULL);
	assert(second != NULL);
	assert(error != NULL);

	if (error == NULL) {
		return NULL;
	}

	if ((first == NULL) || (second == NULL)) {
		*error = WRONG_INPUT;
		return NULL;
	}
	
	numericSet_t *result = NULL;
	switch (operator) {
	case 'U': {
		result = get_union_of_sets(first, second, error);
		break;
	}
	case '\\': {
		result = get_subtraction_of_sets(first, second, error);
		break;
	}
	case '^': {
		result = get_intersection_of_sets(first, second, error);
		break;
	}
	default: {
		*error = ERROR_WRONG_SYMBOL;
		set_clear(first);
		set_clear(second);
	}
	}
	return result;
}

int calculate_sets_in_string(char* string, char* begin, char* end,
									 char current_operator)
{
	numericSet_t *first = NULL, *second = NULL;
	
	char *localBegin = NULL;
	char operator = ' ';
	
	int error = NO_ERROR;
	
	int number = 0;
	
	// Именованные константы
	const char symbol_space = ' ';
	const char symbol_comma = ',';
	const char symbol_left_square_bracket = '[';
	const char symbol_right_square_bracket = ']';
	
	for (char *letter = begin;
		 error == NO_ERROR && letter < end;
		 letter++) {
		if (*letter == symbol_left_square_bracket) {
			first = set_create(&error);
			number = 0;

			if (!(operator == current_operator)) {
				localBegin = letter;
			}
		} else if (*letter == symbol_right_square_bracket) {
			if (number != 0) {
				error = set_add(first, number);
			}
			number = 0;
			
			if (error == NO_ERROR)
			{
				if (operator == current_operator) {
					second = apply_operation(operator, first, second, &error);
					if (error == NO_ERROR)
					{
						numeric_set_to_char(second, localBegin, letter);

						operator = ' ';
					}
				} else {
					if (second != NULL) {
						set_clear(second);
					}
					second = first;
					first = NULL;
				}
			}
		} else if (*letter == symbol_comma) {
			set_add(first, number);
			number = 0;
		} else if (is_symbol_number(*letter)) {
			number *= 10;
			number += (*letter - '0');
		} else if (*letter == symbol_space) {
		} else {
			operator = *letter;
		}
	}
	set_clear(second);
	return error;
}

void print_without_spaces(const char string[])
{
	size_t size = get_string_size(string);
	for (size_t i = 0; i < size; i++)
	{
		if (string[i] != ' ')
		{
			printf("%c", string[i]);
		}
	}
}

// Возвращает true, если удалось найти пару скобок и false если нет
// В случае успеха left и right указывают на положение левой и правой скобок
// в строке.
// Подразумевается, что на вход приходит строка, в которой круглые скобки
// расставлены в правильном порядке и количество '(' и ')' совпадает 	
bool find_circle_brackets(char string[], size_t string_size,
							char** left, char** right) {
	*left = NULL;
	*right = NULL;
	
	const char left_circle = '(';
	const char right_circle = ')';
	for (size_t i = 0; // пока right не найдено
		i < string_size && *right == NULL;
		i++) {
		if (string[i] == left_circle) {
			*left = string + i;
		} else if (string[i] == right_circle) {
			*right = string + i;
		}
	}
	if (*right == NULL) {
		return false;
	}
	return true;
}

// Преобразует строку с несколькими множествами в строку с одним множеством
// В случае неудачи возвращает код ошибки, иначе NO_ERROR
int merge_all_sets_in_string_into_one(char string[]) {
	assert(string != NULL);

	if (string == NULL) {
		return WRONG_INPUT;
	}

	int strsize = strlen(string);
	int error = NO_ERROR;

	// Возможные операции
	char operations[] = "^U\\";
	
	char *left_circle, *right_circle;

	while (find_circle_brackets(string, strsize,
								&left_circle, &right_circle)) {
		
		for (char *currentOperator = operations;
			*currentOperator != '\0' && error == NO_ERROR;
				currentOperator++) {
			error = calculate_sets_in_string(string, left_circle,
									right_circle,
									*currentOperator);
		}
		*left_circle = ' ';
		*right_circle = ' ';
	}
	
	return error;
}

void print_result_of_unit_test(int real_error, int my_error, char *name) {
	printf("\n %s", name);
	if (real_error == my_error)
		printf(" OK");
	else
		printf(" Failed. Found %d, but need %d", my_error, real_error);
}

void case_error1() {
	char *string = "(";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NO_CIRCLE_END, error, string);
}

void case_error2() {
	char *string = "()";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error3() {
	char *string = "[1,2,3,]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_COMMA_WITHOUT_NUMBER, error, string);
}

void case_error4() {
	char *string = "[1,,3]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_COMMA_NOT_AFTER_NUMBER, error, string);
}

void case_error5() {
	char *string = "[1]3";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NUMBER_NOT_IN_SET, error, string);
}

void case_error6() {
	char *string = "[1]()";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error7() {
	char *string = "()[1]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error8() {
	char *string = "[[1]]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}

void case_error9() {
	char *string = "[[1] U]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}
void case_error10() {
	char *string = "[1,2,3,4";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_NO_SQUERE_END, error, string);
}
void case_error11() {
	char *string = "[1,2,3,4] U][";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_SQUERE_END_BEFORE_START, error, string);
}
void case_error12() {
	char *string = "([1,2,3,4] U))U([])";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_CIRCLE_END_BEFORE_START, error, string);
}
void case_error13() {
	char *string = "([1,2,3,4] U)[]";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}
void case_error14() {
	char *string = "     ";
	int error = check_string(string);
	print_result_of_unit_test(ERROR_WRONG_NUMBER_OF_OPERATORS, error, string);
}

void error_tests() {
	printf("\n\n Error tests of check_string()\n");
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

void case_right4() {
	char *string = "([3,4] U [2])";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}

void case_right5() {
	char *string = "([]U([]\\([])^([] ^   ([] U []))))";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right6() {
	char *string = "([1])";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right7() {
	char *string = "(([1]))";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right8() {
	char *string = "[]U[]";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right9() {
	char *string = "([])U([])";
	int error = check_string(string);
	print_result_of_unit_test(NO_ERROR, error, string);
}

void right_tests() {
	printf("\n\n Right tests of check_string()\n");
	case_right4();
	case_right5();
	case_right6();
	case_right7();
	case_right8();
	case_right9();
}

void unit_tests() {
	right_tests();
	error_tests();
}

int get_string(char **lineptr, size_t *n, int delimiter, FILE *stream) {
	assert(lineptr != NULL);
	assert(n != NULL);
	assert(stream != NULL);

	if ((lineptr == NULL) || (n == NULL) || (stream == NULL)) {
		return WRONG_INPUT;
	}

	size_t count = 0;
	char *pb = NULL;
	char c = 0;

	char *tmp = NULL;

	if (*lineptr == NULL) {
		tmp = (char *)calloc(*n, sizeof(char));
		if (!tmp) {
			free(*lineptr);
			return MEMORY_ERROR;
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
				count = 0;
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

	if (count == 0) {
		return EOF;
	}

	*pb = '\0';

	return NO_ERROR;
}