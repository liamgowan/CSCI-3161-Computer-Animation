/************************************************************************
File:			solarsystem.c

Description:	This is a C program that uses opengl and free glut to
display a solar system. It displays the planets, including the moon around 
the earth, which rotate around the sun. By pressing 'r', the orbits will 
appear or vanish. By pressing 's', stars will vanish or appear. By
pressing 'c', the corona will appear or vanish. By pressing 's', the 
shield around the enterprise will appear or vanish. The camera can be
controlled by pressing the arrow keys, page up (to go forward.), and 
page down (to go backwards).

Bonus: 

Author: Liam Gowan B00673126
Note: This program uses some ideas discussed from the course website.
Specific information used for this assignment can be found from the
following links:

Planet size: http://www.universetoday.com/36649/planets-in-order-of-size/
Planet order: http://www.universetoday.com/72305/order-of-the-planets-from-the-sun/
Planet speed: http://www.sjsu.edu/faculty/watkins/orbital.htm
Color chart: http://www.rapidtables.com/web/color/RGB_Color.htm
*************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <GL/freeglut.h>
#include <math.h>

//dimensions for window
float windowHeight = 1200;
float windowWidth = 800;

//used for rotation
int year = 0;

//stores X, Y, and Z values for all 500 stars
int starX[500];
int starY[500];
int starZ[500];


//global states of features, orbits corona and shield are off by default, stars on by default
int orbitsOn = 0;
int starsOn = 1;
int coronaOn = 0;
int shieldOn = 0;
int voyagerOn = 0;

//used to store the vertex (float) value for interprise.txt file
struct modelVertex {
	GLfloat point1;
	GLfloat point2;
	GLfloat point3;
};

//used to store the faces (int) value for interprise.txt file
struct modelFace {
	GLint point1;
	GLint point2;
	GLint point3;
};
//The number of vertices and faces was found by copying and pasting enterprise.txt
//into a Word Document, where I then used the line count feature to count the number of 
//each lines, which is the resulting size of the arrays below
struct modelVertex vertex[1201];
struct modelFace face[1989];

struct modelVertex voyagerVertex[4404];
struct modelFace voyagerFace[6623];

//used while camera is moving
GLfloat xCamMove = 0;
GLfloat yCamMove = 0;
GLfloat zCamMove = 0;

//camera variables for the eye, also sets the default viewing point
GLfloat xCamEye = 0;
GLfloat yCamEye = 6;
GLfloat zCamEye = 40;

//camera variable for x,y and z center
GLfloat xCamCenter = 0;
GLfloat yCamCenter = 0;
GLfloat zCamCenter = 0;

GLfloat voyagerXPos = -45;

//radii for of all planets 
//Up to mars is relative to earth, after that is now relative because of how big the planets are
//compared to earth
float mercuryRadius = 0.38f;
float venusRadius = 0.95f;
float earthRadius = 1;
float moonRadius = 0.27f;
float marsRadius = 0.53f;
float jupiterRadius = 2;
float saturnRadius = 1.8f;
float uranusRadius = 1.4f;
float neptuneRadius = 1.35f;
float plutoRadius = 0.18;

//distances of each planet from sun (or earth in case of moon)
float mercuryDistanceFromSun = 6;
float venusDistanceFromSun = 9;
float earthDistanceFromSun = 10;
float moonDistanceFromEarth = 2.5f;
float marsDistanceFromSun = 13;
float jupiterDistanceFromSun = 17;
float saturnDistanceFromSun = 22;
float uranusDistanceFromSun = 27;
float neptuneDistanceFromSun = 31;
float plutoDistanceFromSun = 33;

/************************************************************************
Function:		drawOrbit
Description:	This function accepts the distance of the planet away from earth,
and will then draw an orbit accordingly. It can also be used to draw orbits,
like the one around Saturn. Also takes in the colors for the orbit.
*************************************************************************/
void drawOrbit(float r, float red, float green, float blue) {
	glPushMatrix();
	//makes sure that orbits are on correct axis
	glRotatef(90, 0, 1, 0);
	glRotatef(90, 1, 0, 0);
	glBegin(GL_LINE_LOOP);
	glColor3f(red, green, blue);
	for (float i = 0; i < 360; i++) {
		float theta = 2.0f * 3.1415926 * i / 360;
		float xPos = r * cos(theta);
		float yPos = r * sin(theta);
		glVertex2f(xPos, yPos);
	}
	glEnd();
	glPopMatrix();
}

/************************************************************************
Function:		drawAllOrbits
Description:	This function implements the drawOrbit method to draw all 
the orbits.
*************************************************************************/
void drawAllOrbits() {
	//mercury orbit
	drawOrbit( mercuryDistanceFromSun,1,1,1);
	//venus orbit
	drawOrbit( venusDistanceFromSun,1,1,1);
	//earth orbit
	drawOrbit( earthDistanceFromSun,1,1,1);
	//mars orbit
	drawOrbit( marsDistanceFromSun,1,1,1);
	//jupiter orbit
	drawOrbit( jupiterDistanceFromSun,1,1,1);
	//saturn orbit
	drawOrbit( saturnDistanceFromSun,1,1,1);
	//uranus orbit
	drawOrbit( uranusDistanceFromSun,1,1,1);
	//neptune orbit
	drawOrbit( neptuneDistanceFromSun,1,1,1);
	//pluto orbit
	drawOrbit(plutoDistanceFromSun,1,1,1);
}

/************************************************************************
Function:		drawPlanet
Description:	This function will draw a sphere, or planet. It accepts an angle
to be rotated by around the y axis, a distance away from the center, a radius, 
and color values.
*************************************************************************/
void drawPlanet(GLfloat angle, GLfloat distance, float radius, float red, float green, float blue) {
	glPushMatrix();
	//rotates around y axis
	glRotatef(angle, 0, 1, 0);
	//translate away appropriately
	glTranslatef(distance, 0, 0);
	//makes it the correct color and draws a solid sphere
	glColor3f(red, green, blue);
	glutSolidSphere(radius, 60, 60); 
	glTranslatef(-distance, 0, 0);
	glPopMatrix();
}

/************************************************************************
Function:		drawSaturn
Description:	This function is the same as drawPlanet, however it draws a ring
around it, to make Saturn look like Saturn.
*************************************************************************/
void drawSaturn(GLfloat angle, GLfloat distance, float radius, float red, float green, float blue) {
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(distance, 0, 0);\
	glRotatef(45, 1, 0, 0);
	//increases line size
	glLineWidth(3);
	//makes ring and planet the same color, except the ring is slightly more red.
	drawOrbit(3,red, green,blue);
	glColor3f(red+.5, green, blue);
	//sets line width back to default
	glLineWidth(1);
	glutSolidSphere(radius, 60, 60);
	glPopMatrix();
}

/************************************************************************
Function:		drawEarthAndMoon
Description:	This function is the same as drawPlanet, however it also 
draws a moon rotating around Earth and its orbit
*************************************************************************/
void drawEarthAndMoon(float earthYear) {
	glPushMatrix();
	//handles earth translation and rotation
	glRotatef((GLfloat)-earthYear, 0, 1, 0);
	glTranslatef(earthDistanceFromSun, 0, 0);
	glRotatef((GLfloat)earthYear, 0, 1, 0);
	glPushMatrix();
	//draws orbit if on
	if(orbitsOn)
		drawOrbit(moonDistanceFromEarth,1,1,1);
	glColor3f(0, 0, 1);
	//draws earth
	glutSolidSphere(earthRadius, 60, 60);
	glPopMatrix();
	//handles moon translation and rotation
	glRotatef((GLfloat)5 * earthYear, 0, 1, 0);
	glTranslatef(moonDistanceFromEarth, 0, 0);
	glRotatef((GLfloat)-5 * earthYear, 0, 1, 0);
	glColor3f(1, 1, 1);
	//draws moon
	glutSolidSphere(moonRadius, 60, 60);
	glPopMatrix();
}

/************************************************************************
Function:		drawAllPlanets
Description:	This function draws all the planets.
*************************************************************************/
void drawAllPlanets() {
	//The following multiply the year value by what the years for the planets 
	//are relative to earth years
	int mercuryYear = 1.607 * year;
	int venusYear = 1.174 * year;
	int earthYear = year;
	int marsYear = 0.802*year;
	int jupiterYear = 0.434 * year;
	int saturnYear = 0.323* year;
	int uranusYear = 0.228 * year;
	int neptuneYear = 0.182 *year;
	int plutoYear = 0.159 *year;
	//Draw the sun
	glColor3f(1, 1, 0);
	glutSolidSphere(3, 60, 60);
	//draw mercury
	drawPlanet(-mercuryYear, mercuryDistanceFromSun, mercuryRadius, 0.5f, 0.5f, 0);
	//draw venus
	drawPlanet(venusYear, venusDistanceFromSun, venusRadius, 1, 0.5f, 0);
	//draw earth and moon
	drawEarthAndMoon(earthYear);
	//draw Mars
	drawPlanet(-marsYear, marsDistanceFromSun, marsRadius, 1, 0, 0);
	//draw Jupiter
	drawPlanet(-jupiterYear, jupiterDistanceFromSun, jupiterRadius, 1, 0.498, 0.313);
	//draw Saturn
	drawSaturn(-saturnYear, saturnDistanceFromSun, saturnRadius, .96f, 0.64f, 0.376);
	//draw Uranus
	drawPlanet(uranusYear, uranusDistanceFromSun, uranusRadius, 0, 1, 1);
	//draw Neptune
	drawPlanet(-neptuneYear, neptuneDistanceFromSun, uranusRadius, 0.5, .5, 1);
	//draw Pluto
	drawPlanet(-plutoYear, plutoDistanceFromSun, plutoRadius, 0.8, .52, 0.25);
}

/************************************************************************
Function:		starLocationMaker
Description:	This function is only executed once. It makes the x, y and
z location of all 500 stars. 
*************************************************************************/
void starLocationMaker() {
	srand(time(NULL));
	for (int i = 0; i < 500; i++) {
		//The first 125 are in the positive x, positve y region, and the z
		//can be between -10 to 10
		if (i < 125) {
			starX[i] = (rand() % 100);
			starY[i] = (rand() % 100);
			if(rand()%2==1)
				starZ[i] = (rand() % 10);
			else
				starZ[i] = -(rand() % 10);

		}
		//The stars between 125 and 249 are in the negative x, positve y region, and the z
		//can be between -10 to 10
		if (i >= 125 && i<250) {
			starX[i] = -(rand() % 100);
			starY[i] = (rand() % 100);
			if (rand() % 2 == 1)
				starZ[i] = (rand() % 10);
			else
				starZ[i] = -(rand() % 10);
		}
		//The stars between 250 and 379 are in the negative x, negative y region, and the z
		//can be between -10 to 10
		if (i >= 250 && i<375) {
			starX[i] = -(rand() % 100);
			starY[i] = -(rand() % 100);
			if (rand() % 2 == 1)
				starZ[i] = (rand() % 10);
			else
				starZ[i] = -(rand() % 10);
		}
		//The stars over and including 375 are in the postive x, negative y region, and the z
		//can be between -10 to 10
		if (i >= 375) {
			starX[i] = (rand() %100);
			starY[i] = -(rand() % 100);
			if (rand() % 2 == 1)
				starZ[i] = (rand() % 10);
			else
				starZ[i] = -(rand() % 10);
		}
	}
}

/************************************************************************
Function:		starDrawer
Description:	This function is executed infinitly. It draws all the stars
at locations determined in the starLocationMaker function, and changes
the color randomly.
*************************************************************************/
void starDrawer() {
	for (int i = 0; i < 500; i++) {
		glPushMatrix();
		glTranslatef(starX[i], starY[i], starZ[i]);
		glColor3f((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
		glutSolidSphere(.1, 20, 20);
		glPopMatrix();
	}
}

/************************************************************************
Function:		drawCorona
Description:	This function draws the suns corona. Since the sun has a 
radius of 3, the lines start at 2.5, and end at a number picked between 1 
and 7, plus 3. A line which stars off yellow and opaque, is then drawn
all around the sun, and becomes orange and transparent. There is also a 1 
in 3 chance that a line will be drawn for every value between 1 and 360.
*************************************************************************/
void drawCorona() {
	GLfloat length =0;
	GLfloat start = 2.5;//starting value
	//makes line look smooth
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBegin(GL_LINES);
	GLfloat startX = 0;
	GLfloat startY = 0;
	GLfloat xCoord=0;
	GLfloat yCoord=0;
	for (int i = 0; i < 360; i++) {
		if (rand() % 3 == 1) {//ensures that it look random
			//yellow, opaque
			glColor4f(1, 1, 0, 1);
			length = (rand() % 7) + 3;//random length
			startX = start*cos(i*0.0174532777);//finds start X
			startY = start*sin(i*0.0174532777);//finds start Y
			glVertex2f(startX, startY);
			xCoord = length*cos(i*0.0174532777);//finds end X
			yCoord = length*sin(i*0.0174532777);//find end Y
			//Orange, transparent
			glColor4f(1, .3, 0, 0);
			glVertex2f(xCoord, yCoord);
		}
	}
	glEnd();
}

/************************************************************************
Function:		drawShield
Description:	This function draws the shield around the enterprise. 
It uses two wire spheres (grey and blue) which spin, and a solid streched 
sphere.
*************************************************************************/
void drawShield() {
	GLfloat randomBlue = (rand() % 100) / 100.0f;//random blue color is picked
	GLfloat randomGrey = (rand() % 100) / 100.0f;//random grey color is picked
	glPushMatrix();
	glTranslatef(0 + xCamCenter,yCamCenter-4, zCamCenter+18);//translate to where enterprise is, accounts for camera change
	glRotatef(-20, 1, 0, 0);
	glRotatef(-xCamMove * 10, 0, 1, 1);
	glRotatef(yCamMove * 10, 1, 0, 0);
	glRotatef(zCamMove * 10, 1, 0, 0);
	glScalef(2.5, 2.5, 5);//scales in a way that the enterprise fits in it
	//draws a spinning blue wire sphere for a twinkling effect
	glRotatef(30 * year, 0, 0, 1);
	glColor4f(0, 0, randomBlue, 0.3);
	glutWireSphere(2, 10, 10);
	//draws a solid sphere, except it is translucent
	glColor4f(randomGrey, randomGrey, randomGrey, 0.2);
	glutSolidSphere(2, 10, 10);
	//draws a spinning (in other direction) grey wire sphere for a twinkling effect
	glRotatef(25 * -year, 0, 0, 1);
	glColor4f(randomGrey, randomGrey, randomGrey, 0.3);
	glutWireSphere(2, 10, 10);
	glPopMatrix();
}

/************************************************************************
Function:		 readEnterpriseIn
Description:	This function, which is called in main method, reads in 
the enterprise.txt file and saves the value to appropriate structs.
*************************************************************************/
void readEnterpriseIn() {
	//pointer to file
	FILE *pToFile = fopen("enterprise.txt", "r");
	//assigns which index in array values go to
	int vectorTot = 0;
	int facesTot = 0;
	char input[50];

	//occurs if file not in directory
	if (pToFile == NULL) {
		printf("ERROR: No such file.");
	}
	else {
		//where the magic happens, saves values appropriately 
		while (fgets(input, sizeof(input), pToFile) != NULL) {
			if (input[0] == 'v') {
				sscanf(input, "v %f %f %f", &vertex[vectorTot].point1, &vertex[vectorTot].point2, &vertex[vectorTot].point3);
				vectorTot++;
			}
			if (input[0] == 'f') {
				sscanf(input, "f %d %d %d", &face[facesTot].point1, &face[facesTot].point2, &face[facesTot].point3);
				facesTot++;
			}

		}
	}
	fclose(pToFile);//closes .txt file
}

/************************************************************************
Function:		 drawEnterprise
Description:	This function draws the enterprise, and makes sure that
it stays at a certain location when the camera moves.
*************************************************************************/
void drawEnterprise() {
	glPushMatrix();
	//translate to bottom of screen, even if camera moves
	glTranslatef(0 + xCamCenter, yCamCenter-7, zCamCenter+15);
	glRotatef(-20, 1, 0, 0);
	glRotatef(-xCamMove*10, 0, 1, 1);
	glRotatef(yCamMove * 10, 1, 0, 0);
	glRotatef(zCamMove * 10, 1, 0, 0);
	glScalef(10, 10, 10);//makes it bigger
	int i = 0;
	while (i < 1989) {
		glBegin(GL_TRIANGLES);
		glColor3f(.8, .8, .8);
		glVertex3f(vertex[face[i].point1 - 1].point1, vertex[face[i].point1 - 1].point2, vertex[face[i].point1 - 1].point3);
		glVertex3f(vertex[face[i].point2 - 1].point1, vertex[face[i].point2 - 1].point2, vertex[face[i].point2 - 1].point3);
		glVertex3f(vertex[face[i].point3 - 1].point1, vertex[face[i].point3 - 1].point2, vertex[face[i].point3 - 1].point3);
		i++;
		glVertex3f(vertex[face[i].point1 - 1].point1, vertex[face[i].point1 - 1].point2, vertex[face[i].point1 - 1].point3);
		glVertex3f(vertex[face[i].point2 - 1].point1, vertex[face[i].point2 - 1].point2, vertex[face[i].point2 - 1].point3);
		glVertex3f(vertex[face[i].point3 - 1].point1, vertex[face[i].point3 - 1].point2, vertex[face[i].point3 - 1].point3);
		i++;
		glVertex3f(vertex[face[i].point1 - 1].point1, vertex[face[i].point1 - 1].point2, vertex[face[i].point1 - 1].point3);
		glVertex3f(vertex[face[i].point2 - 1].point1, vertex[face[i].point2 - 1].point2, vertex[face[i].point2 - 1].point3);
		glVertex3f(vertex[face[i].point3 - 1].point1, vertex[face[i].point3 - 1].point2, vertex[face[i].point3 - 1].point3);
		i++;
		glEnd();
	}
	glPopMatrix();
}

void readVoyagerIn() {
	//pointer to file
	FILE *pToFile = fopen("voyager.txt", "r");
	//assigns which index in array values go to
	int vectorTot = 0;
	int facesTot = 0;
	char input[50];

	//occurs if file not in directory
	if (pToFile == NULL) {
		printf("ERROR: No such file.");
	}
	else {
		//where the magic happens, saves values appropriately 
		while (fgets(input, sizeof(input), pToFile) != NULL) {
			if (input[0] == 'v') {
				sscanf(input, "v %f %f %f", &voyagerVertex[vectorTot].point1, &voyagerVertex[vectorTot].point2, &voyagerVertex[vectorTot].point3);
				vectorTot++;
			}
			if (input[0] == 'f') {
				sscanf(input, "f %d %d %d", &voyagerFace[facesTot].point1, &voyagerFace[facesTot].point2, &voyagerFace[facesTot].point3);
				facesTot++;
			}

		}
	}
	fclose(pToFile);//closes .txt file
}

void drawVoyager() {
	glPushMatrix();
	//translate to bottom of screen, even if camera moves
	glTranslatef(voyagerXPos+xCamCenter, yCamCenter , zCamCenter );
	glRotatef(year, 1, 1, 1);
	glRotatef(-xCamMove * 10, 0, 1, 1);
	glRotatef(yCamMove * 10, 1, 0, 0);
	glRotatef(zCamMove * 10, 1, 0, 0);
	glScalef(10, 10, 10);//makes it bigger
	int i = 0;
	while (i < 6623) {
		glBegin(GL_TRIANGLES);
		glColor3f(.8, 0, 0);
		glVertex3f(voyagerVertex[voyagerFace[i].point1 - 1].point1, voyagerVertex[voyagerFace[i].point1 - 1].point2, voyagerVertex[voyagerFace[i].point1 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point2 - 1].point1, voyagerVertex[voyagerFace[i].point2 - 1].point2, voyagerVertex[voyagerFace[i].point2 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point3 - 1].point1, voyagerVertex[voyagerFace[i].point3 - 1].point2, voyagerVertex[voyagerFace[i].point3 - 1].point3);
		i++;
		glVertex3f(voyagerVertex[voyagerFace[i].point1 - 1].point1, voyagerVertex[voyagerFace[i].point1 - 1].point2, voyagerVertex[voyagerFace[i].point1 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point2 - 1].point1, voyagerVertex[voyagerFace[i].point2 - 1].point2, voyagerVertex[voyagerFace[i].point2 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point3 - 1].point1, voyagerVertex[voyagerFace[i].point3 - 1].point2, voyagerVertex[voyagerFace[i].point3 - 1].point3);
		i++;
		glVertex3f(voyagerVertex[voyagerFace[i].point1 - 1].point1, voyagerVertex[voyagerFace[i].point1 - 1].point2, voyagerVertex[voyagerFace[i].point1 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point2 - 1].point1, voyagerVertex[voyagerFace[i].point2 - 1].point2, voyagerVertex[voyagerFace[i].point2 - 1].point3);
		glVertex3f(voyagerVertex[voyagerFace[i].point3 - 1].point1, voyagerVertex[voyagerFace[i].point3 - 1].point2, voyagerVertex[voyagerFace[i].point3 - 1].point3);
		i++;
		glEnd();
	}
	glPopMatrix();
}


/************************************************************************
Function:		 display
Description:	This function first makes the camera where it's supposed to,
calls methods to draw stars (if on), orbits (if on), planets, suns corona 
(if on), the enterprise, and the enterprise's shield (if on).
*************************************************************************/
void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gluLookAt(xCamEye, yCamEye, zCamEye, xCamCenter, yCamCenter, zCamCenter,0, 1, 0);
	//only draws stars if on
	if (starsOn) {
		starDrawer();
	}
	//only draws orbits if on
	if(orbitsOn)
		drawAllOrbits();
	//draws planets
	drawAllPlanets();
	//only draws corona if on
	if(coronaOn)
		drawCorona();
	//draws enterprise
	if (voyagerOn);
		drawVoyager();
	drawEnterprise();
	//only draws shield if on
	if(shieldOn)
		drawShield();
	glLoadIdentity();
	glFlush();
	glutSwapBuffers();
}

/************************************************************************
Function:		 timer
Description:	This function makes sure that the program doesn't run too 
fast, and increases the year.
*************************************************************************/
void timer(int t) {
	year++;//increases year
	glLoadIdentity();
	glutPostRedisplay();
	glutTimerFunc(15, timer, t);//makes sure program doesn't run too fast
}

/************************************************************************
Function:		 reshape
Description:	This function handles a window reshape if required, and sets
the viewing moves
*************************************************************************/
void reshape(GLint w, GLint h) {
	glViewport(0, 0, w, h);//uses a view port when window shape changes
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1, 1000.0);//xFar is large to account for the camera being able to move in and out
	glMatrixMode(GL_MODELVIEW);
}

/************************************************************************
Function:		 myKeyboard
Description:	This function checks to see if a button which changes the 
scene has been pressed. (NOT CAMERA)
*************************************************************************/
void myKeyboard(unsigned char key, int x, int y) {
	//turn orbits on if off
	if (key == 'r' && !orbitsOn) {
		orbitsOn = 1;
	}
	//turn orbits off if on
	else if (key == 'r' && orbitsOn) {
		orbitsOn = 0;
	}
	//turn stars on if off
	else if (key == 's' && !starsOn) {
		starsOn = 1;
	}
	//turn stars off if on
	else if (key == 's' && starsOn) {
		starsOn = 0;
	}
	//turn corona on if off
	else if (key == 'c' && !coronaOn) {
		coronaOn = 1;
	}
	//turn corona off if on
	else if (key == 'c' && coronaOn) {
		coronaOn = 0;
	}
	//turn shield on if off
	else if (key == 'k' && !shieldOn) {
		shieldOn = 1;
	}
	//turn shield off if on
	else if (key == 'k' && shieldOn) {
		shieldOn = 0;
	}
	//turn voyager on if off
	else if (key == 'v' && !voyagerOn) {
		voyagerOn = 1;
	}

}

	/************************************************************************
	Function:		 specialKeyPressed
	Description:	This function checks to see if a button which changes the
	camera has been pressed. If so, it changes the amount to move that
	camera coordinate by.
	*************************************************************************/
	void specialKeyPressed(int key) {
		if (key == GLUT_KEY_LEFT) {
			xCamMove = -0.5;
		}
		else if (key == GLUT_KEY_RIGHT) {
			xCamMove = 0.5;
		}
		else if (key == GLUT_KEY_UP) {
			yCamMove = 0.5;
		}
		else if (key == GLUT_KEY_DOWN) {
			yCamMove = -0.5;
		}
		else if (key == GLUT_KEY_PAGE_UP) {
			zCamMove = -0.5;
		}
		else if (key == GLUT_KEY_PAGE_DOWN) {
			zCamMove = 0.5;
		}
	}

/************************************************************************
Function:		 myIdle
Description:	This function changes the value of the camera eye and 
centers for each x, y and z value.
*************************************************************************/
void myIdle(){
	if (xCamMove!=0) {
		xCamEye += xCamMove;
		xCamCenter += xCamMove;
	}
	if (yCamMove != 0) {
		yCamEye += yCamMove;
		yCamCenter += yCamMove;
	}
	if (zCamMove != 0) {
		zCamEye += zCamMove;
		zCamCenter += zCamMove;
	}
	if (voyagerOn&&voyagerXPos<45) {
		voyagerXPos = voyagerXPos+0.5;
	}
	if (voyagerXPos >= 45) {
		voyagerXPos = -45;
		voyagerOn = 0;
	}
	glutPostRedisplay();//updates display
}

/************************************************************************
Function:		 specialKeyReleased
Description:	This function checks to see if a button which changes the
camera has been released. If so, it changes the amount to move that
camera coordinate by.
*************************************************************************/
void specialKeyReleased(int key) {
	if (key == GLUT_KEY_LEFT) {
		xCamMove = 0;
	}
	else if (key == GLUT_KEY_RIGHT) {
		xCamMove = 0;
	}
	else if (key == GLUT_KEY_UP) {
		yCamMove = 0;
	}
	else if (key == GLUT_KEY_DOWN) {
		yCamMove = 0;
	}
	else if (key == GLUT_KEY_PAGE_UP) {
		zCamMove = 0;
	}
	else if (key == GLUT_KEY_PAGE_DOWN) {
		zCamMove = 0;
	}
}

/************************************************************************
Function:		initializeGL
Description:	This function sets up openGL and enables depth testing 
and the background color to black.
*************************************************************************/
void initializeGL(){
	// Enables depth testing
	glEnable(GL_DEPTH_TEST);
	//Set background to black
	glClearColor(0, 0, 0, 1.0);

}

/************************************************************************
Function:		printControls
Description:	This function prints how to control the scene and camera.
*************************************************************************/
void printControls() {
	printf("Space Controls\n");
	printf("-------------------------------\n\n");
	printf("r: toggle orbits\n");
	printf("s: toggle stars\n");
	printf("c: toggle corona\n");
	printf("k: toggle shield\n\n");
	printf("Camera Controls\n");
	printf("-------------------------------\n\n");
	printf("Up\tArrow: \tmove up\n");
	printf("Down\tArrow: \tmove down\n");
	printf("Right\tArrow: \tmove right\n");
	printf("Left\tArrow: \tmove left\n");
	printf("PAGE\tUP: \tmove forwards\n");
	printf("PAGE\tDOWN: \tmove backwards");
}

/************************************************************************
Function:		main
Description:	This function calls all important functions, and then
turns into an infinite loop.
*************************************************************************/
int main(int argc, char** argv) {
	//prints controls
	printControls();
	//reads the text file, 'enterprise.txt' in
	readEnterpriseIn();
	readVoyagerIn();
	//makes locations of all stars
	starLocationMaker();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	//assigns window size
	glutInitWindowSize(windowHeight, windowWidth);
	glutCreateWindow("Liam Gowan's CSCI 3161 Assignment 2 - Hitch Hiker's Guide to the Planets");
	//displays everything and changes rotation values 
	glutDisplayFunc(display);
	glutIdleFunc(myIdle);
	//reshapes window shape and perspective
	glutReshapeFunc(reshape);
	glutTimerFunc(100, timer, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	//calls keyboard functions
	glutKeyboardFunc(myKeyboard);
	glutSpecialFunc(specialKeyPressed);
	glutSpecialUpFunc(specialKeyReleased);
	//initializes
	initializeGL();
	//enters infinite loop
	glutMainLoop();
}