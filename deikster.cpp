#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF INT_MAX

void dijkstra(int graph[MAX_VERTICES][MAX_VERTICES], int num_vertices, int start_vertex, int distances[], int previous[]) {
    int visited[MAX_VERTICES] = { 0 };

    for (int i = 0; i < num_vertices; i++) {
        distances[i] = INF;
        previous[i] = -1;
    }
    distances[start_vertex] = 0;

    for (int i = 0; i < num_vertices; i++) {
        int min_distance = INF;
        int current_vertex = -1;

        for (int j = 0; j < num_vertices; j++) {
            if (!visited[j] && distances[j] < min_distance) {
                min_distance = distances[j];
                current_vertex = j;
            }
        }

        if (current_vertex == -1 || min_distance == INF) {
            break;
        }

        visited[current_vertex] = 1;

        for (int j = 0; j < num_vertices; j++) {
            if (graph[current_vertex][j] != INF &&
                !visited[j] &&
                current_vertex != j &&  
                graph[current_vertex][j] > 0) {  

                int new_distance;
                if (distances[current_vertex] == INF) {
                    new_distance = INF;
                }
                else {
                    new_distance = distances[current_vertex] + graph[current_vertex][j];
                }

                if (new_distance < distances[j]) {
                    distances[j] = new_distance;
                    previous[j] = current_vertex;
                }
            }
        }
    }

    for (int i = 0; i < num_vertices; i++) {
        if (distances[i] == INF) {
            previous[i] = -1;
        }
    }
}

void restore_path(int previous[], int destination, int path[], int* path_length) {
    *path_length = 0;

    if (previous[destination] == -1 && destination != 0) { 
        return;
    }

    int current = destination;

    while (current != -1) {
        path[(*path_length)++] = current;
        current = previous[current];
    }

    if (*path_length > 0) {
        for (int i = 0; i < *path_length / 2; i++) {
            int temp = path[i];
            path[i] = path[*path_length - i - 1];
            path[*path_length - i - 1] = temp;
        }
    }
}