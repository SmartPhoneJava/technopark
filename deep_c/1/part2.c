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

typedef struct Numeric_set {
    int *numbers;
    size_t max_size;
    size_t real_size;
} Numeric_set;

// Обменять местами двух целочисленных значений
void swap(int *a, int *b);

// Печатать множество
void print_set(const Numeric_set *ns);

// Создать множество
Numeric_set *create_set(int *error);

// Очистить множество
void clear_set(Numeric_set *ns);

// Добавить элемент в множество
int push_to_set(Numeric_set *ns, int num);

// Добавить в множество to элементы из множества for
void add_from_set_to_set(Numeric_set *to, const Numeric_set *from, int *error);

// Переместить множество
Numeric_set *move_set(Numeric_set *from);

// Получить пересечение множеств(с очисткой)
Numeric_set *get_union_of_sets(Numeric_set *A, Numeric_set *B, int *error);

// Получить пересечение множеств(с очисткой)
Numeric_set *get_intersection_of_sets(Numeric_set *A, Numeric_set *B, int *error);

// получить разность множеств(с очисткой)
Numeric_set *get_subtraction_of_sets(Numeric_set *A, Numeric_set *B, int *error);

// Получить размер числа
size_t get_number_size(int number);

// Вставить число в строку
void insert_number_in_string(int number, char *begin[]);

// Вставить множество в строку
void numeric_set_to_char(Numeric_set *ns, char *begin, char *end);

// Проверить строку на корректность
char *check_string(const char *string, int *error);

// Получить ответ
void get_answer(char *string);

void unit_tests();

int get_string(char **lineptr, size_t *n, int delimiter, FILE *stream);

int main() {
    // unit_tests();

    char *buffer = NULL, *string = NULL;
    int error = NO_ERROR;
    size_t max_size = BUFFER_SIZE;
    while (get_string(&buffer, &max_size, '\n', stdin) == NO_ERROR && error == NO_ERROR) {
        string = check_string(buffer, &error);
        free(buffer);
		// В случае любой ошибочной ситуации get_string не выделяет память, поэтому free
		// выполняется только если ошибок нет
        if (error == NO_ERROR) {
            get_answer(string);
			free(string);
        } else {
            printf("[error]");
        }
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

void print_set(const Numeric_set *ns) {
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

Numeric_set *create_set(int *error) {
    assert(error != NULL);

    if (error == NULL) {
        return NULL;
    }

    Numeric_set *ns = (Numeric_set *)calloc(1, sizeof(Numeric_set));
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

void clear_set(Numeric_set *ns) {
    if (ns != NULL) {
        free(ns->numbers);
        free(ns);
    }
}

int push_to_set(Numeric_set *ns, int num) {
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
        ns->numbers = (int *)realloc(ns->numbers, ns->max_size * sizeof(int));
        if (ns->numbers == NULL) {
            return MEMORY_ERROR;
        }
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

void add_from_set_to_set(Numeric_set *to, const Numeric_set *from, int *error) {
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
        *error = push_to_set(to, from->numbers[i]);
    }
}

Numeric_set *move_set(Numeric_set *from) {
    Numeric_set *ret = from;
    from = NULL;
    return ret;
}

Numeric_set *get_union_of_sets(Numeric_set *A, Numeric_set *B, int *error) {
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

    Numeric_set *ret = create_set(error);
    if (*error != NO_ERROR) {
        return NULL;
    }
    add_from_set_to_set(ret, A, error);
    add_from_set_to_set(ret, B, error);
    clear_set(A);
    clear_set(B);
    return ret;
}

Numeric_set *get_intersection_of_sets(Numeric_set *A, Numeric_set *B, int *error) {
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
    Numeric_set *ret = create_set(error);

    size_t sizeA = A->real_size, sizeB = B->real_size;
    size_t iA = 0, iB = 0;

    while ((iA < sizeA) && (iB < sizeB)  && (*error == NO_ERROR)) {
        if (A->numbers[iA] < B->numbers[iB]) {
            iA++;
        } else if (A->numbers[iA] > B->numbers[iB]) {
            iB++;
        } else {
			*error = push_to_set(ret, A->numbers[iA]);
			iB++;
			iA++;
		}
    }
	
	if (*error != NO_ERROR) {
		clear_set(ret);
		return NULL;
	}

    clear_set(A);
    clear_set(B);
    return ret;
}

Numeric_set *get_subtraction_of_sets(Numeric_set *A, Numeric_set *B, int *error) {
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
	
    Numeric_set *ret = create_set(error);

    while ((iA < sizeA) && (iB < sizeB) && (*error == NO_ERROR)) {
        if (A->numbers[iA] < B->numbers[iB]) {
            iA++;
        } else if (A->numbers[iA] > B->numbers[iB]) {
            *error = push_to_set(ret, B->numbers[iB]);
            iB++;
        } else {
			iB++;
			iA++;
		}
    }

    // Если не все элементы из множества B успели залететь
    // Например если размер B гораздо больше А
    while ((iB < sizeB) && (*error == NO_ERROR)) {
        *error = push_to_set(ret, A->numbers[iB]);
        iB++;
    }
	
	if (*error != NO_ERROR) {
		clear_set(ret);
		return NULL;
	}

    clear_set(A);
    clear_set(B);
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

void numeric_set_to_char(Numeric_set *ns, char *begin, char *end) {
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

bool is_symbol_comma(char symbol) {
    return symbol == ',';
}

bool is_symbol_left_circle_bracket(char symbol) {
    return symbol == '(';
}

bool is_symbol_right_circle_bracket(char symbol) {
    return symbol == ')';
}

bool is_symbol_left_square_bracket(char symbol) {
    return symbol == '[';
}

bool is_symbol_right_square_bracket(char symbol) {
    return symbol == ']';
}

bool is_symbol_space(char symbol) {
    return symbol == ' ';
}

bool is_symbol_number(char symbol) {
    return symbol >= '0' && symbol <= '9';
}

bool is_symbol_operator(char symbol) {
    return symbol == 'U' || symbol == '^' || symbol == '\\';
}

void check_number(const bool after_left_square, bool *after_comma,
                 bool *after_number, int *error) {	
    // Если число вне скобок
    if (after_left_square == false) {
        *error = ERROR_NUMBER_NOT_IN_SET;
    }
    *after_comma = false;  // следующий символ  не находится за запятой
    *after_number = true;  // следующий символ находится за числом
}

void check_comma(bool *after_number, bool *after_comma, int *error) {
    // Если запятая не идет после числа
    if (*after_number == false) {
        *error = ERROR_COMMA_NOT_AFTER_NUMBER;
    }
    *after_number = false;  // следующий символ не находится за числом
    *after_comma = true;  // следующий символ находится за запятой
}

void check_operator(const bool after_left_square, int *operator_amount,
                   int *after_operator, int *error) {
    // Если оператор идет после '['
    if (after_left_square == true) {
        *error = ERROR_OPERATOR_IN_SET;
    }
    *operator_amount = *operator_amount + 1;  // Инкремент количества операторов
    *after_operator = 1;
}

void check_left_square_bracket(const int circle_brackets_amount, int *after_operator,
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

void check_right_square_bracket(const bool after_comma, bool *after_left_square,
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

void check_left_circle_bracket(const bool after_left_square, bool *square_in_circle,
                            int *circle_brackets_amount, int *error) {
    // Если круглая скобка внутри квадратной
    if (after_left_square == 1) {
        *error = ERROR_CIRCLE_IN_SQUERE;
    }
    // Последующие символы после '('
    *circle_brackets_amount = *circle_brackets_amount + 1;
    *square_in_circle = 0;  // Скобки не содержат множеств
}

void check_right_circle_bracket(int *circle_brackets_amount, int *after_operator,
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

char* add_circle_brackets_to_string(const char* string, const size_t size, int *error) {
	char *ret = (char*)calloc(size + 2, sizeof(Numeric_set *));
    if (!ret) {
        *error = MEMORY_ERROR;
        return NULL;
    }
	 ret[0] = '(';
    ret[size + 1] = ')';
    for (size_t i = 0; i < size; i++) {
        ret[i + 1] = string[i];
    }
	return ret;
}

char *check_string(const char *string, int *error) {
    assert(string != NULL);
    assert(error != NULL);

    if (error == NULL) {
        return NULL;
    }

    if (string == NULL) {
        *error = WRONG_INPUT;
        return NULL;
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

    for (const char *symbol = string; *symbol != '\0' && *error == NO_ERROR; symbol++) {
        if (is_symbol_space(*symbol)) {
        } else if (is_symbol_number(*symbol)) {
            check_number(after_left_square, &after_comma, &after_number, error);
        } else if (is_symbol_comma(*symbol)) {
            check_comma(&after_number, &after_comma, error);
        } else if (is_symbol_operator(*symbol)) {
            check_operator(after_left_square, &operator_amount, &after_operator, error);
        } else if (is_symbol_left_square_bracket(*symbol)) {
            check_left_square_bracket(circle_brackets_amount, &after_operator,
                                   &after_left_square, &square_in_circle, &set_amount,
                                   error);
        } else if (is_symbol_right_square_bracket(*symbol)) {
            check_right_square_bracket(after_comma, &after_left_square, error);
        } else if (is_symbol_left_circle_bracket(*symbol)) {
            check_left_circle_bracket(after_left_square, &square_in_circle,
                                   &circle_brackets_amount, error);
        } else if (is_symbol_right_circle_bracket(*symbol)) {
            check_right_circle_bracket(&circle_brackets_amount, &after_operator, error);
        } else {  // если запрещенный символ
            *error = ERROR_WRONG_SYMBOL;
        }
    }

    // Если одна из круглых скобок не закрылась
    if (circle_brackets_amount > 0) {
        *error = ERROR_NO_CIRCLE_END;
    }
    // Если одна из квадратных скобок не закрылась
    else if (after_left_square > 0) {
        *error = ERROR_NO_SQUERE_END;
    }
    // Операторов всегда на 1 меньше чем множеств(например [] U [] ^ [])
    // Если это не так, то значит входные данные не верны
    else if (set_amount - operator_amount != 1) {
        *error = ERROR_WRONG_NUMBER_OF_OPERATORS;
    }
    // Круглые скобки не содержут в себе множеств
    else if (square_in_circle == false) {
        *error = ERROR_NO_SQUERE_IN_CIRCLE;
    }
	
	if (*error != NO_ERROR) {
        return NULL;
    }
	
	char *ret = add_circle_brackets_to_string(string, size, error);
    return ret;
}

Numeric_set *apply_operation(char operator, Numeric_set *first, Numeric_set *second,
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
	
    Numeric_set *result = NULL;
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
        clear_set(first);
        clear_set(second);
    }
    }
    return result;
}

void get_answer(char *string) {
    assert(string != NULL);

    if (string == NULL) {
        return;
    }

    int strsize = strlen(string);

    int number = 0, begin = 0;

    int error = NO_ERROR;

    Numeric_set *first = NULL, *second = NULL;

    char operator= ' ';

    char *localBegin = NULL;

    int circleBrackets = 1;

    // Возможные операции
    char operations[] = "^U\\";

    while (circleBrackets > 0) {
        circleBrackets = 0;
        for (size_t i = 0; i < strsize; i++) {
            if (is_symbol_left_circle_bracket(string[i])) {
                circleBrackets++;
                begin = i;
            } else if (is_symbol_right_circle_bracket(string[i])) {
                for (char *currentOperator = operations; *currentOperator != '\0';
                        currentOperator++) {
                    for (char *letter = string + begin; error == NO_ERROR && letter < string + i; letter++) {
                        if (is_symbol_left_square_bracket(*letter)) {
                            first = create_set(&error);
                            number = 0;

                            if (!(*currentOperator == operator)) {
                                localBegin = letter;
                            }
                        } else if (is_symbol_right_square_bracket(*letter)) {
                            if (number != 0) {
                                error = push_to_set(first, number);
                            }
                            number = 0;
							
							if (error == NO_ERROR)
							{
								if (*currentOperator == operator) {
									second = apply_operation(operator, first, second, &error);
									if (error == NO_ERROR)
									{
										numeric_set_to_char(second, localBegin, letter);

										operator = ' ';
									}
								} else {
									if (second != NULL) {
										clear_set(second);
									}
									second = move_set(first);
								}
							}
                        } else if (is_symbol_comma(*letter)) {
                            push_to_set(first, number);
                            number = 0;
                        } else if (is_symbol_number(*letter)) {
                            number *= 10;
                            number += (*letter - '0');
                        } else if (is_symbol_space(*letter)) {
                        } else {
                            operator = *letter;
                        }
                    }
                }
                numeric_set_to_char(second, string + begin, string + i);
                break;
            }
        }
    }
    if (error != NO_ERROR) {
        printf("[error]");
    } else {
        print_set(second);
    }
    clear_set(second);
}

void printResult(int real_error, int my_error, char *string, char *name) {
    printf("\n %s", name);
    if (real_error == my_error)
        printf(" OK");
    else
        printf(" Failed. Found %d, but need %d", my_error, real_error);
}

void case_error1() {
    char *stroka = "(";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NO_CIRCLE_END, error, string, stroka);
}

void case_error2() {
    char *stroka = "()";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error3() {
    char *stroka = "[1,2,3,]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_COMMA_WITHOUT_NUMBER, error, string, stroka);
}

void case_error4() {
    char *stroka = "[1,,3]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_COMMA_NOT_AFTER_NUMBER, error, string, stroka);
}

void case_error5() {
    char *stroka = "[1]3";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NUMBER_NOT_IN_SET, error, string, stroka);
}

void case_error6() {
    char *stroka = "[1]()";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error7() {
    char *stroka = "()[1]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NO_SQUERE_IN_CIRCLE, error, string, stroka);
}

void case_error8() {
    char *stroka = "[[1]]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}

void case_error9() {
    char *stroka = "[[1] U]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}
void case_error10() {
    char *stroka = "[1,2,3,4";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_NO_SQUERE_END, error, string, stroka);
}
void case_error11() {
    char *stroka = "[1,2,3,4] U][";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_SQUERE_END_BEFORE_START, error, string, stroka);
}
void case_error12() {
    char *stroka = "([1,2,3,4] U))U([])";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_CIRCLE_END_BEFORE_START, error, string, stroka);
}
void case_error13() {
    char *stroka = "([1,2,3,4] U)[]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_OPERATOR_WITHOUT_SETS, error, string, stroka);
}
void case_error14() {
    char *stroka = "     ";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(ERROR_WRONG_NUMBER_OF_OPERATORS, error, string, stroka);
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

void case_right5() {
    char *stroka = "([]U([]\\([])^([] ^   ([] U []))))";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(NO_ERROR, error, string, stroka);
}
void case_right6() {
    char *stroka = "([1])";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(NO_ERROR, error, string, stroka);
}
void case_right7() {
    char *stroka = "(([1]))";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(NO_ERROR, error, string, stroka);
}
void case_right8() {
    char *stroka = "[]U[]";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(NO_ERROR, error, string, stroka);
}
void case_right9() {
    char *stroka = "([])U([])";
    int error = NO_ERROR;
    char *string = check_string(stroka, &error);
    printResult(NO_ERROR, error, string, stroka);
}

void right_tests() {
    printf("\n\n Right tests of check_string()\n");
    case_right5();
    case_right6();
    case_right7();
    case_right8();
    case_right9();
}

void unit_tests() {
    error_tests();
    right_tests();
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