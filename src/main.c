#include "globals.h"
#include "cgb/gameboy.h"
#include "tools.h"

#include <stdlib.h>

//temp for debuggin
#include "debug.h"

int main(int argc, char** argv) {

	//for debugging
	int i;
	for(i = 0; i < 0xFF; ++i) {
		executed[0][i] = 0;
		executed[1][i] = 0;
	}

	
	if(debug_mode) {
		enable_debug_console();
	}

	fprintf(stdout, "welcome to cgbemu - debug mode is %s\n", debug_mode ? "enabled" : "disabled");

	gameboy_power_on();
	
    return 0;
}