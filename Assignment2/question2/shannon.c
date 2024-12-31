#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHAR 256

typedef struct {
    char symbol;
    int frequency;
} CharFreq;

// Function to calculate frequency of characters
void calculateFrequency(FILE *file, int frequency[]) {
    int c;
    while ((c = fgetc(file)) != EOF) {
        frequency[c]++;
    }
}

// Function to sort characters by frequency
void sortByFrequency(CharFreq chars[], int size) {
    int i, j;
    CharFreq temp;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            if (chars[j].frequency < chars[j + 1].frequency) {
                temp = chars[j];
                chars[j] = chars[j + 1];
                chars[j + 1] = temp;
            }
        }
    }
}

// Function to calculate Shannon-Fano compression ratio
float calculateCompressionRatio(CharFreq chars[], int size, int frequency[]) {
    int i;
    float originalSize = 0, compressedSize = 0;
    
    for (i = 0; i < size; i++) {
        originalSize += frequency[(int)chars[i].symbol] * 8; // 8 bits per character
        compressedSize += frequency[(int)chars[i].symbol] * (size - i); // Variable length codes
    }
    
    return originalSize / compressedSize;
}

int main() {
    FILE *file;
    char filename[100];
    int frequency[MAX_CHAR] = {0};
    
    printf("Enter the filename: ");
    scanf("%s", filename);
    
    file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return 1;
    }
    
    calculateFrequency(file, frequency);
    fclose(file);
    
    CharFreq chars[MAX_CHAR];
    int i, size = 0;
    for (i = 0; i < MAX_CHAR; i++) {
        if (frequency[i] != 0) {
            chars[size].symbol = (char)i;
            chars[size].frequency = frequency[i];
            size++;
        }
    }
    
    sortByFrequency(chars, size);
    
    printf("Character\tFrequency\n");
    for (i = 0; i < size; i++) {
        printf("%c\t\t%d\n", chars[i].symbol, chars[i].frequency);
    }
    
    float compressionRatio = calculateCompressionRatio(chars, size, frequency);
    printf("Shannon-Fano Compression Ratio: %.2f\n", compressionRatio);
    
    return 0;
}

