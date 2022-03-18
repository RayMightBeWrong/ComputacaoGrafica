#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>

float alfa = 0.0f, beta = 0.5f, radius = 100.0f;
float camX, camY, camZ;


void spherical2Cartesian() {
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}


void changeSize(int w, int h) {

	// Prevent a divide by zero, when window is too short
	// (you cant make a window with zero width).
	if(h == 0)
		h = 1;

	// compute window's aspect ratio 
	float ratio = w * 1.0 / h;

	// Set the projection matrix as current
	glMatrixMode(GL_PROJECTION);
	// Load Identity Matrix
	glLoadIdentity();
	
	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set perspective
	gluPerspective(45.0f ,ratio, 1.0f ,1000.0f);

	// return to the model view matrix mode
	glMatrixMode(GL_MODELVIEW);
}


void drawTerrain(){
	glColor3f(0.2f, 0.8f, 0.2f);
	glBegin(GL_TRIANGLES);
		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);

		glVertex3f(100.0f, 0, -100.0f);
		glVertex3f(-100.0f, 0, 100.0f);
		glVertex3f(100.0f, 0, 100.0f);
	glEnd();
}

void drawTorus(){
	glColor3f(0.8f, 0.0f, 0.8f);
	glutSolidTorus(1.2f, 3.0f, 64, 64);
}

void drawInnerTeapotCircle(float angle){
	int teapotNr = 8;
	float radius = 8.5;
	float teapotAngleDeg = 360 / teapotNr;
	float angleDegInc = teapotAngleDeg;
	float newAngleDeg = angle * 360 / (M_PI * 2);
	float teapotAngle = 2 * M_PI / teapotNr;
	float angleInc = teapotAngle; 

	for(int i = 0; i < teapotNr; i++){
		glPushMatrix();
		glColor3f(0.0f, 0.0f, 1.0f);
		glTranslatef(radius * sin(teapotAngle + angle), 1.4, radius * cos(teapotAngle + angle));
		glRotatef(teapotAngleDeg + newAngleDeg - 90, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
		teapotAngleDeg += angleDegInc;
		teapotAngle += angleInc;
	}
}


void drawOuterTeapotCircle(float angle){
	int teapotNr = 16;
	float radius = 35;
	float teapotAngleDeg = 360 / teapotNr;
	float angleDegInc = teapotAngleDeg;
	float newAngleDeg = angle * 360 / (M_PI * 2);
	float teapotAngle = 2 * M_PI / teapotNr;
	float angleInc = teapotAngle; 

	for(int i = 0; i < teapotNr; i++){
		glPushMatrix();
		glColor3f(1.0f, 0.0f, 0.0f);
		glTranslatef(radius * sin(teapotAngle + angle), 1.4, radius * cos(teapotAngle + angle));
		glRotatef(teapotAngleDeg + newAngleDeg, 0, 1, 0);
		glutSolidTeapot(2);
		glPopMatrix();
		teapotAngleDeg += angleDegInc;
		teapotAngle += angleInc;
	}
}

float* generateRandoms(int seed){
	float *rs = (float *) malloc(sizeof(float) * 4);

	srand(seed);
	float ranX = (rand()*1.0/RAND_MAX) * 200 - 100;
	srand(seed * 1000);
	float ranZ = (rand()*1.0/RAND_MAX) * 200 - 100;
	while ((ranX < 35 && ranX > -35) && (ranZ < 35 && ranZ > -35)){	
		seed++;
		srand(seed);
		ranZ = (rand()*1.0/RAND_MAX) * 200 - 100;
	}
	srand(seed + 1);
	float radius = (rand()*1.0/RAND_MAX) * 0.5;
	srand(seed + 2);
	float height = (rand()*1.0/RAND_MAX) * 4 - 2;
	srand(seed + 3);
	float color = (rand()*1.0/RAND_MAX) * 0.4 - 0.2;

	rs[0] = ranX;
	rs[1] = ranZ;
	rs[2] = radius;
	rs[3] = height;
	rs[4] = color;

	return rs;
}


void drawTree(int seed){
	float radius = 1, height = 10;
	int slices = 48, stacks = 5;

	float *rands = generateRandoms(seed);
	// BASE
	glPushMatrix();
	glTranslatef(rands[0], 1.4, rands[1]);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glColor3f(0.4f + rands[4], 0.2f + rands[4], 0.0f);
	glutSolidCone(radius, height, slices, stacks);
	glPopMatrix();

	// COPA
	glPushMatrix();
	glColor3f(0.0f, 0.8 + rands[4], 0.0f);
	int translateHeight = height - height/3;
	glTranslatef(rands[0], translateHeight, rands[1]);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
	glutSolidCone((radius + rands[2]) * 4, (height + rands[3]) * 2, slices, stacks);

	glPopMatrix();
}


void drawTrees(){
	srand(10);
	float ran = floor((rand()*1.0/RAND_MAX) * 200);
	int seed = 0;
	for(int i = 0; i < ran; i++){
		drawTree(seed);
		seed = (seed + 1);
	}
}


float angleInner = 0;
float angleOuter = 0;

void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	angleInner += 0.005;
	angleOuter += 0.015;
	drawTerrain();	
	drawTorus();
	drawInnerTeapotCircle(angleInner);
	drawOuterTeapotCircle(angleOuter);
	drawTrees();
	
	// put code to draw scene in here
	glutSwapBuffers();
}


void processKeys(unsigned char c, int xx, int yy) {
// put code to process regular keys in here
	if(c == 27)
		exit(0);

}


void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {

	case GLUT_KEY_RIGHT:
		alfa -= 0.1; break;

	case GLUT_KEY_LEFT:
		alfa += 0.1; break;

	case GLUT_KEY_UP:
		beta += 0.1f;
		if (beta > 1.5f)
			beta = 1.5f;
		break;

	case GLUT_KEY_DOWN:
		beta -= 0.1f;
		if (beta < -1.5f)
			beta = -1.5f;
		break;

	case GLUT_KEY_PAGE_DOWN: radius -= 1.0f;
		if (radius < 1.0f)
			radius = 1.0f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 1.0f; break;

	}

	spherical2Cartesian();
	glutPostRedisplay();

}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin\n");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP5");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutIdleFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);

	spherical2Cartesian();
	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
