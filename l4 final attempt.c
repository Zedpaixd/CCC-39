#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define LEVEL 4
#define LEVEL_STAGE "1"
char LEVEL_IN[20];
char LEVEL_OUT[20];

void initialize_filenames() {
    sprintf(LEVEL_IN, "level%d_%s.in", LEVEL, LEVEL_STAGE);
    sprintf(LEVEL_OUT, "output%d.out%s", LEVEL, LEVEL_STAGE);
}

void writeOutput(const char *whatToWrite) {
    FILE *f = fopen(LEVEL_OUT, "w");
    if (f == NULL) {
        perror("Failed to open file");
        return;
    }
    fprintf(f, "%s\n", whatToWrite);
    fclose(f);
}

typedef struct {
    int width, height;
    char **matrix;
} Lawn;

Lawn *readInput(int *num_lawns) {
    FILE *f = fopen(LEVEL_IN, "r");
    if (!f) {
        perror("Failed to open input file");
        exit(1);
    }

    fscanf(f, "%d\n", num_lawns);
    Lawn *lawns = malloc(sizeof(Lawn) * (*num_lawns));

    for (int i = 0; i < *num_lawns; i++) {
        fscanf(f, "%d %d\n", &lawns[i].width, &lawns[i].height);

        lawns[i].matrix = malloc(lawns[i].height * sizeof(char *));
        for (int j = 0; j < lawns[i].height; j++) {
            lawns[i].matrix[j] = malloc(lawns[i].width + 1); // +1 for null terminator
            fgets(lawns[i].matrix[j], lawns[i].width + 2, f); // +2 for the newline and null terminator
        }
    }

    fclose(f);
    return lawns;
}

bool is_valid(int x, int y, int width, int height, char **matrix, bool **visited) {
    return x >= 0 && x < height && y >= 0 && y < width && matrix[x][y] != 'X' && !visited[x][y];
}

bool backtrack(char **matrix, int x, int y, char *path, int path_len, bool **visited, char *solution, int width, int height) {
    if (strlen(path) == width * height - 1) {
        strcpy(solution, path);
        return true;
    }

    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    char dir_names[4] = {'W', 'S', 'A', 'D'};
    bool found = false;

    for (int i = 0; i < 4; i++) {
        int nx = x + directions[i][0];
        int ny = y + directions[i][1];
        if (is_valid(nx, ny, width, height, matrix, visited)) {
            visited[nx][ny] = true;
            path[path_len] = dir_names[i];
            path[path_len + 1] = '\0';
            if (backtrack(matrix, nx, ny, path, path_len + 1, visited, solution, width, height)) {
                found = true;
                break;
            }
            visited[nx][ny] = false;
            path[path_len] = '\0';
        }
    }
    return found;
}

void problem4() {
    int num_lawns;
    Lawn *lawns = readInput(&num_lawns);
    char results[1024] = "";
    char solution[1024];

    for (int i = 0; i < num_lawns; i++) {
        bool found = false;
        for (int h = 0; h < lawns[i].height; h++) {
            for (int w = 0; w < lawns[i].width; w++) {
                if (lawns[i].matrix[h][w] != 'X') {
                    bool **visited = malloc(lawns[i].height * sizeof(bool *));
                    for (int j = 0; j < lawns[i].height; j++) {
                        visited[j] = calloc(lawns[i].width, sizeof(bool));
                    }
                    visited[h][w] = true;
                    if (backtrack(lawns[i].matrix, h, w, "", 0, visited, solution, lawns[i].width, lawns[i].height)) {
                        strcat(results, solution);
                        strcat(results, "\n");
                        found = true;
                    }
                    for (int j = 0; j < lawns[i].height; j++) {
                        free(visited[j]);
                    }
                    free(visited);
                    if (found) break;
                }
            }
            if (found) break;
        }
        if (!found) {
            strcat(results, "INVALID\n");
        }
        for (int j = 0; j < lawns[i].height; j++) {
            free(lawns[i].matrix[j]);
        }
        free(lawns[i].matrix);
    }
    free(lawns);
    writeOutput(results);
}

int main() {
    initialize_filenames();
    problem4();
    return 0;
}
