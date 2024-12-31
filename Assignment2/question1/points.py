import re
import matplotlib.pyplot as plt

# Function to plot a polygon
def plot_polygon(vertices):
    # Append the first vertex to the end to close the polygon
    vertices.append(vertices[0])
    x, y = zip(*vertices)
    plt.plot(x, y)

# Read vertices from the file
file_path = "/home/ramesh/Desktop/assignment4/Algorithm/question1/dataset.txt"
with open(file_path, "r") as file:
    for line in file:
        # Extract vertices enclosed in curly braces and split them
        vertices_str = re.findall(r'{([^}]*)}', line)
        if vertices_str:  # Check if vertices_str is not empty
            vertices = [tuple(map(float, vertex.split(','))) for vertex in vertices_str[0].split() if vertex.strip()]
            # Plot the polygon if there are vertices
            if vertices:
                plot_polygon(vertices)

# Set plot title and labels
plt.title('Polygons from dataset.txt')
plt.xlabel('X')
plt.ylabel('Y')

# Display the plot
plt.grid(True)
plt.axis('equal')  # Equal aspect ratio
plt.show()

