#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h> 
#include <math.h>

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

void drawTriangles(){
	glBegin(GL_TRIANGLES);
	// base a vermelho
	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// base ao contrário
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	// faces verdes 
	glColor3f(0.0, 0.5, 0.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);

	// faces verdes ao contrário
	glColor3f(0.0, 0.5, 0.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);


	// faces azuis	
	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	// faces azuis ao contrário
	glColor3f(0.0, 0.0, 0.5);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -1.0f);

	glEnd();
}

float nowX = 0;
float nowY = 0;
float nowZ = 0;
float angleX = 0;
float angleY = 0;
float angleZ = 0;
float scaleValue = 1;

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	gluLookAt(5.0,5.0,5.0, 
		      0.0,0.0,0.0,
			  0.0f,1.0f,0.0f);

	axis_system();

	// put the geometric transformations here
	glTranslatef(nowX, nowY, nowZ);
	glRotatef(angleX, 1.0f, 0.0f, 0.0f);
	glRotatef(angleY, 0.0f, 1.0f, 0.0f);
	glRotatef(angleZ, 0.0f, 0.0f, 1.0f);
	glScalef(scaleValue, scaleValue, scaleValue);

	// put drawing instructions here
	drawTriangles();

	// End of frame
	glutSwapBuffers();
}


int modo = 1;


void translate(unsigned char key, int x, int y){
	switch(key){
		// 'a'
		case 97:
		nowX -= 0.2; break;

		// 'd'
		case 100:
		nowX += 0.2; break;

		// 's'
		case 115:
		nowZ += 0.2; break;

		// 'w'
		case 119:
		nowZ -= 0.2; break;

		// 'q'
		case 113:
		nowY -= 0.2; break;

		// 'e'
		case 101:
		nowY += 0.2; break;
	}
}

void rotate(unsigned char key, int x, int y){
	switch(key){
		// 'r'
		case 114:
		angleX -= 10; break;

		// 't'
		case 116:
		angleX += 10; break;

		// 'y'
		case 121:
		angleY -= 10; break;

		// 'u'
		case 117:
		angleY += 10; break;

		// 'i'
		case 105:
		angleZ -= 10; break;

		// 'o'
		case 111:
		angleZ += 10; break;
	}
}

void scale(unsigned char key, int x, int y){
	// 'S'	
	if (key == 83)
		scaleValue += 0.1;
	// 'D'	
	else if (key == 68)
		scaleValue -= 0.1;
}

void changeModo(unsigned char key, int x, int y){
	if(key == 32){
		if(modo == 3)
			modo = 1;
		else
			modo ++;

		switch(modo){
			case 1: 
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;

			case 2: 
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				break;

			case 3: 
				glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
				break;
		}
	}
}


// write function to process keyboard events
void key_handler(unsigned char key, int x, int y){
	translate(key, x, y);
	rotate(key, x, y);
	scale(key, x, y);
	changeModo(key, x, y);
	
	// ESCAPE
	if (key == 27)
		exit(0);

	glutPostRedisplay();
}


void mouse_movement(int x, int y){
	glClearColor(x/1000.0, y/1000.0, 0.0f, 1.0f);
	glutPostRedisplay();
}



int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@TP2");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);

	
// put here the registration of the keyboard callbacks
	glutKeyboardFunc(key_handler);
	glutPassiveMotionFunc(mouse_movement);


//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
