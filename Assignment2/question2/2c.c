#include <stdio.h>
#include <string.h>
#include<stdlib.h>

#define MAX_CHAR 256
#define MAX_CODE_LEN 10

// Structure to store code and character
typedef struct {
  char code[MAX_CODE_LEN];
  char character;
} CodeTableEntry;

// Function to build the code table from the input
int buildCodeTable(CodeTableEntry table[], char characters[], char codes[][MAX_CODE_LEN], int num_codes) {
  for (int i = 0; i < num_codes; i++) {
    strcpy(table[i].code, codes[i]);
    table[i].character = characters[i];
  }
  return 1;
}

// Function to decode the encoded string
char* decodeString(CodeTableEntry table[], char encoded_string[], int num_codes) {
  int curr_pos = 0;
  int decoded_index = 0;
  char* decoded_string = (char*)malloc(strlen(encoded_string) + 1); // Allocate space for decoded string

  while (encoded_string[curr_pos] != '\0') {
    int found = 0;
    for (int i = 0; i < num_codes; i++) {
      // Check if the current encoded substring matches a code in the table
      if (strncmp(encoded_string + curr_pos, table[i].code, strlen(table[i].code)) == 0) {
        decoded_string[decoded_index++] = table[i].character;
        curr_pos += strlen(table[i].code);  // Move to next character
        found = 1;
        break;
      }
    }
    if (!found) {
      // If no match found, handle error (invalid code)
      printf("Error: Invalid code encountered during decoding.\n");
      return NULL;
    }
  }

  decoded_string[decoded_index] = '\0'; // Add null terminator
  return decoded_string;
}

int main() {
  char characters[] = {'a', 'b', 'c', 'd'}; // Replace with actual characters
  char codes[][MAX_CODE_LEN] = {"00", "10", "01", "11"}; // Replace with actual codes (ensure prefix property)
  int num_codes = sizeof(characters) / sizeof(characters[0]);

  CodeTableEntry table[MAX_CHAR];
  buildCodeTable(table, characters, codes, num_codes);

  char encoded_string[] = "00100111"; // Replace with encoded string

  char* decoded_string = decodeString(table, encoded_string, num_codes);
  if (decoded_string != NULL) {
    printf("Decoded String: %s\n", decoded_string);
    free(decoded_string); // Free memory allocated for decoded string
  }

  return 0;
}

