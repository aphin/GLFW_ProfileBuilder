#include <GLFW/glfw3.h>		// Need GLFW 3.0 or newer. Change library name if it is wrong
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "profb.h"			// Header file

struct GLFWvidmode *current_vmode;
GLFWwindow* window;

char *coo_file;
char *data_file;

// Flags
	unsigned char MCursor=1;	// mouse cursor form: 0 - invisible, 1 - cross, 2 - vert. arrows, 3 - horiz. arrows, 4 - box
	unsigned char fDisplayUnSelected=1;	// 1 - displays all, 0 - displays only selected
// ---

// Data
	float data[86400][2];	// experimental data according time in seconds - 0, "selected" status for each point - 1
	float coord[86400][3];	// coordinates: N, E and altitude
// ---

void Draw ()
{
	char txt[256];
	int i;

	glClear(GL_COLOR_BUFFER_BIT);

//window borders drawing
	glLineWidth(7);
	glGray(0.3);
	glBegin(GL_LINES);
	glVertex2f(wd_x1,1.0);
	glVertex2f(wd_x1,wd_y1);
	glVertex2f(-1.0,wd_y1);
	glVertex2f(1.0,wd_y1);
	glEnd();

	glLineWidth(5);
	glGray(0.5);
	glBegin(GL_LINES);
	glVertex2f(wd_x1,1.0);
	glVertex2f(wd_x1,wd_y1);
	glVertex2f(-1.0,wd_y1);
	glVertex2f(1.0,wd_y1);
	glEnd();

	glLineWidth(3);
	glGray(0.8);
	glBegin(GL_LINES);
	glVertex2f(wd_x1,1.0);
	glVertex2f(wd_x1,wd_y1);
	glVertex2f(-1.0,wd_y1);
	glVertex2f(1.0,wd_y1);
	glEnd();

//---

//mouse cursor drawing
	glBlack;
	glLineWidth(1);
	glBegin(GL_LINES);
	glVertex2f(mx,my-0.02*arat);
	glVertex2f(mx,my+0.02*arat);
	glVertex2f(mx+0.02,my);
	glVertex2f(mx-0.02,my);
	glEnd();
	glLineWidth(3);
	glBegin(GL_LINES);
	glVertex2f(mx,my-0.01*arat);
	glVertex2f(mx,my+0.01*arat);
	glVertex2f(mx+0.01,my);
	glVertex2f(mx-0.01,my);
	glEnd();
//---

	glfwSwapBuffers(window);

	Redisplay = 0;
}

void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
    if ((action == GLFW_PRESS)||(action == GLFW_REPEAT)) switch (key)
    {
        case GLFW_KEY_Q:
        {
            if (mods==GLFW_MOD_CONTROL) glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        }
        default:
			keycode=key;
	}
	Redisplay = 1;
}

void mousemove(GLFWwindow *wnd, double x, double y)
{
    mx = ((float)x / (screen_width/2)) - 1.0F; my = 1.0F-((float)y / (screen_height/2));
    mix = (int)round(x); miy = (int)round(y);
    Redisplay=1;
}

void mousepress(GLFWwindow *wnd, int button, int state, int mods)
{
}

void mouseweelscroll(GLFWwindow *wnd, double xoffset, double yoffset)
{
}

void print_usage()
{
  printf("Usage: glfw <coordinates_file> <data_file>\n");
}

int main (int argc, char *argv[])	// MAIN
{
    FILE *f;
    int i=0;

//    if (argc!=3)
//    {
//      print_usage();
//      exit(0);
//    }
//   coo_file=argv[1];
//    data_file=argv[2];

    load_chr_font("DRFT.CHR");

    if (!glfwInit())
        exit(EXIT_FAILURE);
    current_vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screen_width = current_vmode->width;
    screen_height = current_vmode->height;
    arat = (float)screen_width / (float)screen_height;
    window = glfwCreateWindow(screen_width, screen_height, "PROFB", glfwGetPrimaryMonitor(), NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mousepress);
    glfwSetCursorPosCallback(window, mousemove);
    glfwSetScrollCallback(window, mouseweelscroll);

    glClearColor(1.0,1.0,1.0,0);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        if (Redisplay==1)
        {
            Draw();
            Redisplay=0;
        }
    }
    glfwDestroyWindow(window);
    glfwTerminate();

	return 0;
}
