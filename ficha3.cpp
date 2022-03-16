#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdio>

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


void drawCylinder(float radius, float height, int slices) {
	// put code to draw cylinder in here
	glBegin(GL_TRIANGLES);
	float ogAngle = 2 * M_PI / slices;
	float angle = 2 * M_PI / slices;

	for(int i = 0; i < slices; i++){
		glVertex3f(0,height,0);
		glVertex3f(radius * sin(angle),height,radius * cos(angle));
		glVertex3f(radius * sin(angle + ogAngle),height,radius * cos(angle + ogAngle));
		angle += 2 * M_PI / slices;
	}

	for(int i = 0; i < slices; i++){
		glVertex3f(0,0,0);
		glVertex3f(radius * sin(angle),0,radius * cos(angle));
		glVertex3f(radius * sin(angle + ogAngle),0,radius * cos(angle + ogAngle));
		angle += 2 * M_PI / slices;
	}


	for(int i = 0; i < slices; i++){
		glColor3f(0.3,0.3,0.3);
		glVertex3f(radius * sin(angle),height,radius * cos(angle));
		glVertex3f(radius * sin(angle),0,radius * cos(angle));
		glVertex3f(radius * sin(angle + ogAngle),height,radius * cos(angle + ogAngle));

		glVertex3f(radius * sin(angle + ogAngle),height,radius * cos(angle + ogAngle));
		glVertex3f(radius * sin(angle),0,radius * cos(angle));
		glVertex3f(radius * sin(angle + ogAngle),0,radius * cos(angle + ogAngle));
		angle += 2 * M_PI / slices;
	}
	glEnd();
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

float desvioX = 0;
float desvioZ = 0; 
float radius = 8;
float alfa = 0;
float beta = M_PI / 4;

void renderScene(void) {

	// clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// set the camera
	glLoadIdentity();
	float px = radius * cos(beta) * sin(alfa);
	float py = radius * sin(beta);
	float pz = radius * cos(beta) * cos(alfa);

	gluLookAt(px, py, pz, 
		      0.0, 0.0, 0.0,
			  0.0f, 1.0f, 0.0f);
	printf("x: %f | y: %f | z: %f\n", px, py, pz);
	printf("alfa: %f | beta: %f\n\n", alfa, beta);

	glPolygonMode(GL_FRONT, GL_LINE);
	axis_system();
	glColor3f(1,1,1);
	drawCylinder(1,2,3);

	// End of frame
	glutSwapBuffers();
}


void processKeys(unsigned char key, int xx, int yy) {
// put code to process regular keys in here
	switch(key){
		case 115: // 's'
			radius += 0.2; break;

		case 119: // 'w'
			radius -= 0.2; break;

		case 27:
			exit(0); break;
	}
	glutPostRedisplay();
}


void processSpecialKeys(int key, int xx, int yy) {
// put code to process special keys in here
	switch(key){
		case GLUT_KEY_LEFT:
			alfa += M_PI / 12; 
			break;

		case GLUT_KEY_RIGHT:
			alfa -= M_PI / 12; 
			break;

		case GLUT_KEY_UP:
			if(beta < 1.57)
				beta += M_PI / 12; 
			break;

		case GLUT_KEY_DOWN:
			if(beta > -1.57)
				beta -= M_PI / 12;
 			break;
	}
	glutPostRedisplay();
}


int main(int argc, char **argv) {

// init GLUT and the window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH|GLUT_DOUBLE|GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800,800);
	glutCreateWindow("CG@DI-UM");
		
// Required callback registry 
	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	
// Callback registration for keyboard processing
	glutKeyboardFunc(processKeys);
	glutSpecialFunc(processSpecialKeys);

//  OpenGL settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	
// enter GLUT's main cycle
	glutMainLoop();
	
	return 1;
}
