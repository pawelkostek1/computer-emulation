#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

enum OpCodes
{
	LD, ST, ADD, AND, NOT, SHF, BR, JMP 
};

int main()
{
	// Define memory for three programs
	unsigned short mem[0x200] = {0};
	unsigned short mem2[0x200] = {0};
	unsigned short mem3[0x200] = {0};
	
	// Define a simple program using Assembly for testing instructions
	mem[0] = (LD<<13) | 5<<8 | 13<<4;      // Load A:                     LD $TM1, $GP, 0
	mem[1] = (LD<<13) | 6<<8 | 13<<4 | 1;  // Load B:                     LD $TM2, $GP, 1
	mem[2] = (ADD<<13) | 7<<8 | 5<<4 | 6;  // Add A and B:                ADD $TM3, $TM1, $TM2
	mem[3] = (AND<<13) | 8<<8 | 5<<4 | 6;  // Execute bitwise A AND B:    AND $TM4, $TM1, $TM2
	mem[4] = (NOT<<13) | 9<<8 | 5<<4;      // Execute NOT A:              NOT $TM5, $TM1
	mem[5] = (SHF<<13) | 10<<8 | 6<<4 | 2; // Execute B >> 2:             RSHF $TM6, $TM2, 2
	mem[6] = (ST<<13) | 7<<8 | 13<<4 | 2;  // Store the sum in C:         ST $TM3, $GP, 2
	mem[7] = (ST<<13) | 8<<8 | 13<<4 | 3;  // Store the bitwise AND in D: ST $TM4, $GP, 3
	mem[8] = (ST<<13) | 9<<8 | 13<<4 | 4;  // Store the NOT A in E:       ST $TM5, $GP, 4
	mem[9] = (ST<<13) | 10<<8 | 13<<4 | 5; // Store the NOT A in F:       ST $TM6, $GP, 
	mem[10] = (NOT<<13) | 1<<12;		   // Stop the processor:         NOTR
	
	mem[0x100] = 3;                        // Define A to be 3.
	mem[0x101] = 127;                      // Define B to be 127.
	mem[0x102] = 0;                        // Set C initially to be 0.
	mem[0x103] = 0;                        // Set D initially to be 0.
	mem[0x104] = 0;                        // Set E initially to be 0.
	mem[0x105] = 0;                        // Set F initially to be 0.
	
	// Define a program using Assembly for multiplication
	mem2[0] = LD<<13 | 5<<8 | 13<<4;               // Load ZERO:                 LD $TM0, $GP, 0
	mem2[1] = LD<<13 | 6<<8 | 13<<4| 1;            // Load A:                    LD $TM1, $GP, 1
	mem2[2] = LD<<13 | 7<<8 | 13<<4| 2;            // Load B:                    LD $TM2, $GP, 2
	mem2[3] = LD<<13 | 8<<8 | 13<<4| 3;            // Load Q:                    LD $TM3, $GP, 3
	mem2[4] = LD<<13 | 9<<8 | 13<<4| 4;            // Load N:                    LD $TM4, $GP, 4
	mem2[5] = AND<<13| 1<<12| 10<<8| 8<<4| 1;      // Execute Q0 = Q AND 0x0001: ANDI $TM5, $TM3, 1
	mem2[6] = BR<<13 | 10<<8| 5<<4 | 1;            // Execute BRANCH if Q0==0:   BRE $TM5, $TM0, 1
	mem2[7] = ADD<<13| 6<<8 | 6<<4 | 7;            // Else A = A + B:            ADD $TM1, $TM1, $TM2
	mem2[8] = SHF<<13| 1<<12| 7<<8 | 7<<4| 1;      // Execute B << 1:            LSHF $TM2, $TM2, 1
	mem2[9] = SHF<<13| 8<<8 | 8<<4 | 1;            // Execute Q >> 1:            RSHF $TM3, $TM3, 1
	mem2[10] = ADD<<13| 1<<12| 9<<8 | 9<<4|0b1001; // Decrement by 1 N--:        ADDI $TM4, $TM4, -1
	mem2[11] = BR<<13 | 9<<8 | 5<<4 | 1;           // Execute BRANCH if(N==0):   BRE $TM4, $TM0, 1
	mem2[12] = JMP<<13| 1<<12| 5;		           // Else if N!=0 repeat cycle: JMP 5
	mem2[13] = ST<<13 | 6<<8 | 13<<4| 1;           // Store the A into memory:   ST $TM1, $GP, 1
	mem2[14] = NOT<<13| 1<<12;		               // Stop the processor:        NOTR
	
	mem2[0x100] = 0;                    // Define a Constant Value ZERO.
	mem2[0x101] = 0;                    // Set A (Product) initially to be 0.
	mem2[0x102] = 12;                   // Set B (Multiplicand) to be 12.
	mem2[0x103] = 5;                    // Set Q (Multipliplier) to be 5.
	mem2[0x104] = 16;                   // Set N to be 16.
	
	// Define a program using Assembly for calculating recursively 1+2+...+N
	mem3[0] = LD<<13 | 5<<8 | 13<<4;                 // Load ZERO:                 LD $TM0, $GP, 0
	mem3[1] = LD<<13 | 6<<8 | 13<<4| 1;              // Load ONE:                  LD $TM1, $GP, 1
	mem3[2] = LD<<13 | 2<<8 | 13<<4| 2;              // Load N into Argument Reg:  LD $AG0, $GP, 2
	mem3[3] = JMP<<13| 1<<12| 6;		             // Go to the fact procedure:  JMP 6
	mem3[4] = ST<<13 | 1<<8 | 13<<4| 3;              // Store fact output:         ST $VR, $GP, 3
	mem3[5] = NOT<<13| 1<<12;		                 // Stop the processor:        NOTR
	mem3[6] = ADD<<13| 1<<12| 14<<8| 14<<4| 2;       // Adjust stack for 2 items:  ADDI $SP, $SP, 2
	mem3[7] = ST<<13 | 2<<8 | 14<<4;                 // Save the argument:         ST $AG0, $SP, 0
	mem3[8] = ST<<13 | 15<<8| 14<<4| 0b1001;         // Save the return address:   ST $RA, $SP, -1
	mem3[9] = BR<<13 | 1<<12| 2<<8 | 5<<4 | 3;       // Execute BRANCH if(N>0):    BRMT $AG0, $TM0, 3
	mem3[10] = ADD<<13| 1<<12| 1<<8 | 9<<4| 0;       // Return 0:                  ADDI $VR, $TM0, 0
	mem3[11] = ADD<<13| 1<<12| 14<<8|14<<4| 0b1010;  // Pop 2 items of the stack:  ADDI $SP, $SP, -2
	mem3[12] = JMP<<13| 15<<8;		                 // Return to caller:          JMPR $RA
	mem3[13] = ADD<<13| 1<<12| 2<<8 | 2<<4| 0b1001;  // Else decrement by 1 N--:   ADDI $AG0, $AG0, -1
	mem3[14] = JMP<<13| 1<<12| 6;		             // Go to the fact procedure:  JMP 6
	mem3[15] = LD<<13 | 15<<8| 14<<4| 0b1001;        // Restore return address:    LD $RA, $SP, -1
	mem3[16] = LD<<13 | 2<<8 | 14<<4| 0;             // Restore argument N:        LD $AG0, $SP, 0
	mem3[17] = ADD<<13| 1<<12| 14<<8| 14<<4| 0b1010; // Pop 2 items from stack:    ADDI $SP, $SP, -2
	mem3[18] = ADD<<13| 1<<8 | 2<<4 | 1;             // Return N + (N-1):          ADD $VR, $AG0, $VR
	mem3[19] = JMP<<13| 15<<8;		                 // Return to caller:          JMPR $RA
	
	mem3[0x100] = 0;                    // Define a Constant Value ZERO.
	mem3[0x101] = 1;                    // Define a Constant Value ONE.
	mem3[0x102] = 5;                    // Define procedure ardument n to be 4.
	mem3[0x103] = 0;                    // Here, the calculated factiorial will be stored.
	
	// Generate an assembly test files for comp_emu
	ofstream instr_test, mult, rec_sum;
	instr_test.open("instruction_test.txt");
	mult.open("multiplication.txt");
	rec_sum.open("recurrent_sum.txt");
	
	
	for (int i=0; i<512; i++){
		bitset<16>x(mem[i]);
		bitset<16>y(mem2[i]);
		bitset<16>z(mem3[i]);
		cout<<x<<" "<<y<<" "<<z<<endl;
		instr_test <<(unsigned short) mem[i]<<"\n";
		mult <<(unsigned short) mem2[i]<<"\n";
		rec_sum <<(unsigned short) mem3[i]<<"\n";
	}
	instr_test.close();
	mult.close();
	rec_sum.close();
	
	return 0;
}

