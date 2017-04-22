// This file implements the class for the core processor to be emulated
#include <iostream>
#include <fstream>
#include <bitset>
#include "core.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// CLASS FUNCTIONS //////////////////////////////////////
void CORE::load(char* file)
{
	// Load the program into memory from file
	WORD line;
	ifstream myfile(file);
	if (myfile.is_open())
	{
		for(int i=1; i<=512; i++)
		{
			myfile >> line;
			mem[i-1] = line;
		}
	myfile.close();
	}
	else 
		cout << "Unable to load the program.\n" << endl; 
}
void CORE::save()
{
	// Save a memory state into the text file
	ofstream myfile;
	myfile.open("memory_state.txt");
	
	for (int i=0; i<512; i++)
	{
		bitset<16>x(mem[i]);
		myfile << "0b" << x <<" "<< (signed short) mem[i] << "\n";
	}
	myfile.close();
}
void CORE::init()
{
	// Initialize program counter to zero
	PC = 0;
	//Initialize Global Pointer $GP to its corresponding initial address
	reg[13] = 0x100;
	// Initialize Stack Pointer $SP to its corresponding initial address
	reg[14] = 0x180;
	// Initialize Stack Pointer $AT, $VR and $RA to zero
	reg[0] = 0;
	reg[1] = 0;
	reg[15] = 0;
	// Set the variable Run to true to enable cpu
	Run = true;
}
void CORE::runLOAD()
{
	reg[OPERAND1] = mem[(WORD)(reg[OPERAND2] + SIGN_EXTEND(OPERAND3))];
}
void CORE::runSTORE()
{
	mem[(WORD)(reg[OPERAND2] + SIGN_EXTEND(OPERAND3))] = reg[OPERAND1];
}
void CORE::runADD()
{
	if (C)
		reg[OPERAND1] = (WORD)(reg[OPERAND2] + SIGN_EXTEND(OPERAND3));
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
	if(C)
		Run = false;
	else
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
		PC = (reg[OPERAND1] > reg[OPERAND2]) ? PC + (SIGN_EXTEND(OPERAND3)) : PC;
	else
		PC = (reg[OPERAND1] == reg[OPERAND2]) ? PC + (SIGN_EXTEND(OPERAND3)) : PC;
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
	//cout<<"Decoded instruction: "<<OPCODE<<" "<<C<<" "<<OPERAND1<<" "<<OPERAND2<<" "<<OPERAND3<<" "<<JUMP_ADDRESS<<endl;
}
// Define execute step
void CORE::execute(){
	
	switch(OPCODE){
		case LD:
			runLOAD();
			break;
		case ST:
			runSTORE();
			break;
		case ADD:
			runADD();
			break;
		case AND:
			runAND();
			break;
		case NOT:
			runNOT();
			break;
		case SHF:
			runSHIFT();
			break;
		case BR:
			runBRANCH();
			break;
		case JMP:
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
	
	//Print some of the registers values for debugging
	//cout<<"PC: "<<PC<<endl;
	//cout<<"$VR "<<reg[1]<<endl;
	//cout<<"$AG0 "<<reg[2]<<endl;
	//cout<<"$TM0 "<<reg[5]<<endl;
	//cout<<"$TM1 "<<reg[6]<<endl;
	//cout<<"$TM2 "<<reg[7]<<endl;
	//cout<<"$TM3 "<<reg[8]<<endl;
	//cout<<"$TM4 "<<reg[9]<<endl;
	//cout<<"$TM5 "<<reg[10]<<endl;
	//cout<<"$TM6 "<<reg[11]<<endl;
	//cout<<"$SP "<<reg[14]<<endl;
	//cout<<"$RA "<<reg[15]<<endl;
	//cout<<"Stack0: "<<mem[0x181]<<endl;
	//cout<<"Stack1: "<<mem[0x182]<<endl;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// SUPPORTING FUNCTIONS ///////////////////////////////////////
// Function that returns the value between the given bit range. This is used for decoding.
WORD BIT_RANGE(WORD instr, WORD hi, WORD lo)
{
	WORD mask = (1 << (hi - lo + 1)) - 1;
	return (instr >> lo) & mask;
}
// Function that sign extends the 4-bit immediate number to a  WORD (16-bit number).
WORD SIGN_EXTEND(WORD x)
{
	WORD y = ((x >> 3)==1 ? -(0b0111&x) : (0b0111&x));
	return y;
}
// Function that zero extends the 4-bit immediate number to a  WORD (16-bit number).
WORD ZERO_EXTEND(WORD x)
{
    return x;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
