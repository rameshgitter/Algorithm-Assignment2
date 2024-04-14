#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <string.h>

#define MAX_POINTS 100
#define MAX_LINE_LENGTH 1000

// Define the Point structure
typedef struct {
    double x, y;
} Point;

// Function to calculate distance between two points
double distance(Point p1, Point p2) {
    return sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
}

// Function to find minimum of two values
double min(double a, double b) {
    return (a < b) ? a : b;
}

// Memoization table to store results of subproblems
double dp[MAX_POINTS][MAX_POINTS];

// Function to find minimum length of triangulation for a polygon using dynamic programming
double minTriangulationLength(Point polygon[], int i, int j) {
    // Base case: a polygon with less than 3 vertices cannot be triangulated
    if (j < i + 2)
        return 0.0;

    // If result is already computed, return it
    if (dp[i][j] != 0)
        return dp[i][j];

    // Initialize result for current subproblem
    double min_length = INFINITY;

    // Try all possible splits and find the minimum
    for (int k = i + 1; k < j; k++) {
        double length = minTriangulationLength(polygon, i, k) +
                        minTriangulationLength(polygon, k, j) +
                        distance(polygon[i], polygon[j]) +     // Calculate the cost of the triangle formed by (i, j, k)
                        distance(polygon[j], polygon[k]) +
                        distance(polygon[k], polygon[i]);
        min_length = min(min_length, length);
    }

    // Memoize the result
    dp[i][j] = min_length;

    return min_length;
}

int main() {
    FILE *input_file = fopen("dataset.txt", "r");
    if (input_file == NULL) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    FILE *output_file = fopen("triangulation_lengths.txt", "w");
    if (output_file == NULL) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input_file);
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL) {
        Point points[MAX_POINTS];
        int num_points = 0;

        // Read points from the line
        char *token = strtok(line, "{}");
        while (token != NULL) {
            sscanf(token, "%lf, %lf", &points[num_points].x, &points[num_points].y);
            num_points++;
            token = strtok(NULL, "{}");
        }

        // Initialize memoization table
        memset(dp, 0, sizeof(dp));

        // Calculate minimum triangulation length for the current set of points
        double min_length = minTriangulationLength(points, 0, num_points - 1);

        // Write the result to the output file
        fprintf(output_file, "%lf\n", min_length);
    }

    fclose(input_file);
    fclose(output_file);
    return 0;
}

