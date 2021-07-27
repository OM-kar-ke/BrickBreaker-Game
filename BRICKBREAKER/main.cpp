

#include <iostream>
#include <GL/glut.h>

#include "BrickBreaker.h"


BrickBreaker game;

// Define the display function
void myDisplay()
{
	game.display();
}



// Define keystroke events
void myKeyStroke(unsigned char key, int x, int y)
{
	game.keyStroke(key, x, y);
}

void mySpecialKeyStroke(int key, int x, int y)
{
	game.specialKeyPos(key, x, y);
}

// The main function here
int main(int argc, char ** argv)
{
	// Init glut
	glutInit(&argc, argv);


	// Init window size, position, title
	glutInitWindowSize(WINWIDTH, WINHEIGHT);        //700*500
	glutInitWindowPosition(100, 100);
	glutCreateWindow(WINTITLE);



	// Draw scene
    	
    	
	// Handle keyboard strokes
	glutKeyboardFunc(myKeyStroke);
	// specify keyboard special key input events
	glutSpecialFunc(mySpecialKeyStroke);
	
	//display function
	glutDisplayFunc(myDisplay);
	// Enter the opengl event processing loop
	glutMainLoop();
	
	return 0;
}
