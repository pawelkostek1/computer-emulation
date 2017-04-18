// This file implements the class for the core processor to be emulated
#include <iostream>
#include <fstream>
#include "core.h"

using namespace std;

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

void CORE::init_reg(){
	PC = 0;
	AC = 0;
	IR = 0;
}

void CORE::runLOAD()
{
	AC = mem[0x1F & IR];
}
void CORE::runSTORE()
{
	mem[0x1F & IR] = AC;
}
void CORE::runADD()
{
	AC += mem[0x1F & IR];
}
void CORE::runSUB()
{
	AC -= mem[0x1F & IR];
}
void CORE::runAND()
{
	AC &= mem[0x1F & IR];
}
void CORE::runOR()
{
	AC |= mem[0x1F & IR];
}
void CORE::runNOT()
{
	AC = ~AC;
}
void CORE::runJUMP()
{
	PC = 0x1F & IR;
}

// Define fetch and execute steps
void CORE::fetch()
{
	// Load the instruction into Instruction Register
	IR = mem[PC];
		
	// Increment program counter at the end of the execution
	PC++;
	
	//cout<<PC<<endl;
}
// We use aproach of incorporating a array of function pointers
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
			runSUB();
			break;
		case 4:
			runAND();
			break;
		case 5:
			runOR();
			break;
		case 6:
			runNOT();
			break;
		case 7:
			runJUMP();
			Run = false;
			break;
	}
}
void CORE::run_cycle()
{
		// Perform instruction fetching according to the program counter
	fetch();
	
	// Perform instruction execute step
	execute();
	
	cout<<(int)mem[19]<<endl;
	
}
	