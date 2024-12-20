#include <GL/glew.h>
#include <GL/freeglut.h>
#include "game.h"


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

    Game* game = new Game();
    game->run();

    return 0;
}