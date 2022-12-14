/* Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above notice and this permission notice shall be included in all copies
 * or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *

/* Source code for "Skybox" by Mark Daniel G. Dacer
 * www.github.com/Jaeger47/Learning-OpenGL
 */

#include <iostream>
#include <glut.h>
#include "imageloader.h"
#include "primitives.h"
#include <irr/irrKlang.h>

using namespace irrklang;

ISoundEngine *engine = createIrrKlangDevice();
ISoundEngine *gine = createIrrKlangDevice();



  
      
using namespace std;

void display();
void reshape(int, int);
void update(int);
void cube();
GLuint loadTexture(Image* image);
void processNormalKeys(unsigned char, int, int);
void renderBitmapString(
		float x,
		float y,
		float z,
		void *font,
		char *string) {
  char *c;
  glRasterPos3f(x, y,z);
  for (c=string; *c != '\0'; c++) {
    glutBitmapCharacter(font, *c);
  }
}


float angle = 0.0;// angle of rotation for the camera direction
long count = 0;
float lx = 0.0f, lz = -1.0f;// actual vector representing the camera's direction
float cameraX = 0.0f, cameraZ = 5.0f;// XZ position of the camera
float _angle = -70.0f; // objects angle
float x_pos = -9.0f; //x position
int state = 1; //back&fort logic

//Emissions
GLfloat no_mat[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_emissionblue[] = { 0.0, 0.0, 1.0, 1.0 };
//set materials
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat mat_shininess[] = { 60.0 };


//set world light
GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light[] = { 0.5, 0.5, 0.5 };
GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat static_ambient[] = { 0.8, 0.8, 0.8, 1.0 };

GLuint _textureBall;
GLuint _textureGround;//The id of the texture
GLuint _textureNew;
GLuint _textureWood;


GLUquadric* will;
GLUquadric* ball; //pointer quadric shape for the sphere
GLUquadric* ground;
GLUquadric* wood;
static GLint fogMode;







//Initializes 3D rendering
void initRendering() {

	 
	engine->play2D("village.wav");
	
	glClearColor(0.0, 0.4, 0.7, 1.0); //set background to black

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glEnable(GL_AUTO_NORMAL);
	glShadeModel(GL_SMOOTH); //Enable smooth shading


	glDisable(GL_FOG);

	glDisable(GL_BLEND);

	//glDepthMask(GL_FALSE); 
    glBlendFunc(GL_ZERO, GL_SRC_COLOR); //default



	{
		GLfloat fogColor[4] = {1.0, 1.0, 1.0, 1.0};

      fogMode = GL_EXP2;
      glFogi (GL_FOG_MODE, fogMode);
      glFogfv (GL_FOG_COLOR, fogColor);
      glFogf (GL_FOG_DENSITY, 0.1);
      glHint (GL_FOG_HINT, GL_DONT_CARE);
      glFogf (GL_FOG_START, 1.0);
      glFogf (GL_FOG_END, 5.0);
   }
   
	

	
	
	Image* ballImg = loadBMP("new1.bmp");
	_textureBall = loadTexture(ballImg);
	delete ballImg;
	//loading texture for the cylinder
	Image* canImg = loadBMP("grass123.bmp");
	_textureGround = loadTexture(canImg);
	delete canImg;

	Image* willImg = loadBMP("wood.bmp");
	_textureNew = loadTexture(willImg);
	delete willImg;

	Image* woodImg = loadBMP("wood1.bmp");
	_textureWood = loadTexture(woodImg);
	delete woodImg;
	

	ball = gluNewQuadric(); //declared as quadric objects
	ground = gluNewQuadric();
	will = gluNewQuadric();
	wood = gluNewQuadric();

}



int main(int argc, char** argv) {
	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(640, 480);

	//Create the window
	glutCreateWindow("Octiva-My Home- T85- Project");
	initRendering();
	//Set handler functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);//controls
	glutTimerFunc(1000 / 60, update, 0); //Add a timer

	glutMainLoop();
	return 0;
}

//Draws the 3D scene
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//camera controls
	gluLookAt(cameraX, 1.0f, cameraZ,
		cameraX + lx, 1.0f, cameraZ + lz,
		0.0f, 1.0f, 0.0f);


	//roof
	glPushMatrix();
	glTranslatef(0.0f, 3.3f, -1.3f);
	glRotatef(360, 1.0f, 0.0f, 0.0f);
	line();
	glPopMatrix();

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_SPECULAR, light ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light ); //add specular lighting
	
	//setting object materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	//primitives--------------------------
	glPushMatrix();
	glTranslatef(0.0f, 3.3f, -1.3f);
	glRotatef(360, 1.0f, 0.0f, 0.0f);
	pyramid();
	glPopMatrix();

	

	//tree
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting
	
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_SPECULAR, light ); //add diffuse specular lighting
    glLightfv(GL_LIGHT0, GL_SPECULAR, light ); //add specular lighting

	//setting object materials
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess
	

	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(5.0f,1.0f,1.5f);
	glColor4ub(15,117,20,1);
	glutSolidCone(1.5,7,20,100);
	glPopMatrix();

	

	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(8.0f,-1.5f,1.5f);
	glColor4ub(15,117,20,1);
	glutSolidCone(1.5,10,20,100);
	glPopMatrix();

	// tree

	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(0.5f,-5.0f,3.5f);
	glColor4ub(15,117,20,1);
	glutSolidCone(2.5,10,20,100);
	glPopMatrix();

	//new tree

	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(-8.0f,3.0f,0.0f);
	glColor4ub(171,125,91,1);
	glutSolidCone(0.5,5,20,100);
	glPopMatrix();


	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(-8.0f,3.0f,5.0f);
	glColor3f(0,1,0);
	glutSolidSphere(3,7,7);
	glPopMatrix();

	//new tree
	//tree

	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(-3.0f,-3.0f,0.0f);
	glColor4ub(171,125,91,1);
	glutSolidCone(0.5,5,20,100);
	glPopMatrix();


	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(-3.0f,-3.0f,5.0f);
	glColor3f(0,1,0);
	glutSolidSphere(3,7,7);
	glPopMatrix();

	
	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(7.0f,3.0f,0.0f);
	glColor4ub(171,125,91,1);
	glutSolidCone(0.5,5,20,100);
	glPopMatrix();


	glPushMatrix();
	glRotatef(-180, 0.0f, 1.0f, 1.0f);
	glTranslatef(7.0f,3.0f,5.0f);
	glColor3f(0,1,0);
	glutSolidSphere(3,7,7);
	glPopMatrix();

	
	//---------SKYBOX-----------------------//
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureBall);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient); //add ambient lighting
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glColor3f(1,1,1); // needed to make the skybox visible
	gluQuadricTexture(ball, 1);
	glRotatef(180, 0.0f, 0.0f, 1.0f);
	glRotatef(90, 1.0f, 0.0f, 0.0f); 
	gluSphere(ball, 1000, 128, 128);
	glPopMatrix();

	glDisable(GL_TEXTURE_2D);
	

	
	//______________________________________________________


	glPushMatrix(); 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureNew);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(will,1);
	glColor4ub(100,100,100,1);
	//pillar 1
	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	//pillar 2
	glPushMatrix();
	glTranslatef(1.5f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	
	//pillar 3
	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	//pillar 4
	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();
	glEnd();

	//koral
	glColor4ub(165,165,165,1);
	glPushMatrix();
	glTranslatef(10.0f, 0.5f, -5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,20,20,100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 0.1f, -5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,20,20,100);
	glPopMatrix();
	
	//koral 2
	glPushMatrix();
	glTranslatef(10.0f, 0.1f, -5.5f);
	glRotatef(360,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,11,20,100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 0.5f, -5.5f);
	glRotatef(360,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,11,20,100);
	glPopMatrix();

	
	//1
	glPushMatrix();
	glTranslatef(-10.0f, 0.1f, -5.5f);
	glRotatef(360,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,11,20,100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-10.0f, 0.5f, -5.5f);
	glRotatef(360,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,11,20,100);
	glPopMatrix();

	//2
	glPushMatrix();
	glTranslatef(10.0f, 0.5f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,8,20,100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10.0f, 0.1f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,8,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-2.0f, 0.5f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,8,20,100);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-2.0f, 0.1f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,8,20,100);
	glPopMatrix();

	// 3

	
	//front
	glPushMatrix();
	glTranslatef(-2.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	//back

	glPushMatrix();
	glTranslatef(-2.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-5.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(5.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	//right
	glPushMatrix();
	glTranslatef(10.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(10.0f, -0.1f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	//left
	glPushMatrix();
	glTranslatef(-10.0f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.0f, -0.1f, -5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-10.0f, -0.1f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();






	//---------SKYBOX-----------------------//
	
	//setting world lighting

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); //add ambient lighting
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);//set light position
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light); //add diffuse specular lighting
	glLightfv(GL_LIGHT0, GL_SPECULAR, light); //add specular lighting
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular); //set object specular mateial
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);// set shininess

	

	glPushMatrix();

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureGround);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTranslatef(0.0f, -1.0f, 0.0f);
	glBegin(GL_QUADS);
		glColor3f(1.0f,1.0f,1.0f);
		glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0 , 0);
		glVertex3f(-1000.0f, -5.0f, -1000.0f);
			glTexCoord2f(1000, 0);
		glVertex3f(-1000.0f, -5.0f, 1000.0f);
			glTexCoord2f(1000.0f, 1000.0f);
		glVertex3f(1000.0f, -5.0f, 1000.0f);
			glTexCoord2f(0.0f, 1000.f);
		glVertex3f(1000.0f, -5.0f, -1000.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//wall
	glPushMatrix(); 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureWood);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(wood,1);
	//wall
	//back
	glPushMatrix();
	glTranslatef(-1.3f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.1f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.9f, 0.0f ,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.7f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.5f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7f, 0.0f, -2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.1f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3f, 0.0f,-2.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();
	
	//front

	glPushMatrix();
	glTranslatef(-1.3f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.1f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.9f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.7f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(-0.5f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.3f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.1f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.1f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.3f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	//window
	glPushMatrix();
	glTranslatef(0.5f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.5f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.7f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9f, 1.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,0.5,20,20);
	glPopMatrix();

	//__________________________________

	glPushMatrix();
	glTranslatef(1.1f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.3f, 0.0f, 0.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(wood,0.1,0.1,1.5,20,20);
	glPopMatrix();
	

	//left


	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -0.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -0.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -0.6f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -0.8f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -1.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -1.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -1.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -1.6f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -1.8f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -2.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -2.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.5f, 0.0f, -2.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();


	//right
	
	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -0.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -0.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -0.6f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -0.8f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -1.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -1.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -1.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -1.6f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -1.8f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -2.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();
	

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -2.2f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.5f, 0.0f, -2.4f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	gluCylinder(will,0.1,0.1,1.5,20,20);
	glPopMatrix();


    glDisable(GL_TEXTURE_2D);
	
	
	//gate

	glPushMatrix(); 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, static_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
	glLightfv(GL_LIGHT0, GL_POSITION,light_position);
	
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, _textureNew);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	gluQuadricTexture(will,1);

	

	glPushMatrix();
	glTranslatef(x_pos,0.0f,0.0f);
	glTranslatef(1.7f, 0.1f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,3.5,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x_pos,0.0f,0.0f);
	glTranslatef(1.7f, 0.5f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,3.5,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x_pos,0.0f,0.0f);
	glTranslatef(1.7f, 0.7f, 5.5f);
	glRotatef(270,0.0f, 1.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,3.5,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x_pos,0.0f,0.0f);
	glTranslatef(1.8f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(x_pos,0.0f,0.0f);
	glTranslatef(-1.8f, -0.1f, 5.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.1,0.1,1,20,100);
	glPopMatrix();
	

	////////////////////
	//trunk
	glPushMatrix();

	glTranslatef(-0.5f, 0.0f, -5.0f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.5,0.5,5,20,100);
	glPopMatrix();

	glTranslatef(-5.0f, 0.0f, 1.1f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.5,0.5,1.5,20,100);
	glPopMatrix();


	glPushMatrix();

	glTranslatef(-8.0f, 0.0f, -1.5f);
	glRotatef(270,1.0f, 0.0f, 0.0f);
	glRotatef(angle,0.0f, 0.0f, 1.0f); //animated rotation
	gluCylinder(will,0.5,0.5,1.5,20,100);
	glPopMatrix();
glDisable(GL_TEXTURE_2D);





	///-----------------------------------text------------------------------------------------
	glDisable(GL_LIGHTING);
	glColor4f(0.0f,1.0f,0.0f,1.0f);
	 renderBitmapString(-0.3f, 1.5f, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24,"welcome to my home");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(0.0f,1.0f,0.0f,5.0f);
	 renderBitmapString(-0.3f, 1.0f, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24,"press f for fog");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(0.0f,1.0f,0.0f,5.0f);
	 renderBitmapString(-0.3f, 0.5f, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24,"press b for blend");
	glEnable(GL_LIGHTING);

	glDisable(GL_LIGHTING);
	glColor4f(0.0f,1.0f,0.0f,5.0f);
	 renderBitmapString(-0.3f, 0.0f, 3.0f, GLUT_BITMAP_TIMES_ROMAN_24,"press q for disable fog and blend");
	glEnable(GL_LIGHTING);

	glutSwapBuffers();


	

	

	

}




//Called when the window is resized
void reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)w / (double)h, 1.0, 2000.0);
}

void update(int value) {
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}

	switch(state){
	case 1:
		if(x_pos < 5)
			x_pos+=0.15f;
		else
			state = -1;
		break;
	case -1:
		if(x_pos >-5)
			x_pos-=0.15f;
		else
			state=1;
		break;
	
	}

	glutPostRedisplay();
	glutTimerFunc(1000 / 60, update, 0);
}
//keyboard controls
void processNormalKeys(unsigned char key, int x, int y) {
	float fraction = 1.5f;


	switch (key) {
	case 'w':
		cameraX += lx * fraction;
		cameraZ += lz * fraction;
		break;
	case 's':
		cameraX -= lx * fraction;
		cameraZ -= lz * fraction;
		break;
	case 'a':
		angle -= 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;
	case 'd':
		angle += 0.1f;
		lx = sin(angle);
		lz = -cos(angle);
		break;

		case 'f':


		 engine->removeSoundSource("William.wav");
	 engine->removeSoundSource("village.wav");
	engine->play2D("Christmas.wav");

	glEnable(GL_FOG);
	
	

	
		break;
		case 'b':
		 engine->removeSoundSource("Christmas.wav");
	engine->removeSoundSource("village.wav");
	engine->play2D("William.wav");
	glEnable(GL_BLEND);

	
		break;

		case 'q':


			engine->removeSoundSource("William.wav");
		 engine->play2D("village.wav");
		 engine->removeSoundSource("Christmas.wav");
	glDisable(GL_FOG);
	glDisable(GL_BLEND);

	
		break;
	}
}


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB, //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}


