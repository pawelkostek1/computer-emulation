#include "core.h"
#include <iostream>

int main(int argc, char *argv[])
{
	// Define a core.
	CORE core;
	
	// Load the program into memory from a file.
	if(argc >= 2)
	{
		core.load(argv[1]);
	}
	else
	{
		std::cout<<"Assembly program file was not provided.\n";
	}
	
	// Initialize the processor.
	core.init();

	while(core.Run)
	{	
		core.run_cycle();
	}
	
	// Store the memory state into a text file.
	core.save();
	
	return 0;
}

