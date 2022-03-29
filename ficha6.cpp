#include<stdio.h>
#include<stdlib.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

#include <IL/il.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif


float camX = 100, camY = 150, camZ = 400;
int startX, startY, tracking = 0;

int alpha = 0, beta = 45, r = 50;

GLuint buffers; 
std::vector<float> vertexB;

unsigned int t, tw, th;
unsigned char *imageData;

void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective
	gluPerspective(45,ratio,1,1000);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


// tree height
// h1 = (1 - Fz) . h(x1, z1) + Fz (x1, z2)
// h2 = (1 - Fz) . h(x2, z1) + Fz (x2, z2)
// h = (1 - Fx) . h1 + Fx . h2


float height(int i, int j){
	return imageData[i * tw + j];
}

void drawTerrain() {
    // colocar aqui o código de desnho do terreno usando VBOs com TRIANGLE_STRIPS
	glBegin(GL_TRIANGLE_STRIP);
    
	float x = -127.5;
	float z = -127.5;
	int i, j;

	i = 0; j = 0;
	while(z < 127.5){
    		while(x <= 127.5){
			glColor3f(0.1 + 0.01*i, 0, 0);
			float h1 = height(i, j);
			float h2 = height(i, j + 1);
    			glVertex3f(x, h1, z);
    			glVertex3f(x, h2, z + 1);
			x += 1;
			i++;
	    	}
		z += 1;
		x = -127.5;
		j++;
		i = 0;
	}
	glEnd();
}


void renderScene(void) {
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT, GL_LINE);
	glLoadIdentity();
	gluLookAt(camX, camY, camZ, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	glBindBuffer(GL_ARRAY_BUFFER, buffers);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexB.size(), vertexB.data(), GL_STATIC_DRAW);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	for(int i = 0; i < th - 1; i++){
		glColor3f(1, 0.002 * i, 0);
		glDrawArrays(GL_TRIANGLE_STRIP, tw * 2 * i, tw * 2);
	}

	// End of frame
	glutSwapBuffers();
}



void processKeys(unsigned char key, int xx, int yy) {
	// put code to process regular keys in here
	if (key == 27)
		exit(0);
}


void processMouseButtons(int button, int state, int xx, int yy) {
	if (state == GLUT_DOWN)  {
		startX = xx;
		startY = yy;
		if (button == GLUT_LEFT_BUTTON)
			tracking = 1;
		else if (button == GLUT_RIGHT_BUTTON)
			tracking = 2;
		else
			tracking = 0;
	}
	else if (state == GLUT_UP) {
		if (tracking == 1) {
			alpha += (xx - startX);
			beta += (yy - startY);
		}
		else if (tracking == 2) {
			r -= yy - startY;
			if (r < 3)
				r = 3.0;
		}
		tracking = 0;
	}
}


void processMouseMotion(int xx, int yy) {
	int deltaX, deltaY;
	int alphaAux, betaAux;
	int rAux;

	if (!tracking)
		return;

	deltaX = xx - startX;
	deltaY = yy - startY;

	if (tracking == 1) {
		alphaAux = alpha + deltaX;
		betaAux = beta + deltaY;

		if (betaAux > 85.0)
			betaAux = 85.0;
		else if (betaAux < -85.0)
			betaAux = -85.0;
		rAux = r;
	}
	else if (tracking == 2) {
		alphaAux = alpha;
		betaAux = beta;
		rAux = r - deltaY;
		if (rAux < 3)
			rAux = 3;
	}
	camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
	camY = rAux * sin(betaAux * 3.14 / 180.0);
}


void prepareData(){
	float x = -127.5;
	float z = -127.5;
	int i, j;

	i = 0; j = 0;
	while(z < 127.5){
    		while(x <= 127.5){
			float h1 = height(i, j);
			float h2 = height(i, j + 1);
			
			vertexB.push_back(x);
			vertexB.push_back(h1);
			vertexB.push_back(z);

			vertexB.push_back(x);
			vertexB.push_back(h2);
			vertexB.push_back(z + 1);

			x += 1;
			i++;
	    	}
		z += 1;
		x = -127.5;
		j++;
		i = 0;
	}
}


void init() {
	ilInit();

// 	Load the height map "terreno.jpg"
	ilGenImages(1, &t);
	ilBindImage(t);

	ilLoadImage((ILstring) "terreno.jpg");
	ilConvertImage(IL_LUMINANCE, IL_UNSIGNED_BYTE);

	tw = ilGetInteger(IL_IMAGE_WIDTH);
	th = ilGetInteger(IL_IMAGE_HEIGHT);

	imageData = ilGetData();

// 	Build the vertex arrays
	prepareData();
	glGenBuffers(1, &buffers);

// 	OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP6");
	
	glEnableClientState(GL_VERTEX_ARRAY);

// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);

// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutMouseFunc(processMouseButtons);
	glutMotionFunc(processMouseMotion);

#ifndef __APPLE__
	glewInit();
#endif
	init();


// enter GLUT's main cycle
	glutMainLoop();
	
	return 0;
}

