#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> // this allows us to utilise booleans in this program

// this program iterates through the inputInstructions array, a list of all of the instructions which this program is able to disassemble
// if the program finds an instruction that has been loaded into the instruction array which it is able to disassemble, it will disassemble it - if not it will print out an error message.
// it retrieves this disassembled instruction by looking at the index (i) of outputInstructions
// if an instruction is found to match the instruction at index i of the inputInstructions array, then to find its disassembled instruction we just have to look at index i of outputInstructions. Eg. If index i is 1, meaning an instruction was found to match the item at index 1 of the inputInstructions array, then to find its disassembled instruction, we simply have to look at the corresponding position in the outputInstructions array (or vice versa) - in this case that position would be 1, since index i is 1.
// once we have found the instruction, we find the registers which it has and calculate its immediate value, if it has one, then print out the disassembled instruction to the screen in its specific formatting
// this is why specific instructions are grouped together in the outputInstructions array - it allows us to print out all of the instructions in their specific formatting, while additionally minimising the amount of conditional checks that we are performing


//This is an array of register mnemonics in y86
const char *register_names[] =
    {
        "%rax", // here when you are looking at registers you are able to use the index to get the correct one, since they are in order / correspond to the numbers that the registers are actually represented by - if a register is unknown then "UNKNOWN_REGISTER" will be printed
        "%rcx",
        "%rdx",
        "%rbx",
        "%rsp",
        "%rbp",
        "%rsi",
        "%rdi",
        "%r8",
        "%r9",
        "%r10",
        "%r11",
        "%r12",
        "%r13",
        "%r14",
        "UNKNOWN_REGISTER"};

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

    unsigned char inputInstructions[28] = {0x10,0x90,0x00,0x62,0x61, 0x63, 0x20, 0x21, 0x23, 0x22, 0x24, 0x26, 0x25, 0x60, 0x50, 0x40, 0x74, 0x73, 0x80, 0x70, 0x75, 0x72, 0x71, 0x76, 0x30, 0xA0, 0xB0}; // an array of all opcodes that can be converted
    char outputInstructions[][7] = {"nop", "ret", "halt", "andq", "subq", "xorq", "rrmovq", "cmovle", "cmove", "cmovl", "cmovne", "cmovg", "cmovge", "addq", "mrmovq", "rmmovq", "jne", "je", "call", "jmp", "jge", "jl", "jle", "jg", "irmovq", "pushq", "popq"}; // an array of corresponding commands that can be output

    convertStrToByteCode(buffer, instruction, 10);

    bool instructionfound = false; // the boolean instructionfound initially set to false. Once an instruction has been found, this boolean will be set to true.

      for (int i = 0;i < 28;i++) // this loops 28 times, so it can search through all of the instructions in the array - i is incremented by 1 at the end of every loop
        {
          if (instruction[0] == inputInstructions[i]) // if the first byte of the instruction is equal to an instruction in the inputInstructions array...
            {
              if (i <= 2) // if the value of i is less than or equal to 2 (if the instruction is equal to nop, ret or halt)
              {
                printf("%s\n", outputInstructions[i]); // print the corresponding disassembled instruction, at the same index (i) where the inputInstruction   was located, is output.
              }
              else if(i >=3 && i <= 13) // otherwise, if the value of i is greater than or equal to 3 and the value of i is less than or equal to 13... (if the instruction is equal to andq, subq, xorq, rrmovq, cmovle, cmove, cmovl, cmovne, cmovg, cmovge, addq)
              {
                int registerA = instruction[1] >> 4; // finding the first register for the instruction
                int registerB = instruction[1] % 0x10; // finding the second register for the instruction
                printf("%s, %s, %s \n", outputInstructions[i], register_names[registerA], register_names[registerB]); // outputs the instruction in its specific format
              }
              else if(i == 14) // otherwise, if the value of i is equal to 14... (mrmovq)
              {
                int registerA = instruction[1] >> 4; // finding the first register for the instruction
                int registerB = instruction[1] % 0x10; // finding the second register for the instruction
                long long value = (instruction[2]) + (instruction[3] << 8)  + (instruction[4] << 16) + ((long long) instruction[5] << 24) + ((long long)instruction[6] << 32) + ((long long)instruction[7] << 40) + ((long long)instruction[8] << 48) + ((long long)instruction[9] << 56); //calculating the immediate value for the instruction
                printf("%s, %lld(%s), %s \n", outputInstructions[i], value ,register_names[registerB], register_names[registerA]); // outputs the instruction in its specific format
              }
              else if(i == 15) // if the value of i is equal to 15... (the instruction is equal to rmmovq)
              {
                int registerA = instruction[1] >> 4; // finding the first register for the instruction
                int registerB = instruction[1] % 0x10; // finding the second register for the instruction
                long long value = (instruction[2]) + (instruction[3] << 8)  + (instruction[4] << 16) + ((long long) instruction[5] << 24) + ((long long)instruction[6] << 32) + ((long long)instruction[7] << 40) + ((long long)instruction[8] << 48) + ((long long)instruction[9] << 56); // calculating the immediate value for the instruction
                printf("%s %s, %lld(%s) \n", outputInstructions[i], register_names[registerA], value, register_names[registerB]); // outputs the corresponding disassembled instruction, at the same index (i) where the inputInstruction was located
              }
              else if(i >=16 && i <= 23) // if the value of i is greater than or equal to 16 and if the value of i is less than or equal to 23... (if the instruction is equal to jne, je, call, jmp, jge, jl, jle, jg) 
              {    
                long long value = (instruction[1]) + (instruction[2] << 8) + (instruction[3] << 16)  + (instruction[4] << 24) + ((long long) instruction[5] << 32) + ((long long)instruction[6] << 40) + ((long long)instruction[7] << 48) + ((long long)instruction[8] << 56); // calculating the immediate value for this instruction
                printf("%s %lld\n",  outputInstructions[i], value); // outputs the corresponding disassembled instruction, at the same index (i) where the inputInstruction was located
              }
              else if(i == 24) // if the value of i is equal to 24... (if the instruction is equal to irmovq)
              {
                int registerB = instruction[1] % 0x10; // finding the second register for the instruction
                long long value = (instruction[2]) + (instruction[3] << 8)  + (instruction[4] << 16) + ((long long) instruction[5] << 24) + ((long long)instruction[6] << 32) + ((long long)instruction[7] << 40) + ((long long)instruction[8] << 48) + ((long long)instruction[9] << 56); // calculating the immediate value for the instruction
                printf("%s $%lld %s\n", outputInstructions[i], value, register_names[registerB]); // outputs the corresponding disassembled instruction, at the same index (i) where the inputInstruction was located
              }
              else if(i == 25 || i == 26) // if the value of i is equal to 25 or the value of i is equal to 26... (the instruction is equal to pushq or popq)
              {
                int registerA = instruction[1] >> 4; // finding the first register for the instruction
                printf("%s, %s\n", outputInstructions[i], register_names[registerA]); // outputs the corresponding disassembled instruction, at the same index (i) where the inputInstruction was located
              }
              instructionfound = true; // the instructionfound flag is now set to true since the instruction has been found
            }
          }
      if (instructionfound == false) // if the instruction has not been found at the end of the search, then we print the message below:
      {
        printf("TODO: undisassembled opcode and operands. The first byte of the instruction is 0x%X\n", instruction[0]);
      }
    }

  fclose(pFile);

  return 0; // this eventually allows for the program to be terminated
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