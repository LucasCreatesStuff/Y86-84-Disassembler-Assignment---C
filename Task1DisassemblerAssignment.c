#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // this allows us to utilise booleans in this program

// this program iterates through the inputInstructions array, a list of all of the opcodes which this program is able to print out the instruction for 
// if the program finds an opcode that has been loaded into the instruction array which it is able print out the instruction for, it will print out the corresponding instruction for that opcode - if not it will print out an error message.
// it retrieves the corresponding instruction for this opcode by looking at the index (i) of outputInstructions
// if an opcode is found to match the instruction at index i of the inputInstructions array, then to find its corresponding instruction we just have to look at index i of outputInstructions. Eg. If index i is 1, meaning an opcode was found to match the item at index 1 of the inputInstructions array, then to find its corresponding instruction, we simply have to look at the corresponding position in the outputInstructions array (or vice versa) - in this case that position would be 1, since index i is 1.

int convertStrToByteCode(char *str, unsigned char inst[], int size);

int main(int argc, char **argv) {
  FILE *pFile = NULL;

  char buffer[22];

  if (argc < 2) {
    pFile = fopen("./test1.txt", "r");
  } else {
    pFile = fopen(argv[1], "r");
  }

  if (pFile == NULL) {
    printf("Error open test file, please make sure they exist.\n");

    return 0;
  }
  
  while (fgets(buffer, 22, pFile) != NULL) { //&& strlen(buffer) > 1) {
    // This unsigned char array stores an instruction read from the file
    // As the largest y86 instruction is 10 bytes, there are 10 unsigned char in
    // the array where each represents a byte.
    unsigned char instruction[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    unsigned char inputInstructions[28] = {0x10,0x90,0x60,0x62,0x61, 0x63, 0x20, 0x21, 0x23, 0x22, 0x24, 0x26, 0x25, 0xA0, 0xB0, 0x00, 0x30, 0x50, 0x40, 0x74, 0x73, 0x80, 0x70, 0x75, 0x72, 0x71, 0x76}; // an array of all opcodes that can be converted
    char outputInstructions[][7] = {"nop", "ret", "addq", "andq", "subq", "xorq", "rrmovq", "cmovle", "cmove", "cmovl", "cmovne", "cmovg", "cmovge", "pushq", "popq", "halt", "irmovq", "mrmovq", "rmmovq", "jne", "je", "call", "jmp", "jge", "jl", "jle", "jg"}; // an array of corresponding instructions that can be output
    convertStrToByteCode(buffer, instruction, 10);
    
    bool instructionfound = false; // instructionfound is initially set to false - we use a boolean for this here so that we are able to identfiy if an instructiono has been found or not at any point in the program, allowing us to print an message later on if no instruction has been found at the end of one full search through the inputInstructions array
    
    for (int i = 0;i < 28;i++) // loops 28 times, allowing us to search through all of the opcodes of which we are able to print out their corresponding instruction, which are in the inputInstructions array
      {
        if (instruction[0] == inputInstructions[i]) // if the first byte of the instruction is equal to an instruction in the inputInstructions array...
          {
            instructionfound = true; // the instructionfound flag is now set to true since the instruction has been found
            printf("%s\n", outputInstructions[i]); // the corresponding instruction, found in the outputInstructions array, at the same index (i) where the inputInstruction was found, is output 
          }
        }
    if (instructionfound == false) // if the instruction has not been found at the end of the search, then we print the message below:
    {
      printf("TODO: undisassembled opcode. The first byte of the instruction is 0x%X\n", instruction[0]);
    }
  }
  fclose(pFile);

  return 0; // this allows the file to be eventually closed
}




/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function converts a line of machine code read from the text file
into machine byte code.
The machine code is stored in an unsigned char array.
******************************************************************************/
int convertStrToByteCode(char *str, unsigned char inst[], int size) {
  int numHexDigits = 0;
  char *endstr;
  // Each instruction should consist of at most 20 hex digits
  numHexDigits = strlen(str);

  int byteCount = 0; // numHexDigits >> 1;

  endstr = str;

  while (*endstr != '\n' && *endstr != '\0') {
    char byteValue[3];
    byteValue[0] = *endstr;
    byteValue[1] = *(endstr + 1);
    byteValue[2] = '\0';

    long long value = strtol(byteValue, NULL, 16);
    inst[byteCount++] = value;
    endstr += 2;
  }

  // Return the size of the instruction in bytes
  return numHexDigits >> 1;
}


