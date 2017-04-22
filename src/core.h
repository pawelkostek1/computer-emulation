#ifndef __CORE_H
#define __CORE_H

///////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// DATA TYPES ///////////////////////////////////////////
typedef unsigned short WORD;
typedef unsigned char BYTE;
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// INSTRUCTION SET ARCHITECTURE //////////////////////////////////
//
//                                ||15|14|13||12||11|10| 9| 8|| 7| 6| 5| 4|| 3| 2| 1| 0||
// REGISTER FORMAT                ||  OPCODE|| C||   OPERAND1||   OPERAND2||   OPERAND3||
// JUMP FORMAT                    ||  OPCODE|| C||                         JUMP_ADDRESS||
//
// LD   -> LOAD A WORD            || 0| 0| 0|| 0||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// ST   -> STORE A WORD           || 0| 0| 1|| 0||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// ADD  -> ADD                    || 0| 1| 0|| 0||   ADDRESS1||   ADDRESS2||   ADDRESS3||
// ADDI -> ADD IMMEDIATE          || 0| 1| 0|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// AND  -> AND                    || 0| 1| 1|| 0||   ADDRESS1||   ADDRESS2||   ADDRESS3||
// ANDI -> AND IMMEDIATE          || 0| 1| 1|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// NOT -> BITWISE COMPLEMENT      || 1| 0| 0|| 0||   ADDRESS1||   ADDRESS2|| 0| 0| 0| 0||
// NOTR -> NOT RUN (STOP CORE)    || 1| 0| 0|| 1|| 0| 0| 0| 0|| 0| 0| 0| 0|| 0| 0| 0| 0||
//
// RSHF -> RIGHT SHIFT            || 1| 0| 1|| 0||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
// LSHF -> LEFT SHIFT             || 1| 0| 1|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// BRE  -> BRANCH ON EQUAL        || 1| 1| 0|| 0||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
// BRMT -> BRANCH ON MORE THAN    || 1| 1| 0|| 1||   ADDRESS1||   ADDRESS2||  IMMEDIATE||
//
// JMPR -> JUMP REGISTER          || 1| 1| 1|| 0||   ADDRESS1|| 0| 0| 0| 0|| 0| 0| 0| 0||
// JMP  -> JUMP                   || 1| 1| 1|| 1||                         JUMP_ADDRESS||
//
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// REGISTER TOPOLOGY /////////////////////////////////////
// 
// |   NAME   |NUMBER|              COMMENT             |
// |       $AT|     0|               Assembler Temporary|
// |       $VR|     1|     Value Return by the procedure|
// | $AG0-$AG3|   2-4|  ArGument passed to the procedure|
// | $TM0-$TM7|  5-12|                   TeMporary value|
// |       $GP|    13|                    Global Pointer|
// |       $SP|    14|                     Stack Pointer|
// |       $RA|    15| Return Address from the procedure|
// 
///////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// MEMORY CONVENTION /////////////////////////////////////
// 
// |STARTING ADDRESS|                        COMMENT                         |
// |           0x000|     Space dedicated to program running on the processor|
// |           0x100|  Space for storing static variables used by the program|
// |           0x180| Space for storing dynamic variables used by the program|
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
	
	// Declare program counter and instruction register.
	WORD PC, IR;
	// Declare 16-bit registers.
	WORD reg[16];
	
	// Declare 1 kbyte of memory (512 words), each with 16-bits length.
	WORD mem[512];
	
	// Declare  variables used for instruction decoding.
	WORD OPCODE, C, OPERAND1, OPERAND2, OPERAND3, JUMP_ADDRESS;
	
	public:
		// 	This variable is responsible for running the core.
		bool Run;
		
		// Function that loads the program from the file into into the memory.
		void load(char* file);
		
		// Function that saves the memory state after program execution into the text file.
		void save();
		
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