#include "core.h"

int main()
{
	// Define a core
	CORE core;
	
	// Load the program into memory from a file
	core.load_prog();
	
	// Initialize cpu
	core.init();
	
	// Run computer infinitely
	while(core.Run)
	{	
		core.run_cycle();
	}
	return 0;
}

