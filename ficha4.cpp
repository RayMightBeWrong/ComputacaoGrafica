#include <stdio.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>

float alfa = 0.0f, beta = 0.0f, radius = 5.0f;
float camX, camY, camZ;

int timebase;
float frame;

GLuint vertices, indexes;
GLuint verticeCount = 0;
unsigned int indexCount;

std::vector<float> vertexs;
std::vector<unsigned int> i;

void spherical2Cartesian() {
	camX = radius * cos(beta) * sin(alfa);
	camY = radius * sin(beta);
	camZ = radius * cos(beta) * cos(alfa);
}

int compareFloat(float a, float b){
    if (abs(a - b) < 1e-9)
	    return 1;
    else
	    return 0;
}

int vertexInVector(float x, float y, float z){
	int r = -1;

	for(int i = 0; r == -1 && i < verticeCount; i++){
		if(x == vertexs[i*3])
			if(y == vertexs[i*3 + 1])
				if(z == vertexs[i*3 + 2])
					r = i;
	}

	return r;
}

void addVertex(float x, float y, float z){
	int r;
	if((r = vertexInVector(x, y, z)) == -1){
		vertexs.push_back(x);
		vertexs.push_back(y);
		vertexs.push_back(z);
		i.push_back(verticeCount);
		verticeCount++;
	}
	else
		i.push_back(r);
}

void cylinderVertexes(float radius, float height, int sides){
	int i;
	float step;

	step = 360.0 / sides;

	//glBegin(GL_TRIANGLES);

	for (i = 0; i < sides; i++) {
		// top
		addVertex(0, height * 0.5, 0);
		addVertex(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		addVertex(cos((i+1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);

		// bottom
		addVertex(0, -height*0.5, 0);
		addVertex(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		addVertex(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);

		// body
		addVertex(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		addVertex(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		addVertex(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);

		addVertex(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		addVertex(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		addVertex(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
	}
}

void prepareData(){
	// preencher o vector com as coordenadas dos pontos
	/*
	for(int k = 0; k < i.size(); k++)
		printf("index[%d]: %d\n", k, i[k]);
	for(int k = 0; k < vertexs.size(); k+=3)
		printf("%f %f %f\n", vertexs[k], vertexs[k + 1], vertexs[k + 2]);
		*/

	cylinderVertexes(1, 2, 10);

	printf("verticeCount: %d\n", verticeCount);
	printf("indexCount: %ld\n", i.size());

	// criar o VBO
	glGenBuffers(1, &vertices);	
	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertexs.size(), vertexs.data(), GL_STATIC_DRAW);


	glGenBuffers(2, &indexes);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 
			sizeof(unsigned int) * i.size(), 
			i.data(), GL_STATIC_DRAW);
	indexCount = i.size();
	
	// copiar o vector para a memória gráfica
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


/*----------------------------------------------------------------------------------- 
	Draw Cylinder with strips and fans

	  parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder0(float radius, float height, int sides) {
	int i;
	float step;

	step = 360.0/sides;

	glColor3f(1,0,0);
	glBegin(GL_TRIANGLE_FAN);

		
	glVertex3f(0,height*0.5,0);
	for (i=0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI/180.0)*radius,height*0.5,-sin(i * step *M_PI/180.0)*radius);
	}
	glEnd();

	glColor3f(0,1,0);
	glBegin(GL_TRIANGLE_FAN);

	
	glVertex3f(0,-height*0.5,0);
	for (i=0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,sin(i * step *M_PI/180.0)*radius);
	}
	glEnd();

	glColor3f(0,0,1);
	glBegin(GL_TRIANGLE_STRIP);

	for (i=0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI/180.0)*radius, height*0.5,-sin(i * step *M_PI/180.0)*radius);
		glVertex3f(cos(i * step * M_PI/180.0)*radius,-height*0.5,-sin(i * step *M_PI/180.0)*radius);
	}
	glEnd();
}


/*-----------------------------------------------------------------------------------
	Draw Cylinder

		parameters: radius, height, sides

-----------------------------------------------------------------------------------*/


void cylinder(float radius, float height, int sides) {
	int i;
	float step;

	step = 360.0 / sides;

	glBegin(GL_TRIANGLES);

	// top
	for (i = 0; i < sides; i++) {
		glVertex3f(0, height*0.5, 0);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		glVertex3f(cos((i+1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
	}

	// bottom
	for (i = 0; i < sides; i++) {
		glVertex3f(0, -height*0.5, 0);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
	}

	// body
	for (i = 0; i <= sides; i++) {
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);

		glVertex3f(cos(i * step * M_PI / 180.0)*radius, -height*0.5, -sin(i * step *M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, -height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
		glVertex3f(cos((i + 1) * step * M_PI / 180.0)*radius, height*0.5, -sin((i + 1) * step *M_PI / 180.0)*radius);
	}
	glEnd();
}


void renderScene(void) {
	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(camX, camY, camZ,
		0.0, 0.0, 0.0,
		0.0f, 1.0f, 0.0f);

	//cylinder0(1,2,10);

	glBindBuffer(GL_ARRAY_BUFFER, vertices);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexes);

	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);

	// ver fps
	frame++;
	float time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000){
		float fps = frame * 1000.0 / (time - timebase);
		printf("FPS: %f\n", fps);
		timebase = time;
		frame = 0;
	}

	// End of frame
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

	case GLUT_KEY_PAGE_DOWN: radius -= 0.1f;
		if (radius < 0.1f)
			radius = 0.1f;
		break;

	case GLUT_KEY_PAGE_UP: radius += 0.1f; break;
	}
	spherical2Cartesian();
	glutPostRedisplay();
}


void printInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version: %s\n", glGetString(GL_VERSION));

	printf("\nUse Arrows to move the camera up/down and left/right\n");
	printf("Page Up and Page Down control the distance from the camera to the origin");
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP4");

	glEnableClientState(GL_VERTEX_ARRAY);

	timebase = glutGet(GLUT_ELAPSED_TIME);
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

	// init GLEW
#ifndef __APPLE__
	glewInit();
#endif

	prepareData();

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT, GL_LINE);

	spherical2Cartesian();

	printInfo();

// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
