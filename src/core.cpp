// This file implements the class for the core processor to be emulated
#include <iostream>
#include <fstream>
#include "core.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// CLASS FUNCTIONS ////////////////////////////////////
void CORE::load_prog()
{
	// Load the program into memory from file
	unsigned int line;
	int i = 0;
	ifstream myfile ("binary_prog.txt");
	if (myfile.is_open())
	{
		for(int i=1;i<=32;i++){
			myfile>>line;
			//cout<<line<<endl;
			mem[i-1] = line;
		}
	myfile.close();
	}

  else cout << "Unable to load the program" << endl; 
}
void CORE::init()
{
	// Initialize program counter to zero
	PC = 0;
	// Set the variable Run to true to enable cpu
	Run = true;
}
void CORE::runLOAD()
{
	reg[OPERAND1] = mem[reg[OPERAND2] + SIGN_EXTEND(OPERAND3)];
}
void CORE::runSTORE()
{
	mem[reg[OPERAND2] + SIGN_EXTEND(OPERAND3)] = reg[OPERAND1];
}
void CORE::runADD()
{
	if (C)
		reg[OPERAND1] = reg[OPERAND2] + SIGN_EXTEND(OPERAND3);
	else
		reg[OPERAND1] = reg[OPERAND2] + reg[OPERAND3];
}
void CORE::runAND()
{
	if (C)
		reg[OPERAND1] = reg[OPERAND2] & SIGN_EXTEND(OPERAND3);
	else
		reg[OPERAND1] = reg[OPERAND2] & reg[OPERAND3];
}
void CORE::runNOT()
{
	reg[OPERAND1] = ~reg[OPERAND2];
}
void CORE::runSHIFT()
{
	if(C)
		reg[OPERAND1] = reg[OPERAND2] << OPERAND3;
	else
		reg[OPERAND1] = reg[OPERAND2] >> OPERAND3;
}
void CORE::runBRANCH()
{
	if(C)
		PC = (reg[OPERAND1] < reg[OPERAND2]) ? PC + ((SIGN_EXTEND(OPERAND3)) << 1) : PC;
	else
		PC = (reg[OPERAND1] == reg[OPERAND2]) ? PC + ((SIGN_EXTEND(OPERAND3)) << 1) : PC;
}
void CORE::runJUMP()
{
	
	if(C)
	{
		reg[15] = PC;
		PC = JUMP_ADDRESS;
	}
	else
		PC = reg[OPERAND1];
}
// Define fetch step
void CORE::fetch()
{
	// Load the instruction into Instruction Register
	IR = mem[PC];
		
	// Increment program counter at the end of the execution
	PC += 1;
	
}
// Define decode step
void CORE::decode()
{
	OPCODE = BIT_RANGE(IR, 15, 13);
	C = BIT_RANGE(IR, 12, 12);
	OPERAND1 = BIT_RANGE(IR, 11, 8);
	OPERAND2 = BIT_RANGE(IR, 7, 4);
	OPERAND3 = BIT_RANGE(IR, 3, 0);
	JUMP_ADDRESS = BIT_RANGE(IR, 11, 0);
}
// Define execute step
void CORE::execute(){
	
	switch(IR>>5){
		case 0:
			runLOAD();
			break;
		case 1:
			runSTORE();
			break;
		case 2:
			runADD();
			break;
		case 3:
			runAND();
			break;
		case 4:
			runNOT();
			break;
		case 5:
			runSHIFT();
			break;
		case 6:
			runBRANCH();
			break;
		case 7:
			runJUMP();
			break;
	}
}
// Define a function that performs one cycle 
void CORE::run_cycle()
{
	// Perform instruction fetching according to the program counter
	fetch();
	// Peform instruction decoding step
	decode();
	// Perform instruction execute step
	execute();
	
	//cout<<(int)mem[19]<<endl
}
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// SUPPORTING FUNCTIONS ////////////////////////////////////
// Function that returns the value between the given bit range. This is used for decoding.
WORD BIT_RANGE(WORD instr, WORD hi, WORD lo)
{
	WORD mask = (1 << (hi - lo + 1)) - 1;
	return (instr >> lo) & mask;
}
// Function that sign extends the 4-bit immediate number to a  WORD (16-bit number).
WORD SIGN_EXTEND(WORD x)
{
	return (((signed short)x << 12) >> 12);
}
// Function that zero extends the 4-bit immediate number to a  WORD (16-bit number).
WORD ZERO_EXTEND(WORD x)
{
    return x;
}
///////////////////////////////////////////////////////////////////////////////////////////////
