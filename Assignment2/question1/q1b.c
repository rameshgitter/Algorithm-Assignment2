#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<string.h>

#define MAX_POINTS 100
#define MAX_LINE_LENGTH 10000

// Structure of a point in 2D plane
struct Point
{
    double x, y;
};

// Utility function to find minimum of two double values
double min(double x, double y)
{
    return (x <= y) ? x : y;
}

// A utility function to find distance between two points in a plane
double dist(struct Point p1, struct Point p2)
{
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) +
                (p1.y - p2.y)*(p1.y - p2.y));
}

// A utility function to find cost of a triangle. The cost is considered
// as perimeter (sum of lengths of all edges) of the triangle
double cost(struct Point points[], int i, int j, int k)
{
    struct Point p1 = points[i], p2 = points[j], p3 = points[k];
    return dist(p1, p2) + dist(p2, p3) + dist(p3, p1);
}

// A recursive function to find minimum cost of polygon triangulation
// The polygon is represented by points[i..j].
double mTC(struct Point points[], int i, int j)
{
   // There must be at least three points between i and j
   // (including i and j)
   if (j < i+2)
      return 0;

   // Initialize result as infinite
   double res = INFINITY;

   // Find minimum triangulation by considering all
   for (int k=i+1; k<j; k++)
        res = min(res, (mTC(points, i, k) + mTC(points, k, j) +
                        cost(points, i, k, j)));
   return  res;
}

int main(){
    FILE *file = fopen("dataset.txt", "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL) {
        struct Point points[MAX_POINTS];
        int num_points = 0;

        // Read points from the line
        char *token = strtok(line, "{}");
        while (token != NULL) {
            sscanf(token, "%lf, %lf", &points[num_points].x, &points[num_points].y);
            num_points++;
            token = strtok(NULL, "{}");
        }

        // Calculate and print the cost
        printf("Cost for points: %lf\n", mTC(points, 0, num_points - 1));
    }

    fclose(file);
    return 0;
}

