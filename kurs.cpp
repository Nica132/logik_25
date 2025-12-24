#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <locale.h>
#include <ctype.h>

#define MAX_VERTICES 100
#define MAX_FILENAME 256
#define INF INT_MAX

void clear_input_buffer();
void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices, int start_vertex, int distances[], int previous[]);
void restore_path(int previous[], int destination, int path[], int* path_length);
int load_graph_from_file(const char* filename, int graph[MAX_VERTICES][MAX_VERTICES], int* num_vertices);
void save_results_to_file(const char* filename, int start_vertex, int distances[], int previous[], int num_vertices, int graph[MAX_VERTICES][MAX_VERTICES]);
void print_graph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices);
void print_distances(int distances[], int num_vertices, int start_vertex);
void create_graph(int graph[MAX_VERTICES][MAX_VERTICES], int* num_vertices);
void display_menu();
void create_random_graph(int graph[MAX_VERTICES][MAX_VERTICES], int n);
void create_manual_graph(int graph[MAX_VERTICES][MAX_VERTICES], int n);
int get_int_input(const char* prompt, int min_val, int max_val);
float get_float_input(const char* prompt, float min_val, float max_val);


void create_manual_graph(int graph[MAX_VERTICES][MAX_VERTICES], int n); void display_menu();

int main() {
    setlocale(LC_ALL, "Russian");
    int graph[MAX_VERTICES][MAX_VERTICES] = { 0 };
    int distances[MAX_VERTICES];
    int previous[MAX_VERTICES];
    int num_vertices = 0;
    int start_vertex = -1;
    int results_calculated = 0;
    char filename[MAX_FILENAME];

    printf("========================================\n");
    printf("   Программа реализации алгоритма\n");
    printf("          Дейкстры на C\n");
    printf("========================================\n");

    while (1) {
        display_menu();

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число от 1 до 7.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        switch (choice) {
        case 1:
            printf("Введите имя файла для загрузки: ");
            if (scanf("%255s", filename) != 1) {
                printf("Ошибка ввода имени файла\n");
                break;
            }
            clear_input_buffer();

            if (load_graph_from_file(filename, graph, &num_vertices)) {
                results_calculated = 0;
            }
            break;

        case 2:
            create_graph(graph, &num_vertices);
            results_calculated = 0;
            break;

        case 3:
            if (num_vertices == 0) {
                printf("Граф не загружен. Сначала загрузите или создайте граф.\n");
            }
            else {
                print_graph(graph, num_vertices);
            }
            break;

        case 4:
            if (num_vertices == 0) {
                printf("Граф не загружен. Сначала загрузите или создайте граф.\n");
                break;
            }

            printf("Введите номер начальной вершины (1-%d): ", num_vertices);
            if (scanf("%d", &start_vertex) != 1 || start_vertex < 1 || start_vertex > num_vertices) {
                printf("Ошибка: некорректный номер вершины\n");
                clear_input_buffer();
                break;
            }
            clear_input_buffer();

            start_vertex--;

            dijkstra(graph, num_vertices, start_vertex, distances, previous);
            results_calculated = 1;

            printf("\nАлгоритм Дейкстры выполнен успешно!\n");
            print_distances(distances, num_vertices, start_vertex);
            break;

        case 5:
            if (!results_calculated) {
                printf("Результаты еще не рассчитаны. Сначала выполните алгоритм Дейкстры.\n");
                break;
            }

            printf("Введите имя файла для сохранения результатов: ");
            if (scanf("%255s", filename) != 1) {
                printf("Ошибка ввода имени файла\n");
                break;
            }
            clear_input_buffer();

            save_results_to_file(filename, start_vertex, distances, previous, num_vertices, graph);
            break;

        case 6:
            printf("Выход из программы. До свидания!\n");
            return 0;

        default:
            printf("Неверный выбор. Пожалуйста, выберите опцию от 1 до 7.\n");
            break;
        }
    }

    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int get_int_input(const char* prompt, int min_val, int max_val) {
    int value;
    while (1) {
        printf("%s (%d-%d): ", prompt, min_val, max_val);
        if (scanf("%d", &value) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (value < min_val || value > max_val) {
            printf("Ошибка: значение должно быть от %d до %d.\n", min_val, max_val);
            continue;
        }

        return value;
    }
}

float get_float_input(const char* prompt, float min_val, float max_val) {
    float value;
    while (1) {
        printf("%s (%.1f-%.1f): ", prompt, min_val, max_val);
        if (scanf("%f", &value) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (value < min_val || value > max_val) {
            printf("Ошибка: значение должно быть от %.1f до %.1f.\n", min_val, max_val);
            continue;
        }

        return value;
    }
}


void print_graph(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices) {
    printf("\nТекущий граф (матрица смежности):\n");
    printf("    ");
    for (int j = 0; j < num_vertices; j++) {
        printf("%3d ", j + 1);
    }
    printf("\n");

    for (int i = 0; i < num_vertices; i++) {
        printf("%3d ", i + 1);
        for (int j = 0; j < num_vertices; j++) {
            if (graph[i][j] == INF) {
                printf(" INF");
            }
            else {
                printf("%4d", graph[i][j]);
            }
        }
        printf("\n");
    }
}

void print_distances(int distances[], int num_vertices, int start_vertex) {
    printf("\nКратчайшие расстояния от вершины %d:\n", start_vertex + 1);
    for (int i = 0; i < num_vertices; i++) {
        if (distances[i] == INF) {
            printf("%d -> %d: недостижима\n", start_vertex + 1, i + 1);
        }
        else {
            printf("%d -> %d: %d\n", start_vertex + 1, i + 1, distances[i]);
        }
    }
}


void create_graph(int graph[MAX_VERTICES][MAX_VERTICES], int* num_vertices) {
    int n = get_int_input("Введите количество вершин графа", 1, MAX_VERTICES);
    if (n == -1) {
        return;
    }

    *num_vertices = n;


    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
        }
    }

    int creation_type;
    while (1) {
        printf("\nВыберите способ создания графа:\n");
        printf("1. Ручной ввод\n");
        printf("2. Случайная генерация\n");
        printf("Выберите опцию (1-2): ");

        if (scanf("%d", &creation_type) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите 1 или 2.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (creation_type != 1 && creation_type != 2) {
            printf("Ошибка: пожалуйста, выберите 1 или 2.\n");
            continue;
        }
        break;
    }

    if (creation_type == 1) {
        create_manual_graph(graph, n);
    }
    else {
        create_random_graph(graph, n);
    }
}

void create_manual_graph(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    printf("\nВведите матрицу смежности %dx%d:\n", n, n);
    printf("(0 - отсутствие ребра, >0 - вес ребра)\n\n");

    for (int i = 0; i < n; i++) {
        printf("Строка %d (для вершин 1-%d): ", i + 1, n);
        for (int j = 0; j < n; j++) {
            if (scanf("%d", &graph[i][j]) != 1) {
                printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
                clear_input_buffer();

                j = -1;
                printf("Повторите ввод строки %d: ", i + 1);
                continue;
            }
        }
        clear_input_buffer();
    }
    printf("Граф успешно создан вручную!\n");
}

void create_random_graph(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    int graph_type;
    while (1) {
        printf("\nВыберите тип графа:\n");
        printf("1. Неориентированный\n");
        printf("2. Ориентированный\n");
        printf("Выберите опцию (1-2): ");

        if (scanf("%d", &graph_type) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите 1 или 2.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (graph_type != 1 && graph_type != 2) {
            printf("Ошибка: пожалуйста, выберите 1 или 2.\n");
            continue;
        }
        break;
    }

    int allow_loops;
    while (1) {
        printf("Включить петли? (1 - да, 0 - нет): ");
        if (scanf("%d", &allow_loops) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите 0 или 1.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (allow_loops != 0 && allow_loops != 1) {
            printf("Ошибка: пожалуйста, введите 0 или 1.\n");
            continue;
        }
        break;
    }

    int max_weight;
    while (1) {
        printf("Введите максимальный вес ребра (1-50): ");
        if (scanf("%d", &max_weight) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите целое число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (max_weight < 1 || max_weight > 50) {
            printf("Ошибка: значение должно быть от 1 до 50.\n");
            continue;
        }
        break;
    }

    float density;
    while (1) {
        printf("Введите плотность графа (0.0-1.0): ");
        if (scanf("%f", &density) != 1) {
            printf("Ошибка ввода. Пожалуйста, введите число.\n");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();

        if (density < 0.0 || density > 1.0) {
            printf("Ошибка: значение должно быть от 0.0 до 1.0.\n");
            continue;
        }
        break;
    }

    printf("Генерирую случайный граф...\n");

    if (graph_type == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = i; j < n; j++) {
                if (i == j) {
                    if (allow_loops && (float)rand() / RAND_MAX < density / 2) {
                        graph[i][j] = (rand() % max_weight) + 1;
                    }
                }
                else {
                    if ((float)rand() / RAND_MAX < density) {
                        int weight = (rand() % max_weight) + 1;
                        graph[i][j] = weight;
                        graph[j][i] = weight;
                    }
                }
            }
        }
    }
    else {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) {
                    if (allow_loops && (float)rand() / RAND_MAX < density / 2) {
                        graph[i][j] = (rand() % max_weight) + 1;
                    }
                }
                else {
                    if ((float)rand() / RAND_MAX < density) {
                        graph[i][j] = (rand() % max_weight) + 1;
                    }
                }
            }
        }
    }

    printf("\nСлучайный граф с %d вершинами успешно сгенерирован!\n", n);
    printf("Параметры генерации:\n");
    printf("- Тип: %s\n", graph_type == 1 ? "неориентированный" : "ориентированный");
    printf("- Петли: %s\n", allow_loops ? "да" : "нет");
    printf("- Макс. вес ребра: %d\n", max_weight);
    printf("- Плотность: %.2f\n", density);
}
void display_menu() {
    printf("\n=== МЕНЮ ПРОГРАММЫ ===\n");
    printf("1. Загрузить граф из файла\n");
    printf("2. Создание графа\n");
    printf("3. Вывести текущий граф\n");
    printf("4. Выполнить алгоритм Дейкстры\n");
    printf("5. Сохранить результаты в файл\n");
    printf("6. Выход\n");
    printf("Выберите опцию (1-7): ");
}