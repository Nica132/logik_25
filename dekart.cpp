#define _CRT_SECURE_NO_WARNINGS
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int** cartesianProductMatrix(int** m1, int size1, int** m2, int size2, int* result_size) {
    *result_size = size1 * size2;

    int** result = (int**)malloc((*result_size) * sizeof(int*));
    for (int i = 0; i < *result_size; i++) {
        result[i] = (int*)calloc(*result_size, sizeof(int));
    }

    for (int i1 = 0; i1 < size1; i1++) {
        for (int j1 = 0; j1 < size2; j1++) {
            for (int i2 = 0; i2 < size1; i2++) {
                for (int j2 = 0; j2 < size2; j2++) {
                    int idx1 = i1 * size2 + j1;
                    int idx2 = i2 * size2 + j2;

                    if (i1 == i2 && m2[j1][j2] == 1) {
                        result[idx1][idx2] = 1;
                    }
                    else if (j1 == j2 && m1[i1][i2] == 1) {
                        result[idx1][idx2] = 1;
                    }
                }
            }
        }
    }

    return result;
}

void main() {
    setlocale(LC_ALL, "RUS");
    srand(time(NULL));

    int size1;
    printf("Введите размерность матрицы 1: ");
    scanf("%d", &size1);

    int size2;
    printf("Введите размерность матрицы 2: ");
    scanf("%d", &size2);

    int** m1 = (int**)malloc(size1 * sizeof(int*));
    for (int i = 0; i < size1; i++) {
        m1[i] = (int*)malloc(size1 * sizeof(int));
    }

    printf("Матрица смежности G1:\n");
    for (int i = 0; i < size1; i++) {
        for (int j = 0; j < size1; j++) {
            if (i == j) {
                m1[i][j] = 0;
            }
            else {
                m1[i][j] = rand() % 2;
                m1[j][i] = m1[i][j];
            }
            printf("%3d", m1[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int** m2 = (int**)malloc(size2 * sizeof(int*));
    for (int i = 0; i < size2; i++) {
        m2[i] = (int*)malloc(size2 * sizeof(int));
    }

    printf("Матрица смежности G2:\n");
    for (int i = 0; i < size2; i++) {
        for (int j = 0; j < size2; j++) {
            if (i == j) {
                m2[i][j] = 0;
            }
            else {
                m2[i][j] = rand() % 2;
                m2[j][i] = m2[i][j];
            }
            printf("%3d", m2[i][j]);
        }
        printf("\n");
    }
    printf("\n");

    int result_size;
    int** result = cartesianProductMatrix(m1, size1, m2, size2, &result_size);

    printf("Декартово произведение G1 и G2 (%dx%d):\n", result_size, result_size);
    for (int i = 0; i < result_size; i++) {
        for (int j = 0; j < result_size; j++) {
            printf("%3d", result[i][j]);
        }
        printf("\n");
    }

    for (int i = 0; i < size1; i++) free(m1[i]);
    free(m1);
    for (int i = 0; i < size2; i++) free(m2[i]);
    free(m2);
    for (int i = 0; i < result_size; i++) free(result[i]);
    free(result);
}