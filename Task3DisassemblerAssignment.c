#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h> // this allows us to utilise booleans in this program

// this program iterates through the inputInstructions array, a list of all of the instructions which this program is able to disassemble
// if the program finds an instruction that has been loaded into the instruction array which it is able to disassemble, it will disassemble it - if not it will print out an error message.
// it retrieves this disassembled instruction by looking at the index (i) of outputInstructions
// if an instruction is found to match the instruction at index i of the inputInstructions array we then have to find its disassembled instruction. To do this, we just have to look at index i of outputInstructions. Eg. If index i is 1, meaning an instruction was found to match the item at index 1 of the inputInstructions array, then to find its disassembled instruction, we simply have to look at the corresponding position in the outputInstructions array (or vice versa) - in this case that position would be 1, since index i is 1.
// once we have found the instruction, we find the registers which it has and calculate its immediate value, if it has one, then print out the disassembled instruction to the screen in its specific formatting
// this is why specific instructions are grouped together in the outputInstructions array - it allows us to print out all of the instructions in their specific formatting, while additionally minimising the amount of conditional checks that we are performing
// this program disassembles these instructions based on the position and advancement of the program counter 

//This is the help function that reads y86 binary code from a file
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize);

int main(int argc, char **argv)
{
  unsigned int MAX_MEMSIZE = 4096;
  unsigned char memory[MAX_MEMSIZE]; //This array represents the 4KB memory space
  unsigned int PC = 0;               //This is the initial program counter address

  if (argc < 2)
  {
    printf("Usage: please specify a y86 binary program file in the argument.\n");
    return -1;
  }

  unsigned int program_size = loadBinFromFile(argv[1], memory, MAX_MEMSIZE);

  if (program_size == 0)
    return 0;

  //This is an array of register mnemonics in y86
  const char *register_names[] =
      {
          "%rax", // here when you are looking at registers you are able to use the index to get the correct one, since they are in order / correspond to the numbers that the registers are actually represented by
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

    unsigned char inputInstructions[14] = {0x30, 0x60, 0x63, 0x62, 0x61, 0x26, 0x40, 0x50, 0x80, 0x74, 0x70, 0x00, 0x10, 0x90}; // an array of all the opcodes that can be converted
    
    unsigned char outputInstructions[][7] = {"irmovq", "addq", "xorq", "andq", "subq", "cmovg", "rmmovq", "mrmovq", "call", "jne", "jmp", "halt", "nop", "ret"}; // an array with all of the instructions that the disassembler is able to output

bool instructionDecoded = false; // a boolean variable to keep track of if an instruction has been successfully decoded or not
   
  while (PC < program_size) // while the program counter is less than the program_size 
  {
    for(int i = 0; i<14; i++) // this for loop executes 14 times, incrementing the value of i by 1 at the end of each execution
      // it loops through the list of available instructions to check if the one that we are reading from the file is the same as the one at the current position (index i) of the inputInstructions array
      {
        if (memory[PC] == inputInstructions[i]) //if the instruction currently stored in the program counter is equal to the instruction at index[i] of the inputInstructions array...
        {
          instructionDecoded = true; // the boolean instruction decoded is set to true
          if (i>=11 && i<=13) // if the value of i is greater than or equal to 11 and less than or equal to 13... (if the instruction is halt, nop or ret)
          {
            // printing out these instructions in their unique formatting 
            printf("%s\n", outputInstructions[i]); // print the corresponding instruction, with its specific formatting, that should be output
            PC +=1; // increment the value of the program counter by 1 since the instruction that was decoded was 1 byte long
          }
          else if(i == 0) // if i is equal to 0... (if the instruction is equal to irmovq)
          {
            // obtaining all the information required to print this instruction out in its unique formatting
            int registerB = memory[PC+1] % 0x10; // finding the second register of the instruction
            long long value = memory[PC+2] + (memory[PC + 3] << 8) + (memory[PC+4] << 16) + (memory[PC+5] << 24) + ((long long)memory[PC+6] << 32) + ((long long)memory[PC +7] << 40) + ((long long)memory[PC+8] << 48) + ((long long)memory[PC+9] >> 56); // calculating the immediate value of the instruction
            printf("%s $%lld %s\n", outputInstructions[i], value, register_names[registerB]); // print the corresponding instruction, with its specific formatting, that should be output
            PC +=10; // the value of the program counter needs to be incremented by 10, since the length of the instruction that we just decoded was 10 bytes
          }
          else if(i>= 1 && i<=5) // if the value of i is greater than or equal to 1 and less than or equal to 5... (if the instruction is equal to addq, xorq, andq, subq, cmovg)
          {
            int registerA = memory[PC+1] >> 4; // finding the first register for the instruction
            int registerB = memory[PC+1] % 0x10; // finding the second register for the instruction

            printf("%s %s %s\n", outputInstructions[i], register_names[registerA], register_names[registerB]); // print the corresponding instruction, with its specific formatting, that should be output 
            PC+=2; // the value of the program counter needs to be incremented by 2, since the length of the instruction that we just decoded was 2 bytes
          }
          else if(i == 6) // otherwise, if the value of i is equal to 6... (the instruction is equal to rmmovq)
          {
            int registerA = memory[PC+1] >> 4; // finding the first register for the instruction
            int registerB = memory[PC+1] % 0x10; // finding the second register for the instruction
    
            long long value = memory[PC+2] + (memory[PC + 3] << 8) + (memory[PC+4] << 16) + (memory[PC+5] << 24) + ((long long)memory[PC+6] << 32) + ((long long)memory[PC +7] << 40) + ((long long)memory[PC+8] << 48) + ((long long)memory[PC+9] >> 56); // calculating the immediate value of the instruction

            printf("%s %s, %lld (%s)\n"  ,outputInstructions[i], register_names[registerA], value, register_names[registerB]); // printing the corresponding instruction, with its specific formatting, that should be output 
            
            PC+=10; // the value of the program counter needs to be incremented by 10, since the length of the instruction that we just decoded was 10 bytes
          }
          else if(i == 7) // otherwise, if the value of i is equal to 7 (the instruction is equal to mrmovq)
          {
            int registerA = memory[PC+1] >> 4; // finding the first register for the instruction
            int registerB = memory[PC+1] % 0x10; // finding the second register for the instruction

            long long value = memory[PC+2] + (memory[PC + 3] << 8) + (memory[PC+4] << 16) + (memory[PC+5] << 24) + ((long long)memory[PC+6] << 32) + ((long long)memory[PC +7] << 40) + ((long long)memory[PC+8] << 48) + ((long long)memory[PC+9] >> 56); //calculating the immediate value of the instruction

            printf("%s %lld (%s), %s\n", outputInstructions[i], value, register_names[registerB], register_names[registerA]); // printing the corresponding instruction, with its specific formatting, that should be output 
            PC+=10; // the program counter must be incremented by 10, since the length of the instruction that we just decoded was 10 bytes 
          }
          else if(i>=8 && i<= 10) // if the value of i is greater than or equal to 8 and less than or equal to 10... (if the instruction is equal to call, jne or jmp)
          {
            long long value = memory[PC+2] + (memory[PC + 3] << 8) + (memory[PC+4] << 16) + (memory[PC+5] << 24) + ((long long)memory[PC+6] << 32) + ((long long)memory[PC +7] << 40) + ((long long)memory[PC+8] << 48); // calculating the immediate value of the instruction
            printf("%s %lld\n", outputInstructions[i], value); // printing the corresponding instruction, with its specific formatting, that should be output 
            PC+=9; // we must increment the value of the program counter by 9, since the length of the instruction that we just decoded was 9 bytes
          }
        }
  }
    if (instructionDecoded == false) // if the boolean instructionDecoded is equal to false
    {
      return 0; // an instruction has not been found / cannot be decoded by the disassembler, therefore the program will terminate
    }

    instructionDecoded = false; // if the program reaches here, that means an instruction has already been decoded - therefore, we must revert this flag back to false, like it was at the beginning. This ensures that the program still functions as intended once it loops again. If this boolean was not reset to false, the program would go through the next search not finding a single instruction, as it would only be able to search properly if this boolean was set to false.
  }

  return 0; // automatically terminates the program once the value of the program counter is greater than or equal to the value of the program size
}

/****************************************************************************
N.B. You do not need to modify or work in this function.
Description:
This function reads in a y86 machine bytecode from file and
store them in an unsigned char array.
******************************************************************************/
unsigned int loadBinFromFile(const char *filename, unsigned char memory[], unsigned int memsize)
{
  unsigned int bytes_read = 0;

  unsigned int file_size = 0;

  FILE *pfile = fopen(filename, "rb");

  if (!pfile)
  {
    printf("Unable to load file %s, please check if the path and name are correct.\n", filename);
    return 0;
  }

  fseek(pfile, 0, SEEK_END);
  file_size = ftell(pfile);
  rewind(pfile);



  if (file_size > memsize)
  {
    printf("Program size exceeds memory size of %d.\n", memsize);
    return 0;
  }

  bytes_read = fread(memory, 1, file_size, pfile);

  if (bytes_read != file_size)
  {
    printf("Bytes read does not match the file size.\n");
    return 0;
  }

  fclose(pfile);

  return bytes_read;
}

