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
	float data[86400][2];	// experimental data according time in seconds
	float coord[86400][3];	// coordinates: N, E and altitude
	char selected[86400]; // "selected" status of each point (0 - unselected, 1 -selected, -1 - no data for point)
	float min_coo_x;
	float max_coo_x;
	float min_coo_y;
	float max_coo_y;
	float min_coo_h;
	float max_coo_h;
// ---

void Draw ()
{
	char txt[256];
	int i;
	float x;
	float y;

	glClear(GL_COLOR_BUFFER_BIT);

// Coordinates and time strip drawing
	for (i=0;i<86400;i++)
	{
		x = -1.0F+(((float)screen_width/86400)*(float)i*px);
		if ((i % 3600)==0)
		{
			sprintf(txt,"%d",i/3600);
			glLineWidth(1);
			glBlack;
			DrawText(txt,Font,x,-0.99,0.1,0.02);
		}
		glPointSize(5);
		if (selected[i] != -1)
		{
			if (selected[i]==1) glRed(1.0f); else glGray(0.5f);
			glBegin(GL_POINTS);
			glVertex2f(x,wd_y1 - (wd_y1+1.0)/2); // point on time strip
			glVertex2f(-1.0F+((coord[i][0]-min_coo_x)/(max_coo_x-min_coo_x))*(1.0F+wd_x1),1.0F-((coord[i][1]-min_coo_y)/(max_coo_y-min_coo_y))*(1.0F-wd_y1));
			glEnd();
		}
		glPointSize(1);
	}

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
	switch (MCursor)
	{
		case 1:	// cross
		{
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
			break;
		}
		case 2:	// vertical arrows
		{
			glBlack;
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex2f(mx,my-0.02*arat);
			glVertex2f(mx,my+0.02*arat);
			glVertex2f(mx-0.005,my-0.015*arat);
			glVertex2f(mx,my-0.02*arat);
			glVertex2f(mx+0.005,my-0.015*arat);
			glVertex2f(mx,my-0.02*arat);
			glVertex2f(mx-0.005,my+0.015*arat);
			glVertex2f(mx,my+0.02*arat);
			glVertex2f(mx+0.005,my+0.015*arat);
			glVertex2f(mx,my+0.02*arat);
			glEnd();
			break;
		}
		case 3:	// horizontal arrows
		{
			glBlack;
			glLineWidth(1);
			glBegin(GL_LINES);
			glVertex2f(mx-0.02,my);
			glVertex2f(mx+0.02,my);
			glVertex2f(mx-0.015,my-0.005*arat);
			glVertex2f(mx-0.02,my);
			glVertex2f(mx-0.015,my+0.005*arat);
			glVertex2f(mx-0.02,my);
			glVertex2f(mx+0.015,my-0.005*arat);
			glVertex2f(mx+0.02,my);
			glVertex2f(mx+0.015,my+0.005*arat);
			glVertex2f(mx+0.02,my);
			glEnd();
			break;
		}
		case 4:
		{
			glBlack;
			glLineWidth(1);
			glBegin(GL_LINE_LOOP);
			glVertex2f(mx-0.02,my-0.02*arat);
			glVertex2f(mx+0.02,my-0.02*arat);
			glVertex2f(mx+0.02,my+0.02*arat);
			glVertex2f(mx-0.02,my+0.02*arat);
			glEnd();
			break;
		}
	}
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
    if (glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) switch (MCursor)
    {
		case 2:
		{
			wd_y1 = my;
			break;
		}
		case 3:	wd_x1 = mx;
	}
    if ((mx > (wd_x1-(3*px)))&&(mx < (wd_x1+(3*px)))&&(my > (wd_y1+(3*py)))) MCursor = 3;
    else if ((my < (wd_y1+(3*py)))&&(my > (wd_y1-(3*py)))) MCursor = 2;
    else MCursor = 1;
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

    for (i=0;i<86400;i++)
    {
		coord[i][0]=0.0f;
		coord[i][1]=0.0f;
		coord[i][2]=0.0f;
		data[i][0] = 0.0f;
		data[i][1] = 0.0f;
		selected[i]=-1;
	}

    f = fopen(coo_file,"rt");
    if (f == NULL)
    {
		printf("Error opening file %s\n",coo_file);
		exit(0);
	}
	i = 0;
	while (!feof(f))
	{
		int snum;
		float n;
		float e;
		float h;
		fscanf(f,"%d\t%f\t%f\t%f\n",&snum,&n,&e,&h);
		coord[snum][0] = n;
		coord[snum][1] = e;
		coord[snum][2] = h;
		if (i == 0)
		{
			max_coo_x = n;
			max_coo_y = e;
			max_coo_h = h;
			min_coo_x = n;
			min_coo_y = e;
			min_coo_h = h;
			i = 1;
		}
		else
		{
			if (max_coo_x < n) max_coo_x = n;
			if (max_coo_y < e) max_coo_y = e;
			if (max_coo_h < h) max_coo_h = h;
			if (min_coo_x > n) min_coo_x = n;
			if (min_coo_y > e) min_coo_y = e;
			if (min_coo_h > h) min_coo_h = h;
		}
		selected[snum] = 1;
	}
	fclose(f);

	f = fopen(data_file,"rt");
	if (f == NULL)
	{
		printf("Error opening file %s\n",data_file);
		exit(0);
	}
	i = 0;
	while (!feof(f))
	{
		fscanf(f,"%f\t%f\n",&data[i][0],&data[i][1]);
		i++;
	}
	fclose(f);

    if (!glfwInit())
        exit(EXIT_FAILURE);
    current_vmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    screen_width = current_vmode->width;
    screen_height = current_vmode->height;
    arat = (float)screen_width / (float)screen_height;
    px = 2.0F / screen_width;
    py = 2.0F / screen_height;
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
