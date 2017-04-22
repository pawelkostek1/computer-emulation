#include "core.h"
#include <iostream>


int main(int argc, char *argv[])
{
	// Define a core
	CORE core;
	
	// Load the program into memory from a file
	if(argc >= 2)
	{
		core.load_prog(argv[1]);
	}
	else
	{
		std::cout<<"Assembly program file was not provided.\n";
	}
	
	// Initialize cpu registers
	core.init_reg();
	
	// Run the core
	core.Run = true;
	
	// Run computer infinitely
	while(core.Run)
	{	
		core.run_cycle();
	}
	return 0;
}

