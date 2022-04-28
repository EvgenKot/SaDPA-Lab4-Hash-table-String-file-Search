#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

/*
Ращупкин Евгений КЭ-203
28 вариант

Алгоритмы хеширования данных
2 задание
Постройте хеш-таблицу из слов произвольного текстового файла, задав ее размерность с экрана.
Выведите построенную таблицу слов на экран. Осуществите поиск введенного слова.
Выполните программу для различных размерностей таблицы и сравните количество сравнений.
Удалите все слова, начинающиеся на указанную букву, выведите таблицу.
*/

//Узел очереди
typedef struct Node
{
    char *text;
    int amount;
    struct Node *next; //На случай колизии
} Node;

//Очередь
typedef struct Queue
{
    Node *head;
    Node *tail;

    size_t size;
} Queue;

typedef struct Table
{
    Queue **table;
    size_t size;
} Table;

//Выводит элементы очереди
void QueuePrint(Queue *q)
{
    Node *current = q->head;
    while (current)
    {
        printf("%s ", current->text);
        current = current->next;
    }
    printf(" <");
    printf("\n");
}

//Вывод количества каждого элемента
void QueueAmountPrint(Queue *q)
{
    Node *current = q->head;
    while (current)
    {
        printf("%5d ", current->amount);
        current = current->next;
    }
    printf("\n");
}

//Ищет узел со значением
Node *QueueSearch(Queue *q, char *data)
{
    Node *current = q->head;
    while (current)
    {
        if (!(strcmp(current->text, data)))
            return current;
        current = current->next;
    }
    return NULL;
}

//Ищет узел со значением и возвращает номер
Node *QueueSearchNumber(Queue *q, char *data, int *n)
{
    Node *current = q->head;
    *n = 0;
    while (current)
    {
        *n = *n + 1;
        if (!(strcmp(current->text, data)))
            return current;
        current = current->next;
    }
    return NULL;
}

//Добавляет элемент в конец очереди
void QueuePushBack(Queue *q, char *data)
{
    Node *search = QueueSearch(q, data);
    if (!search)
    {
        q->size++;
        Node *newNode = (Node *)malloc(sizeof(Node));

        newNode->text = data;
        newNode->next = NULL;

        if (q->head == NULL)
            q->head = newNode;
        else
            q->tail->next = newNode;
        q->tail = newNode;
    }
    else
        search->amount = search->amount + 1;
}

Node *QueueNodeGetPrevious(Queue *q, Node *target)
{
    Node *current = q->head;
    while (current)
    {
        if (current->next == target)
            return current;
        current = current->next;
    }
    printf("No preveus\n");
    return NULL;
}

//Удалить первый элемент
void QueuePop(Queue *q)
{
    if (q->head == NULL)
    {
        printf("Deleting NULL element error\n");
        exit(-1);
    }
    q->size--;
    Node *target = NULL;
    target = q->head;
    free(target->text);
    q->head = q->head->next;
    free(target);
    return;
}

//Удалить последний элемент
void QueuePopBack(Queue *q)
{
    if (q->tail == NULL)
    {
        printf("Deleting NULL element error\n");
        exit(-1);
    }
    q->size--;
    Node *target;
    target = q->tail;
    Node *previous;
    previous = QueueNodeGetPrevious(q, q->tail);

    free(target->text);
    free(target);
    previous->next = NULL;
    return;
}

//Удаление произвольного элемента
void QueueNodeDelete(Queue *q, Node *target)
{
    if (q->head == target)
    {
        printf("Deleting head\n");
        QueuePop(q);
        return;
    }

    else if (q->tail == target)
    {
        printf("Deleting tail\n");
        QueuePopBack(q);
        return;
    }

    else if (target)
    {
        q->size--;
        Node *previous = QueueNodeGetPrevious(q, target);
        previous->next = target->next;
        free(target->text);
        free(target);
        return;
    }
    printf("No element to delete\n");
    return;
}

//Ищет узел со словом по первой букве
Node *QueueNodeSearchChar(Queue *q, char data)
{
    Node *current = q->head;
    while (current)
    {
        if (current->text[0] == data)
            return current;
        current = current->next;
    }
    return NULL;
}

//Отчистка очереди
void QueueFree(Queue *q)
{
    if (q->head)
        while (q->size != 0)
            QueuePop(q);
}

//Создаёт хеш для числа
int HashGenerate(char data, int size)
{
    return data % size;
}

//Создание хеш-таблицы
Table *HashTableCreate(int size)
{

    Table *t = malloc(sizeof(Table));
    t->table = malloc(size * sizeof(*(t->table)));

    t->size = size;

    for (int i = 0; i < size; i++)
    {
        t->table[i] = malloc(sizeof(*(t->table[i])));
        t->table[i]->head = NULL;
        t->table[i]->tail = t->table[i]->head;
        t->table[i]->size = 0;
    }

    return t;
}

//Вывод хеш-таблицы
void HashTablePrint(Table *t)
{
    printf("Hash-table\n");
    printf("Size = %d\n", t->size);
    printf(" hash  qua\n");
    for (int i = 0; i < t->size; i++)
    {
        printf("|%4d|%4d|", i, t->table[i]->size);
        QueuePrint(t->table[i]);
        // printf("     amount");
        // QueueAmountPrint(t->table[i]);
    }
}

//Добавление элемента в хеш-таблицу
void HashTableAdd(Table *t, char *data)
{
    int index = HashGenerate(data[0], t->size);
    QueuePushBack(t->table[index], data);
}

//Поиск элемента в хеш-таблице
void HashTableSearchPrint(Table *t, char *data)
{
    int position;
    int index = HashGenerate(data[0], t->size);
    Node *n = QueueSearchNumber(t->table[index], data, &position);
    if (n)
    {
        printf("|%4d|%4d|", index, t->table[index]->size);
        QueuePrint(t->table[index]);
        // printf("     amount");
        //  QueueAmountPrint(t->table[index]);
        //  printf("               ");
        //  for (int i = position; i > 1; i--)
        //      printf("      ");
        //  printf("^\n");
        printf("%d steps \n", position);
        return;
    }
    else
        printf("No element.\n");
}

//Поиск узла в хеш таблице
Node *HashTableNodeSearch(Table *t, Queue **q, char *data)
{
    int index = HashGenerate(data[0], t->size);
    *q = t->table[index];
    Node *n = QueueSearch(*q, data);
    return n;
}

//Отчистка памяти
void HashTableFree(Table *t)
{
    printf("Hash table free: ");
    for (int i = 0; i < t->size; i++)
    {
        if (t->table[i])
        {
            QueueFree(t->table[i]);
            free(t->table[i]);
        }
    }
    free(t->table);
    free(t);
    printf("Completed.\n");
}

//Закрытие файла
int FileClose(FILE **file)
{
    printf("Closing a file: ");
    if (fclose(*file) == EOF)
    {
        printf("ERROR\n");
        return -1;
    }

    else
    {
        printf("completed.\n");
        return 1;
    }
}

//Чтение строки из файла
char *FileStringGet(FILE *file)
{
    int len = 0;                                       // изначально строка пуста
    int capacity = 8;                                  // ёмкость контейнера динамической строки (1, так как точно будет '\0')
    char *s = (char *)malloc(capacity * sizeof(char)); // динамическая пустая строка

    char c = fgetc(file); // символ для чтения данных

    // читаем символы, пока не получим один из символов конца строки
    while (!(c == ' ' || c == '\0' || c == EOF))
    {
        s[(len)++] = c; // заносим в строку новый символ

        // если реальный размер больше размера контейнера, то увеличим его размер
        if (len >= capacity)
        {
            capacity = capacity * 2;                         // увеличиваем ёмкость строки на 1
            s = (char *)realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью
        }
        c = fgetc(file); // считываем следующий символ
    }

    s[len] = '\0'; // завершаем строку символом конца строки

    return s; // возвращаем указатель на считанную строку
}

//Перевод строки в очередь
void *FileStringHashTable(FILE *file, Table *t)
{
    char *c;
    while (!feof(file)) //Пока файл не закончился
    {
        c = FileStringGet(file);
        HashTableAdd(t, c);
    }
}

void Menu()
{
    printf("/=====================/\n");
    printf("         Menu\n");
    printf("/=====================/\n");
    printf("1. Print hash-table\n");
    printf("2. Search\n");
    printf("3. Search and delete\n");
    printf("4. Delete by char\n");
    printf("0. Exit\n");
    printf(">>");
}

int main()
{
    //Создание хеш-таблицы
    int table_size;
    printf("Enter size of hash table:\n>>");
    scanf("%d", &table_size);
    printf("Creating table size %d\n", table_size);
    Table *t = HashTableCreate(table_size);
    if (!t)
    {
        printf("ERROR\n");
        return 1;
    }
    printf("Hash table created.\n");

    printf("Openning file: ");
    FILE *file = fopen("input.txt", "r");
    if (!file)
    {
        printf("ERROR");
        return 0;
    }
    printf("Completed.\n");

    printf("File to hash table: \n");
    FileStringHashTable(file, t);
    printf("Completed.\n");

    FileClose(&file);

    HashTablePrint(t);

    char str[80];
    Node *target;
    Queue *queue;
    int choice = 1;
    while (choice)
    {
        getch();
        system("cls");
        Menu();
        scanf("%d", &choice);
        switch (choice)
        {
        case 1: // Print hash-table
            HashTablePrint(t);
            break;

        case 2: // Search by string
            printf("Enter a string (max len 100):\n>>");
            scanf("%s", str);
            HashTableSearchPrint(t, str);
            break;

        case 3: // Search and delete
            printf("Enter a string (max len 100):\n>>");
            scanf("%s", str);
            target = HashTableNodeSearch(t, &queue, str);
            if (!target)
            {
                printf("Nothing to delete\n");
                break;
            }
            printf("Deleting %s\n", target->text);
            QueuePrint(queue);
            QueueNodeDelete(queue, target);
            QueuePrint(queue);
            break;

        case 4: // Delete by char
            printf("Enter a char\n>>");
            scanf("%s", str);
            printf("You wrote %c\n", str[0]);
            printf("Deleting: ");
            int hash = HashGenerate(str[0], t->size);
            target = QueueNodeSearchChar(t->table[hash], str[0]);
            while (target)
            {
                QueueNodeDelete(t->table[hash], target);
                target = QueueNodeSearchChar(t->table[hash], str[0]);
            }
            printf("Completed.\n");
            break;

        case 0: // Exit
            printf("Stop...\n");
            HashTableFree(t);
            printf("Stopped.\n");
            break;

        default: //Ошибка ввода
            printf("Invalid input\n");
            HashTableFree(t);
            exit(1);
            break;
        }
    }

    return 0;
}