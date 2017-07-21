#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#define NDEBUG
#include <GL/freeglut.h>

#include "Rectangle.h"
#include "Circle.h"

const int GAME_LOGIC_REFRESH_TIME = 10;
using namespace std;

MF::Circle ball(0.6, 1.0, 0.0, 0.0);
MF::Rectangle paddle(10.0, 1.0, 1.0, 1.0, 0.0);
vector<MF::Rectangle>blocks;
vector<MF::Rectangle>walls;

vector<MF::Rectangle>blocks1;
int licznik;
bool speed = false;


/* GLUT callback Handlers */
void resize(int width, int height)
{
	const float ar = (float)width / (float)height;

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);
	gluLookAt(0, 0, 45, 0, 0, 0, 0, 1, 0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void passiveMouseMotion(int mouse_x, int mouse_y)
{
	double windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	double windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	double mouse_gl_x = (((double)mouse_x - (windowWidth / 2)) / windowWidth) * ((windowWidth / windowHeight) * 45);
	//double mouse_gl_y = -(((double)mouse_y - (windowHeight / 2))/ windowHeight)*((windowWidth / windowHeight) * 25);
	//paddle.SetPosition(0, mouse_gl_y);

	paddle.SetPosition(mouse_gl_x, -15);
}

void MouseMotion(int mouse_x, int mouse_y)
{
	double windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	double windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	double mouse_gl_x = -(((double)mouse_x - (windowWidth / 2)) / windowWidth) * ((windowWidth / windowHeight) * 45);
	paddle.SetPosition(mouse_gl_x, -15);
}

void gameLogic(int value)
{
	if (value == 0)
	{
		ball.SetInitialParameters(glutGet(GLUT_ELAPSED_TIME), 0.02, 60, 0.0, -90);

	}
	else
	{
		ball.Collision(paddle);

		for (auto itr = walls.begin(); itr != walls.end(); itr++)
		{
			ball.Collision(*itr);
		}

		for (auto itr = blocks.begin(); itr != blocks.end();)
		{
			if (ball.Collision(*itr))
			{
				licznik++;
				printf("Punkty: %d", licznik);
				printf("\n");

				glutPassiveMotionFunc(passiveMouseMotion);
				itr = blocks.erase(itr);
			}
			else itr++;
		}

		for (auto itr = blocks1.begin(); itr != blocks1.end();)
		{
			if (ball.Collision(*itr))
			{
				licznik++;
				printf("Punkty: %d", licznik);
				printf("\n");

				glutPassiveMotionFunc(MouseMotion);
				itr = blocks1.erase(itr);
			}
			else itr++;
		}

		if (licznik >= 10 && speed == false)
		{
			ball.Fast(0.02);
			speed = true;
		}
		
		ball.Update(glutGet(GLUT_ELAPSED_TIME));
	}

	glutTimerFunc(GAME_LOGIC_REFRESH_TIME, gameLogic, 1);
}

void idle()
{
	glutPostRedisplay();
}

void display()
{
	// clear the scene
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	{
		ball.ChangeColour(1, 0, 1);
		ball.Draw();
		paddle.ChangeColour(1, 0, 0);
		paddle.Draw();

		for (auto itr = blocks.begin(); itr != blocks.end(); itr++)
		{
			itr->Draw();
		}

		for (auto itr = blocks1.begin(); itr != blocks1.end(); itr++)
		{
			itr->Draw();
		}

		for (auto itr = walls.begin(); itr != walls.end(); itr++)
		{
			itr->Draw();
		}
	}
	glPopMatrix();

	glutSwapBuffers();
}

void keyboard(unsigned char key_pressed, int mouse_x, int mouse_y)
{
	switch (key_pressed)
	{
	case 'a':case'A':
	{
		paddle.Move(-0.5, 0.0);
		break;
	}
	case 'D':case'd':
	{
		paddle.Move(0.5, 0.0);
		break;
	}
	}
}
/* helper functions for settings options and parameters */
void InitGLUTScene(char* window_name)
{
	glutInitWindowSize(1200, 1000);
	glutInitWindowPosition(40, 40);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	glutCreateWindow(window_name);

	glClearColor(1, 1, 1, 1);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}
void SetCallbackFunctions()
{
	glutReshapeFunc(resize);
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(passiveMouseMotion);
	glutTimerFunc(GAME_LOGIC_REFRESH_TIME, gameLogic, 0);
}

void SetObjectsPositions()
{
	paddle.SetPosition(0, -15);
	ball.SetPosition(0, -8);
	double blockWidth = 3.0;
	double blocHeight = 1.5;

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			double r = rand() % 100 / 100.0;
			double g = rand() % 100 / 100.0;
			double b = rand() % 100 / 100.0;

			MF::Rectangle newBlock(blockWidth, blocHeight, r, g, b);
			newBlock.SetPosition(-13 + blockWidth*j, 10 - blocHeight*i);
			blocks.push_back(newBlock);
		}
	}
	for (int w = 0; w < 1; w++)
	{
		for (int k = 0; k < 10; k++)
		{
			MF::Rectangle block1(blockWidth, blocHeight, 1, 0, 0);
			block1.SetPosition(-13 + blockWidth*k, 11.5 - blocHeight*w);
			blocks1.push_back(block1);
		}
	}

	//sciana dol
	{
		MF::Rectangle newWall(100, 2, 0.5, 0.5, 0.5);
		newWall.SetPosition(0, -22);
		walls.push_back(newWall);
	}

	//sciana gora
	{
		MF::Rectangle newWall(100, 2, 0.5, 0.5, 0.5);
		newWall.SetPosition(0, 22);
		walls.push_back(newWall);
	}

	//sciana lewa
	{
		MF::Rectangle newWall(2, 100, 0.5, 0.5, 0.5);
		newWall.SetPosition(-29, 0);
		walls.push_back(newWall);
	}

	//sciana prawa
	{
		MF::Rectangle newWall(2, 100, 0.5, 0.5, 0.5);
		newWall.SetPosition(29, 0);
		walls.push_back(newWall);
	}

}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	glutInit(&argc, argv);
	InitGLUTScene("Arkanoid");

	SetObjectsPositions();
	SetCallbackFunctions();


	glutMainLoop();

	return 0;
}