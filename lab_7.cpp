#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

void DFS(int** G, int numG, int* visited, int s) {
	visited[s] = 1;
	printf("%3d", s);

	for (int i = 0; i < numG; i++) {
		if (G[s][i] == 1 && visited[i] == 0)
			DFS(G, numG, visited, i);
	}
}



int main() {

	setlocale(LC_ALL, "RUS");

	int** G;
	int numG, current;
	int* visited;

	printf("введите количество вершин : ");
	scanf("%d", &numG);

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

	printf("результат: ");
	DFS(G, numG, visited, current);

	for (int i = 0; i < numG; i++)
		free(G[i]);
	free(G);
	free(visited);

	return 0;
}