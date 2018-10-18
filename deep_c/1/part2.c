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

typedef struct numeric_set_t {
    int *numbers;
    size_t max_size;
    size_t real_size;
} numeric_set_t;

// Обменять местами двух целочисленных значений
void swap(int *a, int *b);

// Печатать множество
void set_print(const numeric_set_t *ns);

// Создать множество
numeric_set_t *set_create(int *error);

// Очистить множество
void set_clear(numeric_set_t *ns);

// Добавить элемент в множество
int set_add(numeric_set_t *ns, int num);

// Добавить в множество to элементы из множества for
void set_add_from_set(const numeric_set_t *from, numeric_set_t *to, int *error);

// Получить пересечение множеств(с очисткой)
numeric_set_t *get_union_of_sets(numeric_set_t *A, numeric_set_t *B,
                                 int *error);

// Получить пересечение множеств(с очисткой)
numeric_set_t *get_intersection_of_sets(numeric_set_t *A, numeric_set_t *B,
                                        int *error);

// получить разность множеств(с очисткой)
numeric_set_t *get_subtraction_of_sets(numeric_set_t *A, numeric_set_t *B,
                                       int *error);

// Получить размер числа
size_t get_digits_count(int number);

// Вставить число в строку
void insert_number_in_string(int number, char *begin[]);

// Вставить множество в строку
void numeric_set_to_string(numeric_set_t *ns, char *begin, char *end);

// Проверить строку на корректность
int check_string(const char string[]);

// Перевыделяет память для строки и добавляет круглые скобки
// в начало и конец строки
// Возвращает код ошибки
int add_circle_brackets_to_string(char *string[]);

// Преобразует строку с несколькими множествами в строку с одним множеством
// В случае неудачи возвращает код ошибки, иначе NO_ERROR
int merge_all_sets_in_string_into_one(char string[]);

// Распечатать строку без пробелов
void print_without_spaces(const char string[]);

// Возвращает true, если удалось найти позиции скобок и false если нет
// В случае успеха left и right указывают на положение левой и правой скобок
// в строке.
// Подразумевается, что на вход приходит строка, в которой круглые скобки
// расставлены в правильном порядке и количество '(' и ')' совпадает

bool find_circle_brackets_position_in_string(char string[], char *left[],
        char *right[]);

void unit_tests();

int get_string(char **lineptr, size_t *n, int delimiter, FILE *stream);

// Подготовить строку к обработке
int prepare_string_for_processing(char *string[]);

// Обработать строку
void process_string(char string[]);

int main() {
// gcc -o main.exe part2.c -D _UNIT_TESTS
#ifdef _UNIT_TESTS
    unit_tests();
    return 0;
#endif
    char *buffer = NULL;
    size_t max_size = BUFFER_SIZE;
    while (get_string(&buffer, &max_size, '\n', stdin) == NO_ERROR) {
        int error = prepare_string_for_processing(&buffer);
        if (error == NO_ERROR) {
            process_string(buffer);
        } else {
            printf("[error]");
        }
        free(buffer);
        buffer = NULL;  // Чтобы при последующем считывании get_string
        // увидел, что строка пуста и необходимо выделить память
    }
    free(buffer);
    return 0;
}

int prepare_string_for_processing(char* string[]) {
	assert(string != NULL)
	
	if (string == NULL) {
		return WRONG_INPUT;
	}
	
	int error = check_string(*string);
	if (error == NO_ERROR) {
		error = add_circle_brackets_to_string(string);
	}
    return error;
}

void process_string(char string[])
{
	assert(string != NULL)
	
	if (string == NULL) {
		return WRONG_INPUT;
	}
	
	int error = merge_all_sets_in_string_into_one(string);
	if (error == NO_ERROR) {
		print_without_spaces(string);
	} else {
		printf("[error]");
	}
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

void set_print(const numeric_set_t *ns) {
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
    for (size_t i = 1; i < size; ++i) {
        printf(",%d", ns->numbers[i]);
    }
    printf("]");
}

numeric_set_t *set_create(int *error) {
    assert(error != NULL);

    if (error == NULL) {
        return NULL;
    }

    numeric_set_t *ns = (numeric_set_t *)calloc(1, sizeof(numeric_set_t));
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

void set_clear(numeric_set_t *ns) {
    if (ns != NULL) {
        free(ns->numbers);
        free(ns);
    }
}

int set_add(numeric_set_t *ns, int num) {
    assert(ns != NULL);
    assert(num > 0);

    if ((ns == NULL) || (num <= 0)) {
        return WRONG_INPUT;
    }

    // Если числовое множество не содержит элементов
    if (ns->real_size == 0) {
        ns->numbers[0] = num;
        ++ns->real_size;
        return NO_ERROR;
    }

    size_t rsize = ns->real_size;

    // Если числовое множество полностью заполнилось
    if (rsize >= ns->max_size) {
        ns->max_size *= 2;
        int *buffer = (int *)realloc(ns->numbers, ns->max_size * sizeof(int));
        if (buffer == NULL) {
            return MEMORY_ERROR;
        }
        ns->numbers = buffer;
    }
    // флаг существования в множестве элемента, который мы хотим вставить
    bool already_exist_in_set = false;
    for (size_t i = 0; i < rsize && !already_exist_in_set; i++) {
        if (num == ns->numbers[i]) {
            // Поскольку числовому множеству не нужны дубли
            // просто не будем их вставлять
            already_exist_in_set = true;
        }
        // Для удобства работы с числовым множеством, пусть оно будет
        // отсортированным по возрастанию
        if (num < ns->numbers[i]) {
            swap(&num, &(ns->numbers[i]));
        }
    }
    // Оставшийся элемент самый большой, помещаем его в конец
    if (!already_exist_in_set) {
        ns->numbers[ns->real_size++] = num;
    }
    return NO_ERROR;
}

void set_add_from_set(const numeric_set_t *from, numeric_set_t *to,
                      int *error) {
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
    for (size_t i = 0; (i < size) && (*error == NO_ERROR); ++i) {
        *error = set_add(to, from->numbers[i]);
    }
}

numeric_set_t *get_union_of_sets(numeric_set_t *A, numeric_set_t *B,
                                 int *error) {
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

    numeric_set_t *ret = set_create(error);
    if (*error != NO_ERROR) {
        return NULL;
    }
    set_add_from_set(A, ret, error);
    set_add_from_set(B, ret, error);
    set_clear(A);
    set_clear(B);
    return ret;
}

numeric_set_t *get_intersection_of_sets(numeric_set_t *A, numeric_set_t *B,
                                        int *error) {
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
    numeric_set_t *ret = set_create(error);

    size_t sizeA = A->real_size;
    size_t sizeB = B->real_size;
    size_t iA = 0;
    size_t iB = 0;

    while ((iA < sizeA) && (iB < sizeB) && (*error == NO_ERROR)) {
        if (A->numbers[iA] < B->numbers[iB]) {
            ++iA;
        } else if (A->numbers[iA] > B->numbers[iB]) {
            ++iB;
        } else {
            *error = set_add(ret, A->numbers[iA]);
            ++iB;
            ++iA;
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

numeric_set_t *get_subtraction_of_sets(numeric_set_t *A, numeric_set_t *B,
                                       int *error) {
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

    size_t sizeA = A->real_size;
    size_t sizeB = B->real_size;
    size_t iA = 0;
    size_t iB = 0;

    numeric_set_t *ret = set_create(error);

    while ((iA < sizeA) && (iB < sizeB) && (*error == NO_ERROR)) {
        if (A->numbers[iA] < B->numbers[iB]) {
            ++iA;
        } else if (A->numbers[iA] > B->numbers[iB]) {
            *error = set_add(ret, B->numbers[iB]);
            ++iB;
        } else {
            ++iB;
            ++iA;
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

size_t get_digits_count(int number) {
    size_t digits_count = 0;
    while (number != 0) {
        number /= 10;
        ++digits_count;
    }
    return digits_count;
}

void insert_number_in_string(int number, char *begin[]) {
    assert(begin != NULL);
    assert(*begin != NULL);

    if ((begin == NULL) || (*begin == NULL)) {
        return;
    }

    size_t size = get_digits_count(number);

    for (size_t i = size; i > 0; i--) {
        *((*begin) + i - 1) = number % 10 + '0';
        number /= 10;
    }

    *begin = *begin + size;
}

void numeric_set_to_string(numeric_set_t *ns, char *begin, char *end) {
    assert(ns != NULL);
    assert(begin != NULL);
    assert(end != NULL);

    if ((begin == NULL) || (end == NULL) || (ns == NULL)) {
        return;
    }

    *(begin++) = '[';
    *end = ']';

    int size = ns->real_size;
    for (int i = 0; i < size; ++i) {
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

void update_flags_after_left_circle_bracket(bool *square_in_circle,
        int *circle_brackets_amount) {
    // Последующие символы после '('
    *circle_brackets_amount = *circle_brackets_amount + 1;
    *square_in_circle = 0;  // Скобки не содержат множеств
}

// Получить размер строки string
size_t get_string_size(const char string[]) {
    assert(string != NULL);

    if (string == NULL) {
        return 0;
    }

    size_t size = 0;
    while (string[size] != '\0') {
        ++size;
    }
    return size;
}

int add_circle_brackets_to_string(char *string[]) {
    assert(string != NULL);
    assert(*string != NULL);

    if ((string == NULL) || (*string == NULL)) {
        return WRONG_INPUT;
    }

    size_t size = get_string_size(*string);

    char *tmp = realloc(*string, size + 3);
    if (!tmp) {
        return MEMORY_ERROR;
    }
    *string = tmp;

    for (size_t i = size; i > 0; i--) {
        (*string)[i] = (*string)[i - 1];
    }
    (*string)[0] = '(';
    (*string)[size + 1] = ')';
    (*string)[size + 2] = '\0';

    return NO_ERROR;
}

// Проверить строку string на корректность в рамках ТЗ
// Возвращает код ошибки
int check_string(const char string[]) {
    assert(string != NULL);

    if (string == NULL) {
        return WRONG_INPUT;
    }

    bool after_number = false;  // находится ли символ после числа
    bool after_operator = false;  // находится ли символ за оператором
    bool after_comma = false;  // находится ли символ за запятой

    bool after_left_square = false;  // после '['
    bool square_in_circle = true;  // Круглые скобки содержат в себе множества

    int circle_brackets_amount = 0;  // Количсетво круглых скобок
    int set_amount = 0;              //количество множеств
    int operator_amount = 0;         //количество операторов

    int error = NO_ERROR;

    // Именованные константы
    const char symbol_space = ' ';
    const char symbol_comma = ',';
    const char symbol_left_square_bracket = '[';
    const char symbol_right_square_bracket = ']';
    const char symbol_left_circle_bracket = '(';
    const char symbol_right_circle_bracket = ')';

    for (const char *symbol = string; *symbol != '\0' && error == NO_ERROR;
            ++symbol) {
        if (*symbol == symbol_space) {
        } else if (is_symbol_number(*symbol)) {
            // Если число вне скобок
            if (after_left_square == false) {
                error = ERROR_NUMBER_NOT_IN_SET;
            }
			after_comma = false;  // следующий символ  не находится за запятой
			after_number = true;  // следующий символ находится за числом
        } else if (*symbol == symbol_comma) {
            // Если запятая не идет после числа
            if (after_number == false) {
                error = ERROR_COMMA_NOT_AFTER_NUMBER;
            }
            after_number = false;  // следующий символ не находится за числом
			after_comma = true;  // следующий символ находится за запятой
        } else if (is_symbol_operator(*symbol)) {
            // Если оператор идет после '['
            if (after_left_square == true) {
                error = ERROR_OPERATOR_IN_SET;
            }
            after_operator = true;
            ++operator_amount;
        } else if (*symbol == symbol_left_square_bracket) {
            // Если множество не за оператором
            if (after_operator == false && set_amount != 0) {
                error = ERROR_OPERATOR_WITHOUT_SETS;
            }
            after_left_square = true;  // Последующие символы после [
			++set_amount;  // Инкремент количества множеств
			square_in_circle = true;  // Обнаружено множество в скобках
			after_operator = false;
        } else if (*symbol == symbol_right_square_bracket) {
            // Если конец множества идёт сразу после запятой
            if (after_comma == true) {
                error = ERROR_COMMA_WITHOUT_NUMBER;
            }
            // Если ']' идет первее '['
            if (after_left_square == false) {
                error = ERROR_SQUERE_END_BEFORE_START;
            }
            after_left_square = false;
        } else if (*symbol == symbol_left_circle_bracket) {
            // Если круглая скобка внутри квадратной
            if (after_left_square == 1) {
                error = ERROR_CIRCLE_IN_SQUERE;
            }
			++circle_brackets_amount;
			square_in_circle = false;  // Скобки не содержат множеств
        } else if (*symbol == symbol_right_circle_bracket) {
            // Количество скобок уменьшилось
            --circle_brackets_amount;
            // Если ')' идет первее '('
            if (circle_brackets_amount < 0) {
                error = ERROR_CIRCLE_END_BEFORE_START;
            }
            // Если еще не было обьявлено ни одного множества
            if (set_amount == 0) {
                error = ERROR_NO_SQUERE_IN_CIRCLE;
            }
            after_operator = false;  // Следующее множество не за оператором
        } else {  // если запрещенный символ
            error = ERROR_WRONG_SYMBOL;
        }
    }
    if (error != NO_ERROR) {
        return error;
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

// Выполнить операцию operator над множествами first и second
// Возвращает указатель на результирующее множество или NULL с кодом ошибки
// в *error в случае, если что то пошло не по плану
numeric_set_t *apply_operation(char operator, numeric_set_t *first,
                               numeric_set_t *second, int *error) {
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

    numeric_set_t *result = NULL;
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

// Произвести соответсвеющие оператору current_operator операции над
// множествами в строке от begin до end.
// Возвращает код ошибки
int calculate_sets_in_string(char *begin, char *end, char current_operator) {
    assert(begin != NULL);
    assert(end != NULL);

    if ((begin == NULL) || (end == NULL)) {
        return WRONG_INPUT;
    }

    numeric_set_t *first = NULL, *second = NULL;

    char *localBegin = NULL;
    char operator= ' ';

    int error = NO_ERROR;

    int number = 0;

    // Именованные константы
    const char symbol_space = ' ';
    const char symbol_comma = ',';
    const char symbol_left_square_bracket = '[';
    const char symbol_right_square_bracket = ']';

    for (char *letter = begin; error == NO_ERROR && letter < end; ++letter) {
        // Если встретили '[', создаем множество
        if (*letter == symbol_left_square_bracket) {
            first = set_create(&error);
            number = 0;

            // Определяем, что перед нами множество, стоящее слева от
            // оператора
            if (!(operator== current_operator)) {
                localBegin = letter;
            }
        } else if (*letter == symbol_right_square_bracket) {
            // Если встретили ']', добавляем последнее найденное число
            if (number != 0) {
                error = set_add(first, number);
            }
            number = 0;

            if (error == NO_ERROR) {
                // Если перед данным множеством стоял оператор, соответсвующий
                // актуальному current_operator (с точки зрения приоритета выполнения)
                if (operator== current_operator) {
                    second = apply_operation(operator, first, second, &error);
                    if (error == NO_ERROR) {
                        // Перезаписываем ту часть строки, где стояли 2 множества
                        // с оператором в результирующее множество, например
                        // '[1,2]U[3]' -> '[1,2,3  ]'
                        numeric_set_to_string(second, localBegin, letter);

                        operator= ' ';
                    }
                } else {
                    // Если оператор не соответсвует приоритету, то сотрём
                    // записанное множество. Если множество было первым, то
                    // стирать нечего, поэтому проверяем second на NULL
                    if (second != NULL) {
                        set_clear(second);
                    }
                    // second указывает на множество, которое левее оператора
                    // first же всегда указывает на последнее найденное множество
                    second = first;
                    first = NULL;
                }
            }
        } else if (*letter == symbol_comma) {
            // Если встретили запятую, значит до этого было число,
            // которое надо записать в множество
            set_add(first, number);
            number = 0;
        } else if (is_symbol_number(*letter)) {
            // найденную цифру помещаем в число
            number *= 10;
            number += (*letter - '0');
        } else if (*letter == symbol_space) {
            // Пробелы пропускаем
        } else {
            operator= *letter;
        }
    }
    set_clear(second);
    return error;
}

void print_without_spaces(const char string[]) {
    assert(string != NULL);
    if (string == NULL) {
        printf("[error]");
    }

    size_t size = get_string_size(string);
    for (size_t i = 0; i < size; ++i) {
        if (string[i] != ' ') {
            printf("%c", string[i]);
        }
    }
}

// Возвращает true, если удалось найти позиции скобок и false если нет
// В случае успеха left и right указывают на положение левой и правой скобок
// в строке.
// Подразумевается, что на вход приходит строка, в которой круглые скобки
// расставлены в правильном порядке и количество '(' и ')' совпадает
bool find_circle_brackets_position_in_string(char string[], char **left,
        char **right) {
    assert(string != NULL);
    assert(left != NULL);
    assert(right != NULL);

    if ((string == NULL) || (left == NULL) || (right == NULL)) {
        return false;
    }

    *left = NULL;
    *right = NULL;

    const char left_circle = '(';
    const char right_circle = ')';
    for (char *letter = string;  // пока right не найдено
            *letter != '\0' && *right == NULL; ++letter) {
        if (*letter == left_circle) {
            *left = letter;
        } else if (*letter == right_circle) {
            *right = letter;
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

    int error = NO_ERROR;

    // Возможные операции
    char operations[] = "^U\\";

    char *left_circle, *right_circle;

    // Пока строка содержит скобки
    while (find_circle_brackets_position_in_string(string, &left_circle,
            &right_circle)) {
        // Цикл по строке из символов - операторов.
        // В строку они записаны в порядке понижения приоритета
        for (char *currentOperator = operations;
                *currentOperator != '\0' && error == NO_ERROR; ++currentOperator) {
            error =
                calculate_sets_in_string(left_circle, right_circle, *currentOperator);
        }
        // Заменяем скобки на пробелы, чтобы второй раз не обрабатывать
        // содержимое уже пройденных скобок
        *left_circle = ' ';
        *right_circle = ' ';
    }

    return error;
}

// Печать результатов юнит-тестов
void print_result_of_unit_test(int real_error, int my_error, char name[]) {
    printf("\n %s", name);
    if (real_error == my_error) {
        printf(" OK");
    } else {
        printf(" Failed. Found %d, but need %d", my_error, real_error);
    }
}

void case_error_one_circle_bracket() {
    char string[] = "(";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NO_CIRCLE_END, error, string);
}

void case_error_empty_circle_brackets() {
    char string[] = "()";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error_extra_comma_in_set() {
    char string[] = "[1,2,3,]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_COMMA_WITHOUT_NUMBER, error, string);
}

void case_error_no_number_between_two_comma() {
    char string[] = "[1,,3]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_COMMA_NOT_AFTER_NUMBER, error, string);
}

void case_error_number_outside_set() {
    char string[] = "[1]3";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NUMBER_NOT_IN_SET, error, string);
}

void case_error_circle_brackets_outside_set_right() {
    char string[] = "[1]()";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error_circle_brackets_outside_set_left() {
    char string[] = "()[1]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NO_SQUERE_IN_CIRCLE, error, string);
}

void case_error_set_in_set() {
    char string[] = "[[1]]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}

void case_operator_without_numbers() {
    char string[] = "[[1] U]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}

void case_operator_between_numbers() {
    char string[] = "[U 1 U]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_OPERATOR_IN_SET, error, string);
}

void case_set_has_no_end() {
    char string[] = "[1,2,3,4";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_NO_SQUERE_END, error, string);
}

void case_set_has_no_start() {
    char string[] = "   ]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_SQUERE_END_BEFORE_START, error, string);
}

void case_error_incorrect_set() {
    char string[] = "[1,2,3,4] U][";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_SQUERE_END_BEFORE_START, error, string);
}
void case_error_incorrect_operator() {
    char string[] = "([1,2,3,4] U))U([])";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_CIRCLE_END_BEFORE_START, error, string);
}
void case_error_incorrect_operator_2() {
    char string[] = "([1,2,3,4] U)[]";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_OPERATOR_WITHOUT_SETS, error, string);
}
void case_error_empty_string() {
    char string[] = "     ";
    int error = check_string(string);
    print_result_of_unit_test(ERROR_WRONG_NUMBER_OF_OPERATORS, error, string);
}

void error_tests() {
    printf("\n\n Error tests of check_string()\n");
    case_error_one_circle_bracket();
    case_error_empty_circle_brackets();
    case_error_extra_comma_in_set();
    case_error_no_number_between_two_comma();
    case_error_number_outside_set();
    case_error_circle_brackets_outside_set_right();
    case_error_circle_brackets_outside_set_left();
    case_error_set_in_set();
    case_operator_without_numbers();
    case_operator_between_numbers();
    case_set_has_no_end();
    case_set_has_no_start();
    case_error_incorrect_set();
    case_error_incorrect_operator();
    case_error_incorrect_operator_2();
    case_error_empty_string();
}

void case_right_two_usual_sets() {
    char string[] = "([3,4] U [2])";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}

void case_right_six_empty_sets() {
    char string[] = "([]U([]\\([])^([] ^   ([] U []))))";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right_set_in_circle_brackets() {
    char string[] = "([1])";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right_set_in_circle_brackets_2_times() {
    char string[] = "(([1]))";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right_two_empty_sets() {
    char string[] = "[]U[]";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}
void case_right_two_empty_sets_in_brackets() {
    char string[] = "([])U([])";
    int error = check_string(string);
    print_result_of_unit_test(NO_ERROR, error, string);
}

void right_tests() {
    printf("\n\n Right tests of check_string()\n");
    case_right_two_usual_sets();
    case_right_six_empty_sets();
    case_right_set_in_circle_brackets();
    case_right_set_in_circle_brackets_2_times();
    case_right_two_empty_sets();
    case_right_two_empty_sets_in_brackets();
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
        ++pb;
        ++count;
    }

    if (count == 0) {
        return EOF;
    }

    *pb = '\0';

    return NO_ERROR;
}