#include "core.h"


int main()
{
	// Define a core
	CORE core;
	
	// Load the program into memory from a file
	core.load_prog();
	
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

