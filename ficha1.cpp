#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>


void changeSize(int w, int h)
{
	// prevent a divide by zero, when window is too short
	// (you can not make a window with zero width).
	if (h == 0)
		h = 1;
	// compute window's aspect ratio
	float ratio = w * 1.0f / h;

	// set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load the identity matrix
	glLoadIdentity();
	// set the perspective
	gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);

	// et the viewport to be the entire window
	glViewport(0, 0, w, h);
}

void axis_system(){
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

	glBegin(GL_LINES);
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(3, 0, 0);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 3, 0);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 3);

	glEnd();
}

float size = 1;
int aumentar = 0;

void renderScene(void)
{
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// set camera
	glLoadIdentity();
	gluLookAt(5.0f, 3.0f, 5.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 1.0f, 0.0f);
	
	// put drawing instructions here
	
	float g_time = glutGet(GLUT_ELAPSED_TIME) / 1000.f;
	float frequency = 0.5;
	float scale = sin(frequency * 2.0 * M_PI * g_time) * 0.5 + 1.0;

	axis_system();
	glColor3f(1.0, 1.0, 1.0);
	glutWireTeapot(scale);
	printf("scale: %f\n", scale);
	/*
	if(aumentar == 0){
		printf("size: %f\n", size);
		size -= 0.015;
		if(size < 0.5) aumentar = 1;
	}
	else{
		printf("size: %f\n", size);
		size += 0.015;
		if(size > 1.5) aumentar = 0;
	}*/
	
	
	// End of frame
	glutSwapBuffers();
}




void printInfo() {

	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));
}


int main(int argc, char** argv){
	// put GLUT init here
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("CG@ficha1");
	
    
	// put callback registry here
	glutReshapeFunc(changeSize);
	glutIdleFunc(renderScene); 
	glutDisplayFunc(renderScene);
		
	
	// some OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	
	// enter GLUTs main cycle
	glutMainLoop();
	
	return 1;
}
