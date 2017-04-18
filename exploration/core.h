#ifndef __CORE_H
#define __CORE_H

////////////////////////////////////////////////////////////////////////
// Define all the 8 instructions
enum OpCodes
{
	LOAD, STORE, ADD, SUB, AND, OR, NOT, JUMP
};

///////////////////////////////////////////////////////////////////////////

class CORE{
	
	// Declare registers
	unsigned char PC, IR, AC;

	// Declare memory of size 32 words, each with 8 bits length
	unsigned char mem[32];
	
	public:
	
	// Responsible for running the core
	bool Run;
	
	void load_prog();
	
	void init_reg();
	
	//  Declare ALU functions
	void runLOAD();
	void runSTORE();;
	void runADD();
	void runSUB();
	void runAND();
	void runOR();
	void runNOT();
	void runJUMP();
	
	// Declare fetch and execute steps
	void fetch();
	// We use aproach of incorporating a array of function pointers
	void execute();
	void run_cycle();
		
};

#endif