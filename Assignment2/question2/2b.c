#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMBOLS 256

struct Node {
    char symbol;
    int frequency;
    struct Node *left, *right;
};

struct MinHeap {
    int size;
    struct Node *array[MAX_SYMBOLS];
};

struct Node *createNode(char symbol, int frequency) {
    struct Node *node = (struct Node *)malloc(sizeof(struct Node));
    node->symbol = symbol;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

struct MinHeap *createMinHeap() {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    minHeap->size = 0;
    return minHeap;
}

void swapNodes(struct Node **a, struct Node **b) {
    struct Node *temp = *a;
    *a = *b;
    *b = temp;
}

void minHeapify(struct MinHeap *minHeap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[smallest]->frequency)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[smallest]->frequency)
        smallest = right;

    if (smallest != index) {
        swapNodes(&minHeap->array[smallest], &minHeap->array[index]);
        minHeapify(minHeap, smallest);
    }
}

void buildMinHeap(struct MinHeap *minHeap) {
    int n = minHeap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; i--) {
        minHeapify(minHeap, i);
    }
}

void insertMinHeap(struct MinHeap *minHeap, struct Node *node) {
    minHeap->size++;
    int i = minHeap->size - 1;
    while (i && node->frequency < minHeap->array[(i - 1) / 2]->frequency) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = node;
}

struct Node *extractMin(struct MinHeap *minHeap) {
    struct Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];
    --minHeap->size;
    minHeapify(minHeap, 0);
    return temp;
}

struct MinHeap *buildHuffmanTree(char *text, int *frequencies) {
    for (int i = 0; i < strlen(text); i++) {
        frequencies[(int)text[i]]++;
    }

    struct MinHeap *minHeap = createMinHeap();
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i] > 0) {
            insertMinHeap(minHeap, createNode((char)i, frequencies[i]));
        }
    }

    while (minHeap->size != 1) {
        struct Node *left = extractMin(minHeap);
        struct Node *right = extractMin(minHeap);
        struct Node *top = createNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;
        insertMinHeap(minHeap, top);
    }

    return minHeap;
}

void printCodes(struct Node *root, int *codes, int top) {
    if (root->left) {
        codes[top] = 0;
        printCodes(root->left, codes, top + 1);
    }
    if (root->right) {
        codes[top] = 1;
        printCodes(root->right, codes, top + 1);
    }
    if (!root->left && !root->right) {
        printf("%c: ", root->symbol);
        for (int i = 0; i < top; i++) {
            printf("%d", codes[i]);
        }
        printf("\n");
    }
}

void HuffmanCodes(char *text) {
    int frequencies[MAX_SYMBOLS] = {0};
    struct MinHeap *minHeap = buildHuffmanTree(text, frequencies);
    struct Node *root = minHeap->array[0];

    int codes[MAX_SYMBOLS];
    int top = 0;
    printf("Huffman Codes:\n");
    printCodes(root, codes, top);
}

int main() {
    char text[] = "hello world";
    HuffmanCodes(text);
    return 0;
}

