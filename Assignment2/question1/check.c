#include <stdio.h>
#include <stdbool.h>
typedef struct {
    double x;
    double y;
} Point;

// Function to compute the cross product of two vectors
int cross_product(Point p1, Point p2, Point p3) {
    int dx1 = p2.x - p1.x;
    int dy1 = p2.y - p1.y;
    int dx2 = p2.x - p3.x;  // Corrected order of operands
    int dy2 = p2.y - p3.y;  // Corrected order of operands

    return dx1 * dy2 - dx2 * dy1;
}

// Function to check if a polygon is convex
bool isConvex(Point polygon[], int num_vertices) {
    bool has_positive = false;
    bool has_negative = false;

    for (int i = 0; i < num_vertices; i++) {
        Point p1 = polygon[i];
        Point p2 = polygon[(i + 1) % num_vertices];  // Corrected index calculation
        Point p3 = polygon[(i + 2) % num_vertices];  // Corrected index calculation

        int cross = cross_product(p1, p2, p3);

        if (cross > 0) {
            has_positive = true;
        } else if (cross < 0) {
            has_negative = true;
        }

        // If both positive and negative cross products are found, polygon is not convex
        if (has_positive && has_negative) {
            return false;
        }
    }

    // If either positive or negative cross product is missing, polygon is convex
    return true;
}

int main() {
    Point polygon[] = {{-0.352059, -0.009786},{-0.049078, -0.019826},{-0.455584, -0.303373},{-0.047025, -0.047522},{-0.532481, -0.642906},{-0.323283, -0.536412},{0.156447, -0.192869},{0.405091, -0.454900},{0.202401, -0.189568},{0.273266, -0.130731},{0.031092, -0.008694},{0.771735, -0.094075},{0.214197, 0.022933},{0.877465, 0.119655},{0.214448, 0.226705},{0.279671, 0.522031},{0.036228, 0.126296},{-0.020507, 0.062233},{-0.115869, 0.195329},{-0.043492, 0.073310},};
    int num_vertices = sizeof(polygon) / sizeof(polygon[0]);

    if (isConvex(polygon, num_vertices)) {
        printf("The polygon is convex.\n");
    } else {
        printf("The polygon is not convex.\n");
    }

    return 0;
}

