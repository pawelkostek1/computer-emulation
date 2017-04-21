#ifndef __CORE_H
#define __CORE_H

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// DATA TYPES ///////////////////////////////////////////
typedef unsigned short WORD;
typedef unsigned char BYTE;
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// INSTRUCTION SET ARCHITECTURE //////////////////////////////////
//
//                                      ||15|14|13||12||11|10| 9| 8|| 7| 6| 5| 4|| 3| 2| 1| 0||
// REGISTER FORMAT                      ||  OPCODE|| C||   OPERAND1||   OPERAND2||   OPERAND3||
// JUMP FORMAT                          ||  OPCODE|| C||                         JUMP_ADDRESS||
//
// LD load a word                       || 0| 0| 0|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
// ST store a word                      || 0| 0| 1|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// ADD with conrol bit C = 0            || 0| 1| 0|| 0||   ADDRESS1||   ADDRESS2||   ADDRESS3||
// ADD with conrol bit C = 1            || 0| 1| 0|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// AND with conrol bit C = 0            || 0| 1| 1|| 0||   ADDRESS1||   ADDRESS2||   ADDRESS3||
// AND with conrol bit C = 1            || 0| 1| 1|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// NOT bitwise complement               || 1| 0| 0|| 0||   ADDRESS1||   ADDRESS2|| 1| 1| 1| 1||
//
// SHF with C = 0 -> RIGHT SHIFT        || 1| 0| 1|| 0||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
// SHF with C = 1 -> LEFT SHIFT         || 1| 0| 1|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// BR with C = 0 -> BRANCH ON EQUAL     || 1| 1| 0|| C||   OPERAND1||   OPERAND2||   OPERAND3||
// BR with C = 1 -> BRANCH ON LESS THAN || 1| 1| 0|| C||   OPERAND1||   OPERAND2||  IMMEDIATE||
//
// JMP with C = 0 -> JUMP REGISTER      || 1| 1| 1|| 0||   ADDRESS1|| 0| 0| 0| 0|| 0| 0| 0| 0||
// JMP with C = 1 -> JUMP               || 1| 1| 1|| 1||                         JUMP_ADDRESS||
//
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// REGISTER TOPOLOGY /////////////////////////////////////
// 
// |NAME|NUMBER|             COMMENT              |
// | $at|     0|               Assembler Temporary|
// | $vr|     1|     Value return by the procedure|
// | $ag|   2-4|  Argument passed to the procedure|
// | $tm|  5-13|                   Temporary value|
// | $sp|    14|                     Stack pointer|
// | $ra|    15| Return address from the procedure|
// 
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// MEMORY CONVENTION /////////////////////////////////////
// 
// |STARTING ADDRESS|                        COMMENT                         |
// |          0x0000|     Space dedicated to program running on the processor|
// |          0x8000|  Space for storing static variables used by the program|
// |          0xC000| Space for storing dynamic variables used by the program|
// 
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// OPCODES /////////////////////////////////////////////
// Enumerate 8 instructions' opcodes
enum OpCodes
{
	LD, ST, ADD, AND, NOT, SHF, BR, JMP 
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// CLASS CORE ///////////////////////////////////////////
class CORE{
	
	// Declare registers
	WORD PC, IR;
	WORD reg[16];
	
	// Declare 16 kbytes memory (65536 words), each with 16-bits length.
	WORD mem[0x10000];
	
	// Declare  variables used for instruction decoding.
	WORD OPCODE, C, OPERAND1, OPERAND2, OPERAND3, JUMP_ADDRESS;
	
	public:
		// 	This variable is responsible for running the core.
		bool Run;
		
		// Function that loads the program from the file into into the memory.
		void load_prog();
		
		// This function initializes registers 
		// and set the Run variable to true to enable cpu.
		void init();
		
		//  Declare ALU functions.
		void runLOAD();
		void runSTORE();
		void runADD();
		void runAND();
		void runNOT();
		void runSHIFT();
		void runBRANCH();
		void runJUMP();
		
		// Declare fetch, decode and execute steps.
		void fetch();
		void decode();
		void execute();
		
		// Declare a function that perfoms a one cpu cycle.
		void run_cycle();
};
///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// SUPPORTING FUNCTIONS /////////////////////////////////////////
// Function that returns the value between the given bit range. This is used for decoding.
WORD BIT_RANGE(WORD instr, WORD hi, WORD lo);
// Function that sign extends the 4-bit immediate number to a  WORD (16-bit number).
WORD SIGN_EXTEND(WORD x);
// Function that zero extends the 4-bit immediate number to a  WORD (16-bit number).
WORD ZERO_EXTEND(WORD x);
///////////////////////////////////////////////////////////////////////////////////////////////
#endif