#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_VERTICES 100

typedef struct {
    double x;
    double y;
} Point;

// Comparison function for sorting points based on polar angle
int compare_points(const void *p1, const void *p2) {
    Point *point1 = (Point *) p1;
    Point *point2 = (Point *) p2;

    double angle1 = atan2(point1->y, point1->x);
    double angle2 = atan2(point2->y, point2->x);

    if (angle1 < angle2) return -1;
    if (angle1 > angle2) return 1;
    return 0;
}

// Function to generate random convex polygon with given number of vertices
void generateConvexPolygon(int num_vertices, Point polygon[]) {
    int i;
    double angle, radius;
    Point centroid = {0, 0};

    // Generate random vertices
    for (i = 0; i < num_vertices; i++) {
        angle = ((double) rand() / RAND_MAX) * 2 * M_PI; // Random angle
        radius = ((double) rand() / RAND_MAX); // Random radius in [0, 1]
        polygon[i].x = centroid.x + radius * cos(angle);
        polygon[i].y = centroid.y + radius * sin(angle);
    }

    // Sort vertices based on polar angle from centroid
    for (i = 0; i < num_vertices; i++) {
        centroid.x += polygon[i].x;
        centroid.y += polygon[i].y;
    }
    centroid.x /= num_vertices;
    centroid.y /= num_vertices;

    qsort(polygon, num_vertices, sizeof(Point), compare_points);
}

// Function to print the polygon
void print_polygon(int num_vertices, Point polygon[], FILE *fp) {
    fprintf(fp, "Convex polygon with %d vertices:\n", num_vertices);
    for (int i = 0; i < num_vertices; i++) {
        fprintf(fp, "{%lf, %lf}, ", polygon[i].x, polygon[i].y);
    }
    fprintf(fp, "\n");
}

int main() {
    srand(time(NULL)); // Seed the random number generator

    int numVertices = 20; // Change this value for different number of vertices
    FILE *time_fp = fopen("time.txt", "w"); // Open file for writing time taken
    if (time_fp == NULL) {
        printf("Error opening time file!\n");
        return 1;
    }

    Point polygon[MAX_VERTICES]; // Define the polygon array
    clock_t start, end;
    double cpu_time_used;
    
    // Generate convex polygon with increasing number of vertices
    for (int i = 3; i <= numVertices; i++) {
        start = clock(); // Start timing
        generateConvexPolygon(i, polygon);
        end = clock(); // End timing
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        
        fprintf(time_fp, "Time taken for generating %d-vertex polygon: %lf seconds\n", i, cpu_time_used);
    }

    fclose(time_fp); // Close the time file

    return 0;
}

