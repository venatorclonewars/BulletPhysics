#pragma once
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "math.h"
#include "baseTerrain.h"

class Game
{
public:
	Game();
	~Game();

	void run();
	static void renderScene();

	void initializeBulletPhysics();
	// Terrain stuff
	void initTerrainTextureGenerator();

	// Freeglut stuff
    static void displayNewFrame(int deltaTime);  
    static void keyboardListener(unsigned char key, int mouse_x, int mouse_y);
	static void specialKeyboardListener(int key, int mouse_x, int mouse_y);
	static void mouseListener(int x, int y);
	static void glutCallbacks();
	
	
};

