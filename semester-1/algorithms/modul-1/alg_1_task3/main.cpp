/*
В первой строке количество команд n. n ≤ 1000000.

Каждая команда задаётся как 2 целых числа: a b.

a = 2 - pop front
a = 3 - push back

Если дана команда pop front, то число b - ожидаемое значение.
 Если команда pop front вызвана для пустой структуры данных,
 то ожидается “-1”.
*/

#include <stdlib.h>
#include <iostream>

#define BUFFER_SIZE 64

#define MEMORY_ERROR -1;

class stack {
private:
    int *buffer;
    size_t real_size;
    size_t max_size;

    void grow();

public:
    stack();
    ~stack() {}

    void clear();
    void push(int number);
    int pop();
    bool is_empty() const;
};

class Queue {
private:
    stack *input;
    stack *output;

public:
    Queue() {
        input = new stack();
        output = new stack();
    }
    ~Queue() {
        delete input;
        delete output;
    }
    void push_back(int number);

    int pop_front();
};

int main() {
    size_t amount_of_operations = 0;
    std::cin >> amount_of_operations;
    int operation = 0, number = 0;
    bool is_work = true;
    Queue *queue = new Queue();
    for (size_t i = 0; i < amount_of_operations && is_work; i++) {
        std::cin >> operation >> number;
        if (operation == 3) {
            queue->push_back(number);
        } else if (operation == 2) {
            if (queue->pop_front() != number) {
                is_work = false;
            }
        } else {
            is_work = false;
        }
    }

    if (is_work)
        std::cout << "YES";
    else
        std::cout << "NO";

    delete queue;
    return 0;
}

void stack::grow() {
    max_size *= 2;
    int *tmp = nullptr;
    try {
        tmp = static_cast<int *>(realloc(buffer, max_size * sizeof(int)));
        if (!tmp) {
            delete buffer;
            throw MEMORY_ERROR;
        }
        buffer = tmp;
    } catch (int error) {
        std::cout << "NO";
    }
}

stack::stack() {
    real_size = 0;
    max_size = BUFFER_SIZE;
    buffer = nullptr;
    try {
        buffer = static_cast<int *>(malloc(max_size * sizeof(int)));
        if (!buffer) {
            throw MEMORY_ERROR;
        }
    } catch (int error) {
        std::cout << "NO";
    }
}

void stack::push(int number) {
    if (real_size >= max_size) {
        grow();
    }
    buffer[real_size++] = number;
}

int stack::pop() {
    if (real_size == 0) {
        return -1;
    }
    return buffer[--real_size];
}

bool stack::is_empty() const {
    return real_size == 0;
}

void Queue::push_back(int number) {
    input->push(number);
}

int Queue::pop_front() {
    if (output->is_empty())
        while (!input->is_empty()) {
            output->push(input->pop());
        }
    return output->pop();
}
