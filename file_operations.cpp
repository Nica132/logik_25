#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_FILENAME 256
#define INF INT_MAX

void restore_path(int previous[], int destination, int path[], int* path_length);

int load_graph_from_file(const char* filename, int graph[MAX_VERTICES][MAX_VERTICES], int* num_vertices) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Ошибка: не удалось открыть файл %s\n", filename);
        return 0;
    }

    if (fscanf(file, "%d", num_vertices) != 1 || *num_vertices <= 0 || *num_vertices > MAX_VERTICES) {
        printf("Ошибка: некорректное количество вершин в файле\n");
        fclose(file);
        return 0;
    }

    for (int i = 0; i < *num_vertices; i++) {
        for (int j = 0; j < *num_vertices; j++) {
            graph[i][j] = INF;
        }
    }

    for (int i = 0; i < *num_vertices; i++) {
        for (int j = 0; j < *num_vertices; j++) {
            int weight;
            if (fscanf(file, "%d", &weight) != 1) {
                printf("Ошибка: некорректный формат матрицы в файле\n");
                fclose(file);
                return 0;
            }

            if (weight == -1) {
                graph[i][j] = INF;
            }
            else if (weight < 0) {
                printf("Внимание: отрицательный вес в позиции [%d][%d] заменен на INF\n", i + 1, j + 1);
                graph[i][j] = INF;
            }
            else {
                graph[i][j] = weight;
            }
        }
    }

    // Устанавливаем петли в 0, если они не заданы
    for (int i = 0; i < *num_vertices; i++) {
        if (graph[i][i] == INF) {
            graph[i][i] = 0;
        }
    }

    fclose(file);
    printf("Граф успешно загружен из файла %s\n", filename);
    printf("Количество вершин: %d\n", *num_vertices);
    return 1;
}
void save_results_to_file(const char* filename, int start_vertex, int distances[], int previous[], int num_vertices, int graph[MAX_VERTICES][MAX_VERTICES]) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        printf("Ошибка: не удалось создать файл %s\n", filename);
        return;
    }

    fprintf(file, "Результаты работы алгоритма Дейкстры\n");
    fprintf(file, "======================================\n");
    fprintf(file, "Начальная вершина: %d\n\n", start_vertex + 1);

    fprintf(file, "Матрица смежности графа:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            fprintf(file, "%3d ", graph[i][j]);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");

    fprintf(file, "Кратчайшие расстояния от вершины %d:\n", start_vertex + 1);
    for (int i = 0; i < num_vertices; i++) {
        if (distances[i] == INF) {
            fprintf(file, "Вершина %d: недостижима\n", i + 1);
        }
        else {
            fprintf(file, "Вершина %d: расстояние = %d, ", i + 1, distances[i]);

            int path[MAX_VERTICES];
            int path_length;
            restore_path(previous, i, path, &path_length);

            fprintf(file, "путь: ");
            for (int j = 0; j < path_length; j++) {
                fprintf(file, "%d", path[j] + 1);
                if (j < path_length - 1) {
                    fprintf(file, " -> ");
                }
            }
            fprintf(file, "\n");
        }
    }

    fprintf(file, "\n======================================\n");
    fprintf(file, "Конец результатов\n");

    fclose(file);
    printf("Результаты успешно сохранены в файл %s\n", filename);
}