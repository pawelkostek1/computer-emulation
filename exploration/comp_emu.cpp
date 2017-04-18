#include <iostream>
using namespace std;

// Define all the 8 instructions
enum OpCodes
{
	LOAD, STORE, ADD, SUB, AND, OR, NOT, JUMP
};

int main()
{
	// Declare registers
	unsigned char PC, AC, IR;
	
	// Declare memory of size 32 words, each with 8 bits length
	unsigned char mem[32];
	
	// Load the program into memory
	// (The program is hardcoded in this version)
	mem[0] = (LOAD<<5) | 16; // Load A located at 16th memory location
	mem[1] = (ADD<<5) | 17; // Add B located at the 17th memory location
	mem[2] = (STORE<<5) | 19; // Store the summation at the 19th memory location
	mem[3] = (AND<<5) | 18; // Zero out the accumulator by bitwise AND with 0 stored at location 18
	mem[4] = (JUMP<<5); // Jump to location zero and begin the program sequence again 
	mem[16] = 3; // Define A to be 3
	mem[17] = 7; // Define B to be 7
	mem[18] = 0; // Define  C to be 0
	mem[19] = 0; // Initialize memory location 19 to zero
	
	// Initialize cpu registers
	PC = 0;
	AC = 0;
	IR = 0;
	
	//for(int i=0; i<5;i++)
	//cout<< (int)mem[i]<<endl;

	bool RunCore = true;

	// Run computer infinitely
	while(RunCore){
		
		// Load the instruction into Instruction Register
		IR = mem[PC];
		
		// Increment program counter at the end of the execution
		PC++;
		
		//cout <<(int)PC<<endl;
		//cout<<(int)(IR>>5)<<endl;
		//cout<<(int)(0x1F & IR)<<endl<<endl;
		
		// The instruction is 8 bit long
		// There is 3 bit opcode and 5 bit address 
		// In the below we shift the instruction accordingly to 
		// obtain the first 3 bits corresponding to the opcode
		// and later mask it to get the 5bit address
		switch(IR>>5){
			case 0:
				AC = mem[0x1F & IR];
				break;
			case 1:
				mem[0x1F & IR] = AC;
				break;
			case 2:
				AC += mem[0x1F & IR];
				break;
			case 3:
				AC -= mem[0x1F & IR];
				break;
			case 4:
				AC &= mem[0x1F & IR];
				break;
			case 5:
				AC |= mem[0x1F & IR];
				break;
			case 6:
				AC = ~AC;
				break;
			case 7:
				PC = 0x1F & IR;
				//RunCore = false;
				break;
		}
		
		cout<<(int)mem[19]<<endl;
		

		
		//if (PC>5) RunCore = false;
	}
	return 0;
}