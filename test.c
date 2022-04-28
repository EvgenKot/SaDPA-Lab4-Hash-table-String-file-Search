#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node
{
    char *text;
    int amount;
    struct Node *next; //На случай колизии
} Node;

typedef struct Queue
{
    Node *head;
    Node *tail;

    size_t size;
} Queue;

void QueuePrint(Queue *q)
{
    Node *current = q->head;
    while (current)
    {
        printf("%s->", current->text);
        current = current->next;
    }
    printf(" <");
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

//Ввод строки
char *StringGet(int *len)
{
    *len = 0;                                          // изначально строка пуста
    int capacity = 8;                                  // ёмкость контейнера динамической строки (1, так как точно будет '\0')
    char *s = (char *)malloc(capacity * sizeof(char)); // динамическая пустая строка

    char c = getchar(); // символ для чтения данных

    // читаем символы, пока не получим символ переноса строки (\n)
    while (c != '\n')
    {
        s[(*len)++] = c; // заносим в строку новый символ

        // если реальный размер больше размера контейнера, то увеличим его размер
        if (*len >= capacity)
        {
            capacity = capacity * 2;                         // увеличиваем ёмкость строки на 1
            s = (char *)realloc(s, capacity * sizeof(char)); // создаём новую строку с увеличенной ёмкостью
        }
        c = getchar(); // считываем следующий символ
    }

    s[*len] = '\0'; // завершаем строку символом конца строки

    return s; // возвращаем указатель на считанную строку
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
void *FileStringQueue(FILE *file, Queue *q)
{
    char *c;
    while (!feof(file)) //Пока файл не закончился
    {
        c = FileStringGet(file);
        printf("%s added\n", c);
        printf("%c hasha %d\n", c[0], c[0]);
        QueuePushBack(q, c);
    }
}

int main()
{

    Queue *q = malloc(sizeof(Queue));
    q->head = NULL;
    q->tail = q->head;
    q->size = 0;

    /*
    int quanity;
    printf("Quanity:");
    scanf("%d", &quanity);
    getchar();
    */

    char *text;

    printf("Openning file...\n");
    FILE *file = fopen("input.txt", "r");
    if (!file);
    {
        printf("ERROR");
        exit(0);
    }
    printf("Completed.\n");
        
    /*
    for (int i = 0; i < 15; i++)
    {
        text = FileStringGet(file);

        QueuePushBack(q, text);
    }
    */

    FileStringQueue(file, q);

    QueuePrint(q);

    fclose(file);
    getchar();
    return 1;
}