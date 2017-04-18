#include <iostream>
#include <fstream>
#include <bitset>
using namespace std;

enum OpCodes
{
	LOAD, STORE, ADD, SUB, AND, OR, NOT, JUMP
};

int main()
{
	char mem[32] = {0};
	
	mem[0] = (LOAD<<5) | 16; // Load A located at 16th memory location
	mem[1] = (ADD<<5) | 17; // Add B located at the 17th memory location
	mem[2] = (STORE<<5) | 19; // Store the summation at the 19th memory location
	mem[3] = (AND<<5) | 18; // Zero out the accumulator by bitwise AND with 0 stored at location 18
	mem[4] = (JUMP<<5); // Jump to location zero and begin the program sequence again 
	mem[16] = 3; // Define A to be 3
	mem[17] = 7; // Define B to be 7
	mem[18] = 0; // Define  C to be 0
	mem[19] = 0; // Initialize memory location 19 to zero
	
	ofstream myfile;
	myfile.open("binary_prog.txt");
	
	for (int i=0; i<32; i++){
		bitset<8>x(mem[i]);
		cout<<x<<endl;
		myfile <<(unsigned int) mem[i]<<"\n";
	}
	myfile.close();
	
	return 0;
}

