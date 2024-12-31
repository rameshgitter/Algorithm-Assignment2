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

char *getCode(struct Node *root, char symbol);
struct MinHeap *buildHuffmanTree(int *frequencies);


// Struct and function definitions for Huffman encoding (from previous code)
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

struct MinHeap *buildHuffmanTree(int *frequencies) {
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
   for (int i = 0; i < strlen(text); i++) {
       frequencies[(int)text[i]]++;
   }
   struct MinHeap *minHeap = buildHuffmanTree(frequencies);
   struct Node *root = minHeap->array[0];

   int codes[MAX_SYMBOLS];
   int top = 0;
   printf("Huffman Codes:\n");
   printCodes(root, codes, top);
}


void calculateFrequencies(FILE *file, int *frequencies) {
   int c;
   while ((c = fgetc(file)) != EOF) {
       frequencies[c]++;
   }
}

char *getCode(struct Node *root, char symbol) {
  static char code[MAX_SYMBOLS]; // Static buffer to hold the constructed Huffman code
  code[0] = '\0';

  // Helper function for recursive traversal
  void getCodeHelper(struct Node *node, char *currentCode, char symbol) {
    if (node == NULL) {
      return; // Character not found
    }

    if (node->symbol == symbol) {
      currentCode[strlen(currentCode)] = '\0'; // Add null terminator
      strcpy(code, currentCode); // Found the symbol, copy the code
      return;
    }

    // Explore left subtree
    char leftCode[MAX_SYMBOLS];
    strcpy(leftCode, currentCode);
    strcat(leftCode, "0");
    getCodeHelper(node->left, leftCode, symbol);

    // Explore right subtree
    char rightCode[MAX_SYMBOLS];
    strcpy(rightCode, currentCode);
    strcat(rightCode, "1");
    getCodeHelper(node->right, rightCode, symbol);
  }

  // Start traversal from the root node with an empty code
  getCodeHelper(root, "", symbol);

  return code;
}


void writeEncodedText(FILE *inputFile, FILE *outputFile, struct Node *root) {
   rewind(inputFile);
   int bitBuffer = 0;
   int bitsWritten = 0;
   int c;
   while ((c = fgetc(inputFile)) != EOF) {
       char *code = getCode(root, c);
       int len = strlen(code);
       for (int i = 0; i < len; i++) {
           bitBuffer <<= 1;
           if (code[i] == '1') {
               bitBuffer |= 1;
           }
           bitsWritten++;
           if (bitsWritten == 8) {
               fputc(bitBuffer, outputFile);
               bitBuffer = 0;
               bitsWritten = 0;
           }
       }
   }
   if (bitsWritten > 0) {
       bitBuffer <<= (8 - bitsWritten);
       fputc(bitBuffer, outputFile);
   }
}

float calculateCompressionRatio(FILE *originalFile, FILE *compressedFile) {
   fseek(originalFile, 0L, SEEK_END);
   long originalSize = ftell(originalFile);
   fseek(compressedFile, 0L, SEEK_END);
   long compressedSize = ftell(compressedFile);
   return (float)compressedSize / originalSize;
}

int main() {
   char filename[] = "document.txt";
   FILE *file = fopen(filename, "r");
   if (file == NULL) {
       printf("Error opening file.\n");
       return 1;
   }

   int frequencies[MAX_SYMBOLS] = {0};
   calculateFrequencies(file, frequencies);

   struct MinHeap *minHeap = buildHuffmanTree(frequencies);
   struct Node *root = minHeap->array[0];

   char outputFilename[] = "compressed.bin";
   FILE *outputFile = fopen(outputFilename, "wb");
   if (outputFile == NULL) {
       printf("Error creating output file.\n");
       fclose(file);
       return 1;
   }

   writeEncodedText(file, outputFile, root);

   fclose(file);
   fclose(outputFile);

   // Calculate compression ratio
   file = fopen(filename, "r");
   outputFile = fopen(outputFilename, "rb");
   if (file == NULL || outputFile == NULL) {
       printf("Error opening files for compression ratio calculation.\n");
       return 1;
   }

   float compressionRatio = calculateCompressionRatio(file, outputFile);
   printf("Compression ratio: %.2f\n", compressionRatio);

   fclose(file);
   fclose(outputFile);

   return 0;
}
