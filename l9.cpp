#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <windows.h>

typedef struct {
    int* data;      
    int front;      
    int rear;     
    int capacity;   
    int size;       
} Queue;

Queue* createQueue(int capacity) {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->data = (int*)malloc(capacity * sizeof(int));
    q->front = 0;
    q->rear = -1;
    q->size = 0;
    q->capacity = capacity;
    return q;
}

int isQueueEmpty(Queue* q) {
    return q->size == 0;
}

void enqueue(Queue* q, int value) {
    if (q->size == q->capacity) {
        printf("Очередь переполнена!\n");
        return;
    }
    q->rear = (q->rear + 1) % q->capacity;
    q->data[q->rear] = value;
    q->size++;
}

int dequeue(Queue* q) {
    if (isQueueEmpty(q)) {
        printf("Очередь пуста!\n");
        return -1;
    }
    int value = q->data[q->front];
    q->front = (q->front + 1) % q->capacity;
    q->size--;
    return value;
}

void freeQueue(Queue* q) {
    free(q->data);
    free(q);
}

typedef struct Node {
    int vertex;
    struct Node* next;
} Node;

Node* createNode(int v) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->vertex = v;
    newNode->next = NULL;
    return newNode;
}

int** generateAdjacencyMatrix(int n) {
    int** matrix = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        matrix[i] = (int*)malloc(n * sizeof(int));
        for (int j = 0; j < n; j++) {
            matrix[i][j] = 0;
        }
    }

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            int random_value = rand() % 2;
            matrix[i][j] = random_value;
            matrix[j][i] = random_value; 
        }
    }

    return matrix;
}

void printMatrix(int** matrix, int n) {
    printf("\nМатрица смежности:\n");

    printf("   |");
    for (int i = 0; i < n; i++) {
        printf(" %2d", i);
    }
    printf("\n");

    printf("----");
    for (int i = 0; i < n; i++) {
        printf("---");
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("%2d |", i); 
        for (int j = 0; j < n; j++) {
            printf(" %2d", matrix[i][j]);
        }
        printf("\n");
    }
}

Node** createAdjacencyList(int** matrix, int n) {
    Node** adjList = (Node**)malloc(n * sizeof(Node*));

    for (int i = 0; i < n; i++) {
        adjList[i] = NULL;
        for (int j = 0; j < n; j++) {
            if (matrix[i][j] == 1) {
                Node* newNode = createNode(j);
                newNode->next = adjList[i];
                adjList[i] = newNode;
            }
        }
    }

    return adjList;
}

void printAdjacencyList(Node** adjList, int n) {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < n; i++) {
        printf("Вершина %d: ", i);
        Node* temp = adjList[i];
        while (temp != NULL) {
            printf("%d", temp->vertex);
            temp = temp->next;
            if (temp != NULL) {
                printf("; ");
            }
        }
        printf("\n");
    }
}

int* BFSD_matrix(int** matrix, int n, int startVertex, int* operationCount, double* timeSpent) {
    int* DIST = (int*)malloc(n * sizeof(int));
    *operationCount = 0;
    for (int i = 0; i < n; i++) {
        DIST[i] = -1;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    Queue* q = createQueue(n);

    enqueue(q, startVertex);
    DIST[startVertex] = 0;
    (*operationCount)++;

    printf("\nПорядок обхода вершин (матрица): ");

    while (!isQueueEmpty(q)) {
        int current = dequeue(q);
        printf("%d ", current);

        for (int i = 0; i < n; i++) {
            (*operationCount)++;
            if (matrix[current][i] == 1 && DIST[i] == -1) {
                enqueue(q, i);
                DIST[i] = DIST[current] + 1;
                (*operationCount) += 2;
            }
        }
    }
    printf("\n");

    QueryPerformanceCounter(&end);
    *timeSpent = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    freeQueue(q);
    return DIST;
}

// для списков смежности
int* BFSD_adjList(Node** adjList, int n, int startVertex, int* operationCount, double* timeSpent) {
    int* DIST = (int*)malloc(n * sizeof(int));
    *operationCount = 0;
    for (int i = 0; i < n; i++) {
        DIST[i] = -1;
    }

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    Queue* q = createQueue(n);

    enqueue(q, startVertex);
    DIST[startVertex] = 0;
    (*operationCount)++;

    printf("\nПорядок обхода вершин (списки): ");

    while (!isQueueEmpty(q)) {
        int current = dequeue(q);
        printf("%d ", current);
        Node* temp = adjList[current];
        while (temp != NULL) {
            (*operationCount)++;
            int neighbor = temp->vertex;
            if (DIST[neighbor] == -1) {
                enqueue(q, neighbor);
                DIST[neighbor] = DIST[current] + 1;
                (*operationCount) += 2;
            }
            temp = temp->next;
        }
    }
    printf("\n");

    QueryPerformanceCounter(&end);
    *timeSpent = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    freeQueue(q);
    return DIST;
}
void DFSD_recursive(int** matrix, int n, int current, int* DIST, int* visited, int* operationCount) {
    visited[current] = 1;
    (*operationCount)++;
    printf("%d ", current);

    for (int i = 0; i < n; i++) {
        (*operationCount)++;
        if (matrix[current][i] == 1 && !visited[i]) {
            if (DIST[i] == -1 || DIST[current] + 1 < DIST[i]) {
                DIST[i] = DIST[current] + 1;
                (*operationCount)++;
            }
            DFSD_recursive(matrix, n, i, DIST, visited, operationCount);
        }
    }
}
int* DFSD_matrix(int** matrix, int n, int startVertex, int* operationCount, double* timeSpent) {
    int* DIST = (int*)malloc(n * sizeof(int));
    int* visited = (int*)malloc(n * sizeof(int));
    *operationCount = 0;
    for (int i = 0; i < n; i++) {
        DIST[i] = -1;
        visited[i] = 0;
    }

    DIST[startVertex] = 0;

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    printf("\nПорядок обхода вершин (DFS): ");
    DFSD_recursive(matrix, n, startVertex, DIST, visited, operationCount);

    QueryPerformanceCounter(&end);
    *timeSpent = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    free(visited);
    return DIST;
}
void DFSD_recursive_adjList(Node** adjList, int n, int current, int* DIST, int* visited, int* operationCount) {
    visited[current] = 1;
    (*operationCount)++;
    printf("%d ", current);

    Node* temp = adjList[current];
    while (temp != NULL) {
        (*operationCount)++;
        int neighbor = temp->vertex;
        if (!visited[neighbor]) {
            if (DIST[neighbor] == -1 || DIST[current] + 1 < DIST[neighbor]) {
                DIST[neighbor] = DIST[current] + 1;
                (*operationCount)++;
            }
            DFSD_recursive_adjList(adjList, n, neighbor, DIST, visited, operationCount);
        }
        temp = temp->next;
    }
}

int* DFSD_adjList(Node** adjList, int n, int startVertex, int* operationCount, double* timeSpent) {
    int* DIST = (int*)malloc(n * sizeof(int));
    int* visited = (int*)malloc(n * sizeof(int));
    *operationCount = 0;

    for (int i = 0; i < n; i++) {
        DIST[i] = -1;
        visited[i] = 0;
    }

    DIST[startVertex] = 0;

    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    printf("\nПорядок обхода вершин (DFS списки): ");
    DFSD_recursive_adjList(adjList, n, startVertex, DIST, visited, operationCount);

    QueryPerformanceCounter(&end);
    *timeSpent = (double)(end.QuadPart - start.QuadPart) * 1000.0 / frequency.QuadPart;

    free(visited);
    return DIST;
}
void printDistances(int* DIST, int n, int startVertex) {
    printf("\nРасстояния от вершины %d:\n", startVertex);
    for (int i = 0; i < n; i++) {
        if (DIST[i] == -1) {
            printf("Вершина %d: недостижима\n", i);
        }
        else {
            printf("Вершина %d: расстояние %d\n", i, DIST[i]);
        }
    }
}

void freeMatrix(int** matrix, int n) {
    for (int i = 0; i < n; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
void freeAdjList(Node** adjList, int n) {
    for (int i = 0; i < n; i++) {
        Node* temp = adjList[i];
        while (temp != NULL) {
            Node* toDelete = temp;
            temp = temp->next;
            free(toDelete);
        }
    }
    free(adjList);
}

int main() {
    setlocale(LC_ALL, "Russian");

    int n, startVertex;


    printf("Введите количество вершин в графе: ");
    scanf_s("%d", &n);

    if (n <= 0) {
        printf("Ошибка: количество вершин должно быть положительным!\n");
        return 1;
    }


    printf("\n1.1: Генерация матрицы смежности");
    int** matrix = generateAdjacencyMatrix(n);
    printMatrix(matrix, n);


    Node** adjList = createAdjacencyList(matrix, n);
    printAdjacencyList(adjList, n);


    printf("\nВведите стартовую вершину (0-%d): ", n - 1);
    scanf_s("%d", &startVertex);

    if (startVertex < 0 || startVertex >= n) {
        printf("Ошибка: стартовая вершина должна быть в диапазоне 0-%d!\n", n - 1);
        return 1;
    }


    int opsBFS_matrix = 0, opsBFS_list = 0, opsDFS = 0;
    double timeBFS_matrix = 0, timeBFS_list = 0, timeDFS = 0;

    printf("\n1.2: Поиск расстояний в ширину (матрица смежности)");
    int* distances_matrix = BFSD_matrix(matrix, n, startVertex, &opsBFS_matrix, &timeBFS_matrix);
    printDistances(distances_matrix, n, startVertex);
    printf("Время выполнения: %.2f мс\n", timeBFS_matrix);
    printf("Количество операций: %d\n", opsBFS_matrix);


    printf("\n1.3: Поиск расстояний (списки смежности)");
    int* distances_list = BFSD_adjList(adjList, n, startVertex, &opsBFS_list, &timeBFS_list);
    printDistances(distances_list, n, startVertex);
    printf("Время выполнения: %.2f мс\n", timeBFS_list);
    printf("Количество операций: %d\n", opsBFS_list);


    printf("\n2.1: Поиск расстояний в глубину (DFS)");
    int* distances_dfs = DFSD_matrix(matrix, n, startVertex, &opsDFS, &timeDFS);
    printDistances(distances_dfs, n, startVertex);
    printf("Время выполнения: %.2f мс\n", timeDFS);
    printf("Количество операций: %d\n", opsDFS);


    printf("\n2.2: Поиск расстояний (DFS списки)");
    int opsDFS_list = 0;
    double timeDFS_list = 0;
    int* distances_dfs_list = DFSD_adjList(adjList, n, startVertex, &opsDFS_list, &timeDFS_list);
    printDistances(distances_dfs_list, n, startVertex);
    printf("Время выполнения: %.2f мс\n", timeDFS_list);
    printf("Количество операций: %d\n", opsDFS_list);


    int resultsMatch = 1;
    for (int i = 0; i < n; i++) {
        if (distances_matrix[i] != distances_list[i]) {
            resultsMatch = 0;
            break;
        }
    }
    if (resultsMatch) {
        printf("Результаты для матрицы смежности и списков смежности совпадают!\n");
    }
    else {
        printf("Ошибка: результаты не совпадают!\n");
    }



    freeMatrix(matrix, n);
    freeAdjList(adjList, n);
    free(distances_matrix);
    free(distances_list);
    free(distances_dfs);
    free(distances_dfs_list);
    return 0;
}