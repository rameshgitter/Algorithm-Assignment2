#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#define MAX_LINE_LENGTH 1000
#define MAX_POINTS 100

// Define the Point structure
typedef struct {
    double x,y;
} Point;

// Function to calculate distance between two points
double distance(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Define a structure for storing diagonals
typedef struct {
    int start;
    int end;
    double length;
} Diagonal;

// Function to compare diagonals based on length (for sorting)
int compareDiagonals(const void *a, const void *b) {
    Diagonal *d1 = (Diagonal *)a;
    Diagonal *d2 = (Diagonal *)b;
    return (d1->length > d2->length) - (d1->length < d2->length);
}


// Function to read polygon vertices from file
int readPolygon(const char *filename, Point polygon[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return -1;
    }

    int numVertices = 0;
    double x, y;
    while (fscanf(file, "%lf %lf", &x, &y) == 2) {
        polygon[numVertices].x = x;
        polygon[numVertices].y = y;
        numVertices++;
    }

    fclose(file);
    return numVertices;
}

// Function to find minimum length of triangulation for a polygon using greedy strategy
double minTriangulationLengthGreedy(Point polygon[], int n) {
    // Base case: a polygon with less than 3 vertices cannot be triangulated
    if (n < 3)
        return 0;

    // Create an array to store diagonals
    Diagonal diagonals[n * (n - 1) / 2];
    int idx = 0;

    // Generate all possible diagonals and calculate their lengths
    for (int i = 0; i < n; i++) {
        for (int j = i + 2; j < n; j++) {
            diagonals[idx].start = i;
            diagonals[idx].end = j;
            diagonals[idx].length = distance(polygon[i], polygon[j]);
            idx++;
        }
    }

    // Sort diagonals based on length
    qsort(diagonals, idx, sizeof(Diagonal), compareDiagonals);

    // Array to mark vertices as visited
    int visited[n];
    for (int i = 0; i < n; i++)
        visited[i] = 0;

    // Initialize total length of triangulation
    double totalLength = 0;

    // Greedily select non-intersecting diagonals in sorted order
    for (int i = 0; i < idx; i++) {
        int start = diagonals[i].start;
        int end = diagonals[i].end;
        
        if (!visited[start] && !visited[end]) {
            visited[start] = visited[end] = 1;
            totalLength += diagonals[i].length;
        }
    }

    return totalLength;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    FILE *output_file = fopen("greedy.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input_file);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        Point polygon[MAX_POINTS];
        int numVertices = 0;

        char *token = strtok(line, "{}");
        while (token != NULL) {
            sscanf(token, "%lf, %lf", &polygon[numVertices].x, &polygon[numVertices].y);
            numVertices++;
            token = strtok(NULL, "{}");
        }

        double minTriangulationGreedy = minTriangulationLengthGreedy(polygon, numVertices);

        fprintf(output_file, "%lf\n", minTriangulationGreedy);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

