/************************************************************************
File:			flightsim.c

Description: This is a C program that uses OpenGL to create a flight 
simulator. The program reads in a .txt file and builds and colours a plane
out of it. The altitude of the plane can be changed by using the up and 
down keys. The speed of the plane can be changed by using page up and page
down, however they plane cannot move at a speed of less than 0.05. The 
propeller speed moves with the speed of the plane. By pressing the 's' key, 
you can toggle between a sea/sky view and a grid view mode. The sea and sky
are a disk and cylinder, respectively, that have a .PPM texture wrapped 
around them. Wire frame and solid rendering can be toggled by pressing 'w'.
By pressing 'f', you will switch between full screen and windowed mode. 
Pressing 'q' will quit the program. You can move the camera left and right
depending on where your cursor is. Lighting has been used in this project
to make things look 3D. Fog on the ocean can be toggled by pressing the 'b'
key. This program does create mountains that are made from streched cylinders 
in the shape of cones. The mountain texturing can be toggled using 't', and
the mountains can be toggled themselves by pressing 'm'.

Bonus: The bonus parts are as follows: By pressing 'a', a bomb will drop 
from the plane and when it gets to y=0, the sea will flash red. By pressing
'v' the plane will engage in a vertical take off. By pressing 'n', the 
plane will nose dive. By pressing 'c', the plane will spin out of control 
and when y=0 the program will exit. Pressing 'd' toggles a sunset. Pressing
'i' toggles a Great Britain plane insignia. Pressing 'd' will draw missiles 
on the wings. Pressing 'o' will shoot the missiles, and once they have reached 
a certain distance they will then respawn back on the wings. Also, when the 
altitude is changing, the plane will tilt on the z axis appropriately. 

Author: Liam Gowan B00673126
Note: This program uses some ideas discussed from the course website.
It also uses .txt and .ppm files found on the project page.
following links:

*************************************************************************/

//includes
#include <GL/freeglut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <time.h>

#define PI 3.14159265
#define DEG_TO_RAD PI/180
#define C 100 //For the reference grid


//global variables used for display, window and camera settings
GLfloat lightZ = 0;
GLfloat lightZDelta = 1;

GLfloat windowWidth = 800;
GLfloat windowHeight = 600;
float mouseX = 0;
float mouseY = 0;

GLfloat diffuse[] = { 1,1,1,1 };
GLfloat ambient[] = { 0,0,0,1 };
GLfloat specular[] = { 1,1,1,1 };
GLfloat globalAmbient[] = { 0.1,0.1,0.1,0.1 };

GLfloat xCameraEye = 0;
GLfloat yCameraEye = 2;
GLfloat zCameraEye = 0;

GLfloat xCameraCenter = 0;
GLfloat yCameraCenter = 2;
GLfloat zCameraCenter = 0;

//colours used in this project
typedef GLfloat color[4];
color yellow = { 1,1,0,1 };
color white = { 1,1,1,1 };
color black = { 0,0,0,1 };
color blue = { 0,0,1,1 };
color red = { 1,0,0,1 };
color lightBlue = { 0,.2,.7,1 };
color green = { 0,1,0,1 };
color lightGreen = { 0.5,1,0.5,1 };
color purple = { 1,0,1,1 };
color pink = { 1,0.8,1,1 };
color grey = { 0.5, 0.5, 0.5,1 };

//Everything that makes the plane
//vertices, normals and faces for the planes stored here
//The size values in the arrays were obtained by going through
//the .txt files and using the line count feature
GLfloat planeVertex[6763][3];
GLfloat planeNormal[6763][3];
GLfloat planeFace1[701][30];//yellow
GLfloat planeFace2[156][30];//black
GLfloat planeFace3[186][30];//purple
GLfloat planeFace4[175][30];//blue
GLfloat planeFace5[62][30];//yellow
GLfloat planeFace6[1617][30];//blue
GLfloat planeFace7[743][30];//yellow
GLfloat propellerVertex[6763][3];
GLfloat propellerNormal[6763][3];
GLfloat propellerFace1[120][30];
GLfloat propellerFace2[12][30];

//controls speeds and tilts of the plane and propeller
GLfloat speedOfCamera = 0.05;
GLfloat speedOfPlane = 0.005;
GLfloat propellerRotationSpeed = 3;
GLfloat propellerRate = 3;
GLfloat tiltRate = 5;
GLfloat planeTilt = 0;
GLfloat angle = 0;
GLfloat planeRotation = 0;
GLfloat planeSpeedIncrease = 0.001;
GLfloat zTilt = 0;
GLfloat zTiltRate = 0.05;
int yToBe = 0;
GLfloat xMotion = 0;
GLfloat yMotion = 0;
GLfloat zMotion = 0;
GLfloat propellerRotation = 100;
GLfloat propellerAngle = 0;

//global states of things that can be toggled
int fillOnWireOff = 1;
int fullScreen = 0;
int seaSkyOnRefOff = 1;
int fog = 0;
int mountainsOn = 1;
int mountainTextureOn = 1;

//global states of bonus things that can be toggled
int engageVerticalTakeOff = 0;
int engageNoseDive = 0;
int crashLanding = 0;
int bombsAway = 0;
int dayTime = 1;
int drawMissilesOnScreen = 0;
int shootMissiles = 0;
int drawInsignia = 0;

//bomb variables
GLfloat bombY = 0;
GLfloat bombYDecreaseBy = 0.005;
GLfloat bombYScreen = 0;
int bombFogOn = 0;

//missile variables
GLfloat missileZ = 0;
GLfloat missileZIncreaseBy = 0.005;

//variables for that contain the information required for texturing
GLuint seaPPM;
int seaWidth, seaHeight;
GLuint skyPPM;
int skyWidth, skyHeight;
GLuint mountainPPM;
int mountainPPMWidth, mountainPPMHeight;

//stores values for the mountains
int mountain1Height;
GLfloat mountain1Width;
GLfloat mountain1X;
GLfloat mountain1Z;
//stores values for the heights and base widths for the mountains
GLfloat level1Height;
GLfloat level1Width;
GLfloat level2Height;
GLfloat level2Width;
GLfloat level3Height;
GLfloat level3Width;
GLfloat level4Height;
GLfloat level4Width;
GLfloat level5Height;
GLfloat level5Width;

/************************************************************************
Function:		lineDrawer
Description : This function accepts a color array, the floats for a color,
and 3 coordinates. It is used by drawFrameOfReference() to draw the lines
in the reference grid.
*************************************************************************/
void lineDrawer(color col, float r, float g, float b, float x, float y, float z) {
	//set ambient to black, specular to black, specular to white, col to the
	//given color
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	glBegin(GL_LINES);
	glLineWidth(5);
	//set color, set normals which are y=0 because it's flat, draw vertices.
	glColor3f(r, g, b);
	glNormal3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glNormal3f(0, 1, 0);
	glVertex3f(x, y, z);
	glEnd();
	glLineWidth(1);
}

/************************************************************************
Function:		drawFrameOfReference
Description : This function draws the reference lines, grid, and a sphere. 
*************************************************************************/
void drawFrameOfReference() {
	//set whether or not lines should be filled in
	if (fillOnWireOff)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//draws the 3 lines
	lineDrawer(red, 1, 0, 0, 1, 0, 0);
	lineDrawer(green, 0, 1, 0, 0, 1, 0);
	lineDrawer(blue, 0, 0, 1, 0, 0, 1);

	// white sphere for origin
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glutSolidSphere(0.05, 50, 50);

	//draws the grid, the normals are all y=1 because it is a flat surface
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, black);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glBegin(GL_QUADS);
	for (int x = -C / 2; x < C / 2; x++) {
		for (int z = -C / 2; z < C / 2; z++) {
			glNormal3f(0, 1, 0);
			glVertex3f(x, 0, z);
			glNormal3d(0, 1, 0);
			glVertex3f(x + 1, 0, z);
			glNormal3d(0, 1, 0);
			glVertex3f(x + 1, 0, z + 1);
			glNormal3d(0, 1, 0);
			glVertex3f(x, 0, z + 1);
		}
	}
	glEnd();
}

/************************************************************************
Function:		readInCessnaAndPropeller
Description : This function reads in the cessna and propeller .txt files 
and stores in the data in arrays above.
*************************************************************************/
void readInCessnaAndPropeller() {
	FILE *pToFile = fopen("cessna.txt", "r");
	//reads input from the files
	char input[300];
	//values store information that are later incremented or decreased as required
	int vectorCount = 0, normalCount = 0, planePartCount = -1, faceCount = 0, offset = 0, i = 0, j = 0;
	GLfloat val = 0;

	if (pToFile == NULL) //print error message if file doesn't exist
		printf("Error, cannot open file.");
	else{
		while (fgets(input, sizeof(input), pToFile) != NULL) {
			char *counterOffset = input + 2;
			if (input[0] == 'v') {//fills up the plane vertex arrays
				sscanf(input, "v %f %f %f", &planeVertex[vectorCount][0], &planeVertex[vectorCount][1], &planeVertex[vectorCount][2]);
				vectorCount++;
			}
			else if (input[0] == 'n') {//fills up the plane normal arrays
				sscanf(input, "n %f %f %f", &planeNormal[normalCount][0], &planeNormal[normalCount][1], &planeNormal[normalCount][2]);
				normalCount++;
			}
			else if (input[0] == 'g') {//keeps track of which part the plane is being read in
				planePartCount++;
				if (planePartCount == 4) 
					i = 0;
				if (planePartCount == 6) 
					i = 0;
				if (planePartCount == 7) 
					i = 0;
				if (planePartCount == 8) 
					i = 0;
				if (planePartCount == 14) 
					i = 0;
				if (planePartCount == 26) 
					i = 0;
				if (planePartCount == 34) 
					i = 0;
			}
			//stores faces for objects 0-3, is yellow
			else if (input[0] == 'f' && planePartCount <= 3) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace1[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for objects 4-5, is black
			else if (input[0] == 'f' && planePartCount >3&&planePartCount <6) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace2[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for object 6, is light purple
			else if (input[0] == 'f' && planePartCount==6) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace3[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for objects 7, is blue
			else if (input[0] == 'f' && planePartCount==7) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace4[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for objects 8-13, is yellow
			else if (input[0] == 'f' && planePartCount >7 && planePartCount <14) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace5[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for objects 14-25, is blue
			else if (input[0] == 'f' && planePartCount >13&&planePartCount<26) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace6[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for objects 26-31, is yellow
			else if (input[0] == 'f' && planePartCount>=26&&planePartCount<=32) {
				j = 0;
				while (1==sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					planeFace7[i][j] = val;
					j++;
				}
				i++;
			}
		}
	}
	fclose(pToFile);

	//resets values
	vectorCount = 0; 
	normalCount = 0; 
	faceCount = 0;
	offset = 0;
	i = 0;
	j = 0;
	int propellerPartCount = -1;
	val = 0;
	FILE *pToFile2 = fopen("propeller.txt", "r");
	if (pToFile2 == NULL) {//print error message if file doesn't exist
		printf("Error, can't open file");
	}
	else {
		while (fgets(input, sizeof(input), pToFile2) != NULL) {
			char *counterOffset = input + 2;
			if (input[0] == 'v') {//fills up the propeller vertex arrays
				sscanf(input, "v %f %f %f", &propellerVertex[vectorCount][0], &propellerVertex[vectorCount][1], &propellerVertex[vectorCount][2]);
				vectorCount++;
			}
			else if (input[0] == 'n') {//fills up the propeller vertex arrays
				sscanf(input, "n %f %f %f", &propellerNormal[normalCount][0], &propellerNormal[normalCount][1], &propellerNormal[normalCount][2]);
				normalCount++;
			}
			else if (input[0] == 'g') {//keeps track of which part the plane is being read in
				propellerPartCount++;
				if (propellerPartCount == 1)
					i = 0;
			}
			//stores faces for object 0, is yellow
			else if (input[0] == 'f' && propellerPartCount == 0) {
				j = 0;
				while (1 == sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					propellerFace1[i][j] = val;
					j++;
				}
				i++;
			}
			//stores faces for object 1, is red
			else if (input[0] == 'f' && propellerPartCount == 1) {
				j = 0;
				while (1 == sscanf(counterOffset, " %f%n", &val, &offset)) {
					counterOffset += offset;
					propellerFace2[i][j] = val;
					j++;
				}
				i++;
			}
		}
	}
	fclose(pToFile2);
}

/************************************************************************
Function:		drawGreatBritainInsignia
Description : This function draws the British insignia you see on planes
in WW2 movies by drawing a blue disk, then a white disk, then a red disk.
*************************************************************************/
void drawGreatBritainInsignia() {
	//draws blue disk
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(0.04, 0.04, 0.04);
	GLUquadricObj* blueInsig;
	blueInsig = gluNewQuadric();
	gluQuadricDrawStyle(blueInsig, GLU_FILL);
	gluQuadricOrientation(blueInsig, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(blueInsig, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, blue);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluDisk(blueInsig, 1, 2, 30, 30);
	gluDeleteQuadric(blueInsig);
	glPopMatrix();

	//draws white disk
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(0.04, 0.04, 0.04);
	GLUquadricObj* whiteInsig;
	whiteInsig = gluNewQuadric();
	gluQuadricDrawStyle(whiteInsig, GLU_FILL);
	gluQuadricOrientation(whiteInsig, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(whiteInsig, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluDisk(whiteInsig, 0.5, 1, 30, 30);
	gluDeleteQuadric(whiteInsig);
	glPopMatrix();

	//draws red disk
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glScalef(0.04, 0.04, 0.04);
	GLUquadricObj* redInsig;
	redInsig = gluNewQuadric();
	gluQuadricDrawStyle(redInsig, GLU_FILL);
	gluQuadricOrientation(redInsig, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(redInsig, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluDisk(redInsig, 0, 0.5, 30, 30);
	gluDeleteQuadric(redInsig);
	glPopMatrix();
}

/************************************************************************
Function:		drawMissile
Description : This function draws  two missiles. They are composed of two shapes,
a white sphere which has been stretched in the y direction, and a red
cylinder that has been made to be a cone.
*************************************************************************/
void drawMissiles() {
	//draws the white streched sphere
	glPushMatrix();
	glTranslatef(-0.22, -0.162, -0.5);
	glRotatef(90, 0, 0, 1);
	glScalef(0.05, 0.25, 0.05);
	GLUquadricObj* whitePart;
	whitePart = gluNewQuadric();
	gluQuadricDrawStyle(whitePart, GLU_FILL);
	gluQuadricOrientation(whitePart, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(whitePart, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluSphere(whitePart, 0.5, 50, 50);
	gluDeleteQuadric(whitePart);
	glPopMatrix();

	
	//draws the red cone
	glPushMatrix();
	glTranslatef(-0.42, -0.162, -0.5);
	glRotatef(90, 0, 1, 0);
	glScalef(0.05, 0.025, 0.05);
	GLUquadricObj* redPart;
	redPart = gluNewQuadric();
	gluQuadricDrawStyle(redPart, GLU_FILL);
	gluQuadricOrientation(redPart, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(redPart, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(redPart, 0, 0.4, 2, 50, 50);
	gluDeleteQuadric(redPart);
	glPopMatrix();

	//draws the white streched sphere
	glPushMatrix();
	glTranslatef(-0.22, -0.162, 0.5);
	glRotatef(90, 0, 0, 1);
	glScalef(0.05, 0.25, 0.05);
	GLUquadricObj* whitePart2;
	whitePart2 = gluNewQuadric();
	gluQuadricDrawStyle(whitePart2, GLU_FILL);
	gluQuadricOrientation(whitePart2, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(whitePart2, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluSphere(whitePart2, 0.5, 50, 50);
	gluDeleteQuadric(whitePart2);
	glPopMatrix();


	//draws the red cone
	glPushMatrix();
	glTranslatef(-0.42, -0.162, 0.5);
	glRotatef(90, 0, 1, 0);
	glScalef(0.05, 0.025, 0.05);
	GLUquadricObj* redPart2;
	redPart2 = gluNewQuadric();
	gluQuadricDrawStyle(redPart2, GLU_FILL);
	gluQuadricOrientation(redPart2, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(redPart2, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, red);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(redPart2, 0, 0.4, 2, 50, 50);
	gluDeleteQuadric(redPart2);
	glPopMatrix();
}

/************************************************************************
Function:		drawCessnaPart
Description : This function draws each part of the plane. It accepts a value
that will be the array size, 3 color arrays, and an integer that represents 
the parts being drawn. It copies the global array to the local face array.
*************************************************************************/
void drawCessnaPart(int iGoto, color col1, color col2, color col3, int faceSelector) {
	GLfloat face[1617][30];//set to max size
	if (faceSelector == 1) //draw objects 0-3
		memcpy(&face, &planeFace1, sizeof(planeFace1));
	else if (faceSelector == 2)//draw objects 4-5
		memcpy(&face, &planeFace2, sizeof(planeFace2));
	else if (faceSelector == 3)//draw object 6
		memcpy(&face, &planeFace3, sizeof(planeFace3));
	else if (faceSelector == 4)//draw objects 7
		memcpy(&face, &planeFace4, sizeof(planeFace4));
	else if (faceSelector == 5)//draw objects 8-13
		memcpy(&face, &planeFace5, sizeof(planeFace5));
	else if (faceSelector == 6)//draw objects 14-25
		memcpy(&face, &planeFace6, sizeof(planeFace6));
	else if (faceSelector == 7)//draw objects 26-31
		memcpy(&face, &planeFace7, sizeof(planeFace7));
	else if(faceSelector==8)//draw propeller object 0
		memcpy(&face, &propellerFace1, sizeof(propellerFace1));
	else if (faceSelector == 9)//draw propeller object 1
		memcpy(&face, &propellerFace2, sizeof(propellerFace2));
	int index = 0;
	//make the right color
	glMaterialfv(GL_FRONT, GL_AMBIENT, col1);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, col2);
	glMaterialfv(GL_FRONT, GL_SPECULAR, col3);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	if (faceSelector < 8) {//for plane parts
		for (int i = 0; i < iGoto; i++) {
			glColor3f(0.8, 0.8, 0.8);
			glBegin(GL_POLYGON);
			//draw it from the normals and vertices, and the index
			for (int j = 0; face[i][j] > 0; j++) {
				index = face[i][j] - 1;
				glNormal3f(planeNormal[index][0], planeNormal[index][1], planeNormal[index][2]);
				glVertex3f(planeVertex[index][0], planeVertex[index][1], planeVertex[index][2]);
			}
			glEnd();
		}
	}
	else{//for propeller parts
		for (int i = 0; i < iGoto; i++) {
			glColor3f(0.8, 0.8, 0.8);
			glBegin(GL_POLYGON);
			//draw it from the normals and vertices, and the index
			for (int j = 0; face[i][j] > 0; j++) {
				index = face[i][j] - 1;
				glNormal3f(propellerNormal[index][0], propellerNormal[index][1], propellerNormal[index][2]);
				glVertex3f(propellerVertex[index][0], propellerVertex[index][1], propellerVertex[index][2]);
			}
			glEnd();
		}
	}
}

/************************************************************************
Function:		drawCessna()
Description : This function uses the drawCessnaPart method to draw each
part of the plane and propellers. It also translate the propellers to the
correct location.
*************************************************************************/
void drawCessna() {
	//draws insignia if on
	if (drawInsignia) {
		//draw right insignia
		glPushMatrix();
		glTranslatef(-0.22, -0.162, -0.5);
		drawGreatBritainInsignia();
		glPopMatrix();
		//draw left insignia
		glPushMatrix();
		glTranslatef(-0.22, -0.162, 0.5);
		drawGreatBritainInsignia();
		glPopMatrix();
	}
	if (drawMissilesOnScreen) {
		glPushMatrix();
		glTranslatef(missileZ, 0,0);
		drawMissiles();
		glPopMatrix();
	}
	glPushMatrix();
	//fills in polygons or leaves them as the grid appropriately
	if (fillOnWireOff)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	drawCessnaPart(701, black, yellow, white, 1);//draws objects 0-3
	drawCessnaPart(156, black, black, white, 2);//draws objects 4-5
	drawCessnaPart(186, black, purple, white, 3);//draws objects 6
	drawCessnaPart(175, black, blue, white, 4);//draws objects 7
	drawCessnaPart(762, black, yellow, white, 5);//draws objects 8--13
	drawCessnaPart(1617, black, blue, white, 6);//draws objects 14-25
	drawCessnaPart(743, black, yellow, white, 7);//draws objects 26-31

	//draws right propeller
	glPushMatrix();
	glTranslatef(0.0, 0.0, -0.7);
	glTranslatef(-3, -0.15, 0.35);
	glRotatef(propellerAngle, 1.0, 0.0, 0.0);
	glTranslatef(3, 0.15, -0.35);
	drawCessnaPart(120, black, yellow, white,8);
	drawCessnaPart(12, black, red, white, 9);
	glPopMatrix();

	//draws left propeller
	glPushMatrix();
	glTranslatef(-3, -0.15, 0.35);
	glRotatef(propellerAngle, 1.0, 0.0, 0.0);
	glTranslatef(3, 0.15, -0.35);
	drawCessnaPart(120, black, yellow, white, 8);
	drawCessnaPart(12, black, red, white, 9);
	glPopMatrix();
	glPopMatrix();
}

/************************************************************************
Function:		loadSeaPPM
Description : This function reads in the PPM file and saves it to seaPPM.
*************************************************************************/
void loadSeaPPM() {
	//temp variable storage
	GLubyte *seaData; 
	int totalPixelCount;
	int max;
	char temp;
	char input[200];
	int r, g, b;

	FILE *pToFile = fopen("sea02.ppm", "r");
	fscanf(pToFile,"%[^\n] ", input);//ignores first line
	//calculates the total amount of pixels
	fscanf(pToFile, "%d %d %d)", &seaWidth, &seaHeight, &max);
	totalPixelCount = seaWidth*seaHeight;
	seaData = malloc(3 * sizeof(GLuint)*totalPixelCount);
	//reads in all of the variable, assigns to seaData
	if (max == 255) {
		for (int i = 0; i < totalPixelCount; i++) {
			fscanf(pToFile, "%d %d %d", &r, &g, &b);
			seaData[3 * totalPixelCount-3*i - 3] = r;
			seaData[3 * totalPixelCount-3*i - 2] = g;
			seaData[3 * totalPixelCount-3*i - 1] = b;
		}
	}
	fclose(pToFile);

	//finishes the build of the sea
	glGenTextures(1, &seaPPM);
	glBindTexture(GL_TEXTURE_2D,seaPPM);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, seaWidth, seaHeight,
		GL_RGB, GL_UNSIGNED_BYTE, seaData);
	free(seaData);
}

/************************************************************************
Function:		loadSkyPPM
Description : This function reads in the PPM file and saves it to skyPPM.
*************************************************************************/
void loadSkyPPM() {
	//temp variable storage
	GLubyte *skyData;
	int totalPixelCount;
	int max;
	char temp;
	char input[200];
	int r, g, b;

	FILE *pToFile = fopen("sky08.ppm", "r");
	fscanf(pToFile, "%[^\n] ", input);//ignores first line
									  //calculates the total amount of pixels
	fscanf(pToFile, "%d %d %d)", &skyWidth, &skyHeight, &max);
	totalPixelCount = skyWidth*skyHeight;
	skyData = malloc(3 * sizeof(GLuint)*totalPixelCount);
	//reads in all of the variable, assigns to skyData
	if (max == 255) {
		for (int i = 0; i < totalPixelCount; i++) {
			fscanf(pToFile, "%d %d %d", &r, &g, &b);
			skyData[3 * totalPixelCount - 3 * i - 3] = r;
			skyData[3 * totalPixelCount - 3 * i - 2] = g;
			skyData[3 * totalPixelCount - 3 * i - 1] = b;
		}
	}
	fclose(pToFile);

	//finishes the build of the sky
	glGenTextures(1, &skyPPM);
	glBindTexture(GL_TEXTURE_2D, skyPPM);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, skyWidth, skyHeight,
		GL_RGB, GL_UNSIGNED_BYTE, skyData);
	free(skyData);
}

/************************************************************************
Function:		loadmountainPPM
Description : This function reads in the PPM file and saves it to mountainPPM.
*************************************************************************/
void loadMountainPPM() {
	//temp variable storage
	GLubyte *mountainData;
	int totalPixelCount;
	int max;
	char temp;
	char input[200];
	int r, g, b;

	FILE *pToFile = fopen("mount03.ppm", "r");
	fscanf(pToFile, "%[^\n] ", input);//ignores first line
									  //calculates the total amount of pixels
	fscanf(pToFile, "%d %d %d)", &mountainPPMWidth, &mountainPPMHeight, &max);
	totalPixelCount = mountainPPMWidth*mountainPPMHeight;
	mountainData = malloc(3 * sizeof(GLuint)*totalPixelCount);
	//reads in all of the variable, assigns to mountainData
	if (max == 255) {
		for (int i = 0; i < totalPixelCount; i++) {
			fscanf(pToFile, "%d %d %d", &r, &g, &b);
			mountainData[3 * totalPixelCount - 3 * i - 3] = r;
			mountainData[3 * totalPixelCount - 3 * i - 2] = g;
			mountainData[3 * totalPixelCount - 3 * i - 1] = b;
		}
	}
	fclose(pToFile);

	//finishes the build of the mountain
	glGenTextures(1, &mountainPPM);
	glBindTexture(GL_TEXTURE_2D, mountainPPM);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, mountainPPMWidth, mountainPPMHeight,
		GL_RGB, GL_UNSIGNED_BYTE, mountainData);
	free(mountainData);
}

/************************************************************************
Function:		drawSeaFog
Description : This function draws the pink sea fog exponentially.
*************************************************************************/
void drawSeaFog() {
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, pink);
	glFogf(GL_FOG_MODE, GL_EXP);
	glFogf(GL_FOG_DENSITY, 0.012);	
}

/************************************************************************
Function:		drawBombFog
Description : This function draws the red bomb fog exponentially.
*************************************************************************/
void drawBombFog() {
	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, red);
	glFogf(GL_FOG_MODE, GL_FOG);
	glFogf(GL_FOG_DENSITY, 0.02);
}

/************************************************************************
Function:		seaAndSky
Description : This function draws a disk, and a cylinder. It also loads 
the PPM textures onto them, and sea/bomb fog when applicable.
*************************************************************************/
void seaAndSky() {
	//everything here can be either wire frame or filled in

	//draws the cylinder and applies the texture to it. 
	GLUquadricObj *quadobject;
	quadobject = gluNewQuadric();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_EMISSION, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	glEnable(GL_TEXTURE_2D);
	gluQuadricDrawStyle(quadobject, GLU_FILL);
	gluQuadricOrientation(quadobject, GLU_OUTSIDE);
	gluQuadricNormals(quadobject, GLU_SMOOTH);
	gluQuadricTexture(quadobject, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, skyPPM);
	gluQuadricTexture(quadobject, skyPPM);
	glTranslatef(0, -1, 0);//lowers cylinder
	glRotatef(-90, 1.0, 0.0, 0.0);
	gluCylinder(quadobject, C/2, C/2, 35, 75, 60);
	glDisable(GL_TEXTURE);
	glMaterialfv(GL_FRONT, GL_EMISSION, black);
	glPopMatrix();

	//draws the disk/sea
	glPushMatrix();
	//draws the appropriate fog if applicable
	if (fog)
		drawSeaFog();
	if (bombFogOn)
		drawBombFog();
	glMaterialfv(GL_FRONT, GL_AMBIENT, black);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	glEnable(GL_TEXTURE_2D);
	glRotatef(270, 1, 0, 0);
	quadobject = gluNewQuadric();
	gluQuadricDrawStyle(quadobject, GLU_FILL);
	gluQuadricOrientation(quadobject, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(quadobject, GLU_SMOOTH);
	gluQuadricTexture(quadobject, GL_TRUE);
	glBindTexture(GL_TEXTURE_2D, seaPPM);
	gluQuadricTexture(quadobject, seaPPM);
	gluDisk(quadobject, 0, C/2+1, 75, 75);
	gluDeleteQuadric(quadobject);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

/************************************************************************
Function:		setUpMountainValues
Description : This function sets up the height, width, and level heights
and widths.
*************************************************************************/
void setUpMountainValues() {
	//the following values assigned are random 
	mountain1Height = rand() % 5+5;
	mountain1Width=rand()%5+5;
	mountain1X=rand()%5;
	mountain1Z=rand()%5;
	//these are not, but are based upon the random numbers assigned above
	level1Height = mountain1Height / 2;
	level1Width = mountain1Width / 2;
	level2Height = mountain1Height / 3;
	level2Width = mountain1Width / 3;
	level3Height = mountain1Height / 4;
	level3Width = mountain1Width / 4;
	level4Height = mountain1Height / 5;
	level4Width = mountain1Width / 5;
	level5Height = mountain1Height / 6;
	level5Width = mountain1Width / 6;
}

/************************************************************************
Function:		drawMountains
Description : This function draws a mountain by drawing 4 cylinders that are shaped
like cones all on top of eachother. The first one is green, the second is light green,
the third is grey and the fourth is white. By pressing 't' you can toggle between this,
and the mountain being textured. It used the variables set up in the method
setUpMountainValues.
*************************************************************************/
void drawMountains() {
	//sets up the green part of the mountain
	GLUquadricObj* cylBottom;
	cylBottom = gluNewQuadric();
	gluQuadricDrawStyle(cylBottom, GLU_FILL);
	gluQuadricOrientation(cylBottom, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(cylBottom, GLU_SMOOTH);
	gluQuadricTexture(cylBottom, GL_TRUE);

	//sets up the light green part of the mountain
	GLUquadricObj* cylGreen;
	cylGreen = gluNewQuadric();
	gluQuadricDrawStyle(cylGreen, GLU_FILL);
	gluQuadricOrientation(cylGreen, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(cylGreen, GLU_SMOOTH);
	gluQuadricTexture(cylGreen, GL_TRUE);

	//sets up the grey part of the mountain
	GLUquadricObj* cylGrey;
	cylGrey = gluNewQuadric();
	gluQuadricDrawStyle(cylGrey, GLU_FILL);
	gluQuadricOrientation(cylGrey, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(cylGrey, GLU_SMOOTH);
	gluQuadricTexture(cylGrey, GL_TRUE);

	//sets up the white part of the mountain
	GLUquadricObj* cylWhite;
	cylWhite = gluNewQuadric();
	gluQuadricDrawStyle(cylWhite, GLU_FILL);
	gluQuadricOrientation(cylWhite, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(cylWhite, GLU_SMOOTH);
	gluQuadricTexture(cylWhite, GL_TRUE);


	//draws the green part of the mountain
	glPushMatrix();
	glTranslatef(mountain1X, 0, mountain1Z);
	glRotatef(-90, 1, 0, 0);
	if (mountainTextureOn) {//manages whether or not this part is textured
		glEnable(GL_TEXTURE_2D);
		gluQuadricDrawStyle(cylBottom, GLU_FILL);
		gluQuadricOrientation(cylBottom, GLU_OUTSIDE);
		gluQuadricNormals(cylBottom, GLU_SMOOTH);
		gluQuadricTexture(cylBottom, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, mountainPPM);
		gluQuadricTexture(cylBottom, mountainPPM);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(cylBottom, GL_FALSE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, green);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, green);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(cylBottom, level1Width, level2Width, level1Height, 50, 50);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	gluDeleteQuadric(cylBottom);

	//draws the light green part of the mountain
	glPushMatrix();
	glTranslatef(mountain1X, level1Height, mountain1Z);
	glRotatef(-90, 1, 0, 0);
	if (mountainTextureOn) {//manages whether or not this part is textured
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		gluQuadricDrawStyle(cylGreen, GLU_FILL);
		gluQuadricOrientation(cylGreen, GLU_OUTSIDE);
		gluQuadricNormals(cylGreen, GLU_SMOOTH);
		gluQuadricTexture(cylGreen, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, mountainPPM);
		gluQuadricTexture(cylGreen, mountainPPM);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(cylGreen, GL_FALSE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, lightGreen);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, lightGreen);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(cylGreen, level2Width, level3Width, level2Height, 50, 50);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	gluDeleteQuadric(cylGreen);

	//draws the grey part of the mountain
	glPushMatrix();
	glTranslatef(mountain1X, level1Height + level2Height, mountain1Z);
	glRotatef(-90, 1, 0, 0);
	if (mountainTextureOn) {//manages whether or not this part is textured
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		gluQuadricDrawStyle(cylGrey, GLU_FILL);
		gluQuadricOrientation(cylGrey, GLU_OUTSIDE);
		gluQuadricNormals(cylGrey, GLU_SMOOTH);
		gluQuadricTexture(cylGrey, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, mountainPPM);
		gluQuadricTexture(cylGrey, mountainPPM);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(cylGrey, GL_FALSE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(cylGrey, level3Width, level4Width, level3Height, 50, 50);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	gluDeleteQuadric(cylGrey);

	//draws the white part of the mountain
	glPushMatrix();
	glTranslatef(mountain1X, level1Height + level2Height + level3Height, mountain1Z);
	glRotatef(-90, 1, 0, 0);
	if (mountainTextureOn) {//manages whether or not this part is textured
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
		glEnable(GL_TEXTURE_2D);
		gluQuadricDrawStyle(cylWhite, GLU_FILL);
		gluQuadricOrientation(cylWhite, GLU_OUTSIDE);
		gluQuadricNormals(cylWhite, GLU_SMOOTH);
		gluQuadricTexture(cylWhite, GL_TRUE);
		glBindTexture(GL_TEXTURE_2D, mountainPPM);
		gluQuadricTexture(cylWhite, mountainPPM);
	}
	else {
		glDisable(GL_TEXTURE_2D);
		gluQuadricTexture(cylWhite, GL_FALSE);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, white);
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
	}
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(cylWhite, level4Width, 0, level4Height, 50, 50);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	gluDeleteQuadric(cylWhite);
}

/************************************************************************
Function:		drawBomb
Description : This function draws a bomb. It is composed of two shapes, 
a grey sphere which has been stretched in the y direction, and a grey
cylinder that has been made to be a cone. 
*************************************************************************/
void drawBomb() {
	//draws the bottom grey sphere
	glPushMatrix();
	glTranslatef(0.0, -0.9, -2.5);
	glScalef(0.05, 0.15, 0.05);
	GLUquadricObj* bombBottom;
	bombBottom = gluNewQuadric();
	gluQuadricDrawStyle(bombBottom, GLU_FILL);
	gluQuadricOrientation(bombBottom, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(bombBottom, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,grey);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluSphere(bombBottom, 0.5, 50, 50);
	gluDeleteQuadric(bombBottom);
	glPopMatrix();

	//draws the bottom grey cone
	glPushMatrix();
	glTranslatef(0.0, -0.9, -2.5);
	glRotatef(-90, 1, 0, 0);
	glScalef(0.05, 0.025, 0.05);
	GLUquadricObj* bombTop;
	bombTop = gluNewQuadric();
	gluQuadricDrawStyle(bombTop, GLU_FILL);
	gluQuadricOrientation(bombTop, GLU_OUTSIDE);
	glShadeModel(GL_SMOOTH);
	gluQuadricNormals(bombTop, GLU_SMOOTH);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, grey);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, grey);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, grey);
	glMaterialf(GL_FRONT, GL_SHININESS, 60);
	gluCylinder(bombTop, 0, 1, 2, 50, 50);
	gluDeleteQuadric(bombTop);
	glPopMatrix();
}


/************************************************************************
Function:		 myDisplay
Description:	This function is where everything is drawn. It sets the 
camera location, the lighting, the sea/sky or frame of reference, the 
mountains, the plane, and bombs. It also swaps the buffer.
*************************************************************************/
void myDisplay() {
	GLfloat lightPos[] = { 0, 50, lightZ, 1 };
	//set background to black, clear buffer, load identity
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	//position camera accordingly
	gluLookAt(xCameraEye, yCameraEye, zCameraEye,
		xCameraCenter, yCameraCenter, zCameraCenter,
		0, 1, 0);

	//position light
	glPushMatrix();
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glPopMatrix();

	//draws sea and sky, or frame of reference appropriately. 
	if(seaSkyOnRefOff)
		seaAndSky();
	else
		drawFrameOfReference();

	/*If mountains are on, it will use the same method 4 times to draw
	four mountains, however the mountains will be stretched and positioned
	differently.
	*/
	if (mountainsOn) {
		//draws mountain 1
		glPushMatrix();
		glTranslatef(2, 0, 20);
		glScalef(2, 0.5, 3);
		drawMountains();
		glPopMatrix();

		//draws mountain 2
		glPushMatrix();
		glRotatef(90, 0, 1, 0);
		glScalef(0.5, 0.5, 3);
		drawMountains();
		glPopMatrix();

		//draws mountain 3
		glPushMatrix();
		glTranslatef(20, 0, 15);
		glRotatef(123, 0, 1, 0);
		glScalef(0.5, 0.75, 3);
		drawMountains();
		glPopMatrix();

		//draws mountain 4 - no x or z stretching (other than to shrink)
		glPushMatrix();
		glTranslatef(-20, 0, -35);
		glRotatef(123, 0, 1, 0);
		glScalef(0.5, 0.75, 0.5);
		drawMountains();
		glPopMatrix();
	}
	glPopMatrix();

	// Draw the plane
	glPushMatrix();
	glTranslatef(0.0, -0.5, -2.0);
	glRotatef(-90, 0, 1, 0);
	glRotatef(-planeTilt, 1, 0, 0);
	glRotatef(zTilt, 0, 0, 1);
	glScalef(0.5, 0.5, 0.5);
	drawCessna();
	glPopMatrix();
	
	//draws the bomb
	if (bombsAway) {
		glPushMatrix();
		glTranslatef(0, bombYScreen, 0);
		drawBomb();
		glPopMatrix();
	}

	// uses double buffering
	glutSwapBuffers();
}

/************************************************************************
Function:		 myWindow
Description:	This function handles a window reshape if required, and sets
the viewing modes.
*************************************************************************/
void myWindow(int w, int h) {
	windowWidth = w;
	windowHeight = h;
	//uses a view port
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)windowWidth / (float)windowHeight, .1, 10000);
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************
Function:		initializeGL
Description:	This function sets up openGL, enables depth testing, 
the background color to black, and sets up the lighting.
*************************************************************************/
void initializeGL() {
	//sets where window is located
	glutPositionWindow(0, 0);
	//enables depth
	glEnable(GL_DEPTH_TEST);
	glClearColor(0, 0, 0, 1);//clears colour
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)windowWidth / (float)windowHeight, .1, 10000);
	//sets of lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_NORMALIZE);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************
Function:		 keyStates
Description:	This function checks to see if a button which changes the
scene has been pressed. 
*************************************************************************/
void keyStates(unsigned char key) {
	//turns on full screen
	if (key == 'f'&&fullScreen == 0) {
		fullScreen = 1;
		glutFullScreen();
	}
	//turns off full screen
	else if (key == 'f'&&fullScreen == 1) {
		fullScreen = 0;
		glutPositionWindow(0, 0);
		glutReshapeWindow(800, 600);
	}
	//toggles fill vs lines
	else if (key == 'w')
		fillOnWireOff = !fillOnWireOff;
	//quits the program
	else if (key == 'q')
		exit(1);
	//toggles if you can see the sea and sky, or reference grid
	else if (key == 's')
		seaSkyOnRefOff = !seaSkyOnRefOff;
	//toggles sea fog
	else if (key == 'b')
		fog = !fog;
	//toggles whether the mountains are on or off
	else if (key == 'm')
		mountainsOn = !mountainsOn;
	//toggles whether the mountains textures are on or off
	else if (key == 't')
		mountainTextureOn = !mountainTextureOn;
	//makes a vertical take off happen
	else if (key == 'v') {
		engageVerticalTakeOff = 1;
		yToBe = yCameraCenter + 30;
	}
	//engages a nose dive
	else if (key == 'n') {
		engageNoseDive = 1;
		yToBe = yCameraCenter - 30;
	}
	//crashes the plane, and exits program
	else if (key == 'c') {
		crashLanding = 1;
		yToBe = 0;
	}
	//drops bomb
	else if (key == 'a') {
		bombsAway = 1;
		bombY = yCameraCenter;
	}
	//toggles sunset
	else if (key == 'd') {
		dayTime = !dayTime;
	}
	//toggles insignia
	else if (key == 'i') {
		drawInsignia = !drawInsignia;
	}
	//toggles missiles being on screen
	else if (key == 'p') {
		drawMissilesOnScreen = !drawMissilesOnScreen;
	}
	//toggles shooting missiles
	else if (key == 'o') {
		shootMissiles = 1;
	}
}

/************************************************************************
Function:		 specialKeyPress
Description:	This function checks to see if a button which changes the
camera/plane has been pressed. 
*************************************************************************/
void specialKeyPress(unsigned char key) {
	//increases plane speed
	if (key == GLUT_KEY_PAGE_UP) {
		speedOfPlane += planeSpeedIncrease;
	}
	//decreases plane speed
	else if (key == GLUT_KEY_PAGE_DOWN&&speedOfPlane >= 0.005) {
		speedOfPlane -= planeSpeedIncrease;
	}
	//turns on yMotion to up
	else if (key == GLUT_KEY_UP)
		yMotion = 1;
	//turns on yMotion to down
	else if (key == GLUT_KEY_DOWN)
		if (yCameraCenter >= 2)
			yMotion = -1;
		else
			yMotion = 0;
}

/************************************************************************
Function:		 specialKeyRealese
Description:	This function checks to see if a button which changes the
camera/plane has been pressed.
*************************************************************************/
void specialKeyRelease(unsigned char key) {
	//stops increasing speed
	if (key == GLUT_KEY_PAGE_UP) {
		speedOfPlane += 0;
	}
	//stops decreasing speed
	else if (key == GLUT_KEY_PAGE_UP) {
		speedOfPlane -= 0;
	}
	//stops going up 
	else if (key == GLUT_KEY_UP)
		yMotion = 0;
	//stops going down
	else if (key == GLUT_KEY_DOWN)
		yMotion = 0;
}

/************************************************************************
Function:		 myMouse
Description:	This function checks to see the x position of the cursor,
and updates the tilt of the plane on the xAxis accordingly.  
*************************************************************************/
void myMouse(int x) {
	mouseX = ((x - windowWidth / 2)/(windowWidth/2))*.5;
	if(!crashLanding)//only changes the tilt if the plane isn't crashing
		planeTilt = ((x - windowWidth / 2) / (windowWidth / 2)) * 45;
	angle = mouseX*-1;
}

/************************************************************************
Function:		 idle
Description:	This function changes the value of the camera eye and
centers when appropriate. It handles all animations. It increases the y 
position, tilt on the x axis, vertical take off, propeller rotation, 
nose dives, crash landings and bombings.
*************************************************************************/
void idle() {
	//handles y position
	if (yMotion != 0) {
		if (yMotion ==1 && zTilt >= -10)
			zTilt -= zTiltRate * 5;
		if (yMotion == -1 && zTilt <= 10)
			zTilt += zTiltRate * 5;
		yCameraCenter += yMotion*speedOfCamera;
		yCameraEye += yMotion*speedOfCamera;
	}
	if (yMotion == 0&&engageVerticalTakeOff==0&&engageNoseDive==0) {
		if (zTilt >= 0)
			zTilt -= zTiltRate * 5;
		if (zTilt <= 0)
			zTilt += zTiltRate * 5;
	}
	//handles the tilt on the x axis and changes the direction of the plane, if 
	//plane isn't crashing
	if (!crashLanding) {
		planeRotation += angle;
		zCameraEye += (cos(planeRotation*DEG_TO_RAD)*speedOfPlane);
		xCameraEye += (sin(planeRotation*DEG_TO_RAD)*speedOfPlane);
		zCameraCenter = zCameraEye + cos(planeRotation*DEG_TO_RAD)*tiltRate;
		xCameraCenter = xCameraEye + sin(planeRotation*DEG_TO_RAD)*tiltRate;
	}
	//handles propeller rotation
	propellerAngle += (propellerRotation*speedOfPlane);
	if (propellerAngle >= 360);
		propellerAngle -= 360*propellerRotation*speedOfPlane;

	//handles a vertical take off
	if (engageVerticalTakeOff) {
		if (yCameraCenter <= yToBe) {
			yCameraCenter += speedOfCamera * 2;//increases y postion
			yCameraEye += speedOfCamera * 2;
			if (zTilt >= -50)
				zTilt -= zTiltRate * 5;//decreases z tilt
		}
		else if (yCameraCenter >= yToBe) {//changes z tilt back to normal
			if (zTilt <= 0) {
				zTilt += zTiltRate * 10;
			}
			else if (zTilt >= 0)
				engageVerticalTakeOff = 0;
		}
	}

	//handles a nose dive
	if (engageNoseDive) {
		if (yCameraCenter >= yToBe) {
			yCameraCenter -= speedOfCamera * 2;//decreases y postion
			yCameraEye -= speedOfCamera * 2;
			if(zTilt <= 50)
				zTilt += zTiltRate * 5;//increases z tilt
		}
		else if (yCameraCenter <= yToBe) {//changes z tilt back to normal
			if (zTilt >= 0) {
				zTilt -= zTiltRate * 10;
			}
			else if (zTilt <= 0)
				engageNoseDive = 0;
		}
	}
	//crashes plane, lowers y, changes x and z tilt
	if (crashLanding) {
		if (yCameraCenter >= yToBe) {
			yCameraCenter -= speedOfCamera * 2;
			yCameraEye -= speedOfCamera * 2;
			zTilt += zTiltRate * 5;
			planeTilt += zTiltRate * 5;
		}
		else if (yCameraCenter <= yToBe) {
			exit(1);//exits
		}
	}

	//drops a bomb, once it hits the ocean the ocean goes red for a small amount of time
	if (bombsAway) {
		if (bombY >= 0)
			bombY-= bombYDecreaseBy*2;
		if(bombY>=0)
			bombYScreen -= bombYDecreaseBy;
		if (bombY > 0.1 && bombY <= 1.5)
			bombFogOn = 1;
		else
			bombFogOn = 0;
		if (bombY <= 0) {
			bombsAway = 0;
			bombY = yCameraCenter;
			bombYScreen = 0;
		}
	}
	//toggles sunset effect and makes screen darker
	if (dayTime == 0) {
		if (lightZ <= 100)
			lightZ += lightZDelta;
	}
	if (dayTime == 1) {
		if (lightZ >= 0)
			lightZ -= lightZDelta;
	}
	//controls Z position of missile if shot
	if (shootMissiles) {
		if (missileZ >= -20)
			missileZ -= 25*missileZIncreaseBy;
		if (missileZ <= -20) {//resets it
			missileZ = 0;
			shootMissiles = 0;
		}
	}

	//refresh screen
	glutPostRedisplay();
}

/************************************************************************
Function:		printControls
Description:	This function prints how to control the scene and camera.
*************************************************************************/
void printControls() {
	printf("Scene Controls\n");
	printf("-------------------------------\n");
	printf("w: toggle wire frame and solid rendering\n");
	printf("f: toggle full screen and windowed mode\n");
	printf("q: quit\n");
	printf("s: toggle between sea/sky and reference grid\n");
	printf("b: toggle fog\n");
	printf("m: toggle mountains on and off\n");
	printf("t: toggle mountain texture on and off\n\n");

	printf("Camera Controls\n");
	printf("-------------------------------\n");
	printf("Up\tArrow: \tmove up\n");
	printf("Down\tArrow: \tmove down\n");

	printf("PAGE\tUP: \tspeed up\n");
	printf("PAGE\tDOWN: \tspeed down\n");
	printf("move cursor to left of plane: \tmove left\n");
	printf("move cursor to right of plane: \tmove right\n\n");
	printf("Bonus Controls\n");
	printf("-------------------------------\n");
	printf("a: drop a bomb\n");
	printf("v: vertical take off\n");
	printf("n: nose dive\n");
	printf("c: crash\n");
	printf("d: toggle sunset\n");
	printf("i: toggle insignia\n");
	printf("p: draw missiles\n");
	printf("o: shoot missiles\n");
}

/************************************************************************
Function:		main
Description:	This function is where the magic happens. It calls all 
important functions, and then turns into an infinite loop.
*************************************************************************/
void main(int argc, char** argv) {
	//prints controls
	printControls();
	//makes random numbers truly random
	srand(time(NULL));
	//reads in "cessna.txt" and "propeller.txt"
	readInCessnaAndPropeller();
	//sets up mountain values.
	setUpMountainValues();
	//initalizes important requirements 
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0, 0);
	//creates the window
	glutCreateWindow("Liam Gowan's CSCI 3161 Final Project - Flight Simulator");
	//calls the function that displays everything
	glutDisplayFunc(myDisplay);
	//calls the function that controls the animation
	glutIdleFunc(idle);
	//allows the window to be reshaped
	glutReshapeFunc(myWindow);
	//makes it look out for normal key presses
	glutKeyboardFunc(keyStates);
	//makes it look out for special keys being pressed and released
	glutSpecialFunc(specialKeyPress);
	glutSpecialUpFunc(specialKeyRelease);
	//makes it look out for the x position of the cursor
	glutPassiveMotionFunc(myMouse);
	//initializes openGL
	initializeGL();
	//loads sea texture
	loadSeaPPM();
	//loads sky texture
	loadSkyPPM();
	//loads mountains
	loadMountainPPM();
	//enter infinite loop
	glutMainLoop();
}