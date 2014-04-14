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
 */
/* File for "Animation" lesson of the OpenGL tutorial on
 * www.videotutorialsrock.com
 */



#include <iostream>
#include <stdlib.h>

#define GLEW_STATIC
#include "glew.h"
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "imageloader.h"
#include "md2model.h"
#include "textfile.h"
#include  <Windows.h>
#include <ctime>
#include <cmath>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
 

using namespace std;
int state = 0;
const float FLOOR_TEXTURE_SIZE = 15.0f; //The size of each floor "tile"
const float BOX_SIZE = 7.0f; //The length of each side of the cube
const float BOX_HEIGHT = BOX_SIZE; //The height of the box off of the ground
const float FLOOR_SIZE = 20.0f; //The length of each side of the floor
float _angle = 30.0f;
MD2Model* _model;
int _textureId;
int _textureId2;
static long font = (long)GLUT_BITMAP_8_BY_13; // Font selection.
GLuint makeaTree;
static int animationPeriod = 1000; // Time interval between frames.

//The forward position of the guy relative to an arbitrary floor "tile"
float _guyPos = 0;
bool isPlaying = true;
GLuint v,f,f2,p;
int shadingMode = 1;

void cleanup() {
	delete _model;
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image) {
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGB,
				 image->width, image->height,
				 0,
				 GL_RGB,
				 GL_UNSIGNED_BYTE,
				 image->pixels);
	return textureId;
}

void initRendering() 
{
	for(int i = 3;i > 0;--i)
	{
		
		PlaySound(TEXT("countdown.wav"), NULL, SND_ASYNC|SND_FILENAME);
		Sleep(100);
		 std::cout << i << '\n';
         std::this_thread::sleep_for (std::chrono::seconds(1));
		
	
		Sleep(100);
		
	}
	
	
	 PlaySound(TEXT("mario.wav"), NULL, SND_ASYNC|SND_FILENAME);
	Sleep(100);
	std::cout << "Go!!!!!!!!\n";
	std::this_thread::sleep_for (std::chrono::seconds(1));
	PlaySound(TEXT("outdoors.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	
	//Load the model
	_model = MD2Model::load("tallguy.md2");
	if (_model != NULL) {
		
		_model->setAnimation("run");
	}
	
	//Load the floor texture
	Image* image = loadBMP("vtr.bmp");
	_textureId = loadTexture(image);
	delete image;

	//Load clouds texture
	Image* image2 = loadBMP("clouds.bmp");
	_textureId2 = loadTexture(image2);
	delete image2;

	 glEnable(GL_DEPTH_TEST); // Enable depth testing.

   // Turn on OpenGL lighting.
   glEnable(GL_LIGHTING); 

   // Light property vectors.
   float lightAmb[] = { 0.0, 0.0, 0.0, 1.0 };
   float lightDifAndSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float lightPos[] = {0.0, 1.0, 0.0, 0.0 }; // Overhead directional light source (e.g., sun).
   float globAmb[] = { 0.2, 0.2, 0.2, 1.0 };

   // Light properties.
   glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmb);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_SPECULAR, lightDifAndSpec);
   glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
 
   glEnable(GL_LIGHT0); // Enable particular light source.
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globAmb); // Global ambient light.

	// Material property vectors.
   float matSpec[] = { 1.0, 1.0, 1.0, 1.0 };
   float matShine[] = { 50.0 };

   // Material properties.
   glMaterialfv(GL_FRONT, GL_SPECULAR, matSpec);
   glMaterialfv(GL_FRONT, GL_SHININESS, matShine);

   // Enable color material mode.
   glEnable(GL_COLOR_MATERIAL); 
   glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE); 

   // Cull back faces.
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
}

void handleResize(int w, int h) 
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

// Routine to draw a bitmap character string.
void writeBitmapString(void *font, char *string)
{  
   char *c;

   for (c = string; *c != '\0'; c++) glutBitmapCharacter(font, *c);
}  

void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press a to toggle between animation on/off." << endl
	    << "Press the up/down arrow keys to speed up/slow down animation." << endl
		<< "Press r/R to rotate the viewpoint." << endl
		<< "Press z/Z to zoom in/out." << endl;
}

void setShaders() {

	char *vs = NULL,*fs = NULL,*fs2 = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	f2 = glCreateShader(GL_FRAGMENT_SHADER);
	if (shadingMode == 0){
		// PHONG ours
	   vs = textFileRead("phong.vert");
	   fs = textFileRead("phong.frag");
	}
	if (shadingMode == 1){
		// PHONG ours
	   vs = textFileRead("myShader.vert");
	   fs = textFileRead("myShader.frag");
	}

	const char * ff = fs;
	const char * ff2 = fs2;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);
	glCompileShader(f2);

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
}

void drawScene() 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  gluLookAt(0.0, 5.0, 30.0, 0.0, 10.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	 glClearColor(0.74902, 0.847059, 1, 0.0); 
	glTranslatef(0.0f, 0.0f, -20.0f);
	glRotatef(-_angle, 0.0f, 1.0f, 0.0f);
	
	GLfloat ambientLight[] = {0.3f, 0.3f, 0.3f, 1.0f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	
	GLfloat lightColor[] = {0.7f, 0.7f, 0.7f, 1.0f};
	GLfloat lightPos[] = {-0.2f, 0.3f, -1, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

	
	 glPushMatrix();
	
		// glTranslatef(-16.0f, -6.2f, 0.0f);
	//	glScalef(0.8f, 0.7f, 0.8f);
	   
		_model->draw(1);
	
    glPopMatrix();	

	 // Draw a reflection of the guy by flipping him about the xz-plane
   // using a scaling transformation.
   glPushMatrix();
   glScalef(0.7, -1.0, 1.0);
   
   glRotatef(90,100,200,-1000);
   glTranslatef(-25.0f, 0.0f, -2.0f); 
   glFrontFace(GL_CW); // Because of reflection front-faces are drawn clockwise. 
 // _model->draw();
   glFrontFace(GL_CCW);  
   glPopMatrix();
	
	//Draw the guy
	if (_model != NULL)
	{
		
       
       
		glPushMatrix();
		 
		glTranslatef(0.0f, 4.5f, 0.0f);
		glRotatef(-90.0f, 0.0f, 0.0f, 1.0f);
		glScalef(0.5f, 0.5f, 0.5f);
	    
		_model->draw();
		
		glPopMatrix();

		
		
		
	}
	
	 
	//Draw floor grass
	glTranslatef(0.0f, -5.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, -1000.0f);
	glTexCoord2f(2000 / FLOOR_TEXTURE_SIZE,
				 (2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(-1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, (2000 + _guyPos) / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, 1000.0f);
	glTexCoord2f(0.0f, _guyPos / FLOOR_TEXTURE_SIZE);
	glVertex3f(1000.0f, 0.0f, -1000.0f);
	
	glEnd(); 
    
	// In Progress Draw Clouds
	glTranslatef(0.0f, -5.4f, 0.0f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);




	//glShadeModel(GL_SMOOTH); // Restore smooth shading.
	//glTranslatef(-16.0f, -0.9f, 0.0f);
	//glScalef(0.8f, 0.8f, 0.8f);
  // glPushMatrix();
 //  glTranslatef(-16.0f, -0.9f, 0.0f);
//	glScalef(0.8f, 0.8f, 0.8f);
	//_model->draw(1);
//   glPopMatrix();	

	glutSwapBuffers();
}

float t = 0.0;

void idle (void)
{
    t += 0.1;
    if (t > 2*3.14159)  
    { 
        t = 0.0; 
    }
    glutPostRedisplay();
}

void update(int value) {
	_angle += 0.7f;
	if (_angle > 360) {
		_angle -= 360;
	}
	
	//Advance the animation
	if (_model != NULL) {
		_model->advance(0.025f);
	}
	
	//Update _guyPos
	_guyPos += 0.08f;
	while (_guyPos > FLOOR_TEXTURE_SIZE) {
		_guyPos -= FLOOR_TEXTURE_SIZE;
	}
	
	glutPostRedisplay();
	if(isPlaying) glutTimerFunc(25, update, 0);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
		case 27: //Escape key
			cleanup();
			exit(0);
		case 's':
			if(!isPlaying) {
				isPlaying = true;
				PlaySound(TEXT("outdoor_footsteps.wav"), NULL, SND_ASYNC|SND_FILENAME|SND_LOOP);
				glutTimerFunc(25, update, 0);
			}
			break;
		case 'p':
			isPlaying = false;
			PlaySound(NULL, NULL, 0);
			//glutTimerFunc(25, update, 0);
			break;
		case 't':
			setShaders();
			break;
	}
}


int main(int argc, char** argv)
{
	//Main
	//printInteraction();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutCreateWindow("Animation - videotutorialsrock.com");
	initRendering();
	
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0,1.0,1.0,1.0);
	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}

	glutTimerFunc(25, update, 0);
	
	glutMainLoop();
	return 0;
}









