#include <iostream>
using namespace std;

////////////////////////////////////////////////////////////////////////
// Define all the 8 instructions
enum OpCodes
{
	LOAD, STORE, ADD, SUB, AND, OR, NOT, JUMP
};

// Declare registers
unsigned char PC, IR;

// Define accumulator to be 8-bit signed integer type
signed char AC;

// Declare memory of size 32 words, each with 8 bits length
signed char mem[32];

/////////////////////////////////////////////////////////////////////////

// Define ALU functions
void runLOAD()
{
	AC = mem[0x1F & IR];
}
void runSTORE()
{
	mem[0x1F & IR] = AC;
}
void runADD()
{
	AC += mem[0x1F & IR];
}
void runSUB()
{
	AC -= mem[0x1F & IR];
}
void runAND()
{
	AC &= mem[0x1F & IR];
}
void runOR()
{
	AC |= mem[0x1F & IR];
}
void runNOT()
{
	AC = ~AC;
}
void runJUMP()
{
	PC = 0x1F & IR;
}
///////////////////////////////////////////////////////////////////////////
// Define fetch and execute steps
void fetch()
{
	// Load the instruction into Instruction Register
	IR = mem[PC];
		
	// Increment program counter at the end of the execution
	PC++;
	
	//cout<<PC<<endl;
}
// We use aproach of incorporating a array of function pointers
void (*execute[8])(void) = {
	runLOAD, runSTORE, runADD, runSUB, runAND, runOR, runNOT, runJUMP
};

int main()
{
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

	bool RunCore = true;
	cout<<(int)mem[19]<<endl;
	// Run computer infinitely
	while(RunCore){
		
		// Perform instruction fetching according to the program counter
		fetch();
		
		// The instruction is 8 bit long
		// There is 3 bit opcode and 5 bit address 
		// In the below we shift the instruction accordingly to 
		// obtain the first 3 bits corresponding to the opcode
		(*execute[IR>>5])();
		
		cout<<(int)mem[19]<<endl;
	}
	return 0;
}

