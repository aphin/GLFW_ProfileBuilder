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

void Draw ()
{
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_QUADS);
	glVertex2f(-1.0,1.0);
	glVertex2f(1.0,1.0);
	glVertex2f(1.0,-1.0);
	glVertex2f(-1.0,-1.0);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(mx,my);
	glVertex2f(mx,my-0.01);
	glVertex2f(mx,my);
	glVertex2f(mx-0.01, my+0.005);
	glVertex2f(mx,my);
	glVertex2f(mx+0.01, my+0.005);
	glEnd();
	Redisplay = 0;
}

void key_callback(GLFWwindow* wnd, int key, int scancode, int action, int mods)
{
}

void mousemove(GLFWwindow *wnd, double x, double y)
{
    mx = ((float)x / (screen_width/2)) - 1.0F; my = 1.0F-((float)y / (screen_height/2));
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

    if (argc!=3)
    {
      print_usage();
      exit(0);
    }
    coo_file=argv[1];
    data_file=argv[2];

    load_chr_font("DRFT.CHR");

    if (!glfwInit())
        exit(EXIT_FAILURE);
    current_vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screen_width = current_vmode->width;
    screen_height = current_vmode->height;
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
