#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

typedef struct Node {
    int data;
    struct Node* next;
} Node;

typedef struct {
    Node* front;
    Node* rear;
} MyQueue;

typedef struct ListNode {
    int vertex;
    struct ListNode* next;
} ListNode;

void BFS_matrix_myqueue(int** G, int numG, int* visited, int s) {
    if (s < 0 || s >= numG) return;

    MyQueue q;
    q.front = q.rear = NULL;

    int v;

    visited[s] = 1;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = s;
    newNode->next = NULL;

    if (q.rear == NULL) {
        q.front = q.rear = newNode;
    }
    else {
        q.rear->next = newNode;
        q.rear = newNode;
    }

    while (q.front != NULL) {
        v = q.front->data;
        Node* temp = q.front;
        q.front = q.front->next;

        if (q.front == NULL) {
            q.rear = NULL;
        }

        free(temp);

        printf("%3d", v);

        for (int i = 0; i < numG; i++) {
            if (G[v][i] == 1 && visited[i] == 0) {
                visited[i] = 1;

                newNode = (Node*)malloc(sizeof(Node));
                newNode->data = i;
                newNode->next = NULL;

                if (q.rear == NULL) {
                    q.front = q.rear = newNode;
                }
                else {
                    q.rear->next = newNode;
                    q.rear = newNode;
                }
            }
        }
    }
}

void BFS_adjlist_myqueue(ListNode** adjList, int numG, int* visited, int s) {
    if (s < 0 || s >= numG) return;

    MyQueue q;
    q.front = q.rear = NULL;

    int v;

    visited[s] = 1;

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = s;
    newNode->next = NULL;

    if (q.rear == NULL) {
        q.front = q.rear = newNode;
    }
    else {
        q.rear->next = newNode;
        q.rear = newNode;
    }

    while (q.front != NULL) {
        v = q.front->data;
        Node* temp = q.front;
        q.front = q.front->next;

        if (q.front == NULL) {
            q.rear = NULL;
        }

        free(temp);

        printf("%3d", v);

        ListNode* current = adjList[v];
        while (current != NULL) {
            if (visited[current->vertex] == 0) {
                visited[current->vertex] = 1;

                newNode = (Node*)malloc(sizeof(Node));
                newNode->data = current->vertex;
                newNode->next = NULL;

                if (q.rear == NULL) {
                    q.front = q.rear = newNode;
                }
                else {
                    q.rear->next = newNode;
                    q.rear = newNode;
                }
            }
            current = current->next;
        }
    }
}

ListNode** createAdjacencyList(int** G, int numG) {
    ListNode** adjList = (ListNode**)malloc(numG * sizeof(ListNode*));

    for (int i = 0; i < numG; i++) {
        adjList[i] = NULL;

        for (int j = numG - 1; j >= 0; j--) {
            if (G[i][j] == 1 && i != j) {
                ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
                newNode->vertex = j;
                newNode->next = adjList[i];
                adjList[i] = newNode;
            }
        }
    }

    return adjList;
}

void printAdjacencyList(ListNode** adjList, int numG) {
    printf("\nСписки смежности:\n");
    for (int i = 0; i < numG; i++) {
        printf("%d: ", i);
        ListNode* current = adjList[i];
        while (current != NULL) {
            printf("%d ", current->vertex);
            current = current->next;
        }
        printf("\n");
    }
}

void freeAdjacencyList(ListNode** adjList, int numG) {
    for (int i = 0; i < numG; i++) {
        ListNode* current = adjList[i];
        while (current != NULL) {
            ListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
    free(adjList);
}

void measurePerformance(int** G, int numG, int startVertex) {
    clock_t start, end;
    double cpu_time_used;

    int* visited1 = (int*)malloc(numG * sizeof(int));
    for (int i = 0; i < numG; i++) visited1[i] = 0;

    printf("\nРезультат BFS (матрица смежности): ");
    start = clock();
    BFS_matrix_myqueue(G, numG, visited1, startVertex);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nВремя BFS с матрицей смежности: %f секунд\n", cpu_time_used);
    free(visited1);

    ListNode** adjList = createAdjacencyList(G, numG);

    int* visited2 = (int*)malloc(numG * sizeof(int));
    for (int i = 0; i < numG; i++) visited2[i] = 0;

    printf("\nРезультат BFS (списки смежности): ");
    start = clock();
    BFS_adjlist_myqueue(adjList, numG, visited2, startVertex);
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nВремя BFS со списками смежности: %f секунд\n", cpu_time_used);

    free(visited2);
    freeAdjacencyList(adjList, numG);
}

int main() {
    setlocale(LC_ALL, "RUS");

    int** G;
    int numG, current;
    int* visited;

    printf("введите количество вершин : ");
    scanf("%d", &numG);

    srand((unsigned int)time(NULL));

    visited = (int*)malloc(numG * sizeof(int));
    G = (int**)malloc(numG * sizeof(int*));
    for (int i = 0; i < numG; i++) {
        G[i] = (int*)malloc(numG * sizeof(int));
    }

    for (int i = 0; i < numG; i++) {
        visited[i] = 0;
        for (int j = i; j < numG; j++) {
            G[i][j] = G[j][i] = (j == i ? 0 : rand() % 2);
        }
    }

    for (int i = 0; i < numG; i++) {
        for (int j = 0; j < numG; j++) {
            printf("%3d", G[i][j]);
        }
        printf("\n");
    }

    printf("введите начальную вершину: ");
    scanf("%d", &current);

    ListNode** adjList = createAdjacencyList(G, numG);
    printAdjacencyList(adjList, numG);

    printf("\nрезультат: ");
    for (int i = 0; i < numG; i++) visited[i] = 0;
    BFS_adjlist_myqueue(adjList, numG, visited, current);

    freeAdjacencyList(adjList, numG);

    printf("\n\nСравнение производительности:\n");
    measurePerformance(G, numG, current);

    for (int i = 0; i < numG; i++)
        free(G[i]);
    free(G);
    free(visited);

    return 0;
}