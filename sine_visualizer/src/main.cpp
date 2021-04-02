// Support Code written by Michael D. Shah
// Last Updated: 1/21/17
// Please do not redistribute without asking permission.

// Functionality that we created
#include "SDLGraphicsProgram.h"
#include <iostream>

int main(int argc, char** argv){

	std::cout << "(main.cpp) Beginning Intitialization\n";

	int w, h;
	std::cout << "Please select your window dimensions\n\tWidth: ";
	std::cin >> w;
	std::cout << "\tHeight: ";
	std::cin >> h;

	// Create an instance of an object for a SDLGraphicsProgram
	// and run loop forever
	if (argc == 1) {
		// Use first to create a terrain map
		SDLGraphicsProgram mySDLGraphicsProgram(w, h);
		mySDLGraphicsProgram.loop();
	}
	else if (argc == 2) {
		// Textures have not yet been tested
		SDLGraphicsProgram mySDLGraphicsProgram(w, h, argv[1]);
		mySDLGraphicsProgram.loop();
	} else {
		SDLGraphicsProgram mySDLGraphicsProgram(w, h, argv[1], argv[2]);
		mySDLGraphicsProgram.loop();
	}
	
	// When our program ends, it will exit scope, the
	// destructor will then be called and clean up the program.
	return 0;
}
